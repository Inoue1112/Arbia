#ifndef _C_SPIA_FLOOR_H_
#define _C_SPIA_FLOOR_H_

#include "CharaStatic.h"




class clsSpiaFloor
	: public clsCharaStatic
{
public:
	clsSpiaFloor();
	virtual ~clsSpiaFloor();

	//動きの種類.
	enum enSpeaFloorMode
	{
		enSFM_UNDER = 0,
		enSFM_UP,
		enSFM_TOP,
		enSFM_DOWN,
	};

	//音の種類.
	enum enSound
	{
		enSOUND_VIB = 0,
		enSOUND_UP,
		enSOUND_DOWN,
		  
		enSOUND_MAX,	//最大数.
	};

	virtual void Init( bool bFlg = false );//true = 高いやつ.

	enSound Move();

	enSpeaFloorMode GetMode(){
		return m_Mode;
	}

private:



	enSpeaFloorMode m_Mode;
	int		m_iTimer;
	float	m_fChangeRange;
	float	m_fRootY;
	bool	m_bVib;
	bool	m_bSpnRight;

};

#endif//#ifndef _C_SPIA_FLOOR_H_