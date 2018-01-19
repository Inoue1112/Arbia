#include "Player.h"

#include <Windows.h>

//-----振動-----//.
//頭を打つ.
const unsigned short usVIB_HEAD_POWER = INPUT_VIBRATION_MAX / 4;
const int iVIB_HEAD_TIME = 10;
//ジャンプ攻撃着地.
const unsigned short usVIB_JATK_POWER = INPUT_VIBRATION_MAX;
const int iVIB_JATK_TIME = 20;
//死亡.
const unsigned short usVIB_DEAD_POWER = INPUT_VIBRATION_MAX;
//const int iVIB_DEAD_TIME = 180;
const int iVIB_DEAD_TIME = -1;
const int iVIB_DEAD_DEC = 511;//1フレームでどれだけ弱まるか.



//----- 音 -----//.
//足音.
#define ALIAS_NAME_RUN "AlbiaRun"
#define  FILE_PATH_RUN "SE\\100Arbia\\010Run.wav"
const	  int iVOL_RUN = 1000;

//飛び立つ.
#define ALIAS_NAME_JUMP "AlbiaJump"
#define  FILE_PATH_JUMP "SE\\100Arbia\\020Jump.wav"
const	  int iVOL_JUMP = 1000;

//キック.
#define ALIAS_NAME_ATK "AlbiaAtk"
#define  FILE_PATH_ATK "SE\\100Arbia\\200Atk.wav"
const	  int iVOL_ATK = 1000;

//空中ポーズ.
#define ALIAS_NAME_JUMP_ATK_SPN "AlbiaJumpAtk"
#define  FILE_PATH_JUMP_ATK_SPN "SE\\100Arbia\\600JumpAtkSpn.wav"
const	  int iVOL_JUMP_ATK_SPN = 1000;

//かかと落とし着地.
#define ALIAS_NAME_JUMP_ATK_STMP "AlbiaJumpStmp"
#define  FILE_PATH_JUMP_ATK_STMP "SE\\100Arbia\\610JumpAtkStep.wav"
const	  int iVOL_JUMP_ATK_STMP = 1000;

//頭撃つ.
#define ALIAS_NAME_HEAD_HIT "AlbiaHeadHit"
#define  FILE_PATH_HEAD_HIT "SE\\100Arbia\\700HeadHit.wav"
const	  int iVOL_HEAD_HIT = 1000;

//やられ.
#define ALIAS_NAME_DEAD "AlbiaDead"
#define  FILE_PATH_DEAD "SE\\100Arbia\\900Dead.wav"
const	  int iVOL_DEAD = 1000;




//スタート地点.
const D3DXVECTOR3 vSTART_POSITION = { 0.0f, 0.0f, 9.0f };


//残機.
const int iMAX_HP = 10;


//===== あたり判定 =====//.
const float COL_PLAYER_H  = 2.0f;
const float COL_PLAYER_RANGE  = 0.25f;

//===== Box =====//.
const float COL_PLAYER_BOX_XZ = 0.5f / 2.0f;
const float COL_PLAYER_BOX_Y = COL_PLAYER_H / 2.0f;
const float COL_PLAYER_BOX_CENTER_Y = COL_PLAYER_H / 2.0f;

//===== ﾌﾟﾚｲﾔｰの攻撃 =====//.
const int	COL_PLAYER_KICK_THETA = 120;
const float	COL_PLAYER_KICK_RANGE = 1.25f;
//===== あたり判定 終わり =====//.


const double fANIM_SPD = 0.03125 / 2.0;





//ﾓﾃﾞﾙの都合(前後向き).
const float fMODEL_BACK_TURN = (float)M_PI;

//ｼﾞｬﾝﾌﾟ上昇の上限時間.
const int iJUNP_TIME_UP_TIME = 14;//jUpTime.

//ジャンプ落下加速.
const float fGRAVITY = 0.02f;//gravity.


//最初のフレームに必要.
float fLAND_PLAYER_ZERO = 0.0f;



clsPlayer::clsPlayer()
{
	m_dAnimSpeed = fANIM_SPD;
	
}

clsPlayer::~clsPlayer()
{
	Rerease();
}

void clsPlayer::Create( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	SetSe( hWnd );
	m_iHp = iMAX_HP;
	Init();
	m_pShadow = new clsShadow;
	m_pShadow->Init( pDevice11, pContext11 );

	m_pEffect = clsEffects::GetInstance();
}


