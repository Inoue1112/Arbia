#ifndef _C_SPIA_FLOOR_MANAGER_H_
#define _C_SPIA_FLOOR_MANAGER_H_


#include "SpiaFloor.h"

#include "Sound.h"


class clsSpiaFlorMgr
	: public clsGameObject
{
public:
	clsSpiaFlorMgr();
	~clsSpiaFlorMgr();

	void CreateSpia( HWND hWnd, int iNo );
	void Init();
	void Release();
	void UpDateModel(){};

	virtual void SetPosition( D3DXVECTOR3 vPos );
	COL_STATE* GetPointerSpiaCol( int i );


	void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );

	void Move( float fEarZ );

	int GetSpiaMax();
	clsCharaStatic*	GetWallPointer();


private:


	//Œø‰Ê‰¹.
	void PlaySe( clsSpiaFloor::enSound enSe, float fEarZ );
	clsSound**	m_ppSe;


	int				m_iSpiaMax;
	clsSpiaFloor**	m_ppSpia;

	clsCharaStatic*	m_pSpiaWall;

};


#endif//#ifndef _C_SPIA_FLOOR_MANAGER_H_