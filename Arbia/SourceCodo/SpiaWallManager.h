#ifndef _SPIA_WALL_MANAGER_H_
#define _SPIA_WALL_MANAGER_H_

#include "SpiaWall.h"

#include "Sound.h"


class clsSpiaWallMgr
	: public clsGameObject
{
public:
	clsSpiaWallMgr();
	~clsSpiaWallMgr();

	void CreateSpia( HWND hWnd, int iNo, int iMoveMode );
	void Init();
	void Release();
	void UpDateModel(){};

	virtual void SetPosition( D3DXVECTOR3 vPos );


	void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );

	void Move( float fEarZ );
	void MoveMutualL();
	void MoveMutualR();
	void MoveCloseL();
	void MoveCloseR();



	//ラップ.
	void GoRight();
	void GoLeft();
	//0:左, 1:右, 2:両方, それ以外:無効.
	void GoDown( int iRight );
	int		m_iTimer;


	//ゲットシリーズ.

	COL_STATE* GetPointerSpiaCol( bool bRight, int i );

	int GetSpiaMax(){
		return m_iSpiaMax;
	}
	clsCharaStatic*	GetWallPointer();

	clsCharaStatic* GetWallLCol(){
		return m_pSpiaWall[0];
	}
	clsCharaStatic* GetWallRCol(){
		return m_pSpiaWall[1];
	}

	D3DXVECTOR3 GetTestL(){
		return m_ppSpiaL[0]->GetTest();
	}
	D3DXVECTOR3 GetTestR(){
		return m_ppSpiaR[0]->GetTest();
	}

private:

	enum  enMode
	{
		enMODE_MUTUAL_L = 1,//交互(左が先).
		enMODE_MUTUAL_R = 2,

		enMODE_CLOSE_L = 3,		//閉じる(左が先).
		enMODE_CLOSE_R = 4,
	};
	enMode	m_enMoveMode;


	//効果音.
	void PlaySe( clsSpiaWall::enSound enSe );
	clsSound**	m_ppSe;
	float		m_fEarZ;


	int				m_iSpiaMax;
	clsSpiaWall**	m_ppSpiaL;
	clsSpiaWall**	m_ppSpiaR;

	clsCharaStatic* m_pSpiaWall[2];//左右の二つの壁判定.


};


#endif//#ifndef _SPIA_WALL_MANAGER_H_