void clsPlayer::Init()
{
	m_bDead = false;

	m_bJump = false;
	m_bLanding = true;
	m_fJumpEnagy = 0.0f;
	m_iJumpTimer = 0;
	m_bJumpAtkTopFlg = false;//頂点でしか発動しない.
	m_bRunning = false;
	m_bAtkImpact = false;

	m_fSpd = 1.0f;
	m_fYawTarget = 0.0f;

	m_fFloorY = fLAND_PLAYER_ZERO;

	m_enMove = enPM_IDLE;
	m_enDir = enDirection_Stop;

	//あたり判定.
	//本体.
	ColState.fRange = COL_PLAYER_RANGE;
	ColState.fHeight = COL_PLAYER_H;

	//Box用.
	ColState.vRangeHalf.x = ColState.vRangeHalf.z = COL_PLAYER_BOX_XZ;
	ColState.vRangeHalf.y = COL_PLAYER_BOX_Y;
	ColState.fCenterY = COL_PLAYER_BOX_CENTER_Y;
	ColState.fCenterX = 0.0f;

	//Sub.
	m_colSub.fRange = COL_PLAYER_KICK_RANGE;
	m_colSub.fHeight = COL_PLAYER_H;
	m_colSub.iSarchTheta = COL_PLAYER_KICK_THETA;

	//ｱﾆﾒｰｼｮﾝ.
	ChangeAnimMode( enANIM_IDLE );

}
//ステージスタート.
void clsPlayer::Spawn()
{
	m_vReSpawnPos = vSTART_POSITION;
	ReSpawn();
	m_iHp = iMAX_HP;
	m_vRot.y = m_fYawTarget = (float)M_PI;
}
//復活.
void clsPlayer::ReSpawn()
{
	Init();
	m_iHp --;
	m_vPos = m_vReSpawnPos;
	m_vRot.y = m_fYawTarget = 0.0f;
	m_fOldY = m_vPos.y;
}

