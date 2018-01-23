#ifndef _GOAL_MANAGER_H_
#define _GOAL_MANAGER_H_

#include "CharaStatic.h"
#include "CharaSkin.h"

#include "Sound.h"

#include "Effects.h"

class clsGoalMgr
	: public clsGameObject
{
public:
	clsGoalMgr();
	~clsGoalMgr();


	void Create( HWND hWnd, ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext );
	virtual void Init();
	void Release();
	virtual void UpDateModel(){};

	//描画.
	virtual void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );

	virtual void SetPosition( D3DXVECTOR3 vPos );


	void Move( float fEarZ );

	//箱を開ける.
	void BoxBreak();


	//あたり判定用.
	void SetColPos( D3DXVECTOR3 vPos );



	//地形あたり判定用.
	clsCharaStatic*	GetFloorColPointer(){
		return m_pFloor;
	}
	//宝箱Rayあたり判定用.
	clsCharaStatic*	GetTrBoxColPointer(){
		return m_pTrBoxCol;
	}

	//箱が蹴られるあたり判定.
	COL_STATE* GetBoxHitColPointer(){
		return &ColState;
	}

	//箱は空いているか?.
	bool GetBoxOpened(){
		return m_bOpen;
	}

	//宝箱の座標(宝石用).
	D3DXVECTOR3 GetBoxPos(){
		return m_pTreasurer->GetPosition();
	}

private:
	enum enSound
	{
		enSOUND_OPEN = 0,
		enSOUND_CHAIN,

		enSOUND_MAX,
	};
	//効果音.
	void PlaySe( enSound enSe );
	clsSound**	m_ppSe;
	int			m_iSeMax;

	//モデル.
	clsCharaStatic*	m_pFloor;//床部分.
	clsCharaSkin*	m_pTreasurer;//宝箱.
	clsCharaStatic*	m_pTrBoxCol;//宝箱に重ならない用( Ray ).


	//箱のあたり判定用.
	COL_STATE	ColState;
	bool		m_bOpen;


	//ｱﾆﾒｰｼｮﾝ番号.
	enum enAnimation
	{
		enANIM_IDLE = 0,//開けられるのを待っている.
		enANIM_OPEN,	//開く.
		enANIM_DEAD,	//もう動かない.

		enANIM_MAX 	//最大.
	};
	enAnimation m_enAnimNo;
	void Animation();
	//アニメーションモードを変更.
	void ChangeAnimMode( enAnimation anim );
	double m_dAnimTimer;



	//エフェクト.
	//名前省略用.
	clsEffects*				m_pEffect;
	//ハンドル eh = EffekseerHandle.
	::Effekseer::Handle		m_ehOpen;
	::Effekseer::Handle		m_ehLoop;


};

#endif//#ifndef _GOAL_MANAGER_H_