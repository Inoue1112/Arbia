#include "Sp2DMgrResult.h"


#define sFILE_PATH_BLACK "Data\\Image\\Black.png"

#define sFILE_PATH_STEAL "Data\\Image\\ResultSteal.png"

#define sFILE_PATH_RESULT_TIME		"Data\\Image\\ResultTime.png"//時間.
#define sFILE_PATH_RESULT_DISCOVER	"Data\\Image\\ResultDisc.png"//発見数.
#define sFILE_PATH_RESULT_LIVES		"Data\\Image\\ResultLives.png"//残機.
#define sFILE_PATH_RESULT_PARFECT	"Data\\Image\\ResultParf.png"//パーフェクト.



//----- 音 -----//.
//当てる.
#define ALIAS_NAME_HIT "SP2DMgrResultBoxHit"
#define  FILE_PATH_HIT "SE\\600Result\\500BoxHit.wav"
const	  int iVOL_HIT = 1000;

//開く.
#define ALIAS_NAME_OPEN "SP2DMgrResultBoxOpen"
#define  FILE_PATH_OPEN "SE\\600Result\\510BoxOpen.wav"
const	  int iVOL_OPEN = 1000;


//盗んだ成果.
#define ALIAS_NAME_STEAL "SP2DMgrResultSteal"
#define  FILE_PATH_STEAL "SE\\600Result\\510BoxOpen.wav"
const	  int iVOL_STEAL = 1000;

//各文字.
#define ALIAS_NAME_OTHER "SP2DMgrResultOther"
#define  FILE_PATH_OTHER "SE\\600Result\\300OtherMove.wav"
const	  int iVOL_OTHER = 1000;

//最後の宝石.
#define ALIAS_NAME_LAST_JEWEL "SP2DMgrResultLastJewel"
#define  FILE_PATH_LAST_JEWEL "SE\\600Result\\120LastJewel.wav"
const	  int iVOL_LAST_JEWEL = 750;



//半暗転.
const float fBLACK_ALPHA_RIMIT = 0.5f;//最終的.
const float fBLACK_ALPHA_ADD = fBLACK_ALPHA_RIMIT / 120.0f;//どれくらいずつ変わる?.


//上の文字の最終座標.
const D3DXVECTOR3 vSTEAL_POS = { 450.0f, 100.0f, 0.0f };
//各文字の最終座標.
const float vOTHER_POS_Y = 200.0f;
const float vOTHER_POS_PARFECT_Y = vOTHER_POS_Y + 225.0f;
const D3DXVECTOR3 vRESULT_POS[cSTEAL_RESULT_MAX] = 
{
	{ 90.0f,	vOTHER_POS_Y,			0.0f },
	{ 520.0f,	vOTHER_POS_Y,			0.0f },
	{ 980.0f,	vOTHER_POS_Y,			0.0f },
	{ 450.0f,	vOTHER_POS_PARFECT_Y,	0.0f },
};
//宝石最終地点.
const float vJEWEL_OFFSET_Y = 125.0f;
const float vJEWEL_OFFSET_FIN_Y = 225.0f;
const float vJEWEL_OFFSET_FIN_X = 100.0f;
const D3DXVECTOR3 vJEWEL_POS[sJEWEL_NUM_MAX] = 
{								   	
	{ 110.0f, 325.0f, 0.0f },
	{ vJEWEL_POS[0] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },
	{ vJEWEL_POS[1] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },

	{ 540.0f, 325.0f, 0.0f },
	{ vJEWEL_POS[3] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },
	{ vJEWEL_POS[4] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },

	{ 950.0f, 325.0f, 0.0f },
	{ vJEWEL_POS[6] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },
	{ vJEWEL_POS[7] + D3DXVECTOR3{ vJEWEL_OFFSET_FIN_X,	0.0f,	0.0f } },

	{ vJEWEL_POS[4]  + D3DXVECTOR3{ 0.0f,	vJEWEL_OFFSET_FIN_Y,0.0f } },
};