void clsPlayer::Rerease()
{
	if( m_pShadow != NULL ){
		delete m_pShadow;
		m_pShadow = NULL;
	}


	if( m_ppSe != NULL ){
		for( int i=0; i<enPS_MAX; i++ ){
			delete m_ppSe[i];
			m_ppSe[i] = NULL;
		}
		delete[]m_ppSe;
		m_ppSe = NULL;
	}

	if( m_pModel != NULL ){
		delete m_pModel;
		m_pModel = NULL;
	}

	if( m_pXInput != NULL ){
		m_pXInput = NULL;
	}
}
//============================================================
//	入力.
//============================================================
void clsPlayer::Input( clsXInput* const xInput )
{
	if( m_enMove != enPM_ATK	 &&
		m_enMove != enPM_JUM_ATK &&
		m_enMove != enPM_DEAD )
	{
		//移動.
		Input_Move( xInput );
		//ｱｸｼｮﾝ.
		Input_Action( xInput );
	}

	if( m_pXInput == NULL ){
		m_pXInput = xInput;
	}

	//デバッグ用.
	if( GetAsyncKeyState( VK_SPACE ) & 0x1 ){
		m_iHp = 0;
	}


}
//------------------------------.
//	入力（移動）.
//------------------------------.
void clsPlayer::Input_Move( clsXInput* const xInput )
{
	//入力されているか.
	bool bInputMovePlus = true;
	bool bInputMoveStick = true;


	//十字ｷｰ.
	float closButnTarYaw = m_fYawTarget;//目標角度一時保存.
	m_enDir = enDirection_Foward;
	m_enMove = enPM_RUN;
	m_bJumpSpdDown = false;
	//上.
	if( InputUp( xInput ) ){
		closButnTarYaw = fMODEL_BACK_TURN + 0.0f;
		if( InputRight( xInput ) ){
			closButnTarYaw += (float)M_PI_4;
		}
		else if( InputLeft( xInput ) ){
			closButnTarYaw -= (float)M_PI_4;
		}
	}
	//下.
	else if( InputDown( xInput ) ){
		closButnTarYaw = fMODEL_BACK_TURN + (float)M_PI;
		if( InputRight( xInput ) ){
			closButnTarYaw -= (float)M_PI_4;
		}
		else if( InputLeft( xInput ) ){
			closButnTarYaw += (float)M_PI_4;
		}
	}
	//右.
	else if( InputRight( xInput ) ){
		closButnTarYaw = fMODEL_BACK_TURN + (float)M_PI_2;
	}
	//左.
	else if( InputLeft( xInput ) ){
		closButnTarYaw = fMODEL_BACK_TURN + (float)( M_PI + M_PI_2 );
	}
	//何も押していない.
	else{
		if( !m_bJump ){
			m_enMove = enPM_IDLE;
			m_enDir = enDirection_Stop;
//			m_fSpd = 0.0f;
		}
		m_bJumpSpdDown = true;
		bInputMovePlus = false;
	}

	//十字ｷｰに触れたなら.
	if( !m_bJumpSpdDown ){
		//ｽﾃｨｯｸ方向逆以外.
		if( ThetaCheck( m_vRot.y, m_fYawTarget, 270 ) ){
			//目標の方向を変える.
			m_fYawTarget = closButnTarYaw;
			ThetaOverGuard( m_fYawTarget );
		}
		//ｽﾃｨｯｸ逆入力.
		else{
			m_bJumpSpdDown = true;
		}
	}

	//ｽﾃｨｯｸ.
	if( xInput->GetLStickSlope() != clsXInput::enSS_NOTHING ){
		m_enMove = enPM_RUN;
		m_bJumpSpdDown = false;
		m_enDir = enDirection_Foward;

		//ｽﾃｨｯｸ方向逆以外.
		if( ThetaCheck( m_vRot.y, m_fYawTarget, 270 ) ){
			//目標の方向を変える.
			m_fYawTarget = xInput->GetLStickTheta();
			ThetaOverGuard( m_fYawTarget );
		}
		//ｽﾃｨｯｸ逆入力.
		else{
			m_bJumpSpdDown = true;
		}
	}
	else{
		bInputMoveStick = false;
	}

	//十字キーもスティックも入力が無いなら.
	if( !bInputMovePlus && !bInputMoveStick ){
		//陸上.
		if( !m_bJump ){
			m_fSpd = 0.0f;
			//アニメーション(走り終わる).
			if( m_bRunning ){
				if( m_enAnimNo == enANIM_ATK ){}
				else if( m_enAnimNo == enANIM_RUNNING_L ){
					ChangeAnimMode( enANIM_RUN_END_L );
				}
				else if( m_enAnimNo == enANIM_RUNNING_R ){
					ChangeAnimMode( enANIM_RUN_END_R );
				}
				else if( m_enAnimNo == enANIM_RUN_START ){
					//攻撃は例外.
					if( m_enMove != enPM_ATK ){	
						ChangeAnimMode( enANIM_RUN_END_R );
					}
				}
			}
		}
		m_bRunning = false;//走りアニメーション用フラグ.
	}
	//どちらか入力しているなら.
	else{
		//陸上.
		if( !m_bJump ){
			//走り始め.
			if( !m_bRunning ){
				//攻撃中は除く.
				if( m_enAnimNo != enANIM_ATK ){
					ChangeAnimMode( enANIM_RUN_START );
				}
			}
		}
		m_bRunning = true;
	}


}
//------------------------------.
//	入力（ｱｸｼｮﾝ）.
//------------------------------.
void clsPlayer::Input_Action( clsXInput* const xInput )
{
	//ｼﾞｬﾝﾌﾟ.
	Input_Action_Jump( xInput );
	//攻撃.
	Input_Action_Atk( xInput );
}
//	入力｛ ｱｸｼｮﾝ （ｼﾞｬﾝﾌﾟ）｝.
void clsPlayer::Input_Action_Jump( clsXInput* const xInput )
{
	//ｼﾞｬﾝﾌﾟ開始.
	if( xInput->IsPressEnter( XINPUT_A ) ||
		GetAsyncKeyState( 'Z' ) & 0x1 )
	{
		if( !m_bJump ){
			SetJump();
		}
	}

	//ｼﾞｬﾝﾌﾟ上昇中断.
	if( m_bJump ){
		if( xInput->IsPressExit( XINPUT_A ) ){
			m_iJumpTimer = iJUNP_TIME_UP_TIME;
			ChangeAnimMode( enANIM_JUMP_U_TURN );
		}
	}
}
//	入力｛ ｱｸｼｮﾝ （攻撃）｝.
void clsPlayer::Input_Action_Atk( clsXInput* const xInput )
{
	const float JUMP_STK_PER_ENGY_S = 0.1875f;//Permission許可.//JumpAtkPermEnegyS.
	const float JUMP_STK_PER_ENGY_E = -0.0625f;//Permission許可.//JumpAtkPermEnegyE.

	const float JUMP_ATK_START_ENAGY = 0.15f;//初期上昇力.//JumpAtkStartEneagyB.

	if( xInput->IsPressEnter( XINPUT_X ) ||
		GetAsyncKeyState( 'X' ) )
	{
		//ｼﾞｬﾝﾌﾟ攻撃.
		if( m_bJump ){
			//ｼﾞｬﾝﾌﾟの頂点?.
			if( m_bJumpAtkTopFlg ){
				//頂点近くを判定するif文.
				if( m_fJumpEnagy < JUMP_STK_PER_ENGY_S &&
					m_fJumpEnagy > JUMP_STK_PER_ENGY_E )
				{
					m_enMove = enPM_JUM_ATK;
					m_bLanding = false;//陸地か否か.
					m_fJumpEnagy = JUMP_ATK_START_ENAGY;
					ChangeAnimMode( enANIM_JUMP_ATK_SPN );
					m_bAtkImpact = true;
					PlaySe( enPS_JUMP_ATK_SPN );
				}
			}
		}
		//通常攻撃.
		else{
			Kick();
		}
	}
}

