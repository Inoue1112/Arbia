#include "Resource.h"
#include "SpiaFloorManager.h"


//音.
//振動.
#define ALIAS_NAME_VIB "SpiaFloorVib"
#define FILE_PATH_VIB "SE\\300Trap\\100SpiaVib.wav"
const int iVOL_VIB = 1000;

//上がる.
#define ALIAS_NAME_UP "SpiaFloorTop"
#define FILE_PATH_UP "SE\\300Trap\\110SpiaTop.wav"					
const int iVOL_UP = 1000;

//下がる.
#define ALIAS_NAME_DOWN "SpiaFloorDown"
#define FILE_PATH_DOWN "SE\\300Trap\\130SpiaClose.wav"					
const int iVOL_DOWN = 1000;





const float STAGE_WIDHT = 10.0f;
const float WALL_OFFSET_Y = 0.4375f;

const int iFLOOR_MAX = 20;	//床槍の数.
const float fFLOOR_W_OFFSET = 0.5f;
const float fFLOOR_H_OFFSET = 0.0625f;
const float fFLOOR_H_OFFSET_SECOND = 0.25f;


clsSpiaFlorMgr::clsSpiaFlorMgr()
{
}

clsSpiaFlorMgr::~clsSpiaFlorMgr()
{
	Release();
}

void clsSpiaFlorMgr::CreateSpia( HWND hWnd, int iNo )
{
	//----- モデル -----//
	//槍.
	m_iSpiaMax = iFLOOR_MAX;
	m_ppSpia = new clsSpiaFloor*[m_iSpiaMax];
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpia[i] = new clsSpiaFloor;

		m_ppSpia[i]->AttachModel( 
			clsResource::GetInstance()->GetStaticModels( 
				clsResource::enST_MODEL_SPIA ) );
	}

	//槍壁.
	m_pSpiaWall = new clsCharaStatic;
	m_pSpiaWall->AttachModel(
		clsResource::GetInstance()->GetStaticModels( 
			clsResource::enST_MODEL_SPIA_WALL ) );
	//----- モデル 終了 -----//


	//----- SE -----//
	//サウンド構造体.
	clsSound::SOUND_DATA tmpSData[clsSpiaFloor::enSOUND_MAX] =
	{
		{ ALIAS_NAME_VIB,	FILE_PATH_VIB,	iVOL_VIB	},
		{ ALIAS_NAME_UP,	FILE_PATH_UP,	iVOL_UP		},
		{ ALIAS_NAME_DOWN,	FILE_PATH_DOWN,	iVOL_DOWN	},
	};

	//サウンドクラス作成.
	m_ppSe = new clsSound*[clsSpiaFloor::enSOUND_MAX];
	for( int i=0; i<clsSpiaFloor::enSOUND_MAX; i++ ){
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


	Init();
}

void clsSpiaFlorMgr::Init()
{
	//槍.
	for( int i=0; i<m_iSpiaMax; i++ ){
		float fOffset = fFLOOR_H_OFFSET;
		bool bFlg = false;
		if( i % 2 != 0 ){
			//高いやつ.
			fOffset = fFLOOR_H_OFFSET_SECOND;
			bFlg = true;
		}

		m_ppSpia[i]->SetPosition( 
		D3DXVECTOR3( 
			GetPositionX() - STAGE_WIDHT/2.0f + (float)i * fFLOOR_W_OFFSET + fFLOOR_W_OFFSET / 2.0f,
			GetPositionY() + fOffset, 
			GetPositionZ() ) );
		m_ppSpia[i]->Init( bFlg );
	}

	//槍壁座標.
	m_pSpiaWall->SetPosition( GetPosition() );
	m_pSpiaWall->AddPositionY( WALL_OFFSET_Y );
	m_pSpiaWall->AddPositionX( -STAGE_WIDHT / 2.0f );
}

