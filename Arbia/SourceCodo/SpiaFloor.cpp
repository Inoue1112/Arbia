#include "SpiaFloor.h"


//===== あたり判定 =====//.
const float COL_SPIA_H = 0.5f;
const float COL_SPIA_RANGE = 0.25f;
//===== あたり判定 終わり =====//.



//ﾀｲﾑ.
const int iUNDER_TIME = 120;
const int iVIB_START_TIME = 60;//Vibration Start　がたがた開始.
const int iVIB_INTER_TIME = 4;//Vibration Interval　ガタガタする間隔.
const int iTOP_TIME = 30;	//頂点にいる時間.

//速度.
const float fUP_SPD = 0.75f;
const float fDOWN_SPN_YAW = 0.1f;
const float fDOWN_SPD = -0.035f;

//長さ(動きの).
const float fVIB_RANGE = 0.25f;
const float fSPIA_TOP = 3.5f;

clsSpiaFloor::clsSpiaFloor()
{
	ColState.fRange = COL_SPIA_RANGE;
	ColState.fHeight = COL_SPIA_H;
	ColState.iSarchTheta = 360;
}

clsSpiaFloor::~clsSpiaFloor()
{
}



void clsSpiaFloor::Init( bool bFlg )//右回転のフラグ.
{
	m_Mode = enSFM_UNDER;
	m_iTimer = 0;
	m_fChangeRange = 0.0f;
	m_fRootY = m_vPos.y;
	m_bVib = false;
	m_bSpnRight = bFlg;


}

//戻り値は槍マネージャーでの音再生用.
clsSpiaFloor::enSound clsSpiaFloor::Move()
{
	//マネージャーでの音再生用.
	enSound enSoundFlg = enSOUND_MAX;

	m_iTimer ++;
	switch( m_Mode )
	{
	case enSFM_UNDER:
		//飛び出す.
		if( m_iTimer > iUNDER_TIME ){
			m_Mode = enSFM_UP;
			m_iTimer = 0;
			enSoundFlg = enSOUND_UP;
		}
		//がたがたする.
		else if( m_iTimer == iVIB_START_TIME ){
			enSoundFlg = enSOUND_VIB;
		}
		else if( m_iTimer > iVIB_START_TIME ){
			if( m_iTimer % iVIB_INTER_TIME ){
				if( m_bVib ){
					AddPositionY( -fVIB_RANGE );
					m_bVib = false;
				}
				else{
					AddPositionY( fVIB_RANGE );
					m_bVib = true;
				}
			}
		}
		else{
			if( GetRotationY() > (float)( M_PI * 2.0 ) ){
				AddRotationY( -(float)( D3DX_PI * 2.0 ) );
			}
		}
		break;
	case enSFM_UP:
		AddPositionY( fUP_SPD );
		m_fChangeRange += fUP_SPD;
		if( m_fChangeRange >= fSPIA_TOP ){
			SetPositionY( m_fRootY + fSPIA_TOP );
			m_Mode = enSFM_TOP;
			m_iTimer = 0;
		}
		break;
	case enSFM_TOP:
		if( m_iTimer > iTOP_TIME ){
			m_Mode = enSFM_DOWN;
			m_iTimer = 0;
			enSoundFlg = enSOUND_DOWN;
		}
		break;
	case enSFM_DOWN:
		AddPositionY( fDOWN_SPD );
		//回転.
		if( m_bSpnRight ){
			AddRotationY( -fDOWN_SPN_YAW );
		}
		else{
			AddRotationY( fDOWN_SPN_YAW );
		}
		
		m_fChangeRange += fDOWN_SPD;
		if( GetPositionY()  < m_fRootY ){
			SetPositionY( m_fRootY );
			m_Mode = enSFM_UNDER;
			m_iTimer = 0;
		}
		break;
	}

	UpdateColState();

	return enSoundFlg;
}

