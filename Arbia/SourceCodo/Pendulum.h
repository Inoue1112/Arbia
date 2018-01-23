#ifndef _C_PENDULUM_H_
#define _C_PENDULUM_H_


#include "CharaSkin.h"
#include "Sound.h"

#include "Effects.h"

//ﾒｲﾝで使おう.
const char PEND_BONE_NAME[] = "PendHitCenter"; 




class clsPend
	: public clsCharaSkin
{
public:
	clsPend();
	virtual ~clsPend();


	void Create( HWND hWnd, int iNo, int iPower );

	virtual void Init();

	//ﾚﾝﾀﾞﾘﾝｸﾞ.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye );


	//動作(ゆらゆら).
	void Move( float fEarZ );


	//エフェクト再生.
	void PlayEff();


	//透過フラグの設定.
	void SetAlphaFlg( float fPlayerZ );


	
	//ボーンpos.
	void GetBonePos( 
		const char* cBoneName,
		D3DXVECTOR3 vBonePos = D3DXVECTOR3( -100.0f, -100.0f, -100.0f ) );


private:

	//効果音.
	void PlaySe( float fEarZ );
	clsSound*	m_pSe;


	float m_fMovePower;
	bool m_bRigtFlg;

	int		m_iInitFlg;//初期化フラグ.


	//透過値の設定.
	void SetAlpha();
	//透過.
	float	m_fAlpha;
	bool	m_bAlpha;//モデルのRender()に渡す引数.
	bool	m_bAlphaChange;//透過値をどちらに変えているか.


	//エフェクト.
	//名前省略用.
	clsEffects*				m_pEffect;
	//ハンドル eh = EffekseerHandle.
	::Effekseer::Handle		m_ehFire;




};



#endif//#ifndef _C_PENDULUM_H_