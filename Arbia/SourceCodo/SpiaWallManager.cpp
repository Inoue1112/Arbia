#include "Resource.h"
#include "SpiaWallManager.h"

//音.
//振動.
#define ALIAS_NAME_VIB "SpiaWallVib"
#define FILE_PATH_VIB "SE\\300Trap\\100SpiaVib.wav"
const int iVOL_VIB = 1000;

//上がる.
#define ALIAS_NAME_UP "SpiaWallTop"
#define FILE_PATH_UP "SE\\300Trap\\111SpiaTopwall.wav"					
const int iVOL_UP = 1000;

//下がる.
#define ALIAS_NAME_DOWN "SpiaWallDown"
#define FILE_PATH_DOWN "SE\\300Trap\\130SpiaClose.wav"					
const int iVOL_DOWN = 1000;


//稼働タイミング.
const int iTIME_MUTUAL_FIRST_UP = 0;	//がたがた開始.
const int iTIME_MUTUAL_FIRST_DOWN = 150;	//引っ込み開始.
const int iTIME_MUTUAL_SECOND_UP = 120;	//がたがた開始.
const int iTIME_MUTUAL_SECOND_DOWN = iTIME_MUTUAL_SECOND_UP + ( iTIME_MUTUAL_FIRST_DOWN - iTIME_MUTUAL_FIRST_UP );//引っ込み開始.
const int iTIME_MUTUAL_LOOP = iTIME_MUTUAL_SECOND_DOWN;		//ループ.	





//壁の数.
const char cWALL_MAX = 2;
//槍の数(片側).
const int iSPIA_MAX = 6;

//槍のがたがたならびずらし量.
const float fFLOOR_X_OFFSET = 0.125f;
//槍のがたがたならびずらし量.
const float fFLOOR_Y_OFFSET = 1.0f;
const float fFLOOR_Y_OFFSET_FIRST = 0.5f;


//ステージ幅.
const float fSTAGE_WIDHT = 10.0f;
//中央からの距離.
const float fWALL_SIDE_OFFSET = fSTAGE_WIDHT / 2.0f;
////壁判定の移動量.
const float fWALL_OFFSET = -0.25f;


//壁の高さ.
const float fWALL_HEIGHT = (float)iSPIA_MAX * fFLOOR_Y_OFFSET - fFLOOR_Y_OFFSET_FIRST;



clsSpiaWallMgr::clsSpiaWallMgr()
{
}

clsSpiaWallMgr::~clsSpiaWallMgr()
{
	Release();
}



void clsSpiaWallMgr::CreateSpia( HWND hWnd, int iNo, int iMoveMode )
{
	m_enMoveMode = (enMode)iMoveMode;

	//----- モデル -----//
	//槍.
	m_iSpiaMax = iSPIA_MAX;
	//左.
	m_ppSpiaL = new clsSpiaWall*[m_iSpiaMax];
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpiaL[i] = new clsSpiaWall;
		m_ppSpiaL[i]->Create( false, i );
		m_ppSpiaL[i]->AttachModel( 
			clsResource::GetInstance()->GetStaticModels( 
				clsResource::enST_MODEL_SPIA_OF_WALL ) );
		m_ppSpiaL[i]->SetRotationZ( -(float)M_PI_2 );
	}
	//右.
	m_ppSpiaR = new clsSpiaWall*[m_iSpiaMax];
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpiaR[i] = new clsSpiaWall;
		m_ppSpiaR[i]->Create( true, i );
		m_ppSpiaR[i]->AttachModel( 
			clsResource::GetInstance()->GetStaticModels( 
				clsResource::enST_MODEL_SPIA_OF_WALL ) );
		m_ppSpiaR[i]->SetRotationZ( (float)M_PI_2 );
	}


	//槍壁.
	for( char i=0; i<cWALL_MAX; i++ ){
		m_pSpiaWall[i] = new clsCharaStatic;
		m_pSpiaWall[i]->AttachModel(
			clsResource::GetInstance()->GetStaticModels( 
				clsResource::enST_MODEL_SPIA_WALL ) );
	}
	//----- SE -----//
	//サウンド構造体.
	clsSound::SOUND_DATA tmpSData[clsSpiaWall::enSOUND_MAX] =
	{
		{ ALIAS_NAME_VIB,	FILE_PATH_VIB,	iVOL_VIB	},
		{ ALIAS_NAME_UP,	FILE_PATH_UP,	iVOL_UP		},
		{ ALIAS_NAME_DOWN,	FILE_PATH_DOWN,	iVOL_DOWN	},
	};

	//サウンドクラス作成.
	m_ppSe = new clsSound*[clsSpiaWall::enSOUND_MAX];
	for( int i=0; i<clsSpiaWall::enSOUND_MAX; i++ ){
		m_ppSe[i] = new clsSound;
		//現音量初期化.
		m_ppSe[i]->SetVolume( 0 );
		//名前.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
		//番号.
		char cNumber[] = "  ";
		_itoa_s( iNo, cNumber, 10 );
		//名前と番号合体.
		strcat_s( cAliasName, sizeof( cAliasName ), cNumber );
		//作成.
		m_ppSe[i]->Open( tmpSData[i].sPath, cAliasName, hWnd );
		//最大音量設定.
		m_ppSe[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
	}
	//----- SE 終了 -----//



	//----- モデル 終了 -----//
	Init();
}


