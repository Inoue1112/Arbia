#include "StepCylinder.h"

//“™‘¬ˆÚ“®‹——£ : Constant Speed Range.
const float fCONST_SPD_RANGE = 6.0f;
//“™‘¬ˆÚ“®‘¬“x.
const float fSPD_MAX = 1.0f / 32.0f;

//‰Á‘¬“x.
const float fACCEL = fSPD_MAX / 32.0f;


clsStepCyl::clsStepCyl()
{
}

clsStepCyl::~clsStepCyl()
{
}

void clsStepCyl::CreateMove( bool bNorth )
{
	//Šî“_.
	m_vStartPos = m_vPos;
	m_fGoalPosZ = m_vStartPos.z + fCONST_SPD_RANGE;
	//ƒtƒ‰ƒO.
	m_bInitFlg = bNorth;
	Init();
//	m_vRot.y = (float)M_PI_2;
}

void clsStepCyl::Init()
{
	m_bNorthFlg = m_bInitFlg;//true : –kŒü‚«.

	m_fMovePower = -fSPD_MAX;
	m_vPos = m_vStartPos;
	//“ì‚ÖŒü‚©‚¤Start‚È‚ç.
	if( m_bInitFlg ){
		m_fMovePower *= -1.0f;		//ˆÚ“®—Ê‹t“].
		m_vPos.z = m_fGoalPosZ;//ˆÊ’u‚ðŒü‚±‚¤‚Ö.
	}
}

//“®ì.
void clsStepCyl::Move()
{
	//–k‚ÖGO.
	if( m_bNorthFlg ){
		//‰Á‘¬.
		m_fMovePower += fACCEL;
		//‘¬“x§ŒÀ.
		if( m_fMovePower > fSPD_MAX ){
			m_fMovePower = fSPD_MAX;
		}

		//•ûŒü“]Š·.
		if( m_vPos.z > m_fGoalPosZ ){
			m_bNorthFlg = false;
		}
	}
	//“ì‚ÖGO.
	else{
		m_fMovePower -= fACCEL;
		
		if( m_fMovePower < -fSPD_MAX ){
			m_fMovePower = -fSPD_MAX;
		}

		if( m_vPos.z < m_vStartPos.z ){
			m_bNorthFlg = true;
		}
	}

	//ˆÚ“®.
	m_vPos.z += m_fMovePower;
}

