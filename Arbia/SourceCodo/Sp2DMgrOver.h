#ifndef _SP_2D_MANAGER_OVER_SCENE_H_
#define _SP_2D_MANAGER_OVER_SCENE_H_

#include"Sp2DMgrPushButn.h"
#include"Sp2DMgrOverTxt.h"

class clsSp2dMgrOver
{
public:
	clsSp2dMgrOver();
	~clsSp2dMgrOver();

	void Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void InitSetPos();
	void Render();
	void Release();

private:
	unique_ptr<clsPushTxt>		  m_smpPushTxt;
	unique_ptr<clsSp2dMgrOverTxt> m_smpOverTxt;


};

#endif//#ifndef _SP_2D_MANAGER_OVER_SCENE_H_