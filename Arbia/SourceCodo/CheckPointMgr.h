#ifndef _CHECK_POINT_MANAGER_H_
#define _CHECK_POINT_MANAGER_H_

#include "CheckPoint.h"
#include "Sound.h"

class clsCheckPointMgr
{
public:
	clsCheckPointMgr();
	~clsCheckPointMgr();

	void Create(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, int iNo = -1);

	void Init();

	void Update();

	void Start(float fPosZ);

	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 &vEye);

	void Release();

private:

	clsCheckPoint** m_ppModel;

	clsSound**	m_ppSe;
};

#endif //#ifndef _CHECK_POINT_MANAGER_H_