//各文字の初期座標.
const float fSTEAL_START_POS_Y = -150.0f;
const float fRESULT_START_POS_X = -250.0f;
const float fRESULT_START_POS_X_PARFECT = -400.0f;
//各文字の移動速度.
const float fSTEAL_SPD = 4.0f;
const float fRESULT_SPD = 24.0f;

//宝石移動.
const float fJEWEL_MOVE_TIME = 25.0f;//何フレームで完了する?.
const float fJEWEL_UP = -64.0f;		//初期上昇力.
const float fJEWER_ACC_RATE = 1.75f;	//加速度の倍率.

//MulDisp用.
const float fMUL_DISP_STEAL = 0.5f;	//1/2.
const float fMUL_DISP_OTHER = 0.25f;	//1/4.

//宝石出すかの判断基準( 左から順にゆるい ).
const int iJUDGE_TIME[cJEWEL_NUM_PARTS] = { 1000, 730, 300 };//時間( これより早くゴールする ) (10分, 7分30秒, 3分, 0秒).
const int iJUDGE_DISC[cJEWEL_NUM_PARTS] = { 8, 3, 0 };		//発見数( この数までなら許容できる ).
const int iJUDGE_LIVES[cJEWEL_NUM_PARTS] = { 1, 4, 8 };	//残機( この数までなら許容できる ).

//宝石カウントの初期化数字.
const int iJEWEL_CNT_INIT = -1;

//パーフェクト文字サイズ係数.
const float fPARFECT_SIZE_RATE = 1.5f;


clsSp2dMgrReslt::clsSp2dMgrReslt()
{
	m_enMode = enM_BLACK;
	m_vJewelAppPos = { 0.0f, 0.0f, 0.0f };
	m_iJewelCnt = 0;


	for( int i=0; i<enS_MAX; i++ ){
		m_pSeBox[i] = NULL;
	}
}

clsSp2dMgrReslt::~clsSp2dMgrReslt()
{
	Release();
}



void clsSp2dMgrReslt::Init( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	//半暗転.
	m_smpBlack = make_unique<clsSprite2D>();
	m_smpBlack->Init( pDevice11, pContext11, sFILE_PATH_BLACK );
	m_smpBlack->UpDateSpriteSs();

	//盗んだ成果.
	m_smpSteal = make_unique<clsSprite2D>();	
	m_smpSteal->Init( pDevice11, pContext11, sFILE_PATH_STEAL );
//	//サイズを変更する.
	m_smpSteal->MulDisp( fMUL_DISP_STEAL );
	m_smpSteal->UpDateSpriteSs();

	//項目.
	LPSTR tmpFilePath[] = 
	{
		sFILE_PATH_RESULT_TIME, sFILE_PATH_RESULT_DISCOVER, sFILE_PATH_RESULT_LIVES, sFILE_PATH_RESULT_PARFECT
	};
	for( char i=0; i<cSTEAL_RESULT_MAX; i++ ){
		m_smpResult[i] = make_unique<clsSprite2D>();
		m_smpResult[i]->Init( pDevice11, pContext11, tmpFilePath[i] );
		m_smpResult[i]->MulDisp( fMUL_DISP_OTHER );
		m_smpResult[i]->UpDateSpriteSs();
	}
	m_smpResult[ cSTEAL_RESULT_MAX - 1 ]->MulDisp( fMUL_DISP_OTHER * fPARFECT_SIZE_RATE );
	m_smpResult[ cSTEAL_RESULT_MAX - 1 ]->UpDateSpriteSs();


	//宝石.
	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i] = make_unique<clsJewerSet>();
		m_smpJewel[i]->Create( hWnd, pDevice11, pContext11, i );
	}

	m_smpPushTxt = make_unique<clsPushTxt>();	//.
	m_smpPushTxt->Init( pDevice11, pContext11 );


	//効果音.
	SetSe( hWnd );

}


void clsSp2dMgrReslt::InitSetPos()
{
	m_smpBlack->InitSetPos();
	m_smpSteal->InitSetPos();
	for( char i=0; i<cSTEAL_RESULT_MAX; i++ ){
		m_smpResult[i]->InitSetPos();
	}

	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i]->InitSetPos();
	}

	m_smpPushTxt->InitSetPos();
}


