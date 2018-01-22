#include "NumGroup.h"


const int iSTR_BUFF_MAX = 256;

//数字一つ分の高さ.
const float fNUM_ONE_HEIGHT = 180.0f;

#define FILE_PATH_TEN "Data\\Image\\NumberTen.png"




const char cNUM_MAX = 2;//数字の桁数.
const int iTEN_DECIMAL = 10;//10進数.
const int iSIX_DECIMAL = 6;	//6 進数.
const int iMAX = 99;


//幅高さ.
const float fNUM_DISP = 64.0f;
//二桁目を左にずらす.
const float fOFFSET_LEFT_SECOND = fNUM_DISP / 2.0f;


const WHSIZE_FLOAT NUMSIZE  = { 64.0f, 64.0f };



clsNumGrp::clsNumGrp()
{
}

clsNumGrp::~clsNumGrp()
{
	if( NumImage != NULL ){
		for( char i=0; i<cNUM_MAX; i++ ){
			if( NumImage[i] != NULL ){
				delete NumImage[i];
				NumImage[i] = NULL;
			}
		}
	}
}



void clsNumGrp::Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_bSixDecimal = false;

	for( char i=0; i<cNUM_MAX; i++ ){

		NumImage[i] = new clsSprite2D;

		NumImage[i]->Init( pDevice11, pContext11, FILE_PATH_TEN );

		NumImage[i]->SetSs(
			NumImage[i]->GetSs().Base.w,
			NumImage[i]->GetSs().Base.h,
			NUMSIZE.w, NUMSIZE.h,
			1.0f, iTEN_DECIMAL );

		NumImage[i]->UpDateSpriteSs();
	}
	//最大値.
	m_iMax = iMAX;

	ReSet();
}


void clsNumGrp::ReSet()
{
	m_iRealNum = 0;
	m_iDiffNum = 0;
}

void clsNumGrp::SetPos( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;
	
	for( char i=0; i<cNUM_MAX; i++ ){
		NumImage[i]->SetPos( vPos );
	}

	//1番上の桁以外を右にずらす.
	float fOffset = 0.0f;
	for( char i=cNUM_MAX-1; i>=0; i-- ){
		NumImage[i]->AddPosX( fOFFSET_LEFT_SECOND * fOffset );
		fOffset += 1.0f;
	}
}
void clsNumGrp::AddPos( D3DXVECTOR3 vPos )
{
	m_vPos += vPos;
	SetPos( m_vPos );
}

void clsNumGrp::Move()
{
//	GETKEY_DOWN('G')PlusNum();
//	GETKEY_DOWN('T')PlusNum();
//	GETKEY_DOWN( 'F' )PlusNum();


	ApproachTargetNum();
	SetParam( m_iRealNum );
}


void clsNumGrp::Render()
{
	for( char i=0; i<cNUM_MAX; i++ ){
		NumImage[i]->Render();
	}
}

//実数値を座標値に変換.
float clsNumGrp::ConvNum( float fRendNum, bool bSixFlg )
{
	if( bSixFlg ){
		return ( (float) iSIX_DECIMAL - 1.0f ) - fRendNum;
	}
	return ( (float) iTEN_DECIMAL - 1.0f ) - fRendNum;
}


//数を増やす.
void clsNumGrp::PlusNum()
{
	m_iRealNum ++;

	if( m_iRealNum > m_iMax ){
		m_iRealNum = m_iMax;
	}
}
//数を減らす.
void clsNumGrp::MinusNum()
{
	m_iRealNum --;

	if( m_iRealNum < 0 ){
		m_iRealNum = 0;
	}
}
//数を決め打ち.
void clsNumGrp::SetNum( int iTargetNum )
{
	//引数はSetNum( int )によって指定された数( だんだんこれに近づく )
	m_iDiffNum = iTargetNum - m_iRealNum;
}




//数セット.
void clsNumGrp::SetParam( int iRealNum )
{
	//一ケタずつ分解.
	float fNum = 0.0f;
	for( char i=0; i<cNUM_MAX; i++ ){
		fNum = (float)( iRealNum % iTEN_DECIMAL );
		m_fRendNum[i] = fNum;
		iRealNum /= iTEN_DECIMAL;
	}

	//分解したものをいじる.
	bool bSixFlg = false;

	for( char i=0; i<cNUM_MAX; i++ ){
		//実数値を座標値に変換.
		m_fNumU[i] = ConvNum( m_fRendNum[i], bSixFlg );
		//座標値セット.
		NumImage[i]->SetPatarnV( m_fNumU[i] );
		//タイマーなら2桁目を6進数.
		if( m_bSixDecimal )	bSixFlg = true;
	}
}

//ターゲットに実数値を近づける : approach_to_target.
void clsNumGrp::ApproachTargetNum()
{
	if( m_iDiffNum > 0 ){
		m_iRealNum ++;
		m_iDiffNum --;
	}
	else if( m_iDiffNum < 0 ){
		m_iRealNum --;
		m_iDiffNum ++;
	}
}


