#include "SpShadow.h"

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む)(\\).
const char SHADER_NAME[] = "Shader\\Mesh.hlsl";
const char TEX_FILE_NAME[] = "Data\\Image\\shadow.png";

//============================================================
//ｺﾝｽﾄﾗｸﾀ.
//============================================================
clsShadow::clsShadow()
{
//	//初期化.
//	ZeroMemory( this, sizeof( clsShadow ) );
	m_fYaw = m_fPitch = m_fRoll = 0.0f;
	m_vPos = { 0.0f, 0.0f, 0.0f };
	m_bDispFlg = true;

	m_pVertexShader = NULL;	
	m_pVertexLayout = NULL;	
	m_pPixelShader = NULL;		
	m_pConstantBuffer = NULL;	
	m_pVertexBuffer = NULL;	
	m_pTexture = NULL;		
	m_pSampleLinear = NULL;

	m_fScale = fDEFAULT_SHADOW_SCALE;
}

//============================================================
//ﾃﾞｽﾄﾗｸﾀ.
//============================================================
clsShadow::~clsShadow()
{
	//ここでは解放しない.
	m_pDevice11 = NULL;
	m_pDeviceContext11 = NULL;
}


//============================================================
//	ﾓﾃﾞﾙ作成.clsSprite
//============================================================
HRESULT clsShadow::InitModel()
{
	float itaW = 0.5f;
	float w, h;
	w = 1.0f;
	h = 1.0f;

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	SpriteVertex vertices[] = 
	{
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3(-itaW, 0.0f,-itaW ),	D3DXVECTOR2( 0.0f,    h ),	//頂点1.
		D3DXVECTOR3(-itaW, 0.0f, itaW ),	D3DXVECTOR2( 0.0f, 0.0f ),	//頂点2.
		D3DXVECTOR3( itaW, 0.0f,-itaW ),	D3DXVECTOR2(	w,	  h ),	//頂点3.
		D3DXVECTOR3( itaW, 0.0f, itaW ),	D3DXVECTOR2(	w, 0.0f )	//頂点4.
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//使用法(ﾃﾞﾌｫﾙﾄ).
	bd.ByteWidth		= sizeof( SpriteVertex ) * uVerMax;//頂点ｻｲｽﾞ(頂点*3).
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
		MessageBox( NULL, "頂点ﾊﾞｯﾌｧ作成失敗", "clsShadow::InitModel", MB_OK );
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
		MessageBox( NULL, "ｻﾝﾌﾟﾗ作成失敗", "clsShadow::InitModel", MB_OK );
		return E_FAIL;
	}

	//ﾃｸｽﾁｬ作成.
	if( FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,		//ﾘｿｰｽを使用するﾃﾞﾊﾞｲｽへのﾎﾟｲﾝﾀ.
			TEX_FILE_NAME,	//ﾌｧｲﾙ名(ﾊﾟｽも必要).
			NULL, NULL,
			&m_pTexture,	//(out)ﾃｸｽﾁｬ.
			NULL ) ) )
	{
		MessageBox( NULL, "ﾃｸｽﾁｬ作成失敗", "clsShadow::InitModel", MB_OK );
		return E_FAIL;
	}


	return S_OK;
}


//============================================================
//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsShadow::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 &vEye )
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld;
	D3DXMATRIX	mScale, mYaw, mPitch, mRoll, mTrans;

	//ﾜｰﾙﾄﾞ変換(表示位置を設定する).
	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.

	//拡縮.
	D3DXMatrixScaling( &mScale,
		m_fScale, m_fScale, m_fScale );

	//回転.
	D3DXMatrixRotationX( &mPitch,	m_fPitch ); //X軸回転.
	D3DXMatrixRotationY( &mYaw,		m_fYaw );	//Y軸回転.
	D3DXMatrixRotationZ( &mRoll,	m_fRoll );	//Z軸回転.

	//平行移動.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	//合成(拡縮x回転x移動).
	mWorld = mScale * mRoll * mYaw * mPitch * mTrans;

	//使用するｼｪｰﾀﾞの登録.
	m_pDeviceContext11->VSSetShader( m_pVertexShader, NULL, 0 );
	m_pDeviceContext11->PSSetShader( m_pPixelShader,  NULL, 0 );

	////ﾋﾞﾙﾎﾞｰﾄﾞ用.
	//D3DXMATRIX CancelRotation = mView;//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	//CancelRotation._41 =
	//	CancelRotation._42 = CancelRotation._43 = 0;//xyzを0にする.
	////CancelRotationの逆行列を求める.
	//D3DXMatrixInverse( &CancelRotation, NULL, &CancelRotation );
	//mWorld = CancelRotation * mWorld;



	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADOW_CONSTANT_BUFFER cb;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
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
		cb.mWVP = m;

		//ｶﾗｰ.
		D3DXVECTOR4 vColor( 1.0f, 1.0f, 1.0f, 1.0f );
		cb.vColor = vColor;

		//ﾃｸｽﾁｬ座標.
		D3DXVECTOR4 texUV( 0.0f, 0.0f, 0.0f, 0.0f );
		
		texUV.x = texUV.y = 0.0f;

		cb.vUV = texUV;



		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cb ), sizeof( cb ) );

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



//============================================================
//	影セット.
//============================================================
void clsShadow::SetShadow(  D3DXVECTOR3 vPos, float fFloor, float fScaleMax )
{
	SetPos( vPos, fFloor );
	ShadScale( vPos.y, fFloor, fScaleMax );
}



//============================================================
//	座標セット.
//============================================================
void clsShadow::SetPos( D3DXVECTOR3 vPos, float fFloor )//Y座標は床の高さで更新.
{
	const float fUp = 0.03125f;//床からどれだけ浮かせる?.
	m_vPos = vPos;
	m_vPos.y = fFloor + fUp;
}

//============================================================
//	影サイズ.
//============================================================
void clsShadow::ShadScale( float fPosY,float fFloor, float fScaleMax )
{
	const float fMAX_SCALE_HEIGHT = 0.75f;//影が小さくならない地上からの高さ.

	const float fSHADOW_Y = fFloor;
	const float fSHADOW_Y_2 = fSHADOW_Y - fMAX_SCALE_HEIGHT;

	//ｻｲｽﾞ初期化(最大ｻｲｽﾞ).
	m_fScale = fScaleMax;

	//大きくなりすぎないためのIF.
	if( fPosY >= fSHADOW_Y ){
		//地面との距離.
		float fSadLgth = fPosY - fSHADOW_Y_2;

		//平方根で小さくなりすぎないようにする.
		fSadLgth = sqrtf( fSadLgth );

		//			逆数.
		m_fScale *= 1.0f / fSadLgth ;//実体が高いほど影が小さくなる.
	}
}