//宝石出すかの判断.
void clsSp2dMgrReslt::JewelJudge()
{
	//宝石のNo.
	int i = 0;

	//時間.
	for( char j=0; j<cJEWEL_NUM_PARTS; i++, j++ ){
		if( m_UiState.iTime < iJUDGE_TIME[j] ){
			m_smpJewel[i]->SetGetFlg( true );
		}
		else{
			m_smpJewel[i]->SetGetFlg( false );
		}
	}
	//発見.
	for( char j=0; j<cJEWEL_NUM_PARTS; i++, j++ ){
		if( m_UiState.iDisc <= iJUDGE_DISC[j] ){
			m_smpJewel[i]->SetGetFlg( true );
		}
		else{
			m_smpJewel[i]->SetGetFlg( false );
		}
	}
	//残機.
	//jは各項目の宝石の数字.
	for( char j=0; j<cJEWEL_NUM_PARTS; i++, j++ ){
		if( m_UiState.iLives >= iJUDGE_LIVES[j] ){
			m_smpJewel[i]->SetGetFlg( true );
		}
		else{
			m_smpJewel[i]->SetGetFlg( false );
		}
	}

	//ラスト.
	bool bFlg = true;
	for( i=0; i<sJEWEL_NUM_MAX - 1; i++ ){
		bFlg = m_smpJewel[i]->GetGetFlg();
		if( !bFlg ){
			break;
		}
	}
	m_smpJewel[sJEWEL_NUM_MAX - 1]->SetGetFlg( bFlg );

}

void clsSp2dMgrReslt::StartSet()
{
	m_enMode = enM_BLACK;

	m_bCanBgmRePlay = false;

	m_smpBlack->SetPos( 0.0f, 0.0f );
	m_smpBlack->SetAlpha( 0.0f );

	FinalPos( true );

	m_smpSteal->SetPosY( fSTEAL_START_POS_Y );

	for( char i=0; i<cSTEAL_RESULT_MAX; i++ ){
		m_smpResult[i]->SetPos( vRESULT_POS[i] );
		m_smpResult[i]->SetPosX( fRESULT_START_POS_X );
	}
	m_smpResult[ cSTEAL_RESULT_MAX - 1 ]->SetPosX( fRESULT_START_POS_X_PARFECT );

	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i]->ReSet();
		m_smpJewel[i]->SetPos( m_vJewelAppPos );
		m_smpJewel[i]->SetScale( 0.0f );
	}
	//宝石情報による宝石フラグ確定.
	JewelJudge();
	//宝石情報初期化( 次回の為 ).
	m_UiState.Init();

	m_iJewelCnt = iJEWEL_CNT_INIT;

	m_enParFlg = enPM_SOUND;

	m_pSeBox[enS_HIT]->SeekPlay();
}


void clsSp2dMgrReslt::UpDate()
{
	switch( m_enMode )
	{
	case enM_BLACK:
		BlackMove();
		break;

	case enM_STEAL:
		StealMove();
		break;

	case enM_OTHER:
		OtherMove();
		break;

	case enM_JEWEL:
		JewelMove();
		break;

	case enM_PARF:
		ParfeMove();
		break;

	case enM_FINAL_SET:
		FinalPos();
		NextMove();
		break;

	case enM_END:
		if( m_pSeBox[enS_LAST_JEWEL]->IsStopped() ){
			m_bCanBgmRePlay = true;
		}
		break;

	default:
		m_enMode = enM_FINAL_SET;
		break;
	}
}

//==================================================
//	動き.
//==================================================
void clsSp2dMgrReslt::BlackMove()
{
	//暗くなる.
	if( m_smpBlack->GetAlpha() <= fBLACK_ALPHA_RIMIT ){
		m_smpBlack->AddAlpha( fBLACK_ALPHA_ADD );
	}
	//暗くなった後.
	else{
		NextMove();
		m_pSeBox[enS_OPEN]->SeekPlay();
	}
}

