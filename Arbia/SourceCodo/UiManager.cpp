#include"UiManager.h"

//定数.
const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };

const WHSIZE_FLOAT ICONSIZE = { 180.0f, 180.0f };

const WHSIZE_FLOAT ICONPOS = { 980.0f, 5.0f };

const WHSIZE_FLOAT NUMSIZE = { 64.0f, 64.0f };

clsUiManagar::clsUiManagar()
{
	//ZeroMemory(this,sizeof(this));
}

clsUiManagar::~clsUiManagar()
{
	Delete();
}

void clsUiManagar::Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	m_smpBlack = make_unique<clsSprite2D>();  //暗転用.

	m_smpWhite = make_unique<clsSprite2D>(); //暗転用.

	//複数のシーンで使う場合はここ.

	m_smpTitleScene = make_unique<clsSp2dMgrTitle>();	 //タイトル用.

	m_smpMainScene = make_unique<clsSp2dMgrMain>();
	m_smpActTxt = make_unique<clsActTxt>();	//ジャンプ、攻撃ねぇでやんす.
	m_smpXButton = make_unique<clsSprite2D>();	//Xボタンねぇでやんす.
	m_smpAButton = make_unique<clsSprite2D>();	//Aボタン.

	//ゲームオーバ用.
	m_smpOverScene = make_unique<clsSp2dMgrOver>();
	m_smpOverScene->Init(pDevice11, pContext11);

	//リザルト用.
	m_smpResult = make_unique<clsSp2dMgrReslt>();

	//エンディング.
	m_smpEndingScene = make_unique<clsSp2dMgrEnding>();

	m_smpBlack->Init(pDevice11, pContext11, "Data\\Image\\Black.png");

	m_smpWhite->Init(pDevice11, pContext11, "Data\\Image\\White.png");

	//タイトルシーン.
	m_smpTitleScene->Init(pDevice11, pContext11);

	//メインシーン.
	m_smpMainScene->Init(hWnd, pDevice11, pContext11);

	m_smpActTxt->Init(pDevice11, pContext11, "Data\\Image\\MainTxt.png");

	m_smpXButton->Init(pDevice11, pContext11, "Data\\Image\\XButtun.png");
	m_smpAButton->Init(pDevice11, pContext11, "Data\\Image\\AButtun.png");

	//リザルト用.
	m_smpResult->Init(hWnd, pDevice11, pContext11);

	//エンディング用.
	m_smpEndingScene->Init(pDevice11, pContext11);
	InitPos();
}
void clsUiManagar::RenderTitle()
{
	RenderMain();
	m_smpTitleScene->Render();
	BlackOut();
}
void clsUiManagar::RenderMain()
{
	m_smpMainScene->Render();
	BlackOut();
}
void clsUiManagar::RenderOver()
{
	m_smpOverScene->Render();
	BlackOut();
}
void clsUiManagar::RenderResult()
{
	RenderMain();
	m_smpResult->Render();
	BlackOut();
}
void clsUiManagar::RenderEnding()
{
	m_smpEndingScene->Render();
	BlackOut();
}

void clsUiManagar::SceneTitle()
{
}
void clsUiManagar::SceneMain()
{
	GETKEY_DOWN('U')
		m_smpMainScene->ActTxtMoveIn();
	GETKEY_DOWN('Y')
		m_smpMainScene->ActTxtMoveOut();

	m_smpActTxt->Move();
}

void clsUiManagar::SceneOver()
{
}
void clsUiManagar::SceneResult()
{
	m_smpResult->UpDate();
}
void clsUiManagar::SceneEnding()
{
}
void clsUiManagar::ChangeTitleInit()
{
	m_smpTitleScene->InitSetPos();
	ChangeMainInit();
	m_smpMainScene->ActTxtMoveOut();
}
void clsUiManagar::ChangeMainInit()
{
	m_smpMainScene->InitSetPos();
	m_smpMainScene->ActTxtMoveIn();
}

void clsUiManagar::ChangeResultInit()
{
	InitPos();
	m_smpResult->StartSet();

	m_smpAButton->SetAlpha(1.0f);
}
void clsUiManagar::ChangeOverInit()
{
	InitPos();

	//	m_smpGameOverTxt->SetPosX(CENTER_POS.w - m_smpGameOverTxt->GetCenterDisp().w);
	//	m_smpGameOverTxt->SetPosY(-1 * (m_smpGameOverTxt->GetSs().Disp.h));

	//	m_smpAButton->SetAlpha(1.0f);
}
void clsUiManagar::ChangeEndingInit()
{
	InitPos();
}

