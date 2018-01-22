#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"


#include "Global.h"


#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )



//============================================================
//	列挙体.
//============================================================
////方向列挙体.
//enum enDirection
//{
//	enDirection_Stop	= 0,//停止.
//	enDirection_Foward,		//前進.
//	enDirection_BackWard,	//後退.
//	enDirection_LeftTurn,	//左回転.
//	enDirection_RightTurn	//右回転.
//};


////自機動きの種類.
//enum enPlayerMove
//{
//	enPM_Stop	= 0,//停止.
//	enPM_Run,		//走り.
//	enPM_Atk,		//攻撃.
//	enPM_JumAt,		//ｼﾞｬﾝﾌﾟ攻撃.
//	enPM_Dead		//ﾐｽ.
//};//m_pEnemy_A->m_enPM
//


////敵の動きの種類.
//enum enEnemyMove
//{
//	enEM_Walk = 0,
//	enEM_Run,
//	enEM_Atk,
//	enEM_Back,
//	enEM_Damage,
//	enEM_Death
//};//	enEnemyMove m_enEnemMod;//enEnemyMove.
//







//============================================================
//	構造体.
//============================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Mesh.hlsl).
//ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
//hlslと連動している(疑ったほうがいいｼﾘｰｽﾞ).
struct MESHSHADER_CONSTANT_BUFFER_ZERO
{
	D3DXMATRIX	mW;			//ﾜｰﾙﾄﾞ(位置)座標行列.
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	D3DXVECTOR4 vLightDir;	//ﾗｲﾄ方向.
	D3DXVECTOR4	vEye;		//ｶﾒﾗ位置(視点位置).
	D3DXVECTOR4 vColor;		//ｶﾗｰ.
};
struct MESHSHADER_CONSTANT_BUFFER_FIRST
{
	D3DXVECTOR4 vAmbient;	//ｱﾝﾋﾞｴﾝﾄ.
	D3DXVECTOR4 vDiffuse;	//ﾃﾞｨﾌｭｰｽﾞ色.
	D3DXVECTOR4	vSpecular;	//ｽﾍﾟｷｭﾗ色.
};


//頂点の構造体.
struct MeshVertex
{
	D3DXVECTOR3 vPos;	//頂点座標(x,y,z).
	D3DXVECTOR3 vNormal;//法線(陰影計算に必須).
	D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標.
};

//ﾏﾃﾘｱﾙ構造体.
struct MY_MATERIAL
{
	D3DXVECTOR4		Ambient;	//ｱﾝﾋﾞｴﾝﾄ.
	D3DXVECTOR4		Diffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
	D3DXVECTOR4		Specular;	//ｽﾍﾟｷｭﾗ.
	CHAR	szTextureName[128];	//ﾃｸｽﾁｬﾌｧｲﾙ名.
	ID3D11ShaderResourceView*	pTexture;//ﾃｸｽﾁｬ.
	DWORD	dwNumFace;			//そのﾏﾃﾘｱﾙのﾎﾟﾘｺﾞﾝ数.
	//ｺﾝｽﾄﾗｸﾀ.
	MY_MATERIAL(){
		ZeroMemory( this, sizeof( MY_MATERIAL ) );
	}
	//ﾃﾞｽﾄﾗｸﾀ.
	~MY_MATERIAL(){}
};


//ｽﾌｨｱ構造体.
struct SPHERE
{
	D3DXVECTOR3	vCenter;	//中心.
	float		fRadius;	//半径.

};

//Bﾎﾞｯｸｽ構造体.
struct BBOX
{
	D3DXVECTOR3 vPosMax;	//最大頂点.
	D3DXVECTOR3 vPosMin;	//最小頂点.
	D3DXVECTOR3 vAxisX;		//X軸.
	D3DXVECTOR3 vAxisY;		//Y軸.
	D3DXVECTOR3 vAxisZ;		//Z軸.
	FLOAT		fLengthX;	//X長さ.
	FLOAT		fLengthY;	//Y長さ.
	FLOAT		fLengthZ;	//Z長さ.
	//初期化.
	BBOX(){
		ZeroMemory( this, sizeof( BBOX ) );
		vAxisX = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		vAxisY = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		vAxisZ = D3DXVECTOR3( 0.0f, 0.0f, 1.0f ); 
	}
};







//ﾒｯｼｭﾃﾞｰﾀをﾌｧｲﾙから取り出すためだけにDirectX9を使用する.
//※ﾚﾝﾀﾞﾘﾝｸﾞ(表示)はDirectX11で行う.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//DX9ｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	//D3DMATERIAL9*		m_pMaterials;	//ﾏﾃﾘｱﾙ情報.
	//LPDIRECT3DTEXTURE9*	m_pTextures;	//ﾃｸｽﾁｬ情報.
	//char				m_TexFileName[8][256];	//ﾃｸｽﾁｬﾌｧｲﾙ名(8枚まで).
	DWORD				m_dwNumMaterials;//ﾏﾃﾘｱﾙ数.
	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.

	//Dx11.
	ID3D11Device*			m_pDevice11;		//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer0;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ0.
	ID3D11Buffer*			m_pConstantBuffer1;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ1.
//
//
	ID3D11Buffer*	m_pVertexBuffer;//頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧ.
	ID3D11Buffer**	m_ppIndexBuffer;	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗｰ.//ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
//
//
	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_bTexture;		//ﾃｸｽﾁｬの有無.


	float			m_fScale;	//拡縮.
	float			m_fYaw;		//回転(Y軸).
	float			m_fPitch;	//回転(X軸).
	float			m_fRoll;	//回転(Z軸).
	D3DXVECTOR3		m_vPos;		//位置(X,Y,Z).
	float			m_fYawTarget;//目標角度.



float			m_fSpd;






	SPHERE		m_Sphere;	//ｽﾌｨｱ構造体.
	BBOX		m_BBox;		//BBox構造体.

	D3DXVECTOR3	m_vRay;			//ﾚｲの位置.
	D3DXVECTOR3	m_vAxis;		//回転軸.
	LPD3DXMESH	m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.





	ID3D11BlendState*	m_pBlendState;	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.


	clsDX9Mesh();	//ｺﾝｽﾄﾗｸﾀ.
	~clsDX9Mesh();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName );

	//Dx9初期化用.
	HRESULT InitDx9( HWND hWnd );

	//ﾒｯｼｭ読込.
	HRESULT LoadXMesh( LPSTR fileName );

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();

	//ﾚﾝﾀﾞﾘﾝｸﾞ用(※DX9MESH内とMain内で2つ存在するので注意).
	void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
				D3DXVECTOR4 vColor = D3DXVECTOR4( 1.0f,1.0f,1.0f,1.0f ),
				bool alphaFlg = false );

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend( bool flg );



	//解放.
	HRESULT Release();


};

#endif//#ifndef _DX9MESH_H_
