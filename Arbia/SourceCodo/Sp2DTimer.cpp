#include "Sp2DTimer.h"

#define FILE_PATH_TEN "Data\\Image\\NumberTen.png"
#define FILE_PATH_SIX "Data\\Image\\NumberSix.png"


const char cNUM_MAX = 2;//数字の桁数.


const int iTEN_DECIMAL = 10;//10進数.
const int iSIX_DECIMAL = 6;	//6 進数.
const int iMAX = 60;

//幅高さ.
const float fNUM_DISP = 64.0f;
//二桁目を左にずらす.
const float fOFFSET_LEFT_SECOND = fNUM_DISP / 2.0f;


const WHSIZE_FLOAT NUMSIZE  = { 64.0f, 64.0f };


//一ケタの上限.
const int iTIMER_NUM_MAX = 59;


void clsTimer::Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_bSixDecimal = true;

	for( char i=0; i<cNUM_MAX; i++ ){
		NumImage[i] = new clsSprite2D;
	}
	NumImage[0]->Init( pDevice11, pContext11, FILE_PATH_TEN );
	NumImage[1]->Init( pDevice11, pContext11, FILE_PATH_SIX );

	NumImage[0]->SetSs(
		NumImage[0]->GetSs().Base.w,
		NumImage[0]->GetSs().Base.h,
		NUMSIZE.w, NUMSIZE.h,
		1.0f, iTEN_DECIMAL );

	NumImage[1]->SetSs(
		NumImage[1]->GetSs().Base.w,
		NumImage[1]->GetSs().Base.h,
		NUMSIZE.w, NUMSIZE.h,
		1.0f, iSIX_DECIMAL );

	for( char i=0; i<cNUM_MAX; i++ ){
		NumImage[i]->UpDateSpriteSs();
	}

	//最大値.
	m_iMax = iMAX;

	ReSet();
}

void clsTimer::ReSet()
{
	m_iRealNum = 0;
	m_iDiffNum = 0;
	m_bCarrying = false;
}



//数を増やす.
void clsTimer::PlusNum()
{
	m_iRealNum ++;

	if( m_iRealNum >= m_iMax ){
		m_iRealNum = 0;
		m_bCarrying = true;
	}
}



void clsTimer::AddPosX( float fMove )
{
	for( char i=0; i<cNUM_MAX; i++ ){
		NumImage[i]->AddPosX( fMove );
	}

}




//桁上がり( 繰り上がり )確認.
bool clsTimer::Carrying(){
	if( m_bCarrying ){
		m_bCarrying = false;
		return true;
	}
	return false;
}



//上限設定用.
void clsTimer::Rimit(){
	m_iRealNum = iTIMER_NUM_MAX;
}