void clsSp2dMgrReslt::StealMove()
{
	//結果おろす.
	if( m_smpSteal->GetPos().y <= vSTEAL_POS.y ){
		m_smpSteal->AddPosY( fSTEAL_SPD );
	}
	//おろした後.
	else{
		m_smpSteal->SetPos( vSTEAL_POS );
		m_pSeBox[enS_OTHER]->SeekToStart();
		m_pSeBox[enS_OTHER]->Play();
		//確認用フラグ.
		for( char i=0; i<cSTEAL_RESULT_MAX - 1; i++ ){
			m_bOtherMove[i] = false;
		}
		NextMove();
	}
}

void clsSp2dMgrReslt::OtherMove()
{
	for( char i=0; i<cSTEAL_RESULT_MAX - 1; i++ ){
		if( !m_bOtherMove[i] ){
			//文字を右へ.
			if( m_smpResult[i]->GetPos().x < vRESULT_POS[i].x ){
				m_smpResult[i]->AddPosX( fRESULT_SPD );
			}
			//動かした後.
			else{
				m_smpResult[i]->SetPos( vRESULT_POS[i] );
				//最後のやつでは鳴らさない.
				if( i < cSTEAL_RESULT_MAX - 1 - 1 ){
//					m_pSeBox[enS_OTHER]->Stop();
					m_pSeBox[enS_OTHER]->SeekToStart();
					m_pSeBox[enS_OTHER]->Play();
				}
				m_bOtherMove[i] = true;
			}
		}
	}

	//全て終わったら.
	bool bEndFlg = true;

	for( char i=0; i<cSTEAL_RESULT_MAX - 1; i++ ){
		if( !m_bOtherMove[i] ){
			bEndFlg = false;
			break;
		}
	}

	if( bEndFlg ){
		NextJewerSet();
		//効果音.
		m_smpJewel[m_iJewelCnt]->PlaySe(  );
		NextMove();
	}
}

void clsSp2dMgrReslt::JewelMove()
{
	////確認用フラグ : m_iJewelCnt.

	//動かさないなら.
	if( !m_smpJewel[m_iJewelCnt]->GetGetFlg() ){
		NextJewerSet();
	}

	//動き.
	if( m_smpJewel[m_iJewelCnt]->GetGetFlg() ){
		m_smpJewel[m_iJewelCnt]->Move();
	}

	//行き過ぎない.
	if( vJEWEL_POS[m_iJewelCnt].x > m_vJewelAppPos.x ){
		if( m_smpJewel[m_iJewelCnt]->GetPos().x > vJEWEL_POS[m_iJewelCnt].x ){
			m_smpJewel[m_iJewelCnt]->SetPosX( vJEWEL_POS[m_iJewelCnt].x );
		}
	}
	else if( vJEWEL_POS[m_iJewelCnt].x < m_vJewelAppPos.x ){
		if( m_smpJewel[m_iJewelCnt]->GetPos().x < vJEWEL_POS[m_iJewelCnt].x ){
			m_smpJewel[m_iJewelCnt]->SetPosX( vJEWEL_POS[m_iJewelCnt].x );
		}
	}

	//動きの終わり.
	if( m_smpJewel[m_iJewelCnt]->GetPos().y > vJEWEL_POS[m_iJewelCnt].y &&
		m_smpJewel[m_iJewelCnt]->GetDown() )
	{
		m_smpJewel[m_iJewelCnt]->SetPos( vJEWEL_POS[m_iJewelCnt] );

		//光る効果音.
//		m_smpJewel[m_iJewelCnt]->PlaySe( clsJewerSet::enS_SHINE );
		
		NextJewerSet();
		if( m_iJewelCnt < sJEWEL_NUM_MAX - 1 ){
			//音鳴らすかの判断.
			bool bSound = false;
			for( short i=m_iJewelCnt; i<sJEWEL_NUM_MAX - 1; i++ ){
				if( m_smpJewel[i]->GetGetFlg() ){
					bSound = true;
					break;
				}
			}
			//次に音鳴らす奴があるなら.
			if( bSound ){ 
				//効果音.
				m_smpJewel[m_iJewelCnt - 1]->PlaySe(  );
			}
		}
	}


	//全て終わったら.
	bool bEndFlg = false;

	if( m_iJewelCnt >= sJEWEL_NUM_MAX - 1 ){
		bEndFlg = true;
	}

	if( bEndFlg ){
		NextMove();
	}
}

