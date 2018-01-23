#ifndef _SPRITE_H_
#define _SPRITE_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	ｲﾝｸﾙｰﾄﾞ.
//============================================================
#include "Common.h"//共通ｸﾗｽ.


//============================================================
//	ﾗｲﾌﾞﾗﾘ.
//============================================================



//============================================================
//	定数.
//============================================================


//============================================================
//	構造体.
//============================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl).
//ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	D3DXVECTOR4	vColor;		//ｶﾗｰ(RGBAの型に合わせる) : ﾃｸｽﾁｬの上から色を載せる(赤っぽくも見せるためとか).
	D3DXVECTOR4	vUV;		//UV座標.
};

////頂点の構造体.
//struct SpriteVertex
//{
//	D3DXVECTOR3 vPos;	//頂点座標(x,y,z).
//	D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標.
//};




//============================================================
//	ｽﾌﾟﾗｲﾄｸﾗｽ.
//============================================================
class clsSprite
	: public clsCommon
{
public:

	D3DXVECTOR3		m_vPos;	//位置.

	int		m_AnimCount;//UVｽｸﾛｰﾙ.

	bool	m_bDispFlg;//表示ﾌﾗｸﾞ.


	clsSprite();	//ｺﾝｽﾄﾗｸﾀ.
	virtual ~clsSprite();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	////ﾓﾃﾞﾙ作成.
	virtual HRESULT InitModel();

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
				 D3DXVECTOR3 &vEye );

	void SetScale( float fScale ){
		m_fScale = fScale;
	}

protected:

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11ShaderResourceView*	m_pTexture;		//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗｰ:/ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.



	float m_fScale;


};


#endif	//#ifndef _SPRITE_H_
