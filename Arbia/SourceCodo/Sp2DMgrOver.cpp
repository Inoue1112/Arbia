#include "Sp2DMgrOver.h"

clsSp2dMgrOver::clsSp2dMgrOver()
{
}

clsSp2dMgrOver::~clsSp2dMgrOver()
{
}

void clsSp2dMgrOver::Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpOverTxt = make_unique<clsSp2dMgrOverTxt>();

	m_smpPushTxt->Init(pDevice11, pContext11);
	m_smpOverTxt->Init(pDevice11, pContext11);

}

void clsSp2dMgrOver::InitSetPos()
{
	m_smpPushTxt->InitSetPos();
	m_smpOverTxt->InitSetPos();
}

void clsSp2dMgrOver::Render()
{
	m_smpPushTxt->Render();
	m_smpOverTxt->Render();
}

void clsSp2dMgrOver::Release()
{
	m_smpPushTxt->Release();
	m_smpPushTxt.reset();
	m_smpPushTxt = NULL;

	m_smpOverTxt->Release();
	m_smpOverTxt.reset();
	m_smpOverTxt = NULL;
}