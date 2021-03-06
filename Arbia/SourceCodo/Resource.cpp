#include "Resource.h"

clsResource::clsResource()
{
	m_hWnd = NULL;
	m_pDevice11 = NULL;
	m_pCotext11 = NULL;
	m_ppStaticModels = NULL;
	ZeroMemory( m_FilePath,sizeof( m_FilePath ) );
}

clsResource::~clsResource()
{
	for( int i=0; i<enST_MODEL_MAX; i++ ){
		ReleaseStaticModel( (enStaticModel)i );
	}
	for( int i=0; i<enSK_MODEL_MAX; i++ ){
		ReleaseSkinModel( (enSkinModel)i );
	}
	//ｽｷﾝﾒｯｼｭのﾎﾟｲﾝﾀ領域を解放.
	if( m_ppSkinModels ){
		delete[] m_ppSkinModels;
		m_ppSkinModels = NULL;
	}
	//ｽﾀﾃｨｯｸﾒｯｼｭのﾎﾟｲﾝﾀ領域を解放.
	if( m_ppStaticModels ){
		delete[] m_ppStaticModels;
		m_ppStaticModels = NULL;
	}


	m_pCotext11 = NULL;
	m_pDevice11 = NULL;
	m_hWnd = NULL;
}


//==================================================.
//	初期化.
//==================================================.
HRESULT clsResource::InitStaticModel( HWND hWnd,
			ID3D11Device* pDevice,
			ID3D11DeviceContext* pContext)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice;
	m_pCotext11 = pContext;
	//ｽﾀﾃｨｯｸﾒｯｼｭのﾎﾟｲﾝﾀ領域を確保.
	m_ppStaticModels = new clsDX9Mesh*[ enST_MODEL_MAX ];
	for( int i=0; i<enST_MODEL_MAX; i++ ){
		m_ppStaticModels[i] = NULL;
	}

	return S_OK;
}
HRESULT clsResource::InitSkinModel( HWND hWnd,
			ID3D11Device* pDevice,
			ID3D11DeviceContext* pContext)
{
	m_Si.hWnd = hWnd;
	m_Si.pDevice = pDevice;
	m_Si.pDeviceContext = pContext;
	//ｽｷﾝﾒｯｼｭのﾎﾟｲﾝﾀ領域を確保.
	m_ppSkinModels = new clsD3DXSKINMESH*[ enSK_MODEL_MAX ];
	for( int i=0; i<enSK_MODEL_MAX; i++ ){
		m_ppSkinModels[i] = NULL;
	}

	return S_OK;
}



//==================================================.
//	ﾓﾃﾞﾙ読込.
//==================================================.
// Static Model.
HRESULT clsResource::CreateStaticModel(
	LPSTR fileName, enStaticModel enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//既にﾃﾞｰﾀがあるか.
	if( m_ppStaticModels[ enModel ] != NULL ){
		return E_FAIL;
	}

	//ﾓﾃﾞﾙ読込.
	m_ppStaticModels[ enModel ] = new clsDX9Mesh;
	m_ppStaticModels[ enModel ]->Init(
		m_hWnd, m_pDevice11, m_pCotext11, fileName );

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::CreateSkinModel( 
	LPSTR fileName, enSkinModel enModel )
{
	//列挙体の範囲内か.
	if ( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}

	//既にﾃﾞｰﾀがあるか.
	if ( m_ppSkinModels[ enModel ] != NULL ){
		return E_FAIL;
	}

	//ﾓﾃﾞﾙ読込.
	m_ppSkinModels[ enModel ] = new clsD3DXSKINMESH;
	m_ppSkinModels[ enModel ]->Init( &m_Si );
	m_ppSkinModels[ enModel ]->CreateFromX( fileName );

	return S_OK;
}

//==================================================.
//	解放.
//==================================================.
//	Static Model.
HRESULT clsResource::ReleaseStaticModel( enStaticModel enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//既にﾃﾞｰﾀがあるか.
	if( m_ppStaticModels[ enModel ] == NULL ){
		return E_FAIL;
	}

	delete m_ppStaticModels[ enModel ];
	m_ppStaticModels[ enModel ] = NULL;

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::ReleaseSkinModel( enSkinModel enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}
	//既にﾃﾞｰﾀがあるか.
	if( m_ppSkinModels[ enModel ] == NULL ){
		return E_FAIL;
	}

	delete m_ppSkinModels[ enModel ];
	m_ppSkinModels[ enModel ] = NULL;

	return S_OK;
}

//==================================================.
//	ﾓﾃﾞﾙのﾎﾟｲﾝﾀ取得.
//==================================================.
//	Static Model.
clsDX9Mesh* clsResource::GetStaticModels(
	enStaticModel enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return NULL;
	}
	//既にﾃﾞｰﾀがあるか.
	if( m_ppStaticModels[ enModel ] == NULL ){
		return NULL;
	}
	
	//ﾓﾃﾞﾙのﾎﾟｲﾝﾀ返す.
	return m_ppStaticModels[ enModel ];
	//return (clsDX9Mesh*)S_OK;	//大事故.
}
//	Skin Model.
clsD3DXSKINMESH* clsResource::GetSkinModels(enSkinModel enModel)
{
	//列挙体の範囲内?.
	if ( !IsRangeSkinModel( enModel ) ){
		return NULL;
	}

	//既にﾃﾞｰﾀがあるか.
	if ( m_ppSkinModels[ enModel ] == NULL ){
		return NULL;
	}

	//ﾓﾃﾞﾙのﾎﾟｲﾝﾀ返す.
	return m_ppSkinModels[ enModel ];
}


//==================================================.
//	範囲内かﾁｪｯｸする関数.
//==================================================.
bool clsResource::IsRangeStaticModel( enStaticModel enModel )
{
	if( 0 <= enModel && enModel < enST_MODEL_MAX ){
		return true;//範囲内.
	}
	return false;	//範囲外.
}
bool clsResource::IsRangeSkinModel( enSkinModel enModel )
{
	if ( 0 <= enModel && enModel < enSK_MODEL_MAX ){
		return true;
	}
	return false;
}
