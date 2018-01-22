#include <math.h>
#include "Pendulum.h"


//音.
const char ALIAS_NAME[] = "PendSe";
const char FILE_PATH[] = "SE\\300Trap\\200Pendulum.wav";
const int VOL = 1000;


//===== あたり判定 =====//.
//===== Box =====//.
const float COL_PEND_BOX_X = 6.0f / 2.0f;
const float COL_PEND_BOX_Y = 4.0f / 2.0f;
const float COL_PEND_BOX_Z = 0.875f / 2.0f;
const float COL_PEND_BOX_CENTER_Y = 0.0f;
//===== あたり判定 終わり =====//.




//const float fPEND_SPN_POWER_MAX = 0.05f;
//const float fPEND_SPN_POWER_UP = 0.001f;
//const float fPEND_YAW_MAX = 0.0f;
const float fPEND_SPN_POWER_MAX = 0.025f;
const float fPEND_SPN_POWER_UP = 0.0005f;
const float fPEND_YAW_MAX = 0.0f;




//----- 透過 ------//
//透過境界線Z.
const float fALPHA_BORDER_Z = 0.5f;

//透過速度.
const float fALPHA_SPD = 1.0f / 64.0f;

//一番薄くしても.
const float fALPHA_LIMIT = 0.25f;


//高さ.
const float fHEIGHT_OFFSET = 12.0f;

clsPend::clsPend()
{
	m_pEffect = nullptr;
}

clsPend::~clsPend()
{
	m_pEffect = nullptr;

	if( m_pSe != NULL ){
		m_pSe->Stop();
		m_pSe->Close();
		delete m_pSe;
		m_pSe = NULL;
	}
}

void clsPend::Create( HWND hWnd, int iNo, int iPower  )
{
	Init();

	//あたり判定を設定( ありえない数に入れないと1フレーム目が 0, 0, 0 に来る ).
	float initPos = -100.0f;
	ColState.vPos = { initPos, initPos, initPos };
	//Box用.
	ColState.vRangeHalf.x = COL_PEND_BOX_X;
	ColState.vRangeHalf.y = COL_PEND_BOX_Y;
	ColState.vRangeHalf.z = COL_PEND_BOX_Z;
	ColState.fCenterY = COL_PEND_BOX_CENTER_Y;
	ColState.fCenterX = 0.0f;


	//音クラス.
	m_pSe = new clsSound;
	//現音量初期化.
	m_pSe->SetVolume( 0 );
	//名前.
	char cAliasName[STR_BUFF_MAX] = "";
	strcat_s( cAliasName, sizeof( cAliasName ), ALIAS_NAME );
	//番号.
	char cNumber[] = "  ";
	_itoa_s( iNo, cNumber, 10 );
	//名前と番号合体.
	strcat_s( cAliasName, sizeof( cAliasName ), cNumber );
	//作成.
	m_pSe->Open( FILE_PATH, cAliasName, hWnd );
	//最大音量設定.
	m_pSe->SetMaxVolume( VOL );

	//初期化フラグセット.
	if( iPower <= 0 ){
		iPower = 1;
	}
	 m_iInitFlg = iPower;

	if( m_pEffect == nullptr ){
		m_pEffect = clsEffects::GetInstance();
	}
}


void clsPend::Init()
{
	//エクセルに入っている数字が大きいと最初の揺れが遅れる.
	m_vRot.z = 0.0f;
	//奇数の時（最初は右に行く）.
	if( m_iInitFlg % 2 ){
		m_fMovePower = fPEND_SPN_POWER_MAX;
		m_bRigtFlg = true;
		m_vRot.z -= (float)( m_iInitFlg * 0.01 );
	}
	//偶数の時（最初は左に行く）.
	else{
		m_fMovePower = -fPEND_SPN_POWER_MAX;
		m_bRigtFlg = false;
		m_vRot.z += (float)( ( m_iInitFlg - 1 ) * 0.01 );
	}


	m_fAlpha = 1.0f;
	m_bAlpha = false;
	m_bAlphaChange = false;
}

