#include "Sp2DTimerGroup.h"


#define FILE_PATH_SCOLON "Data\\Image\\Colon.png"

//----- 音 -----//.
//足音.
#define ALIAS_NAME "TImerCountStart"
#define  FILE_PATH "SE\\400Ui\\100Timer.wav"
const	  int iVOL = 1000;


//タイマーの数.
const char cTIMEER_NUM = 3;
//コロンの数.
const char cCOLON_NUM = cTIMEER_NUM - 1;
//数字の幅高さ.
const float fNUM_DISP = 64.0f;
//コロンの幅高さ.
const float fCOLON_DISP = fNUM_DISP / 2.0f;
//二桁目を左にずらす.
const float fOFFSET_LEFT_SECOND = fNUM_DISP / 2.0f;
//コロン座標ずらし.
const float fOFFSET_COLON_X = 72.0f;
const float fOFFSET_COLON_Y = 16.0f;
//一ケタの上限.
const int iTIMER_NUM_MAX = 59;



clsTimerGrp::clsTimerGrp()
{
	for( char i=0; i<cTIMEER_NUM; i++ ){
		m_Timer[i] = NULL;
	}
	for( char i=0; i<cCOLON_NUM; i++ ){
		m_Colon[i] = NULL;
	}
	m_pSe = NULL;
}

clsTimerGrp::~clsTimerGrp()
{
	if( m_pSe != NULL ){
		m_pSe->Stop();
		m_pSe->Close();
		delete m_pSe;
		m_pSe = NULL;
	}

	if( m_Colon != NULL ){
		for( char i=0; i<cCOLON_NUM; i++ ){
			if( m_Colon[i] != NULL ){
				delete m_Colon[i];
				m_Colon[i] = NULL;
			}
		}
	}
	if( m_Timer != NULL ){
		for( char i=0; i<cTIMEER_NUM; i++ ){
			if( m_Timer[i] != NULL ){
				delete m_Timer[i];
				m_Timer[i] = NULL;
			}
		}
	}
}


void clsTimerGrp::Create( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_vPos = { 0.0f, 0.0f, 0.0f };

	for( char i=0; i<cTIMEER_NUM; i++ ){
		m_Timer[i] = new clsTimer;
		m_Timer[i]->Create( pDevice11, pContext11 );
	}
	for( char i=0; i<cCOLON_NUM; i++ ){
		m_Colon[i] = new clsSprite2D;
		m_Colon[i]->Init( pDevice11, pContext11, FILE_PATH_SCOLON );
		m_Colon[i]->SetSs(
			m_Colon[i]->GetSs().Base.w,
			m_Colon[i]->GetSs().Base.h,
			fCOLON_DISP, fCOLON_DISP );

		m_Colon[i]->UpDateSpriteSs();
	}

	//効果音.
	m_pSe = new clsSound;
	m_pSe->SetVolume( iVOL );
	//名前.
	char cAliasName[STR_BUFF_MAX] = "";
	strcat_s( cAliasName, sizeof( cAliasName ), ALIAS_NAME );
	//作成.
	m_pSe->Open( FILE_PATH, cAliasName, hWnd );


	ReSet();
}
void clsTimerGrp::ReSet()
{
	for( char i=0; i<cTIMEER_NUM; i++ ){
		m_Timer[i]->ReSet();
	}
	m_bRimitFlg = false;
	m_bStop = m_bStopOld = false;
}

void clsTimerGrp::SetPos( D3DXVECTOR3 vPos )
{
	const float fOffsetPlus = 2.5f;
	m_vPos = vPos;
	
	for( char i=0; i<cTIMEER_NUM; i++ ){
		m_Timer[i]->SetPos( vPos );
	}

	//1番上の桁以外を右にずらす.
	float fOffset = 0.0f;
	for( char i=cTIMEER_NUM-1; i>=0; i-- ){
		m_Timer[i]->AddPosX( fOFFSET_LEFT_SECOND * fOffset );
		fOffset += fOffsetPlus;
	}


	//「:」.
	for( char i=0; i<cCOLON_NUM; i++ ){
		m_Colon[i]->SetPos( vPos );
	}

	fOffset = 0.0f;
	for( char i=0; i<cCOLON_NUM; i++ ){
		m_Colon[i]->AddPosX( fOFFSET_LEFT_SECOND * fOffset + fOFFSET_COLON_X );
		m_Colon[i]->AddPosY( fOFFSET_COLON_Y );
		fOffset += fOffsetPlus;
	}
}
void clsTimerGrp::AddPos( D3DXVECTOR3 vPos )
{
	m_vPos += vPos;
	SetPos( m_vPos );
}
void clsTimerGrp::AddPosY( float fPos )
{
	m_vPos.y += fPos;
	SetPos( m_vPos );
}


void clsTimerGrp::Move()
{
	//カウント.
	if( !m_bRimitFlg && !m_bStop ){
		m_Timer[0]->PlusNum();
	}
	//カウントし始めに.
	if( !m_bStop && m_bStopOld ){
		m_pSe->SeekPlay();
	}

	m_bStopOld = m_bStop;


	for( char i=0; i<cTIMEER_NUM; i++ ){
		if( m_Timer[i]->Carrying() ){
			//オーバーしない?.
			if( i + 1 < cTIMEER_NUM ){
				m_Timer[ i + 1 ]->PlusNum();
			}
			//上限.
			else{
				for( char j=0; j<cTIMEER_NUM; j++ ){
					m_Timer[j]->Rimit();
					m_bRimitFlg = true;
				}
			}
		}
		m_Timer[i]->Move();
	}
}

void clsTimerGrp::Render()
{
	for( char i=0; i<cTIMEER_NUM; i++ ){
		m_Timer[i]->Render();
	}
	for( char i=0; i<cCOLON_NUM; i++ ){
		m_Colon[i]->Render();
	}

}


int clsTimerGrp::GetTime()
{
	int Time = 0;
	Time +=	m_Timer[2]->GetTime() * 100;
	Time +=	m_Timer[1]->GetTime();
	return Time;
}
