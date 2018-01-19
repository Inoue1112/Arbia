#ifndef _C_GAGESPRITE_2D_H_
#define _C_GAGESPRITE_2D_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通ｸﾗｽ.

#define ALIGN16 _declspec(align(16))

//========================================================
//	構造体.
//========================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Sprite2D.hlsl)
//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SPRITE_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX	mW;				//ﾜｰﾙﾄﾞ行列.
	ALIGN16 float		fViewPortWidth;	//ﾋﾞｭｰﾎﾟｰﾄ幅.
	ALIGN16 float		fViewPortHeight;//ﾋﾞｭｰﾎﾟｰﾄ高さ.
	ALIGN16 float		fAlpha;			//ｱﾙﾌｧ値(透過で使用).
	ALIGN16 D3DXVECTOR2	vUV;			//UV座標.
};
//ｽﾌﾟﾗｲﾄ構造体.
struct GAGE_SPRITE_STATE
{
	WHSIZE_FLOAT	Disp;	//表示幅高さ.
	WHSIZE_FLOAT	Stride;	//1ｺﾏあたりの幅高さ.
};

/*************************************************
*	Sprite2Dｸﾗｽ.
**/
class clsGageSprite2D : public clsCommon //共通ｸﾗｽを継承.
{
public:

	clsGageSprite2D();	//ｺﾝｽﾄﾗｸﾀ.
	~clsGageSprite2D();	//ﾃﾞｽﾄﾗｸﾀ.
	//初期化.
	HRESULT Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPSTR FileName, GAGE_SPRITE_STATE ss);

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾓﾃﾞﾙ作成.
	HRESULT InitModel(GAGE_SPRITE_STATE ss);
	//ﾃｸｽﾁｬ作成.
	HRESULT CreateTexture(LPSTR FileName, ID3D11ShaderResourceView** pTex);

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)
	void Render();

	void SetPos( D3DXVECTOR3 Pos )
	{
		m_vPos = Pos;
	}
	
	void SetAlpha( float Alpha )
	{
		m_fAlpha = Alpha;
	}

	//横のパターン.
	void SetPatarnU( float Patarn )
	{
		m_fUVStart.w = Patarn;
	}

	//縦のパターン.
	void SetPatarnV( float Patarn )
	{
		m_fUVStart.h = Patarn;
	}

private:

	D3DXVECTOR3		m_vPos;			//位置.

	float			m_fAlpha;		//ｱﾙﾌｧ値.

	GAGE_SPRITE_STATE	m_GSState;	//ｽﾌﾟﾗｲﾄ情報.
	WHSIZE_FLOAT		m_fUVStart;	//元画像内で範囲指定を開始する位置.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;//頂点ﾊﾞｯﾌｧ.

	ID3D11ShaderResourceView*	m_pTexture;//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

};

#endif//#ifndef _C_GAGESPRITE_2D_H_