//============================================================
//	自動挙動.
//============================================================
void clsPlayer::Move( float fEarZ )
{

//エフェクシーア.
D3DXVECTOR3 vPos = m_vPos;
vPos.x -= 2.0f;
vPos.y += 2.0f;
vPos.z += 2.0f;
if( !m_pEffect->PlayCheck( m_ehOne ) ){
	m_ehOne = m_pEffect->Play( clsEffects::enEfcType_MgcDrk, vPos );
}
m_pEffect->SetLocation( m_ehOne, vPos );


	//耳の位置を覚えろ.
	m_fEarZ = fEarZ;

	//かかと落とし用.
	m_fOldY = m_vPos.y;

	Move_Move();
	Move_Action();

	SetSpeed();
	UpdateDir();
	UpDateModel();

	UpdatePos();

	UpdateColState();

	Animation();

	m_pShadow->SetShadow( m_vPos, m_fFloorY );
}
//------------------------------.
//	自動挙動（移動）.
//------------------------------.
void clsPlayer::Move_Move()
{
	//加速度.
	const float SPD_UP_POWER = 0.0625f;//highSpdChange

	//北向きﾌﾗｸﾞ(ｷｬﾗが北と南どちらを向いてるか).
	if( ThetaCheck( (float)M_PI, m_vRot.y, 90 ) ){
		m_bDirNorth = true;
	}
	else if( ThetaCheck( 0.0f, m_vRot.y, 90 ) ){
		m_bDirNorth = false;
	}

	//速度変更.
	if( m_bJump ){
		if( m_bJumpSpdDown ){
			Deceleration();
		}
		else{
			float TarSpd = m_fSpd;
			if( m_enMove == enPM_RUN ){
				TarSpd = 1.0f;
			}
			//加速.
			if( m_fSpd == 0.0f )	m_fSpd = 0.1f;
			else m_fSpd += SPD_UP_POWER;
			if( m_fSpd > TarSpd )	m_fSpd = TarSpd;
		}
	}
	else{
		if( m_enMove == enPM_RUN ){
			m_fSpd = 1.0f;
		}
	}

	//振り向き.
	if( m_bJump ){
		if( !m_bJumpSpdDown ){
			//一瞬で振り向かない(徐々に振り向く)if().
			YawSpnToTarg( m_vRot.y, m_fYawTarget, 0.0625f );
		}
		else if( m_fSpd == 0.0f ){
			//一瞬で振り向く.
			m_vRot.y = m_fYawTarget;
		}
	}
	else{
		//一瞬で振り向く.
		m_vRot.y = m_fYawTarget;
	}
	ThetaOverGuard( m_vRot.y );


	//足場から落ちる自由落下.
	//ｼﾞｬﾝﾌﾟ中ではない時.
	if( !m_bJump ){
		//下に足場がない時.
		if( m_bDropout ){
			SetJump( true );
		}
	}

	UpdateDir();
}
//------------------------------.
//	自動挙動（ｱｸｼｮﾝ）.
//------------------------------.
void clsPlayer::Move_Action()
{
	//ｼﾞｬﾝﾌﾟ.
	if( m_enMove != enPM_JUM_ATK ){
		Move_Action_Jump();
	}
	//攻撃.
	Move_Action_Atk();
}
//	自動挙動｛ ｱｸｼｮﾝ （ｼﾞｬﾝﾌﾟ）｝.
void clsPlayer::Move_Action_Jump()
{
	const float DROP_DOWN_SPD_MAX = -0.2f;//jFallSpdMax.
	
	//普通ｼﾞｬﾝﾌﾟ行動途中.
	if( m_bJump ){
		m_iJumpTimer ++;
		//落下速度増速.
		if( m_iJumpTimer > iJUNP_TIME_UP_TIME ){
			m_fJumpEnagy -= fGRAVITY;
		}
		else if( m_iJumpTimer == iJUNP_TIME_UP_TIME ){
			ChangeAnimMode( enANIM_JUMP_U_TURN );
		}
		//落下速くなりすぎない.
		if( m_fJumpEnagy < DROP_DOWN_SPD_MAX ){
			m_fJumpEnagy = DROP_DOWN_SPD_MAX;
		}
		//落下.
		m_vPos.y += m_fJumpEnagy;
		//着地.
		if( m_vPos.y < m_fFloorY ){
			m_vPos.y = m_fFloorY;
			m_bJump = false;
			SetDropoutFlg( false );
			if( m_bRunning ){
				ChangeAnimMode( enANIM_RUNNING_R );
			}
			else{
				ChangeAnimMode( enANIM_JUMP_STMP );
			}
			PlaySe( enPS_RUN );
		}
	}
}
//	自動挙動｛ ｱｸｼｮﾝ （攻撃）｝.
void clsPlayer::Move_Action_Atk()
{
	//減速関数.
	Deceleration();


	//衝撃波のあたり判定にこれを使え.
//					EnemDead( true );


	//通常攻撃.
	if( m_enMove == enPM_ATK ){
//		//攻撃終了（アニメ終了時に引っ越し）.
	}

	//ｼﾞｬﾝﾌﾟ攻撃.
	else if( m_enMove == enPM_JUM_ATK ){
		const int JUMP_ATK_END_TIME = 32;//jumpAtkEndTimer.
		const float JUMP_ATK_GRAVITY_SPN = 0.005f;//0.03025f.//jAtk_gravitySpnB.
		const float JUMP_ATK_GRAVITY_DOWN_START = 0.3625f;//0.03025f.//jAtk_gravityDownStartB.
		const float JUMP_ATK_GRAVITY_DOWN_PLUS = 0.035f;//0.03025f.//jAtk_gravityDownB.

		const float JumpAtkDropDown = 0.08125f;


		//空中機動.
		if( !m_bLanding ){
			//上昇中.
			if( m_fJumpEnagy > 0.0f ){
				m_fJumpEnagy -= JUMP_ATK_GRAVITY_SPN;
				//降下に切り替わる瞬間.
				if( m_fJumpEnagy <= 0.0f ){
					m_fJumpEnagy = -JUMP_ATK_GRAVITY_DOWN_START;
					ChangeAnimMode( enANIM_JUMP_ATK_FALL );
				}
			}
			else{
				m_fJumpEnagy -= JUMP_ATK_GRAVITY_DOWN_PLUS;
			}

			m_vPos.y += m_fJumpEnagy;
			//着地.
			if( m_vPos.y < m_fFloorY ){
				m_vPos.y = m_fFloorY;
				m_bLanding = true;
				m_bJump = false;
				m_bAtkImpact = false;
				ChangeAnimMode( enANIM_JUMP_ATK_STMP );
				PlaySe( enPS_JUMP_ATK_STMP );
				m_pXInput->SetVibPowerL( usVIB_JATK_POWER, iVIB_JATK_TIME );//振動.
			}
		}
//		//衝撃波立ち上がり(アニメ終了へ引っ越し).
	}
}


