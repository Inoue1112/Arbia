#include "Sp2DMgrEnding.h"
//íËêî.
namespace ConstantEnding
{
	const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };
	const float NONPUSH_SCL_SPD = -7.0f;
	const float PUSH_SCL_SPD = -20.0f;
	const float THANK_TXT_POS_Y = 40.0f;
}

clsSp2dMgrEnding::clsSp2dMgrEnding()
{
}

clsSp2dMgrEnding::~clsSp2dMgrEnding()
{
}

void clsSp2dMgrEnding::Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpPushTxt->Init(pDevice11, pContext11);

	m_smpEndingNameTxt = make_unique<clsSprite2D>();
	m_smpEndingSiteTxt = make_unique<clsSprite2D>();
	m_smpEndingThankTxt = make_unique<clsSprite2D>();
	m_smpEndingNameTxt->Init(pDevice11, pContext11, "Data\\Image\\EndingNameList.png");
	m_smpEndingSiteTxt->Init(pDevice11, pContext11, "Data\\Image\\EndingSiteList.png");
	m_smpEndingThankTxt->Init(pDevice11, pContext11, "Data\\Image\\EndingThankYou.png");
}
void clsSp2dMgrEnding::InitSetPos()
{
	//m_smpEndingNameTxt->
	//m_smpEndingSiteTxt->
	//m_smpEndingThankTxt->
	m_smpPushTxt->InitSetPos();
	m_bEndingEnd = false;
	m_bPushButtun= false;
	m_smpEndingNameTxt->SetDispW(WND_W);
	m_smpEndingNameTxt->UpDateSpriteSs();
	m_smpEndingSiteTxt->SetDispW(WND_W);
	m_smpEndingSiteTxt->UpDateSpriteSs();
	m_smpEndingThankTxt->SetDispW(WND_W);
	m_smpEndingThankTxt->UpDateSpriteSs();

	m_smpEndingNameTxt->SetPos(0.0f,WND_H);
	m_smpEndingSiteTxt->SetPos(0.0f,WND_H+m_smpEndingNameTxt->GetSs().Disp.h);
	m_smpEndingThankTxt->SetPos(0.0f,WND_H+m_smpEndingNameTxt->GetSs().Disp.h+m_smpEndingSiteTxt->GetSs().Disp.h+ConstantEnding::THANK_TXT_POS_Y);

}
void clsSp2dMgrEnding::Render()
{
	if (m_smpEndingThankTxt->GetPos().y > WND_H/2 - m_smpEndingThankTxt->GetSs().Disp.h/2 )
	{
		if (m_bPushButtun)
		{
			m_smpEndingNameTxt->AddPosY(ConstantEnding::PUSH_SCL_SPD);
			m_smpEndingSiteTxt->AddPosY(ConstantEnding::PUSH_SCL_SPD);
			m_smpEndingThankTxt->AddPosY(ConstantEnding::PUSH_SCL_SPD);
		}
		else
		{
			m_smpEndingNameTxt->AddPosY(ConstantEnding::NONPUSH_SCL_SPD);
			m_smpEndingSiteTxt->AddPosY(ConstantEnding::NONPUSH_SCL_SPD);
			m_smpEndingThankTxt->AddPosY(ConstantEnding::NONPUSH_SCL_SPD);
		}
	}
	else
	{
		m_bEndingEnd = true;
	}

	m_smpEndingNameTxt->Render();
	m_smpEndingSiteTxt->Render();
	m_smpEndingThankTxt->Render();
	if (m_bEndingEnd)
	{
		m_smpPushTxt->Render();
	}
}

void clsSp2dMgrEnding::Release()
{
	SAFE_RELEASE(m_smpEndingNameTxt);
	SAFE_RELEASE(m_smpEndingSiteTxt);
	SAFE_RELEASE(m_smpEndingThankTxt);
	SAFE_RELEASE(m_smpPushTxt);
}