void clsSp2dMgrReslt::NextJewerSet()
{
	m_iJewelCnt ++;

	//範囲外では無ければ.
	if( m_iJewelCnt < sJEWEL_NUM_MAX ){
		//座標セット & 初動.
		D3DXVECTOR3 vMove = { 0.0f, 10.0f, 0.0f };
		float fAcc = 0.0f;	

		//初動.
		MakeParbPower( vMove, fAcc, m_vJewelAppPos, vJEWEL_POS[m_iJewelCnt] );

		m_smpJewel[m_iJewelCnt]->SetPos( m_vJewelAppPos );
		m_smpJewel[m_iJewelCnt]->SetMove( vMove, fAcc );

		
		
	}
}

//パーフェクト.
void clsSp2dMgrReslt::ParfeMove()
{
	//パーフェクトじゃないなら飛ばす.
	if( !m_smpJewel[sJEWEL_NUM_MAX - 1]->GetGetFlg() ){
		m_enParFlg = enPM_END;
	}

	//音.
	if( m_enParFlg == enPM_SOUND ){
		m_pSeBox[enS_OTHER]->Stop();
		m_pSeBox[enS_OTHER]->SeekToStart();
		m_pSeBox[enS_OTHER]->Play();	
		m_enParFlg = enPM_PARFECT;
	}
	//文字.
	else if( m_enParFlg == enPM_PARFECT ){
		//文字を右へ.
		if( m_smpResult[cSTEAL_RESULT_MAX - 1]->GetPos().x < vRESULT_POS[cSTEAL_RESULT_MAX - 1].x ){
			m_smpResult[cSTEAL_RESULT_MAX - 1]->AddPosX( fRESULT_SPD );
		}
		//動かした後.
		else{
			m_smpResult[cSTEAL_RESULT_MAX - 1]->SetPos( vRESULT_POS[cSTEAL_RESULT_MAX - 1] );
			m_smpJewel[m_iJewelCnt]->PlaySe(  );
			m_enParFlg = enPM_JEWEL;
		}
	}

	//宝石.
	else if( m_enParFlg == enPM_JEWEL ){
		//動き.
		m_smpJewel[m_iJewelCnt]->Move();

		//行き過ぎない.
		if( vJEWEL_POS[m_iJewelCnt].x > m_vJewelAppPos.x ){
			if( m_smpJewel[m_iJewelCnt]->GetPos().x > vJEWEL_POS[m_iJewelCnt].x ){
				m_smpJewel[m_iJewelCnt]->SetPosX( vJEWEL_POS[m_iJewelCnt].x );
			}
		}
		else if( vJEWEL_POS[m_iJewelCnt].x < m_vJewelAppPos.x ){
			if( m_smpJewel[m_iJewelCnt]->GetPos().x < vJEWEL_POS[m_iJewelCnt].x ){
				m_smpJewel[m_iJewelCnt]->SetPosX( vJEWEL_POS[m_iJewelCnt].x );
			}
		}

		//動きの終わり.
		if( m_smpJewel[m_iJewelCnt]->GetPos().y > vJEWEL_POS[m_iJewelCnt].y &&
			m_smpJewel[m_iJewelCnt]->GetDown() )
		{
			m_smpJewel[m_iJewelCnt]->SetPos( vJEWEL_POS[m_iJewelCnt] );
//			m_smpJewel[m_iJewelCnt]->PlaySe( clsJewerSet::enS_SHINE );
			m_enParFlg = enPM_END;
		}
	}

	//全て終わったら.
	if( m_enParFlg >= enPM_END ){
		NextMove();
	}
}