//============================================================
//	ミス（死亡）.
//============================================================
void clsPlayer::Dead()
{
	m_enMove = enPM_DEAD;
	ChangeAnimMode( enANIM_DEAD );
	m_bDead = true;
	PlaySe( enPS_DEAD );
	m_pXInput->SetVibPowerL( usVIB_DEAD_POWER, iVIB_DEAD_TIME, iVIB_DEAD_DEC );//振動.
	m_pXInput->SetVibPowerR( usVIB_DEAD_POWER, iVIB_DEAD_TIME, iVIB_DEAD_DEC );//振動.
}






//============================================================
//	減速関数.
//============================================================
void clsPlayer::Deceleration()
{
	const float lowSpdChange = 0.025f;
	m_fSpd -= lowSpdChange;
	if( m_fSpd < 0.0f )	m_fSpd = 0.0f;
}


//============================================================
//	ラジコン操作Update.
//============================================================
void clsPlayer::UpdateDir()
{
	m_pModel->m_enDir = m_enDir;
}


//============================================================
//	ﾑｰﾌﾞの最後に使う.
//============================================================
void clsPlayer::UpdateColState()
{
	ColState.vPos = m_vPos;
	ColState.fYaw = m_vRot.y;

	//Subあたり判定.
	m_colSub.vPos = m_vPos;
	m_colSub.fYaw = m_vRot.y;
}


