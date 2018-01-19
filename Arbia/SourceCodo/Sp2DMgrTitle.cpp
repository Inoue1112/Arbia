#include "Sp2DMgrTitle.h"

//定数.
namespace ConstantTitle
{
	const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };

	const float ADJUSTTITLELOGOH = 100.0f;

	const float AlphaPoint = -0.01f;
}

clsSp2dMgrTitle::clsSp2dMgrTitle()
{
}

clsSp2dMgrTitle::~clsSp2dMgrTitle()
{
	Release();
}

void clsSp2dMgrTitle::Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_smpTitleLogo = make_unique<clsSprite2D>();	//タイトルロゴねぇでやんす.
	m_smpPushTxt = make_unique<clsPushTxt>();	//PUSH KETねぇでやんす.
	m_smpPushTxt->Init(pDevice11, pContext11);

	//ロゴサイズを2倍にする.
	m_smpTitleLogo->Init(pDevice11, pContext11, "Data\\Image\\TitleLogo.png");
	m_smpTitleLogo->MulDisp(2.0f);
	m_smpTitleLogo->UpDateSpriteSs();
}

void clsSp2dMgrTitle::InitSetPos()
{
	m_smpTitleLogo->InitSetPos();
	m_smpPushTxt->InitSetPos();
	m_smpTitleLogo->SetAlpha(1.0f);
	m_bTransmissionFlg = false;
}

void clsSp2dMgrTitle::Render()
{
	m_smpTitleLogo->SetPos(ConstantTitle::CENTER_POS.w - m_smpTitleLogo->GetCenterDisp().w,
		ConstantTitle::CENTER_POS.h - m_smpTitleLogo->GetCenterDisp().h - ConstantTitle::ADJUSTTITLELOGOH);
	m_smpTitleLogo->Render();

	m_smpPushTxt->Render();
	Transmission();
}
void clsSp2dMgrTitle::Transmission()
{
	if (m_bTransmissionFlg)
	{
		m_smpTitleLogo->AddAlpha(ConstantTitle::AlphaPoint);
		m_smpPushTxt->AddAlpha(ConstantTitle::AlphaPoint);
	}
}

void clsSp2dMgrTitle::Release()
{
	m_smpTitleLogo.reset();

	m_smpPushTxt.reset();

}