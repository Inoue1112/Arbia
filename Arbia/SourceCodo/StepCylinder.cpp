#include "StepCylinder.h"

//等速移動距離 : Constant Speed Range.
const float fCONST_SPD_RANGE = 6.0f;
//等速移動速度.
const float fSPD_MAX = 1.0f / 32.0f;

//加速度.
const float fACCEL = fSPD_MAX / 32.0f;


clsStepCyl::clsStepCyl()
{
}

clsStepCyl::~clsStepCyl()
{
}

void clsStepCyl::CreateMove( bool bNorth )
{
	//基点.
	m_vStartPos = m_vPos;
	m_fGoalPosZ = m_vStartPos.z + fCONST_SPD_RANGE;
	//フラグ.
	m_bInitFlg = bNorth;
	Init();
//	m_vRot.y = (float)M_PI_2;
}

void clsStepCyl::Init()
{
	m_bNorthFlg = m_bInitFlg;//true : 北向き.

	m_fMovePower = -fSPD_MAX;
	m_vPos = m_vStartPos;
	//南へ向かうStartなら.
	if( m_bInitFlg ){
		m_fMovePower *= -1.0f;		//移動量逆転.
		m_vPos.z = m_fGoalPosZ;//位置を向こうへ.
	}
}

//動作.
void clsStepCyl::Move()
{
	//北へGO.
	if( m_bNorthFlg ){
		//加速.
		m_fMovePower += fACCEL;
		//速度制限.
		if( m_fMovePower > fSPD_MAX ){
			m_fMovePower = fSPD_MAX;
		}

		//方向転換.
		if( m_vPos.z > m_fGoalPosZ ){
			m_bNorthFlg = false;
		}
	}
	//南へGO.
	else{
		m_fMovePower -= fACCEL;
		
		if( m_fMovePower < -fSPD_MAX ){
			m_fMovePower = -fSPD_MAX;
		}

		if( m_vPos.z < m_vStartPos.z ){
			m_bNorthFlg = true;
		}
	}

	//移動.
	m_vPos.z += m_fMovePower;
}