//============================================================
//	振り子動き.
//============================================================
void clsPend::Move( float fEarZ )
{
	//振れる.
	m_vRot.z += m_fMovePower;

	//右に振れる.
	if( m_bRigtFlg ){
		//加速.
		m_fMovePower += fPEND_SPN_POWER_UP;
		//速くなりすぎない.
		if( m_fMovePower > fPEND_SPN_POWER_MAX ){
			m_fMovePower = fPEND_SPN_POWER_MAX;
			//反転.
			if( m_vRot.z > fPEND_YAW_MAX ){
				m_bRigtFlg = false;
				PlaySe( fEarZ );
				PlayEff();
			}
		}
	}
	//左に振れる.
	else{
		m_fMovePower -= fPEND_SPN_POWER_UP;
		if( m_fMovePower < -fPEND_SPN_POWER_MAX ){
			m_fMovePower = -fPEND_SPN_POWER_MAX;
			if( m_vRot.z < -fPEND_YAW_MAX ){
				m_bRigtFlg = true;
				PlaySe( fEarZ );
				PlayEff();
			}
		}
	}
	//透過値設定.
	SetAlpha();
}




//============================================================
//	ﾚﾝﾀﾞﾘﾝｸﾞ.
//============================================================
void clsPend::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye )
{
	if( m_pModel == NULL || m_pAnimCtrl == NULL ){
		return;
	}

	UpDateModel();

	m_pModel->Render(
		mView, mProj, vLight, vEye,
		D3DXVECTOR4( 1.0f, 1.0f, 1.0f, m_fAlpha ), m_bAlpha, m_pAnimCtrl );
}


//============================================================
//	透過値の設定.
//============================================================
void clsPend::SetAlphaFlg( float fPlayerZ )
{
	//透過中.
	if( m_bAlphaChange ){
		//透過フラグON.
		m_bAlpha = true;
		//プレイヤーが奥にいるなら.
		if( fPlayerZ > m_vPos.z + fALPHA_BORDER_Z ){
			return;
		}
		//実体化開始.
		m_bAlphaChange = false;
	}
	//透過中ではない.
	else{
		//プレイヤーが手前にいるなら.
		if( fPlayerZ < m_vPos.z + fALPHA_BORDER_Z ){
			return;
		}
		//透過開始.
		m_bAlphaChange = true;
	}
}

//============================================================
//	透過値の設定.
//============================================================
void clsPend::SetAlpha()
{
	//透過中.
	if( m_bAlphaChange ){
		m_fAlpha -= fALPHA_SPD;
		if( m_fAlpha < fALPHA_LIMIT ){
			m_fAlpha = fALPHA_LIMIT;
		}
	}
	//実体化中.
	else{
		m_fAlpha += fALPHA_SPD;
		if( m_fAlpha > 1.0f ){
			m_fAlpha = 1.0f;
			m_bAlpha = false;
		}
	}
}




//============================================================
//	ボーンpos.
//============================================================
void clsPend::GetBonePos( const char* cBoneName, D3DXVECTOR3 vBonePos )
{
	//ボーン座標.
	char* boneName =  (char*)cBoneName;
	if( m_pModel->GetPosFromBone( boneName, &vBonePos ) ){
		SetColPos( &vBonePos );
	}
}



//============================================================
//	効果音.
//============================================================
void clsPend::PlaySe( float fEarZ )
{
	//再生する距離なら.
	int vol = ChangeVolumeDistance( fEarZ, m_vPos.z );
	if( vol ){
		m_pSe->GetMaxVolRate( vol );
		m_pSe->SeekToStart();
		m_pSe->SetVolume( vol );
		m_pSe->Play();
	}
	//再生しない距離なら.
	else{
		m_pSe->SetVolume( 0 );
	}
}

//エフェクト再生.
void clsPend::PlayEff()
{
	if( !m_pEffect->PlayCheck( m_ehFire ) ){
		D3DXVECTOR3 vEffPos = m_vPos;
		vEffPos.y -= fHEIGHT_OFFSET;

		D3DXVECTOR3 vScale = { 0.5f, 0.5f, 0.5f };

		m_ehFire = m_pEffect->Play( clsEffects::enEfcType_PEND_FIRE, vEffPos );
		m_pEffect->SetScale( m_ehFire, vScale );
		m_pEffect->SetSpd( m_ehFire, 1.125f );
	}
}