//============================================================
//	ジャンプ開始.
//============================================================
void clsPlayer::SetJump( bool bDropout )
{
	const float JUMP_POWER = 0.2f;

	m_bJump = true;
	//まっとうなｼﾞｬﾝﾌﾟ.
	if( !bDropout ){
		m_fJumpEnagy = JUMP_POWER;
		m_iJumpTimer = 0;
		m_bJumpAtkTopFlg = true;
		ChangeAnimMode( enANIM_JUMP_START );
		PlaySe( enPS_JUMP );
	}
	//足場踏み外し落下.
	else{
		m_fJumpEnagy = 0;
		m_iJumpTimer = iJUNP_TIME_UP_TIME;
		m_bJumpAtkTopFlg = false;
		ChangeAnimMode( enANIM_JUMP_FALL );
	}
	SetDropoutFlg ( false );
}

//============================================================
//	アニメーション.
//============================================================
void clsPlayer::Animation()
{
#if 1
	//ループしないモーション.
	if( m_enAnimNo == enANIM_RUN_START		||
		m_enAnimNo == enANIM_RUNNING_R		||
		m_enAnimNo == enANIM_RUNNING_L		||
		m_enAnimNo == enANIM_RUN_END_R		||
		m_enAnimNo == enANIM_RUN_END_L		||
		m_enAnimNo == enANIM_JUMP_START		||
		m_enAnimNo == enANIM_JUMP_U_TURN	||
		m_enAnimNo == enANIM_JUMP_STMP		||
		m_enAnimNo == enANIM_ATK			||
		m_enAnimNo == enANIM_JUMP_ATK_SPN	||
		m_enAnimNo == enANIM_JUMP_ATK_STMP	||
		m_enAnimNo == enANIM_JUMP_ATK_STND	||
		m_enAnimNo == enANIM_DEAD			||
		m_enAnimNo == enANIM_LOSE )
	{
		//アニメーション時間加算.
		m_dAnimTimer += m_pModel->GetAnimSpeed();
	}


	//現在のアニメーションを終えたら( アニメNo.に掛ける{ 後ろのアニメになるほど遅れるので } ).
	const int iRATE = 3;//.

	bool bAnimEndFlg = false;

	//アニメーション終わった?.
	//ごり押し( このアニメだけ残りが多い ).
	const int iEXCEPTION_ANIM_END_RATE = 4; //例外アニメのアニメーション終了時間のレート.
	if( m_enAnimNo == enANIM_RUNNING_R ){
		if( m_pModel->GetAnimPeriod( m_enAnimNo ) - ( dANIM_ONE_FRAME_OVER_SOLUTION * (double)( m_enAnimNo * m_enAnimNo / iRATE * iEXCEPTION_ANIM_END_RATE ) ) <= m_dAnimTimer ){
			bAnimEndFlg = true;
		}
	}
	else{
		if( m_pModel->GetAnimPeriod( m_enAnimNo ) - ( dANIM_ONE_FRAME_OVER_SOLUTION * (double)( m_enAnimNo * m_enAnimNo / iRATE ) ) <= m_dAnimTimer ){
			bAnimEndFlg = true;
		}
	}

	//アニメーションが終わったら.
	if( bAnimEndFlg ){
		//ループしないモーション.
		switch( m_enAnimNo )
		{
		//走り.
		case enANIM_RUN_START:
			ChangeAnimMode( enANIM_RUNNING_R );
			PlaySe( enPS_RUN );
			break;
		case enANIM_RUNNING_R:
			ChangeAnimMode( enANIM_RUNNING_L );
			PlaySe( enPS_RUN );
			break;
		case enANIM_RUNNING_L:
			ChangeAnimMode( enANIM_RUNNING_R );
			PlaySe( enPS_RUN );
			break;
		case enANIM_RUN_END_R:
		case enANIM_RUN_END_L:
			ChangeAnimMode( enANIM_IDLE );
			break;

		//ジャンプ.
		case enANIM_JUMP_START:
			ChangeAnimMode( enANIM_JUMP_UP );
			break;
		case enANIM_JUMP_U_TURN:
			ChangeAnimMode( enANIM_JUMP_FALL );
			break;
		case enANIM_JUMP_STMP://着地.
				ChangeAnimMode( enANIM_IDLE );
			break;

		//攻撃.
		case enANIM_ATK:
			//攻撃終了.
			m_bRunning = false;
			m_enMove = enPM_IDLE;
			ChangeAnimMode( enANIM_IDLE );
			m_bAtkImpact = false;
			break;
		//ジャンプ攻撃.
		case enANIM_JUMP_ATK_SPN:
			ChangeAnimMode( enANIM_JUMP_ATK_FALL );
			break;
		case enANIM_JUMP_ATK_STMP:
			ChangeAnimMode( enANIM_JUMP_ATK_STND );
			break;
		case enANIM_JUMP_ATK_STND:
			m_enMove = enPM_IDLE;
			m_bLanding = false;
			m_bRunning = false;
			ChangeAnimMode( enANIM_IDLE );//アニメセット.
			break;

		//やられる.
		case enANIM_DEAD:
			ChangeAnimMode( enANIM_LOSE );//アニメセット.
			break;
		//やられる.
		case enANIM_LOSE:
			ChangeAnimMode( enANIM_LOSE );//アニメセット.
			break;
		}
	}
#else
	m_dAnimTimer += m_pModel->GetAnimSpeed();
	if( m_pModel->GetAnimPeriod( m_enAnimNo ) < m_dAnimTimer ){
		m_enAnimNo = (enAnimation)( (int)m_enAnimNo + 1 );
		if( m_enAnimNo >= enANIM_MAX ){
			m_enAnimNo = enANIM_IDLE;
		}
		ChangeAnimMode( m_enAnimNo );
	}
#endif
}

