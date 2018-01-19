#include "Sp2DJewel.h"


clsJewel::clsJewel()
{
	m_fScale = 1.0f;
}

clsJewel::~clsJewel()
{
}


//============================================================
//	ﾓﾃﾞﾙ作成.
//============================================================
HRESULT clsJewel::InitModel( SPRITE_STATE ss )
{
	float fW = ss.Disp.w;	//表示ｽﾌﾟﾗｲﾄ幅.
	float fH = ss.Disp.h;	//表示ｽﾌﾟﾗｲﾄ高さ.
	float fU = (ss.Base.w / ss.Stride.w)/ss.Base.w;	//一ｺﾏあたりの幅.
	float fV = (ss.Base.h / ss.Stride.h)/ss.Base.h;	//一ｺﾏあたりの高さ.
	
	//中心を基準にするために.
	fW /= 2.0f;
	fH /= 2.0f;

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	Sprite2DVertex vertices[] =
	{
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3(-fW, fH, 0.0f ),	D3DXVECTOR2( 0.0f,	 fV ),	//頂点1(左下).
		D3DXVECTOR3(-fW,-fH, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),	//頂点2(左上).
		D3DXVECTOR3( fW, fH, 0.0f ),	D3DXVECTOR2(   fU,	 fV ),	//頂点3(右下).
		D3DXVECTOR3( fW,-fH, 0.0f ),	D3DXVECTOR2(   fU, 0.0f )	//頂点4(右上).
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//使用法(ﾃﾞﾌｫﾙﾄ).
	bd.ByteWidth		= sizeof( Sprite2DVertex ) * uVerMax;//頂点ｻｲｽﾞ(頂点*4).
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
		MessageBox( NULL, "頂点ﾊﾞｯﾌｧ作成失敗", "clsSprite2D::InitModel", MB_OK );
		return E_FAIL;
	}


	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof( Sprite2DVertex );//ﾃﾞｰﾀ間隔.
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
		MessageBox( NULL, "ｻﾝﾌﾟﾗ作成失敗", "clsJewel::InitModel", MB_OK );
		return E_FAIL;
	}



	return S_OK;
}


//============================================================
//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsJewel::Render()
{
	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mWorld;
	//ﾜｰﾙﾄﾞ変換(表示位置を設定する).
	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.

	//拡縮.
	D3DXMATRIX mScale;
	D3DXMatrixIdentity( &mScale );	//初期化:単位行列作成.
	D3DXMatrixScaling( &mScale, m_fScale, m_fScale, m_fScale );

	//平行移動.
	D3DXMATRIX mTrans;
	D3DXMatrixIdentity( &mTrans );	//初期化:単位行列作成.
	D3DXMatrixTranslation( &mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z );

	//使用するｼｪｰﾀﾞの登録.
	m_pDeviceContext11->VSSetShader( m_pVertexShader, NULL, 0 );
	m_pDeviceContext11->PSSetShader( m_pPixelShader,  NULL, 0 );


	//Matrix合算.
	mWorld = mScale * mTrans;

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE2D_CONSTANT_BUFFER cd;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き方開始時にmap.
	if( SUCCEEDED(
		m_pDeviceContext11->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		//ﾜｰﾙﾄﾞ行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose( &m, &m );	//行列を転置する.
					//行列の計算方法がDirectXとGPUで異なるため.
		cd.mW = m;

		//ﾋﾞｭｰﾎﾟｰﾄの幅、高さを渡す.
		cd.fViewPortWidth = WND_W;
		cd.fViewPortHeight= WND_H;

		//ｱﾙﾌｧ値を渡す.
		cd.fAlpha = m_fAlpha;

		//UV座標.
		//1ｺｱ当たりの割合にｺﾏ番号を掛けて位置を設定する.
		WHSIZE_FLOAT wf;
		wf.w = (m_SState.Base.w / m_SState.Stride.w)/m_SState.Base.w;
		wf.h = (m_SState.Base.h / m_SState.Stride.h)/m_SState.Base.h;
		cd.vUV.x = wf.w * m_fPatternNo.x;
		cd.vUV.y = wf.h * m_fPatternNo.y;

		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cd ), sizeof( cd ) );

		m_pDeviceContext11->Unmap( m_pConstantBuffer, 0 );
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか?.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer );

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof( Sprite2DVertex );
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


void clsJewel::UpDateSpriteSs()
{
	SPRITE_STATE ss = m_SState;
	float fW = ss.Disp.w;	//表示ｽﾌﾟﾗｲﾄ幅.
	float fH = ss.Disp.h;	//表示ｽﾌﾟﾗｲﾄ高さ.
	float fU = (ss.Base.w / ss.Stride.w)/ss.Base.w;	//一ｺﾏあたりの幅.
	float fV = (ss.Base.h / ss.Stride.h)/ss.Base.h;	//一ｺﾏあたりの高さ.

	//中心を基準にするために.
	fW /= 2.0f;
	fH /= 2.0f;

	//板ﾎﾟﾘ(四角形)の頂点を作成.
	Sprite2DVertex vertices[] =
	{
		//頂点座標(x,y,z).					//UV座標( u, v ).
		D3DXVECTOR3(-fW, fH, 0.0f ),	D3DXVECTOR2( 0.0f,	 fV ),	//頂点1(左下).
		D3DXVECTOR3(-fW,-fH, 0.0f ),	D3DXVECTOR2( 0.0f, 0.0f ),	//頂点2(左上).
		D3DXVECTOR3( fW, fH, 0.0f ),	D3DXVECTOR2(   fU,	 fV ),	//頂点3(右下).
		D3DXVECTOR3( fW,-fH, 0.0f ),	D3DXVECTOR2(   fU, 0.0f )	//頂点4(右上).
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//ﾊﾞｯﾌｧ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;		//使用法(ﾃﾞﾌｫﾙﾄ).
	bd.ByteWidth		= sizeof( Sprite2DVertex ) * uVerMax;//頂点ｻｲｽﾞ(頂点*4).
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
		MessageBox( NULL, "頂点ﾊﾞｯﾌｧ作成失敗,", "clsJewel::UpDateSpriteSs", MB_OK );
	}
}
