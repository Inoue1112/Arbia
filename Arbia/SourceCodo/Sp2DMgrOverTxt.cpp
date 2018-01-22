#include "Sp2DMgrOverTxt.h"
//íËêî.
namespace ConstantOver
{
	const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };
}

clsSp2dMgrOverTxt::clsSp2dMgrOverTxt()
{
}

clsSp2dMgrOverTxt::~clsSp2dMgrOverTxt()
{
}

void clsSp2dMgrOverTxt::Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_smpOverTxt = make_unique<clsSprite2D>();
	m_smpOverTxt->Init(pDevice11, pContext11, "Data\\Image\\GameOver.png");
}

void clsSp2dMgrOverTxt::InitSetPos()
{
	m_smpOverTxt->InitSetPos();
	m_smpOverTxt->SetPosX(ConstantOver::CENTER_POS.w - m_smpOverTxt->GetCenterDisp().w);
	m_smpOverTxt->SetPosY(-1 * (m_smpOverTxt->GetSs().Disp.h));
	m_smpOverTxt->SetAlpha(1.0f);
}
void clsSp2dMgrOverTxt::Render()
{
	static float Speed = 5.0f;
	static float Accel = -0.01f;

	GETKEY_DOWN('U')
	{
		Speed = 5.0f;
		Accel = -0.03f;
	}

	if (WND_H / 2 - m_smpOverTxt->GetCenterDisp().h > m_smpOverTxt->GetPos().y)
	{
		if (Speed > 0.0f)
		{
			Speed += Accel;
		}
		m_smpOverTxt->AddPosY(Speed);
	}

	m_smpOverTxt->Render();
}

void clsSp2dMgrOverTxt::Release()
{
	m_smpOverTxt->Release();
}