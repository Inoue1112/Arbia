#ifndef _C_SPIA_WALL_H_
#define _C_SPIA_WALL_H_

#include "CharaStatic.h"




class clsSpiaWall
	: public clsCharaStatic
{
public:
	clsSpiaWall();
	virtual ~clsSpiaWall();

	enum enSpeaWallMode
	{
		enSWM_STOP = 0,		//止まっている（引っ込んでるor出ている）.
		enSWM_MOVE_RIGHT,	//右へ飛び出す.
		enSWM_MOVE_LEFT,
		enSWM_DOWN,			//引っ込む.
	};

	//音の種類.
	enum enSound
	{
		enSOUND_VIB = 0,
		enSOUND_UP,
		enSOUND_DOWN,
		  
		enSOUND_MAX,	//最大数.
	};

	//最初の初期化.
	void Create( bool bRightFlg, int iNo );

	virtual void Init( D3DXVECTOR3 vInitPos, bool bRightFlg = false );//true = .


	bool Move();

	void GoRight(){
		m_enMode = enSWM_MOVE_RIGHT;
	}
	void GoLeft(){
		m_enMode = enSWM_MOVE_LEFT;
	}
	void GoDown(){
		m_enMode = enSWM_DOWN;
	}

	D3DXVECTOR3 GetTest(){
		return ColState.vRangeHalf;
	}

private:

	virtual void UpdateColState();//ﾑｰﾌﾞの最後に使う.
	
	
	//振動動作.
	void Vibration();

	D3DXVECTOR3 m_vInitPos;


	bool	m_bRight;//右側フラグ.

	enSpeaWallMode m_enMode;
	int		m_iTimer;
	float	m_fChangeRange;//移動量(飛び出した時のストップ位置の決定に使う).
	float	m_fRootY;
	bool	m_bVib;	//がたがたが右に行くか左へ行くか.

	bool	m_bSpnRight;//回転が右回転.

};

#endif//#ifndef _C_SPIA_WALL_H_