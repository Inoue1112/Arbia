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
	//½·İÒ¯¼­‚ÌÎß²İÀ—Ìˆæ‚ğ‰ğ•ú.
	if( m_ppSkinModels ){
		delete[] m_ppSkinModels;
		m_ppSkinModels = NULL;
	}
	//½ÀÃ¨¯¸Ò¯¼­‚ÌÎß²İÀ—Ìˆæ‚ğ‰ğ•ú.
	if( m_ppStaticModels ){
		delete[] m_ppStaticModels;
		m_ppStaticModels = NULL;
	}


	m_pCotext11 = NULL;
	m_pDevice11 = NULL;
	m_hWnd = NULL;
}


//==================================================.
//	‰Šú‰».
//==================================================.
HRESULT clsResource::InitStaticModel( HWND hWnd,
			ID3D11Device* pDevice,
			ID3D11DeviceContext* pContext)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice;
	m_pCotext11 = pContext;
	//½ÀÃ¨¯¸Ò¯¼­‚ÌÎß²İÀ—Ìˆæ‚ğŠm•Û.
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
	//½·İÒ¯¼­‚ÌÎß²İÀ—Ìˆæ‚ğŠm•Û.
	m_ppSkinModels = new clsD3DXSKINMESH*[ enSK_MODEL_MAX ];
	for( int i=0; i<enSK_MODEL_MAX; i++ ){
		m_ppSkinModels[i] = NULL;
	}

	return S_OK;
}



//==================================================.
//	ÓÃŞÙ“Ç.
//==================================================.
// Static Model.
HRESULT clsResource::CreateStaticModel(
	LPSTR fileName, enStaticModel enModel )
{
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
	if( m_ppStaticModels[ enModel ] != NULL ){
		return E_FAIL;
	}

	//ÓÃŞÙ“Ç.
	m_ppStaticModels[ enModel ] = new clsDX9Mesh;
	m_ppStaticModels[ enModel ]->Init(
		m_hWnd, m_pDevice11, m_pCotext11, fileName );

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::CreateSkinModel( 
	LPSTR fileName, enSkinModel enModel )
{
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à‚©.
	if ( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}

	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
	if ( m_ppSkinModels[ enModel ] != NULL ){
		return E_FAIL;
	}

	//ÓÃŞÙ“Ç.
	m_ppSkinModels[ enModel ] = new clsD3DXSKINMESH;
	m_ppSkinModels[ enModel ]->Init( &m_Si );
	m_ppSkinModels[ enModel ]->CreateFromX( fileName );

	return S_OK;
}

//==================================================.
//	‰ğ•ú.
//==================================================.
//	Static Model.
HRESULT clsResource::ReleaseStaticModel( enStaticModel enModel )
{
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
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
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à?.
	if( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}
	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
	if( m_ppSkinModels[ enModel ] == NULL ){
		return E_FAIL;
	}

	delete m_ppSkinModels[ enModel ];
	m_ppSkinModels[ enModel ] = NULL;

	return S_OK;
}

//==================================================.
//	ÓÃŞÙ‚ÌÎß²İÀæ“¾.
//==================================================.
//	Static Model.
clsDX9Mesh* clsResource::GetStaticModels(
	enStaticModel enModel )
{
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à?.
	if( !IsRangeStaticModel( enModel ) ){
		return NULL;
	}
	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
	if( m_ppStaticModels[ enModel ] == NULL ){
		return NULL;
	}
	
	//ÓÃŞÙ‚ÌÎß²İÀ•Ô‚·.
	return m_ppStaticModels[ enModel ];
	//return (clsDX9Mesh*)S_OK;	//‘å–ŒÌ.
}
//	Skin Model.
clsD3DXSKINMESH* clsResource::GetSkinModels(enSkinModel enModel)
{
	//—ñ‹“‘Ì‚Ì”ÍˆÍ“à?.
	if ( !IsRangeSkinModel( enModel ) ){
		return NULL;
	}

	//Šù‚ÉÃŞ°À‚ª‚ ‚é‚©.
	if ( m_ppSkinModels[ enModel ] == NULL ){
		return NULL;
	}

	//ÓÃŞÙ‚ÌÎß²İÀ•Ô‚·.
	return m_ppSkinModels[ enModel ];
}


//==================================================.
//	”ÍˆÍ“à‚©Áª¯¸‚·‚éŠÖ”.
//==================================================.
bool clsResource::IsRangeStaticModel( enStaticModel enModel )
{
	if( 0 <= enModel && enModel < enST_MODEL_MAX ){
		return true;//”ÍˆÍ“à.
	}
	return false;	//”ÍˆÍŠO.
}
bool clsResource::IsRangeSkinModel( enSkinModel enModel )
{
	if ( 0 <= enModel && enModel < enSK_MODEL_MAX ){
		return true;
	}
	return false;
}
