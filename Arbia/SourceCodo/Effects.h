#ifndef _CEFFECTS_H_
#define _CEFFECTS_H_

//警告についてのｺｰﾄﾞ分析を無効にする。4005:再定義.
#pragma warning( disable : 4005 )

//==================================================
//	先にしておくべきﾍｯﾀﾞ、ﾗｲﾌﾞﾗﾘの読込.
//==================================================
#include <stdio.h>

#include <D3D11.h>
#include <XAudio2.h>
#pragma comment( lib, "d3d11.lib" )

//==================================================
//	ここからEffekseer関係 ﾍｯﾀﾞ、ﾗｲﾌﾞﾗﾘの読込.
//==================================================
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

#if _DEBUG
#pragma comment( lib, "VS2013\\Debug\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerSoundXAudio2.lib" )
#else//#if _DEBUG
#pragma comment( lib, "VS2013\\Release\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerSoundXAudio2.lib" )
#endif//#if _DEBUG


//「D3DX～」使用で必須.
//注意:Effekseer関係より後に読み込むこと.
#include <D3DX10.h>
#pragma comment( lib, "d3dx10.lib" )


//==================================================
//	フリーソフトEffekseerのﾃﾞｰﾀを使うためのｸﾗｽ.
//==================================================
class clsEffects
{
public:
	//ｴﾌｪｸﾄ種類列挙体.
	enum enEfcType
	{
		enEfcType_Mari = 0,
		enEfcType_Miko,
		enEfcType_MgcDrk,
		enEfcType_Max,
	};


	//ｲﾝｽﾀﾝｽ取得(唯一のｱｸｾｽ経路).
	static clsEffects* GetInstance()
	{
		//唯一のｲﾝｽﾀﾝｽを作成する.
		//(staticで作成されたので2回目は無視される).
		static clsEffects s_Instance;
		return &s_Instance;
	}

	~clsEffects();

	//構築関数.
	HRESULT Create( ID3D11Device* pDevice,
					ID3D11DeviceContext* pContext );
	//破棄関数.
	HRESULT Destroy();
	//ﾃﾞｰﾀ読込関数.
	HRESULT LoadData();
	//ﾃﾞｰﾀ解放関数.
	HRESULT ReleaseData();

	//描画.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vEye );

	//ﾋﾞｭｰ行列設定.
	void SetViewMatrix( D3DXMATRIX& mView );
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列設定.
	void SetProjectionMatrix( D3DXMATRIX& mProj );


	//再生関数.
	::Effekseer::Handle Play(  enEfcType EfcType, D3DXVECTOR3 vPos ){
		return m_pManager->Play(
			m_pEffect[EfcType], vPos.x, vPos.y, vPos.z );
	};
	//一時停止.
	void Paused( ::Effekseer::Handle handle, bool bFlag ){
		m_pManager->SetPaused( handle, bFlag );	//bFlag:true = 一時停止.
	}
	//停止.
	void Stop( ::Effekseer::Handle handle ){
		m_pManager->StopEffect( handle );
	}
	//すべて停止.
	void StopAll( ::Effekseer::Handle handle ){
		m_pManager->StopAllEffects();
	}
	//ｻｲｽﾞ指定.
	void SetScale( ::Effekseer::Handle handle, D3DXVECTOR3 vScale ){
		m_pManager->SetScale( handle,
			vScale.x, vScale.y, vScale.z );
	}
	//回転指定.
	void SetRotation( ::Effekseer::Handle handle, D3DXVECTOR3 vRot ){
		m_pManager->SetRotation( handle,
			vRot.x, vRot.y, vRot.z );
	}
	void SetRotation( ::Effekseer::Handle handle,
			D3DXVECTOR3 vAxis, float fAngle ){
		m_pManager->SetRotation( handle,
			::Effekseer::Vector3D( vAxis.x, vAxis.y, vAxis.z ),
			fAngle );
	}
	//位置指定.
	void SetLocation( ::Effekseer::Handle handle, D3DXVECTOR3 vPos ){
		m_pManager->SetLocation( handle,
			::Effekseer::Vector3D( vPos.x, vPos.y, vPos.z ) );
	}

	//動いてる?.
	bool PlayCheck( ::Effekseer::Handle handle )
	{
		return m_pManager->Exists( handle );
	}



	//ﾍﾞｸﾀｰ変換関数.
	::Effekseer::Vector3D Vector3DDxToEfk( 
					const D3DXVECTOR3* pSrcVec3Dx );
	D3DXVECTOR3 Vector3EfkToDx( 
					const ::Effekseer::Vector3D* pSrcVec3Efk );

	//行列変換関数.
	::Effekseer::Matrix44 MatrixDxToEfk(
					const D3DXMATRIX* pSrcMatDx );
	D3DXMATRIX MatrixEfkToDx(
					const ::Effekseer::Matrix44* pSrcMatEfk );


private:
	//生成やｺﾋﾟｰを禁止する.
	clsEffects();
	clsEffects( const clsEffects& rhs );
	clsEffects& operator = ( const clsEffects& rhs );

	//ｴﾌｪｸﾄを作動させるために必要.
	::Effekseer::Manager*			m_pManager;
	::EffekseerRenderer::Renderer*	m_pRender;
	::EffekseerSound::Sound*		m_pSound;
	//ｴﾌｪｸﾄﾃﾞｰﾀに含まれる音再生に必要.
	IXAudio2*						m_pXA2;
	IXAudio2MasteringVoice*			m_pXA2Master;


	//ｴﾌｪｸﾄの種類ごとに必要.
	::Effekseer::Effect*			m_pEffect[ enEfcType_Max ];


};

//=======================================================
//	使い方.
//=======================================================

//.	//MainのCreateにこれ
//	clsEffects::GetInstance()->Create( m_pDevice, m_pDeviceContext );
//	clsEffects::GetInstance()->LoadData();




//	//エフェクトを出したいクラスのヘッダーに.
//	//これと同じ型の変数を宣言する.
//	::Effekseer::Handle		m_Handle;


//	//----- hでの使用例 -----//.
//	//名前省略用.
//	clsEffects*				m_pEffect;
//	//ハンドル eh = EffekseerHandle.
//	::Effekseer::Handle		m_ehBomb;


//	//----- cppでの使用例 -----//.
//	//コンストラクタかCreateにて.
//	m_pEffect = nulptr;.
//	if( m_pEffect == nulptr ){
//		m_pEffect = clsEffects::GetInstance();
//	}

//	//出したいタイミング.
//	if( !m_pEffect->PlayCheck( m_ehBomb ) ){
//		m_ehBomb = m_pEffect->Play( clsEffects::enEfcType_MgcDrk, m_vPos );
//	}
//	m_pEffect->SetLocation( m_ehBomb, m_vPos );

//	//Release()関数にて.
//	 m_pEffect = nulptr;.
//	※各クラスでこれをdeleteはしてはならない.


//旧解説.
	//ｴﾌｪｸﾄ毎に必要(同じｴﾌｪｸﾄを3つ出すなら3つ必要).


#endif//#ifndef _CEFFECTS_H_