#ifndef _SP_SHADOW_H_
#define _SP_SHADOW_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	ｲﾝｸﾙｰﾄﾞ.
//============================================================
#include "Sprite.h"

//============================================================
//	ﾗｲﾌﾞﾗﾘ.
//============================================================



//============================================================
//	定数.
//===========================================================

const float fDEFAULT_SHADOW_SCALE = 0.825f;



//============================================================
//	構造体.
//============================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl).
//ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SHADOW_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	D3DXVECTOR4	vColor;		//ｶﾗｰ(RGBAの型に合わせる) : ﾃｸｽﾁｬの上から色を載せる(赤っぽくも見せるためとか).
	D3DXVECTOR4	vUV;		//UV座標.
};




//============================================================
//	影ｸﾗｽ.
//============================================================
class clsShadow
	: public clsSprite
{

public:

	float			m_fYaw;		//回転(Y軸).
	float			m_fPitch;	//回転(X軸).
	float			m_fRoll;	//回転(Z軸).



	clsShadow();	//ｺﾝｽﾄﾗｸﾀ.
	~clsShadow();	//ﾃﾞｽﾄﾗｸﾀ.

	////ﾓﾃﾞﾙ作成.
	virtual HRESULT InitModel();

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
				 D3DXVECTOR3 &vEye );



	//影セット.
	void SetShadow( D3DXVECTOR3 vPos, float fFloor, float fScaleMax = fDEFAULT_SHADOW_SCALE );


private:



	//座標セット.
	void SetPos( D3DXVECTOR3 vPos, float fFloor );//Y座標は床の高さで更新.
	//高さで影のサイズ変化.
	void ShadScale( float fPosY, float fFloor, float fScaleMax );


};


#endif	//#ifndef _SP_SHADOW_H_
