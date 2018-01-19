#include "Sprite.h"

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む)(\\).
const char SHADER_NAME[] = "Shader\\Mesh.hlsl";


//============================================================
//ｺﾝｽﾄﾗｸﾀ.
//============================================================
clsSprite::clsSprite()
{
	//初期化.
//	ZeroMemory( this, sizeof( clsSprite ) );
	m_pVertexShader		= nullptr;	
	m_pVertexLayout		= nullptr;	
	m_pPixelShader		= nullptr;		
	m_pConstantBuffer	= nullptr;
	m_pVertexBuffer		= nullptr;
	m_pTexture			= nullptr;		
	m_pSampleLinear		= nullptr;

	m_fScale = 1.0f;
}

//============================================================
//ﾃﾞｽﾄﾗｸﾀ.
//============================================================
clsSprite::~clsSprite()
{
	//ここでは解放しない.
	m_pDevice11 = NULL;
	m_pDeviceContext11 = NULL;
}

//============================================================
//初期化.
//============================================================
HRESULT clsSprite::Init( ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	//ｼｪｰﾀﾞ作成.
	if( FAILED( InitShader() ) ){
		return E_FAIL;
	}
	//板ﾎﾟﾘｺﾞﾝ作成.
	if( FAILED( InitModel() ) ){
		return E_FAIL;
	}





	return S_OK;
}

//============================================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL:HIGE-LEVEL-SHADER-LANGUAGE.
//============================================================
HRESULT clsSprite::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;

	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION; 
#endif//#ifdef _DEBUG


	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			NULL,			//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			NULL,			//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｰｽへのﾎﾟｲﾝﾀ(未使用).
			"VS_Ita",			//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"vs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用).
			NULL,			//ｽﾚｯﾄﾞﾎﾟﾝﾌﾟｲﾝﾀｰﾌｪｰｽへのﾎﾟｲﾝﾀ(未使用).
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			NULL ) ) )		//戻り値へのﾎﾟｲﾝﾀ(未使用).
	{
		MessageBox(NULL, "hlsl(vs)読み込み失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader) ) )//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		MessageBox( NULL, "vs作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{
			"POSITION",						//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*3).
			0,
			0,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",					//ﾃｸｽﾁｬ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2).
			0,
			12,							//ﾃｸｽﾁｬﾃﾞｰﾀの開始位置(頂点ﾃﾞｰﾀがfloat型*3=12biteなので+12).
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );//.

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの作成.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ) ) )//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		MessageBox(NULL, "頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );





	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			NULL,
			NULL,
			"PS_Ita",			//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"ps_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,
			NULL,
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,
			NULL ) ) )
	{
		MessageBox( NULL, "hlsl(ps)読み込み失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pPixelShader ) ) )//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		MessageBox( NULL, "ps作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );//ﾌﾞﾛﾌﾞ解放.


	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(定数)ﾊﾞｯﾌｧ作成　ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//ここでは変換行列渡し用.
	//ｼｪｰﾀﾞにWorld,View,Projection行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;;			//ｺﾝｽﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth	= sizeof( SPRITESHADER_CONSTANT_BUFFER );//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;				//書き込みでｱｸｾｽ.
	cb.MiscFlags	= 0;					//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride	= 0;			//構造体のｻｲｽﾞ(未使用)
	cb.Usage		= D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer ) ) )
	{
		MessageBox( NULL, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "ｴﾗｰ", MB_OK );
		return E_FAIL;
	}




	return S_OK;
}

//============================================================
//	ﾓﾃﾞﾙ作成.
//============================================================
HRESULT clsSprite::InitModel()
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
			"Data\\Image\\CheckPointSet\\Chi.png",	//ﾌｧｲﾙ名(ﾊﾟｽも必要).
			NULL, NULL,
			&m_pTexture,	//(out)ﾃｸｽﾁｬ.
			NULL ) ) )
	{
		MessageBox( NULL, "ﾃｸｽﾁｬ作成失敗w", "InitModel()", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}

//============================================================
//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsSprite::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 &vEye )
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld, mScale, mTrans;
	//ﾜｰﾙﾄﾞ変換(表示位置を設定する).
	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.

	//拡縮.
	D3DXMatrixScaling( &mScale, m_fScale, m_fScale, m_fScale );

	//平行移動.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	//合算.
	mWorld = mScale * mTrans;

	//使用するｼｪｰﾀﾞの登録.
	m_pDeviceContext11->VSSetShader( m_pVertexShader, NULL, 0 );
	m_pDeviceContext11->PSSetShader( m_pPixelShader,  NULL, 0 );

	//ﾋﾞﾙﾎﾞｰﾄﾞ用.
	D3DXMATRIX CancelRotation = mView;//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	CancelRotation._41 =
		CancelRotation._42 = CancelRotation._43 = 0;//xyzを0にする.
	//CancelRotationの逆行列を求める.
	D3DXMatrixInverse( &CancelRotation, NULL, &CancelRotation );
	mWorld = CancelRotation * mWorld;



	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITESHADER_CONSTANT_BUFFER cd;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き方開始時にmap.
	if( SUCCEEDED(
		m_pDeviceContext11->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ﾜｰﾙﾄﾞ,ｶﾒﾗ,ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose( &m, &m );	//行列を転置する.
					//行列の計算方法がDirectXとGPUで異なるため.
		cd.mWVP = m;

		//ｶﾗｰ.
		D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
		cd.vColor = vColor;

		//ﾃｸｽﾁｬ座標.
		D3DXVECTOR4 texUV( 0.0f, 0.0f, 0.0f, 0.0f );
//		if( m_AnimCount % 8 == 0 ){
//			texUV.x += 1.0f / 8.0f;
//			m_AnimCount = 0;
//			if( texUV.x >= 1.0f ){
//				texUV.x = 0.0f;
//				texUV.y += 1.0f / 8.0f;
//			}
//		}
//		m_AnimCount ++;

		cd.vUV = texUV;



		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cd ), sizeof( cd ) );

		m_pDeviceContext11->Unmap( m_pConstantBuffer, 0 );
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか?.
	m_pDeviceContext11->VSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );
	m_pDeviceContext11->PSSetConstantBuffers(
		2, 1, &m_pConstantBuffer );

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof( SpriteVertex );
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pDeviceContext11->IASetInputLayout( m_pVertexLayout );

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear );	//ｻﾝﾌﾟﾗｰをｾｯﾄ.
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pTexture );		//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成&設定.
	SetBlend( true );


	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pDeviceContext11->Draw( 4, 0 );

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを無効にする.
	SetBlend( false );

}


