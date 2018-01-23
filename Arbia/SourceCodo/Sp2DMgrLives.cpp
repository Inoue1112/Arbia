#include "Sp2DMgrLives.h"


const LPSTR FILE_PATH_ICON = "Data\\Image\\Icon.png";

//const float fLIVES_X = static_cast<float>( WND_W - 128 );

namespace MainSceneLives{
	const WHSIZE_FLOAT ICONPOS = { 1050.0f, 5.0f };
	const WHSIZE_FLOAT ICONSIZE = { 108.0f, 108.0f };

	const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };
}

//êîéöÇÉAÉCÉRÉìÇ©ÇÁÇ«ÇÍÇæÇØÇ∏ÇÁÇ∑Ç©.
const float fNUM_OFFSET_FROM_ICON_X = 102.0f/*fLIVES_X - MainSceneLives::ICONPOS.w*/;
const float fNUM_OFFSET_FROM_ICON_Y = 27.0f/*MainSceneLives::ICONSIZE.h / 2.0f / 2.0f*/;

clsLives::clsLives()
{
	m_smpIcon = nullptr;
	m_smpLives = nullptr;
}

clsLives::~clsLives()
{
	Release();
}
void clsLives::Release()
{
	m_smpLives.reset();
	m_smpIcon.reset();
}

//ç≈èâÇÃèâä˙âª.
void clsLives::Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )
{
	m_smpIcon = make_unique<clsSprite2D>();
	m_smpIcon->Init(pDevice11, pContext11, FILE_PATH_ICON );
	m_smpIcon->SetDispW( MainSceneLives::ICONSIZE.w );
	m_smpIcon->SetDispH( MainSceneLives::ICONSIZE.h );
	m_smpIcon->UpDateSpriteSs();

	m_smpLives = make_unique<clsNumGrp>();
	m_smpLives->Create( pDevice11, pContext11 );
	m_smpLives->SetPos( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	ReSet();
}
	
//ìÒâÒñ⁄à»ç~ÇÃèâä˙âª.
void clsLives::ReSet()
{
	m_vPos = D3DXVECTOR3( MainSceneLives::ICONPOS.w, MainSceneLives::ICONPOS.h, 0.0f );
	SetPos( m_vPos );
//	m_smpIcon->SetPos( MainSceneLives::ICONPOS.w, MainSceneLives::ICONPOS.h );
	m_smpLives->ReSet();
	SetNum( 0 );
}

void clsLives::Update()
{
	m_smpLives->Move();
}
void clsLives::Render()
{
	m_smpLives->Render();
	m_smpIcon->Render();
}

void clsLives::SetPos( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;
	m_smpIcon->SetPos( m_vPos );
	D3DXVECTOR3 tmpPos = m_vPos;
	tmpPos.x += fNUM_OFFSET_FROM_ICON_X;
	tmpPos.y += fNUM_OFFSET_FROM_ICON_Y;
	m_smpLives->SetPos( tmpPos );
}
void clsLives::AddPos( D3DXVECTOR3 vPos )
{
	m_vPos += vPos;
	SetPos( m_vPos );
}
void clsLives::AddPosY( float fPos )
{
	m_vPos.y += fPos;
	SetPos( m_vPos );
}
