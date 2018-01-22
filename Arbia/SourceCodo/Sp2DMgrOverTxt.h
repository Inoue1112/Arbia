#ifndef _SP_2D_MANAGER_OVER_SCENE_TXT_H_
#define _SP_2D_MANAGER_OVER_SCENE_TXT_H_

#include"Sprite2D.h"

class clsSp2dMgrOverTxt
{
public:
	clsSp2dMgrOverTxt();
	~clsSp2dMgrOverTxt();

	void Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void InitSetPos();
	void Render();
	void Release();

private:

	unique_ptr<clsSprite2D> m_smpOverTxt;
};

#endif//#ifndef _SP_2D_MANAGER_OVER_SCENE_TXT_H_