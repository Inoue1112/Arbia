#ifndef _PARTICLE_H_
#define _PARTICLE_H_

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
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(PointSprite.hlsl).
//ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct PSPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
//	D3DXVECTOR4	vColor;	//ｶﾗｰ(RGBAの型に合わせる) : ﾃｸｽﾁｬの上から色を載せる(赤っぽくも見せるためとか).
//	D3DXVECTOR4	vUV;	//UV座標.
};

//頂点の構造体.
struct PSpriteVertex
{
	D3DXVECTOR3 vPos;	//頂点座標(x,y,z).
//	D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標.
};


//ﾊﾟｰﾃｨｸﾙ用構造体.
struct PART
{
	D3DXVECTOR3 vPos;	//位置.
	D3DXVECTOR3 vDir;	//方向.
	float		fSpeed;	//速度.
	int			iBirthFrame;//開始ﾌﾚｰﾑ.

	//構造体の初期化(ｺﾝｽﾄﾗｸﾀ).
	PART(){
		ZeroMemory( this, sizeof( PART ) );
	}
};



//============================================================
//	ﾊﾟｰﾃｨｸﾙｸﾗｽ.
//============================================================
class clsParticle
	: public clsCommon	//共通ｸﾗｽ継承
{

public:


	clsParticle(){
		ZeroMemory( this, sizeof( clsParticle ) );//ｺﾝｽﾄﾗｸﾀ.
	};	
	//ｵｰﾊﾞｰﾛｰﾄﾞ関数.
	clsParticle( int MaxParticle, D3DXVECTOR3 &vEmitPos );//ｺﾝｽﾄﾗｸﾀ.
	~clsParticle();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	////ﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄ作成.
	HRESULT InitPointSprite();
	//ﾃｸｽﾁｬ読込&作成.
	HRESULT InitTexture( char *pFileName );



	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
	void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vEye, D3DXVECTOR3 &vPos );


	//ｼｪｰﾀﾞ無効化(ﾃﾞﾀｯﾁ).
	void DetachShader();

	//ﾊﾟｰﾃｨｸﾙの移動.
	void Run();



	D3DXVECTOR3		m_vPos;	//位置.

	int		m_AnimCount;	//UVｽｸﾛｰﾙ.

	bool	m_bDispFlg;		//表示ﾌﾗｸﾞ.

	D3DXVECTOR3		m_vEmitPos;		//放射位置.
	PART*			m_pParticleArray;//ﾊﾟｰﾃｨｸﾙ配列.
	int				m_Frame;		//ﾌﾚｰﾑ数.




	//ﾊﾟｰﾃｨｸﾙ位置の取得.
	D3DXVECTOR3& GetParticlePos( int index ){
		//配列の範囲かどうかﾁｪｯｸ.
		if( 0 <= index && index < m_iMaxParticle ){
			return m_pParticleArray[index].vPos;
		}
		//範囲外の時は先頭を返す.
		return m_pParticleArray[index].vPos;
	}
	//ﾊﾟｰﾃｨｸﾙ最大数の取得.
	int GetMaxParticle(){ return m_iMaxParticle; }

private:
	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.

	ID3D11GeometryShader*	m_pGeometryShader;	//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.

	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11ShaderResourceView*	m_pTexture;		//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗｰ:/ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.

	int				m_iMaxParticle;	//ﾊﾟｰﾃｨｸﾙ最大数.



};


#endif	//#ifndef __PARTICLE_H__H_