//============================================================
//	アニメーションモードを変更.
//============================================================
void clsPlayer::ChangeAnimMode( enAnimation anim )
{
	//飛ばさない.
	if( anim == enANIM_DEAD ||
		anim == enANIM_LOSE)
	{}
	//飛ばす.
	else if( m_bDead )
	{
		return;
	}

	m_dAnimTimer = 0.0;
	m_enAnimNo = anim;
	ChangeAnimSet( m_enAnimNo );//アニメセット.

}



//効果音再生.
void clsPlayer::PlaySe( enPlayerSound enSe )
{
	//再生する距離なら.
	int vol = ChangeVolumeDistance( m_fEarZ, m_vPos.z );
	if( vol ){
		m_ppSe[enSe]->GetMaxVolRate( vol );
		m_ppSe[enSe]->SeekToStart();
		m_ppSe[enSe]->SetVolume( vol );
		m_ppSe[enSe]->Play();
	}
	//再生しない距離なら.
	else{
		m_ppSe[enSe]->SetVolume( 0 );
	}
}

//コンストラクタ内にいます.
void clsPlayer::SetSe( HWND hWnd )
{
	clsSound::SOUND_DATA tmpSData[enPS_MAX] =
	{
		{ ALIAS_NAME_RUN,			FILE_PATH_RUN,			iVOL_RUN			},
		{ ALIAS_NAME_JUMP,			FILE_PATH_JUMP,			iVOL_JUMP			},
		{ ALIAS_NAME_ATK,			FILE_PATH_ATK,			iVOL_ATK			},
		{ ALIAS_NAME_JUMP_ATK_SPN,	FILE_PATH_JUMP_ATK_SPN,	iVOL_JUMP_ATK_SPN	},
		{ ALIAS_NAME_JUMP_ATK_STMP,	FILE_PATH_JUMP_ATK_STMP,iVOL_JUMP_ATK_STMP	},
		{ ALIAS_NAME_HEAD_HIT,		FILE_PATH_HEAD_HIT,		iVOL_HEAD_HIT		},
		{ ALIAS_NAME_DEAD,			FILE_PATH_DEAD,			iVOL_DEAD			},
	};

	m_ppSe = new clsSound*[enPS_MAX];
	for( int i=0; i<enPS_MAX; i++ ){
		m_ppSe[i] = new clsSound;
		//現音量初期化.
		m_ppSe[i]->SetVolume( 0 );
		//名前.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
		//作成.
		m_ppSe[i]->Open( tmpSData[i].sPath, cAliasName, hWnd );
		//最大音量設定.
		m_ppSe[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
	}
}



//背の高さを返す（天井のあたり判定用）.
float clsPlayer::GetHeight(){
	return COL_PLAYER_H;
}

//天井に頭を打ったら.
void clsPlayer::HeadHitToCeiling(){
	if( m_fJumpEnagy > 0.0f){
		m_fJumpEnagy = 0.0f;
	}
	else{
		m_fJumpEnagy -= fGRAVITY;
	}
	m_iJumpTimer = iJUNP_TIME_UP_TIME;
	m_bJumpAtkTopFlg = false;
	PlaySe( enPS_HEAD_HIT );
	m_pXInput->SetVibPowerL( usVIB_HEAD_POWER, iVIB_HEAD_TIME );//振動.
}

//通常攻撃開始.
void clsPlayer::Kick()
{
	m_enMove = enPM_ATK;
	ChangeAnimMode( enANIM_ATK );
	m_bAtkImpact = true;
	PlaySe( enPS_ATK );
}


//かかと落としフラグ.
bool clsPlayer::GetJumpAtkImpact()
{
	if( isJumpAtk() ){
		//落下中.
		if( m_fOldY > m_vPos.y ){
			return true;
		}
	}

	return false;
}



//メイン以外.
void clsPlayer::InitTitleScene()
{
	Spawn();
	ReSpawn();
	m_iTitleTimer = 0;
	m_bTitleSpnFlg = false;
	m_bDropout = false;
}


void clsPlayer::MoveTitleScene()
{
	const int iTITLE_KICK_START_TIMER = 60;
	const float fPLAYER_SPN_SPD = -(float)M_PI_4 / 8.0f;
	const D3DXVECTOR3 vINIT_POS = m_vPos;

	m_iTitleTimer ++;
	//アルビアの動き.
	if( m_iTitleTimer == iTITLE_KICK_START_TIMER ){
		//キック.
		Kick();
	}
	else if( m_iTitleTimer > iTITLE_KICK_START_TIMER ){
		//クルリと回る.
		//真北を向いている時は無視.
		if( !m_bTitleSpnFlg ){
			m_fYawTarget += fPLAYER_SPN_SPD;
			//向こうを向いたら.
			if( m_vRot.y < (float)M_PI &&
				m_vRot.y != 0.0f )
			{
				//回転が止まる.
				m_vRot.y =  (float)M_PI;
				m_bTitleSpnFlg = true;
			}
		}
	}
	Move( m_vPos.z );
	m_vPos = vINIT_POS;
}





void clsPlayer::InitOverScene()
{
	const float fOVER_INIT_ROT_Y = 0.75f;
	m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vRot.y = fOVER_INIT_ROT_Y;
	//倒れているモーション.
	ChangeAnimMode( enANIM_DEAD );
}


void clsPlayer::MoveOverScene()
{
	Animation();
}






//移動補助.
bool clsPlayer::InputUp( clsXInput* const xInput )
{
	bool bFlg = false;
	if( xInput->IsPressStay( XINPUT_UP ) ) bFlg = true;

	if( bFlg || GetAsyncKeyState( VK_UP ) & 0x8000 ){
		return true;
	}
	return false;
}

//移動補助.
bool clsPlayer::InputDown( clsXInput* const xInput )
{
	bool bFlg = false;
	if( xInput->IsPressStay( XINPUT_DOWN ) ) bFlg = true;

	if( bFlg || GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		return true;
	}
	return false;
}
//移動補助.
bool clsPlayer::InputLeft( clsXInput* const xInput )
{
	bool bFlg = false;
	if( xInput->IsPressStay( XINPUT_LEFT ) ) bFlg = true;

	if( bFlg || GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		return true;
	}
	return false;
}
//移動補助.
bool clsPlayer::InputRight( clsXInput* const xInput )
{
	bool bFlg = false;
	if( xInput->IsPressStay( XINPUT_RIGHT ) ) bFlg = true;

	if( bFlg || GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		return true;
	}
	return false;
}





