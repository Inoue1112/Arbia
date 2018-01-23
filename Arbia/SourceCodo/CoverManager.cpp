#include "Resource.h"
#include "CoverManager.h"


//音.
//軋み.
#define ALIAS_NAME_VIB "CoverVib"
#define FILE_PATH_VIB "SE\\300Trap\\310CoverVib.wav"
const int iVOL_VIB = 1000;

//ぱかっ.
#define ALIAS_NAME_OPEN "CoverOpen"
#define FILE_PATH_OPEN "SE\\300Trap\\300Cover.wav"					
const int iVOL_OPEN = 1000;




//奥行の長さ（音の発生位置をど真ん中にするために必要）.
const float OBJ_Z_LONG = 12.0f;

const float STAGE_WIDHT = 10.0f;
const float fOFFSET_X = STAGE_WIDHT / 2.0f;

const float fSPD_OPEN = 0.125f;		//.
const float fSPD_CLOSE = 0.1875f;	//.

const int iVIB_DISTANCE = 8;	//開きかけ時、「何フレームに一回」角度を動かす?.
const int iVIB_TIME = 60 * 1.75;//開きかけ時間.
const int iHOLL_TIME = 90;	//穴としての時間.

const float fOPEN_THETA = (float)( M_PI_2 - ( M_PI_4 / 16.0) );//一気に開く角度.
const float fVIB_THETA = (float)( M_PI_4 / 128.0 );//ゆっくり開く角度.


//Se発信箇所の数.
const char cSE_NUM_MAX = 2;


clsCoverMgr::clsCoverMgr()
{
}

clsCoverMgr::~clsCoverMgr()
{
	Release();
}

void clsCoverMgr::CreateCover( HWND hWnd, int iNo )
{
	//----- モデル -----//
	//全体.
	m_ppCover = new clsCharaStatic*[cCOVER_MAX];
	//左.
	m_ppCover[0] = new clsCharaStatic;
	m_ppCover[0]->AttachModel(
		clsResource::GetInstance()->GetStaticModels(
			clsResource::enST_MODEL_COVER_L ) );
	//右.
	m_ppCover[1] = new clsCharaStatic;
	m_ppCover[1]->AttachModel(
		clsResource::GetInstance()->GetStaticModels(
			clsResource::enST_MODEL_COVER_R ) );
	//----- モデル 終了 -----//




	//----- SE -----//
	//サウンド構造体.
	clsSound::SOUND_DATA tmpSData[enSOUND_MAX] = 
	{
		{ ALIAS_NAME_VIB,	FILE_PATH_VIB,	iVOL_VIB	},
		{ ALIAS_NAME_OPEN,	FILE_PATH_OPEN,	iVOL_OPEN	},
	};

	//サウンドクラス作成.
	m_pppSe = new clsSound**[enSOUND_MAX];
	for( int i=0; i<enSOUND_MAX; i++ ){
		m_pppSe[i] = new clsSound*[enSOUND_MAX];//SEの数分作る.
		for( int j=0; j<cSE_NUM_MAX; j++ ){
			m_pppSe[i][j] = new clsSound;//SEを二つに分身.
			//現音量初期化.
			m_pppSe[i][j]->SetVolume( 0 );
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
			//番号.
			char cNumber[] = "  ";
			_itoa_s( iNo, cNumber, 10 );
			//名前と番号合体.
			strcat_s( cAliasName, sizeof( cAliasName ), cNumber );
			//発信箇所番号.
			char cSeNumber[] = "  ";
			_itoa_s( j, cSeNumber, 10 );
			//名前と番号合体.
			strcat_s( cAliasName, sizeof( cAliasName ), cSeNumber );
			//作成.
			m_pppSe[i][j]->Open( tmpSData[i].sPath, cAliasName, hWnd );
			//最大音量設定.
			m_pppSe[i][j]->SetMaxVolume( tmpSData[i].iMaxVolume );
		}
	}
	//----- SE 終了 -----//



	Init();
}

void clsCoverMgr::Init()
{
	m_enMove = enCM_FLOOR;
	m_iTimer = 0;
	for( char i=0; i<cCOVER_MAX; i++ ){
		m_ppCover[i]->SetPosition( m_vPos );
		m_ppCover[i]->SetRotationZ( 0.0f );
	}
	m_ppCover[0]->AddPositionX(-fOFFSET_X );
	m_ppCover[1]->AddPositionX( fOFFSET_X );
}




