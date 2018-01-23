#ifndef _DOOR_MANAGER_H_
#define _DOOR_MANAGER_H_

#include "CharaStatic.h"
#include "CharaSkin.h"
#include "CheckPointMgr.h"

#include "Sound.h"



class clsDoorMgr
	: public clsGameObject
{
public:
	clsDoorMgr();
	~clsDoorMgr();

	void Create( HWND hWnd, ID3D11Device* pDevice, 
		ID3D11DeviceContext* pContext, int iNo = -1 );
	void Init();
	void ReStart();

	void Release();
	void UpDateModel(){};

	//描画.
	virtual void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );

	virtual void SetPosition( D3DXVECTOR3 vPos );


	void Move( float fEarZ );

	//蹴られたとき.
	//戻り値はプレイヤーの初期化位置の更新座標.
	D3DXVECTOR3 DoorBreak();


	//あたり判定用.
	void SetColPos( D3DXVECTOR3 vPos );


	//透過フラグの設定.
	void SetAlphaFlg( float fPlayerZ );


	//門との壁あたり判定用.
	clsCharaStatic*	GetGateColPointer(){
		return m_pGate;
	}
	clsCharaStatic*	GetWallColPointer(){
		return m_pColWall;
	}
	//蹴られるあたり判定.
	COL_STATE* GetDoorHitColPointer(){
		return &ColState;
	}

	bool GetDoorBroken(){
		return m_bBreak;
	}


	float GetAlp(){
		return m_fAlpha;
	}
	bool GetAlpFlg(){
		return m_bAlpha;
	}
	bool GetAlpChange(){
		return m_bAlphaChange;
	}

private:
	//効果音.
	void PlaySe( /*enSound enSe*/ );
	clsSound*	m_pSe;
	float		m_fEarZ;

	//モデル.
	clsCharaStatic*	m_pGate;//門部分.
	clsCharaSkin*	m_pDoor;//扉部分.

	clsCharaStatic*	m_pColWall;//破壊される前の壁判定.

	clsCheckPointMgr* m_pCheck;

	//あたり判定用.
	COL_STATE	ColState;
	bool		m_bBreak;

	//透過値の設定.
	void SetAlpha();
	//透過.
	float	m_fAlpha;
	bool	m_bAlpha;//モデルのRender()に渡す引数.
	bool	m_bAlphaChange;//透過値をどちらに変えているか.


	//ｱﾆﾒｰｼｮﾝ番号.
	enum enAnimation
	{
		enANIM_IDLE = 0,//壊されるのを待っている.
		enANIM_BREAK,	//壊れる.
		enANIM_DEAD,	//もう動かない.

		enANIM_MAX 	//最大.
	};
	enAnimation m_enAnimNo;
	void Animation();
	//アニメーションモードを変更.
	void ChangeAnimMode( enAnimation anim );
	double m_dAnimTimer;
};



#endif//#ifndef _DOOR_MANAGER_H_