void clsUiManagar::BlackStart(float ChaAmo, bool Color)
{
	if (m_smpBlack->m_BlackMode == Idle && Color)
	{
		m_fChangePoint = ChaAmo;
		m_bColor = Color;
		m_smpBlack->m_BlackMode = In;
		m_smpBlack->SetAlpha(0.0f);
	}
	else if (m_smpWhite->m_BlackMode == Idle && !Color)
	{
		m_fChangePoint = ChaAmo;
		m_bColor = Color;
		m_smpWhite->m_BlackMode = In;
		m_smpWhite->SetAlpha(0.0f);
	}
}
void clsUiManagar::BlackOut()
{
	if (m_bColor)
	{
		if (m_smpBlack->m_BlackMode != Idle)
		{
			if (m_smpBlack->m_BlackMode == Change)
			{
				m_smpBlack->m_BlackMode = Out;
			}
			float tmp = m_smpBlack->GetAlpha();
			m_smpBlack->SetPos(0.0f, 0.0f);
			if (m_smpBlack->m_BlackMode == In){
				m_smpBlack->AddAlpha(m_fChangePoint);
				if (tmp > 1.0f){
					m_smpBlack->m_BlackMode = Change;
				}
			}
			else
			{
				if (m_smpBlack->m_BlackMode == Out)
				{
					if (m_smpBlack->GetAlpha() > 0.0f)
					{
						m_smpBlack->AddAlpha(-1 * m_fChangePoint);
					}
					else
					{
						m_smpBlack->m_BlackMode = Idle;
						m_smpBlack->InitSetPos();
					}
				}
			}
			m_smpBlack->Render();
		}
	}
	else{
		if (m_smpWhite->m_BlackMode != Idle)
		{
			m_smpWhite->SetPos(0.0f, 0.0f);
			if (m_smpWhite->m_BlackMode == Change)
			{
				m_smpWhite->m_BlackMode = Out;
			}
			float tmp = m_smpWhite->GetAlpha();
			if (m_smpWhite->m_BlackMode == In)
			{
				m_smpWhite->AddAlpha(m_fChangePoint);
				if (tmp > 1.0f)
				{
					m_smpWhite->m_BlackMode = Change;
				}
			}
			else
			{
				if (m_smpWhite->m_BlackMode == Out)
				{
					if (m_smpWhite->GetAlpha() > 0.0f)
					{
						m_smpWhite->AddAlpha(-1 * m_fChangePoint);
					}
					else
					{
						m_smpWhite->m_BlackMode = Idle;
						m_smpWhite->InitSetPos();
					}
				}
			}
			m_smpWhite->Render();
		}
	}
}

void clsUiManagar::InitPos()
{
	m_smpBlack->InitSetPos();	//暗転用.
	m_smpActTxt->InitSetPos();

	m_smpTitleScene->InitSetPos();	//タイトル用.

	m_smpActTxt->InitSetPos();	//ジャンプ、攻撃ねぇでやんす.
	m_smpXButton->InitSetPos();	//Xボタンねぇでやんす.
	m_smpAButton->InitSetPos();	//Aボタン.
	//ゲームオーバ用.
	//	m_smpGameOverTxt->InitSetPos();	//ゲームオーバーテキストねぇでやんす.
	m_smpOverScene->InitSetPos();	//ゲームオーバーテキストねぇでやんす.

	//リザルト用.
	m_smpResult->InitSetPos();	//リザルトテキストねぇでやんす.

	//エンディング.
	m_smpEndingScene->InitSetPos();
}
void clsUiManagar::Delete()
{
#if 1

	if (m_smpBlack) {
		m_smpBlack.reset();
	}
	if (m_smpWhite) {
		m_smpWhite.reset();
	}

	//タイトル用.
	//if (m_smpTitleLogo){
	//	m_smpTitleLogo.reset();
	//}
	//if (m_smpPushTxt){
	//	m_smpPushTxt.reset();
	//}

	if (m_smpMainScene){
		m_smpMainScene.reset();
	}

	if (m_smpActTxt){
		m_smpActTxt.reset();
	}

	if (m_smpXButton){
		m_smpXButton.reset();
	}

	if (m_smpAButton){
		m_smpAButton.reset();
	}

	//ゲームオーハ゛ー用.
	if (m_smpOverScene){
		m_smpOverScene.reset();
	}

	//リザルト用.
	if (m_smpResult){
		m_smpResult.reset();
	}

	//エンディング用.
	if (m_smpEndingScene){
		m_smpEndingScene.reset();
	}

#endif // 0
}