void clsSpiaWallMgr::Init()
{
	m_iTimer = 0;
	//槍.
	for( int i=0; i<m_iSpiaMax; i++ ){
		float fOffset = fFLOOR_X_OFFSET;
		if( i % 2 != 0 ){
			//出てるやつ.
			fOffset *= -1.0f;
		}

		//左側.
		m_ppSpiaL[i]->SetPosition( 
		D3DXVECTOR3( 
			GetPositionX() - fWALL_SIDE_OFFSET + fOffset, 
			GetPositionY() + (float)i * fFLOOR_Y_OFFSET + fFLOOR_Y_OFFSET_FIRST,
			GetPositionZ() ) );
		m_ppSpiaL[i]->Init( m_ppSpiaL[i]->GetPosition(), false );

		//右側.
//		fOffset *= -1.0f;
		m_ppSpiaR[i]->SetPosition( 
		D3DXVECTOR3( 
			GetPositionX() + fWALL_SIDE_OFFSET + fOffset,
			GetPositionY() + (float)i * fFLOOR_Y_OFFSET + fFLOOR_Y_OFFSET_FIRST,
			GetPositionZ() ) );
		m_ppSpiaR[i]->Init( m_ppSpiaR[i]->GetPosition(), true );
	}
}


void clsSpiaWallMgr::Release()
{
	//音.
	if( m_ppSe != NULL ){
		for( int i=0; i<clsSpiaWall::enSOUND_MAX; i++ ){
			m_ppSe[i]->Stop();
			m_ppSe[i]->Close();
			delete m_ppSe[i];
			m_ppSe[i] = NULL;
		}
		delete[] m_ppSe;
		m_ppSe = NULL;
	}
	//オブジェクト.
	if( m_pSpiaWall != NULL ){
		for( char i=0; i<cWALL_MAX; i++ ){
			m_pSpiaWall[i]->DetatchModel();
			delete m_pSpiaWall[i];
			m_pSpiaWall[i] = NULL;
		}
	}

	if( m_ppSpiaL != NULL ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpiaL[i]->DetatchModel();
			delete m_ppSpiaL[i];
			m_ppSpiaL[i] = NULL;
		}
		delete[] m_ppSpiaL;
		m_ppSpiaL = NULL;
	}
	if( m_ppSpiaR != NULL ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpiaR[i]->DetatchModel();
			delete m_ppSpiaR[i];
			m_ppSpiaR[i] = NULL;
		}
		delete[] m_ppSpiaR;
		m_ppSpiaR = NULL;
		m_iSpiaMax = 0;
	}
}


void clsSpiaWallMgr::Move( float fEarZ )
{
	m_fEarZ = fEarZ;
	//動きのセット.
	switch( m_enMoveMode )
	{
	case enMODE_MUTUAL_L:
		m_iTimer ++;
		MoveMutualL();
		break;
	case enMODE_MUTUAL_R:
		m_iTimer ++;
		MoveMutualR();
		break;
	case enMODE_CLOSE_L:
		MoveCloseL();
		break;
	case enMODE_CLOSE_R:
		MoveCloseR();
		break;
//	default:
//		m_enMoveMode = enMODE_MUTUAL_L;
		break;
	}



	//動き.
	//飛び出し効果音用.
	bool bSpiaUpSound = false;
	for( int i=0; i<m_iSpiaMax; i++ ){
		bSpiaUpSound = m_ppSpiaL[i]->Move();
	}
	//飛び出す音.
	if( bSpiaUpSound ){
		PlaySe( clsSpiaWall::enSOUND_UP );
	}

	for( int i=0; i<m_iSpiaMax; i++ ){
		bSpiaUpSound = m_ppSpiaR[i]->Move();
	}
	//飛び出す音.
	if( bSpiaUpSound ){
		PlaySe( clsSpiaWall::enSOUND_UP );
	}


	//槍壁座標.
	m_pSpiaWall[0]->SetPosition( m_ppSpiaL[0]->GetPosition() );
	m_pSpiaWall[0]->AddPositionX( -fSTAGE_WIDHT - fWALL_OFFSET );
	m_pSpiaWall[0]->AddPositionY( fWALL_HEIGHT );
	m_pSpiaWall[1]->SetPosition( m_ppSpiaR[0]->GetPosition() );
	m_pSpiaWall[1]->AddPositionX( fWALL_OFFSET );
	m_pSpiaWall[1]->AddPositionY( fWALL_HEIGHT );
}

