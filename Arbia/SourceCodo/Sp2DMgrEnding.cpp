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

	m_smpEndRoll = make_unique<clsSprite2D>();
	m_smpEndRoll->Init(pDevice11, pContext11, "Data\\Image\\StaffLool.png");

	m_smpThank = make_unique<clsSprite2D>();
	m_smpThank->Init(pDevice11, pContext11, "Data\\Image\\EndingThankYou.png");
}
void clsSp2dMgrEnding::InitSetPos()
{
	//m_smpEndingNameTxt->
	//m_smpEndingSiteTxt->
	//m_smpEndingThankTxt->
	m_bEndingEnd = false;
	m_bPushButtun = false;
	m_smpEndRoll->SetDispW(WND_W);
	m_smpEndRoll->UpDateSpriteSs();

	m_smpThank->SetDispW(WND_W);
	m_smpThank->UpDateSpriteSs();

	m_smpEndRoll->SetPos(0.0f, WND_H);

	m_smpThank->SetPos(0.0f, m_smpEndRoll->GetSs().Disp.h + m_smpEndRoll->GetPos().y);
}
void clsSp2dMgrEnding::Render()
{
	if (m_smpThank->GetPos().y > 0.0f)
	{
		if (m_bPushButtun)
		{
			m_smpEndRoll->AddPosY(ConstantEnding::PUSH_SCL_SPD);
		}
		else
		{
			m_smpEndRoll->AddPosY(ConstantEnding::NONPUSH_SCL_SPD);
		}
	}
	else
	{
		m_bEndingEnd = true;
	}

	m_smpThank->SetPosY(m_smpEndRoll->GetSs().Disp.h + m_smpEndRoll->GetPos().y);

	m_smpEndRoll->Render();

	m_smpThank->Render();

	if (m_bEndingEnd)
	{
		m_smpPushTxt->Render();
	}
}

void clsSp2dMgrEnding::Release()
{
	SAFE_RELEASE(m_smpEndRoll);
	SAFE_RELEASE(m_smpPushTxt);
}