void clsCoverMgr::Release()
{
	if( m_pppSe != NULL ){
		for( int i=0; i<enSOUND_MAX; i++ ){
			for( int j=0; j<cSE_NUM_MAX; j++ ){
				m_pppSe[i][j]->Stop();
				m_pppSe[i][j]->Close();
				delete m_pppSe[i][j];
				m_pppSe[i][j] = NULL;
			}
			delete[] m_pppSe[i];
			m_pppSe[i] = NULL;
		}
		delete[] m_pppSe;
		m_pppSe = NULL;
	}

	if( m_ppCover != NULL ){
		for( char i=0; i<cCOVER_MAX; i++ ){
			m_ppCover[i]->DetatchModel();
			delete m_ppCover[i];
			m_ppCover[i] = NULL;
		}
		delete[] m_ppCover;
		m_ppCover = NULL;
	}
}



//動き.
void clsCoverMgr::Move( float fEarZ )
{
	switch( m_enMove )
	{
	case enCM_FLOOR:
		break;
	case enCM_VIB:
		m_iTimer ++;
		//徐々に開く.
		if( m_iTimer % iVIB_DISTANCE == 0 ){
			m_ppCover[0]->AddRotationZ( -fVIB_THETA );
			m_ppCover[1]->AddRotationZ(  fVIB_THETA );
		}
		//開く.
		if( m_iTimer > iVIB_TIME ){
			m_iTimer = 0;
			m_enMove = enCM_OPEN;
			//ぎしぎしストップ.
			for( int j=0; j<cSE_NUM_MAX; j++ ){
				m_pppSe[enSOUND_VIB][j]->Stop();
			}
			PlaySe( enSOUND_OPEN, fEarZ );
		}
		break;
	case enCM_OPEN:
		m_ppCover[0]->AddRotationZ(-fSPD_OPEN );
		m_ppCover[1]->AddRotationZ( fSPD_OPEN );
		if( m_ppCover[1]->GetRotationZ() >  fOPEN_THETA )
		{
			m_ppCover[0]->SetRotationZ( -fOPEN_THETA );
			m_ppCover[1]->SetRotationZ(  fOPEN_THETA );
			m_enMove = enCM_HOLL;
		}
		break;
	case enCM_HOLL:
		m_iTimer ++;
		if( m_iTimer > iHOLL_TIME ){
			m_iTimer = 0;
			m_enMove = enCM_CLOSE;
			PlaySe( enSOUND_OPEN, fEarZ );
		}
		break;
	case enCM_CLOSE:
		m_ppCover[0]->AddRotationZ( fSPD_CLOSE );
		m_ppCover[1]->AddRotationZ(-fSPD_CLOSE );
		if( m_ppCover[1]->GetRotationZ() > (float)M_PI )
		{
			m_ppCover[0]->SetRotationZ( 0.0f );
			m_ppCover[1]->SetRotationZ( 0.0f );
			m_enMove = enCM_FLOOR;
		}
		break;
	default:
		Init();
		break;
	}
}

//踏む.
void clsCoverMgr::StmpCovor( float fEarZ )
{
	if( m_enMove == enCM_FLOOR ){
		m_iTimer = 0;
		m_enMove = enCM_VIB;
		PlaySe( enSOUND_VIB, fEarZ );
	}
}

//強く踏む.
void clsCoverMgr::StmpCovorStrong( float fEarZ )
{
	if( m_enMove == enCM_FLOOR ||
		m_enMove == enCM_VIB ){
		m_iTimer = 0;
		m_enMove = enCM_OPEN;
		//ぎしぎしストップ.
		for( int j=0; j<cSE_NUM_MAX; j++ ){
			m_pppSe[enSOUND_VIB][j]->Stop();
		}
		PlaySe( enSOUND_OPEN, fEarZ );
	}
}

void clsCoverMgr::Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye )
{
	for( char i=0; i<cCOVER_MAX; i++ ){
		m_ppCover[i]->Render( mView, mProj, vLight, vEye );
	}


}


//============================================================
//	効果音.
//============================================================
void clsCoverMgr::PlaySe( enSound enSe, float fEarZ )
{
	//耳を手前として考えさせる(ど真ん中から音を出す).
//	fEarZ -= OBJ_Z_LONG / 2.0f;

	for( int j=0; j<cSE_NUM_MAX; j++ ){
		//音が鳴る距離か否か.
		int vol = ChangeVolumeDistance( fEarZ, m_vPos.z );
		//再生する距離なら.
		if( vol ){
			m_pppSe[enSe][j]->GetMaxVolRate( vol );
			m_pppSe[enSe][j]->SeekToStart();
			m_pppSe[enSe][j]->SetVolume( vol );
			m_pppSe[enSe][j]->Play();
		}
		//再生しない距離なら.
		else{
			m_pppSe[enSe][j]->SetVolume( 0 );
		}

		//0は手前、1は奥.
		fEarZ -= OBJ_Z_LONG;
	}
}