//交互(左が先).
void clsSpiaWallMgr::MoveMutualL()
{
	//最初飛び出し.
	if( m_iTimer == iTIME_MUTUAL_FIRST_UP ){
		GoRight();
	}
	//最初のやつ引っ込み.
	if( m_iTimer == iTIME_MUTUAL_FIRST_DOWN ){
		GoDown( 0 );
	}
	//次のやつ飛び出し.
	if( m_iTimer == iTIME_MUTUAL_SECOND_UP ){
		GoLeft();
	}
	//次のやつ引っ込み.
	if( m_iTimer == iTIME_MUTUAL_SECOND_DOWN ){
		GoDown( 1 );
	}
	//タイマーループ.
	if( m_iTimer == iTIME_MUTUAL_LOOP ){
		m_iTimer = -1;
	}
}
//交互(右が先).
void clsSpiaWallMgr::MoveMutualR()
{
	//最初飛び出し.
	if( m_iTimer == iTIME_MUTUAL_FIRST_UP ){
		GoLeft();
	}
	//最初のやつ引っ込み.
	if( m_iTimer == iTIME_MUTUAL_FIRST_DOWN ){
		GoDown( 1 );
	}
	//次のやつ飛び出し.
	if( m_iTimer == iTIME_MUTUAL_SECOND_UP ){
		GoRight();
	}
	//次のやつ引っ込み.
	if( m_iTimer == iTIME_MUTUAL_SECOND_DOWN ){
		GoDown( 0 );
	}
	//タイマーループ.
	if( m_iTimer == iTIME_MUTUAL_LOOP ){
		m_iTimer = -1;
	}
}
//閉じる(左が先).
void clsSpiaWallMgr::MoveCloseL()
{

}
//閉じる(右が先).
void clsSpiaWallMgr::MoveCloseR()
{

}





void clsSpiaWallMgr::SetPosition( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;

	//子分の座標.
	for( int i=0; i<m_iSpiaMax; i++ ){
		float fOffset = fFLOOR_X_OFFSET;
		if( i % 2 != 0 ){
			//出てるやつ.
			fOffset *= -1.0f;
		}

		m_ppSpiaL[i]->SetPosition( 
			D3DXVECTOR3( 
				GetPositionX() - fWALL_SIDE_OFFSET + fOffset, 
				GetPositionY() + (float)i * fFLOOR_Y_OFFSET + fFLOOR_Y_OFFSET_FIRST,
				GetPositionZ() ) );

		m_ppSpiaR[i]->SetPosition( 
			D3DXVECTOR3( 
				GetPositionX() + fWALL_SIDE_OFFSET + fOffset,
				GetPositionY() + (float)i * fFLOOR_Y_OFFSET + fFLOOR_Y_OFFSET_FIRST,
				GetPositionZ() ) );
	}



	//槍壁座標.
	m_pSpiaWall[0]->SetPosition( GetPosition() );
	m_pSpiaWall[0]->AddPositionX( 0.0f );
	m_pSpiaWall[0]->AddPositionY( fWALL_HEIGHT );
	m_pSpiaWall[1]->SetPosition( GetPosition() );
	m_pSpiaWall[1]->AddPositionX( fSTAGE_WIDHT );
	m_pSpiaWall[1]->AddPositionY( fWALL_HEIGHT );
}



void clsSpiaWallMgr::Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye )
{
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpiaL[i]->Render( mView, mProj, vLight, vEye );
		m_ppSpiaR[i]->Render( mView, mProj, vLight, vEye );
	}


	//壁.
//	m_pSpiaWall[0]->Render( mView, mProj, vLight, vEye );
//	m_pSpiaWall[1]->Render( mView, mProj, vLight, vEye );
}

//槍のあたり判定情報返す.
COL_STATE* clsSpiaWallMgr::GetPointerSpiaCol( bool bRight, int i )
{
	if( bRight ){
		return m_ppSpiaR[i]->GetPointerCol();
	}
	else{
		return m_ppSpiaL[i]->GetPointerCol();
	}
	return NULL;
}


//ラップ.
void clsSpiaWallMgr::GoRight()
{
	PlaySe( clsSpiaWall::enSOUND_VIB );
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpiaL[i]->GoRight();
	}
}
void clsSpiaWallMgr::GoLeft()
{
	PlaySe( clsSpiaWall::enSOUND_VIB );
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpiaR[i]->GoLeft();
	}
}
//0:左, 1:右, 2:両方, それ以外:無効.
void clsSpiaWallMgr::GoDown( int iRight )
{
	PlaySe( clsSpiaWall::enSOUND_DOWN );

	if( iRight == 0 ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpiaL[i]->GoDown();
		}
	}
	else if( iRight == 1 ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpiaR[i]->GoDown();
		}
	}
	else if( iRight == 2 ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpiaL[i]->GoDown();
			m_ppSpiaR[i]->GoDown();
		}
	}
}

//============================================================
//	効果音.
//============================================================
void clsSpiaWallMgr::PlaySe( clsSpiaWall::enSound enSe )
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