void clsSpiaFlorMgr::Release()
{
	//音.
	if( m_ppSe != NULL ){
		for( int i=0; i<clsSpiaFloor::enSOUND_MAX; i++ ){
			m_ppSe[i]->Stop();
			m_ppSe[i]->Close();
			delete m_ppSe[i];
			m_ppSe[i] = NULL;
		}
		delete[] m_ppSe;
		m_ppSe = NULL;
	}
	//オブジェクト.
	if( m_ppSpia != NULL ){
		for( int i=0; i<m_iSpiaMax; i++ ){
			m_ppSpia[i]->DetatchModel();
			delete m_ppSpia[i];
			m_ppSpia[i] = NULL;
		}
		delete[] m_ppSpia;
		m_ppSpia = NULL;
		m_iSpiaMax = 0;
	}
	if( m_pSpiaWall != NULL ){
		delete m_pSpiaWall;
		m_pSpiaWall = NULL;
	}
}

void clsSpiaFlorMgr::Move( float fEarZ )
{
	//どの音を鳴らすかのフラグ.
	clsSpiaFloor::enSound enSoundFlg;

	//動き.
	for( int i=0; i<m_iSpiaMax; i++ ){
		//動きに合わせてフラグを更新.
		enSoundFlg = m_ppSpia[i]->Move();
	}

	//効果音再生（MAXはSpiaFloor内の初期化使っているのでそれ以上では鳴らさない）.
	if( enSoundFlg < clsSpiaFloor::enSOUND_MAX ){
		//飛び出したらぎしぎし音を止める.
		if( enSoundFlg == clsSpiaFloor::enSOUND_UP ){
			m_ppSe[clsSpiaFloor::enSOUND_VIB]->Stop();
		}
		//再生.
		PlaySe( enSoundFlg, fEarZ );
	}

	//槍壁座標.
	m_pSpiaWall->SetPositionY( m_ppSpia[0]->GetPositionY() );
	//上がってる時は.刺さりやすく
	//下がっている時は刺さりにくく.
	if( m_ppSpia[0]->GetMode() == clsSpiaFloor::enSFM_UNDER ||
		m_ppSpia[0]->GetMode() == clsSpiaFloor::enSFM_DOWN ){
		m_pSpiaWall->AddPositionY( WALL_OFFSET_Y );
	}
	
}



void clsSpiaFlorMgr::Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye )
{
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpia[i]->Render( mView, mProj, vLight, vEye );
	}

//	//壁.
//	m_pSpiaWall->Render( mView, mProj, vLight, vEye );

}

void clsSpiaFlorMgr::SetPosition( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;

	//子分の座標.
	for( int i=0; i<m_iSpiaMax; i++ ){
		m_ppSpia[i]->SetPosition( 
			D3DXVECTOR3( 
				GetPositionX() - STAGE_WIDHT/2.0f + (float)i * fFLOOR_W_OFFSET,
				GetPositionY(), 
				GetPositionZ() ) );
	}

	//槍壁座標.
	m_pSpiaWall->SetPosition( GetPosition() );
	m_pSpiaWall->AddPositionY( WALL_OFFSET_Y );
	m_pSpiaWall->AddPositionX( -STAGE_WIDHT / 2.0f );
}

//槍のあたり判定情報返す.
COL_STATE* clsSpiaFlorMgr::GetPointerSpiaCol( int i )
{
	return m_ppSpia[i]->GetPointerCol();
}

//槍の最大数.
int clsSpiaFlorMgr::GetSpiaMax()
{
	return m_iSpiaMax;
}

clsCharaStatic*	clsSpiaFlorMgr::GetWallPointer()
{
	return m_pSpiaWall;
}


//============================================================
//	効果音.
//============================================================
void clsSpiaFlorMgr::PlaySe( clsSpiaFloor::enSound enSe, float fEarZ )
{
	//再生する距離なら.
	int vol = ChangeVolumeDistance( fEarZ, m_vPos.z );
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

