#include "CheckPoint.h"

const int iSTAY_TIME = 120;


const float fFLOAT_MAX = 0.25f * 0.25f * 0.25f;//最大速度.
const float fACC = fFLOAT_MAX * fFLOAT_MAX * 2.0f;//加速度.

clsCheckPoint::clsCheckPoint()
{
	m_sFileName = nullptr;
}

clsCheckPoint::~clsCheckPoint()
{
}

//初期化.
HRESULT clsCheckPoint::Create( ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPSTR sFileName )
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;
	m_sFileName = sFileName;

	//ｼｪｰﾀﾞ作成.
	if( FAILED( InitShader() ) ){
		return E_FAIL;
	}
	//板ﾎﾟﾘｺﾞﾝ作成.
	if( FAILED( InitModel() ) ){
		return E_FAIL;
	}

	Init();

	return S_OK;
}

////ﾓﾃﾞﾙ作成.
HRESULT clsCheckPoint::InitModel()
{
	float itaW = 0.5f;
	float w, h;
	w = h = ( 1.0f / 8.0f );
	w = h = 1.0f;

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	SpriteVertex vertices[] = 
	{
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3(-itaW,-itaW, 0.0f ),	D3DXVECTOR2( 0.0f,    h ),	//頂点1.
		D3DXVECTOR3(-itaW, itaW, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),	//頂点2.
		D3DXVECTOR3( itaW,-itaW, 0.0f ),	D3DXVECTOR2(	w,	  h ),	//頂点3.
		D3DXVECTOR3( itaW, itaW, 0.0f ),	D3DXVECTOR2(	w, 0.0f )	//頂点4.
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//使用法(ﾃﾞﾌｫﾙﾄ).
	bd.ByteWidth		= sizeof( SpriteVertex ) * uVerMax;//頂点ｻｲｽﾞ(頂点*4).
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;	//頂点ﾊﾞｯﾌｧとして扱う.
	bd.CPUAccessFlags	= 0;						//CPUからはｱｸｾｽしない.
	bd.MiscFlags		= 0;						//その他のﾌﾗｸﾞ(未使用).
	bd.StructureByteStride	= 0;					//構造体ｻｲｽﾞ(未使用).

	//ｻﾌﾞﾘｿｰｽﾃﾞｰﾀ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem	= vertices;	//板ﾎﾟﾘの頂点をｾｯﾄ.

	//頂点ﾊﾞｯﾌｧの作成.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer ) ) )
	{
		MessageBox( NULL, "頂点ﾊﾞｯﾌｧ作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}


	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof( SpriteVertex );//ﾃﾞｰﾀ間隔.
	UINT offset = 0;//開始位置.
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, 
		&m_pVertexBuffer, &stride, &offset );


	//ﾃｸｽﾁｬ用のｻﾝﾌﾟﾗｰ構造体.
	D3D11_SAMPLER_DESC	SamDesc;
	ZeroMemory( &SamDesc, sizeof( SamDesc ) );
	//ﾘﾆｱﾌｨﾙﾀｰ(線型補間).
	//	POINT:高速だが粗い.
	SamDesc.Filter	= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ.
	//	WRAP:繰り返し.
	//	MIRROR	:反転繰り返し.
	//	CLAMP	:端の模様を引き延ばす.
	//	BORDER	;別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//ｻﾝﾌﾟﾗｰ作成.
	if( FAILED(
		m_pDevice11->CreateSamplerState(
			&SamDesc, &m_pSampleLinear ) ) )//(out)ｻﾝﾌﾟﾗｰ.
	{
		MessageBox( NULL, "ｻﾝﾌﾟﾗ作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}

	//ﾃｸｽﾁｬ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,		//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽへのﾎﾟｲﾝﾀ.
			m_sFileName,	//ﾌｧｲﾙ名(ﾊﾟｽも必要).
			NULL, NULL,
			&m_pTexture,	//(out)ﾃｸｽﾁｬ.
			NULL ) ) )
	{
		MessageBox( NULL, "ﾃｸｽﾁｬ作成失敗", "InitModel()", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}


void clsCheckPoint::Init()
{
	m_bMobe = false;
	m_enMode = enM_AHEAD;
	m_iTimer = 0;
	m_fFloat = fFLOAT_MAX;
	m_bAcc = false;
}


bool clsCheckPoint::Stay()
{
	//戻り値.
	bool bReturn = false;

	//待機.
	m_iTimer ++;
	if( m_iTimer > iSTAY_TIME ){
		m_enMode = enM_OUT;
		bReturn = true;
	}

	//ふわふわ.
	if( m_bAcc ){
		m_fFloat += fACC; 
	}
	else{
		m_fFloat -= fACC;
	}

	//オーバーしたら.
	if( m_fFloat > fFLOAT_MAX ){
		m_fFloat = fFLOAT_MAX ;
		m_bAcc = false;
	}
	else if( m_fFloat < -fFLOAT_MAX ){
		m_fFloat = -fFLOAT_MAX ;
		m_bAcc = true;
	}

	m_vPos.y += m_fFloat;

	return bReturn;
}