void clsSp2dMgrReslt::FinalPos( bool bInit )
{

	m_smpSteal->SetPos( vSTEAL_POS );
	char i = 0;

	//文字.
	for( i=0; i<cSTEAL_RESULT_MAX - 1; i++ ){
		m_smpResult[i]->SetPos( vRESULT_POS[i] );
	}
	//パーフェクト?.
	if( m_smpJewel[sJEWEL_NUM_MAX - 1]->GetGetFlg() ){
		m_smpResult[cSTEAL_RESULT_MAX - 1]->SetPos( vRESULT_POS[cSTEAL_RESULT_MAX - 1] );
		if( !bInit ){
			m_pSeBox[enS_LAST_JEWEL]->SeekToStart();
			m_pSeBox[enS_LAST_JEWEL]->Play();
		}
	}

	//宝石.
	for( i=0; i<sJEWEL_NUM_MAX; i++ ){
		if( m_smpJewel[i]->GetGetFlg() ){
			m_smpJewel[i]->SetPos( vJEWEL_POS[i] );
			m_smpJewel[i]->SetScale( 1.0f);
		}
	}

	m_smpPushTxt->InitSetPos();
}



void clsSp2dMgrReslt::Render()
{
	m_smpBlack->Render();

	m_smpSteal->Render();

	for( char i=cSTEAL_RESULT_MAX-1; i>=0; i-- ){
		m_smpResult[i]->Render();
	}

	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i]->Render();
	}

	m_smpPushTxt->Render();
}




void clsSp2dMgrReslt::JewelSetReSet()
{
	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i]->ReSet();
	}
	

}


//宝石のMoveとAccを求める. Parad -> parabola : 放物線.
void clsSp2dMgrReslt::MakeParbPower(
	D3DXVECTOR3 &vMove, float &fAcc,
	D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos )
{
	//Move.
	D3DXVECTOR3 vDis = vEndPos - vStartPos;//差.
	vMove.x = vDis.x / fJEWEL_MOVE_TIME;
	vMove.y = fJEWEL_UP;

	//Acc.
	fAcc = -fJEWEL_UP / fJEWEL_MOVE_TIME * fJEWER_ACC_RATE;
}



void clsSp2dMgrReslt::Release()
{
	if( m_pSeBox != NULL ){
		for( int i=0; i<enS_MAX; i++ ){
			m_pSeBox[i]->Stop();
			m_pSeBox[i]->Close();
			delete m_pSeBox[i];
			m_pSeBox[i] = NULL;
		}
	}

	m_smpPushTxt.reset();

	for( char i=0; i<cSTEAL_RESULT_MAX; i++ ){
		m_smpResult[i].reset();
	}

	for( char i=0; i<sJEWEL_NUM_MAX; i++ ){
		m_smpJewel[i].reset();
	}


	m_smpSteal.reset();

	m_smpBlack.reset();
}



void clsSp2dMgrReslt::SetSe( HWND hWnd )
{
	clsSound::SOUND_DATA tmpSData[enS_MAX] =
	{
		{ ALIAS_NAME_HIT,			FILE_PATH_HIT,			iVOL_HIT			},
		{ ALIAS_NAME_OPEN,			FILE_PATH_OPEN,			iVOL_OPEN			},
		{ ALIAS_NAME_STEAL,			FILE_PATH_STEAL,		iVOL_STEAL			},
		{ ALIAS_NAME_OTHER,			FILE_PATH_OTHER,		iVOL_OTHER			},
		{ ALIAS_NAME_LAST_JEWEL,	FILE_PATH_LAST_JEWEL,	iVOL_LAST_JEWEL		},
	};

	for( int i=0; i<enS_MAX; i++ ){
		m_pSeBox[i] = new clsSound;
		//現音量初期化.
		m_pSeBox[i]->SetVolume( tmpSData[i].iMaxVolume );
		//名前.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
		//作成.
		m_pSeBox[i]->Open( tmpSData[i].sPath, cAliasName, hWnd );
		//最大音量設定.
		m_pSeBox[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
	}
}

