#include "StepBox.h"

//等速移動距離 : Constant Speed Range.
const float fCONST_SPD_RANGE = 1.0f;
//等速移動速度.
const float fSPD_MAX = 1.0f / 16.0f;

//加速度.
const float fACCEL = fSPD_MAX / 32.0f;


//ステージ幅.
const float fSTAGE_WIDTH = 10.0f;
const float fSTAGE_WIDTH_HARF = fSTAGE_WIDTH / 2.0f;


clsStepBox::clsStepBox()
{
}

clsStepBox::~clsStepBox()
{
}


void clsStepBox::CreateMove( bool bRight, bool bEast )
{
	//基点.
	m_vStartPos = m_vPos;
	//フラグ.
	m_bInitFlg = bEast;
	//へ向かうStartなら.
	if( bRight ){
		m_vStartPos.x += fSTAGE_WIDTH_HARF;
	}
	else{
		m_vStartPos.x -= fSTAGE_WIDTH_HARF;
	}


	Init();
}

void clsStepBox::Init()
{
	m_bEastFlg = m_bInitFlg;//true : 東向き.

	m_fMovePower = -fSPD_MAX;
	m_vPos = m_vStartPos;
	//南へ向かうStartなら.
	if( m_bInitFlg ){
		m_fMovePower *= -1.0f;		//移動量逆転.
	}
}


//動作.
void clsStepBox::Move()
{
	//東へ.
	if( m_bEastFlg ){
		//加速.
		m_fMovePower += fACCEL;
		//速度制限.
		if( m_fMovePower > fSPD_MAX ){
			m_fMovePower = fSPD_MAX;
		}
		//方向転換.
		if( m_vPos.x > m_vStartPos.x + fCONST_SPD_RANGE ){
			m_bEastFlg = false;
		}

		
	}
	//西へGO.
	else{
		m_fMovePower -= fACCEL;
		
		if( m_fMovePower < -fSPD_MAX ){
			m_fMovePower = -fSPD_MAX;
		}

		if( m_vPos.x < m_vStartPos.x - fCONST_SPD_RANGE ){
			m_bEastFlg = true;
		}
	}

	//移動.
	m_vPos.x += m_fMovePower;
}
