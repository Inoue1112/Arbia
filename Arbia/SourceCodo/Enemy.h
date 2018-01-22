#ifndef _C_ENEMY_H_
#define _C_ENEMY_H_

#include "CharaSkin.h"

#include "Collision.h"

const int TURN_POS_NO_MAX = 4;

class clsEnemy
	: public clsCharaSkin
{
public:
	//敵の動きの種類.
	enum enENEMY_MOVE
	{
		enEM_WALK = 0,
		enEM_RUN,
		enEM_ATK,
		enEM_STOP,
		enEM_DEATH,
		enEM_WIN,
	};//m_enMove.

	clsEnemy();
	~clsEnemy();

	void Create(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, int iNo, int jNo);
	virtual void Init(float fStartZ);

	//敵動き.
	void Move(float fEarZ);
	//死亡処理.
	void Dead(bool FlgPlayerJumpAtk = false);

	//ﾀｰﾝ.
	void SetTurnPos(float x, float z, float SecondX, float SecondZ, bool bReverse, bool bSecond);
	void Turn(int TargetNo);

	//ﾀｰﾝPosをｾｯﾄ.
	void AddTurnPos(D3DXVECTOR3 vPos){
		for (int i = 0; i < TURN_POS_NO_MAX; i++){
			m_colTurn[i].vPos += vPos;
		}
	}

	//状態取得.
	enENEMY_MOVE GetMove(){
		return m_enMove;
	}

	//発見.
	bool Discover(bool bSoundFlg = true);
	//戻る.
	void Back();
	//見失い.
	void Lost();
	//攻撃.
	void Attack();

	//勝利.
	void Win();

	COL_STATE	m_colSub;	//索敵,攻撃範囲.
	COL_STATE	m_colTurn[TURN_POS_NO_MAX];	//角.
	int			m_iTarNo;	//TargetNumber.//追いかけるPosのNo.

	//攻撃の瞬間フラグを返す.
	bool GetAttackImpact(){
		return m_bAtkImpact;
	}

	//追いかけるときに必要.
	void GetSarchTarget(D3DXVECTOR3 vPos){
		m_vTargetPos = vPos;
	}

private:

	void Release();

	//あたり判定.
	clsCollision*	m_pCollision;

	//初期化されるZ座標(これが持ち場の広さの中心点).
	float m_fStartZ;

	//敵動き子分.
	void Move_Walk();
	void Move_Run();
	void Move_Atk();
	void Move_Stop();
	void Move_Death();//死亡の動き.

	//目標との角度.
	float OpponentDirect(D3DXVECTOR3 Attker, D3DXVECTOR3 Target);//

	//見失う処理の発動を判定する.
	bool isLostRange();

	//勝利者.
	void Winner();

	void UpdateDir();

	virtual void UpdateColState();//ﾑｰﾌﾞの最後に使う.

	enENEMY_MOVE m_enMove;

	bool		m_bDead;
	int			m_iStopTimer;
	int			m_iAtkTimer;
	bool		m_bAtkImpact;//攻撃の瞬間.
	bool		m_bWin;//攻撃終了後に勝利モーションへ移行するフラグ.

	//見失った直後に追いかけないタイマー.
	int m_iReDiscTimer;//ReDiscoverTimer.

	//追いかけるときに必要.
	D3DXVECTOR3 m_vTargetPos;//ﾌﾟﾚｲﾔｰの座標を入れる.

	//ｱﾆﾒｰｼｮﾝ番号.
	enum enAnimation
	{
		enANIM_IDLE = 0,

		enANIM_WALKING,

		enANIM_RUNING,

		enANIM_ATK,

		enANIM_WIN,

		enANIM_DEAD,	//倒れる.
		enANIM_HAND_OUT,//手が落ちる.
		enANIM_LOSE,	//倒れてる（ 倒れ終わった{ 不動 } ）.

		enANIM_MAX 	//最大.
	};
	enAnimation m_enAnimNo;
	void Animation();
	//アニメーションモードを変更.
	void ChangeAnimMode(enAnimation anim);

	//音の種類.
	enum enSound
	{
		enSOUND_DCOVER = 0,//発見.//DISCOVER.
		enSOUND_CHANCE,
		enSOUND_SWING,
		enSOUND_DOWN,

		enSOUND_MAX,	//最大数.
	};
	//効果音.
	void PlaySe(enSound enSe);
	clsSound**	m_ppSe;
	float		m_fEarZ;	//耳の位置(トラップ群と違い引数にするのがやばかったので).
};

#endif//#ifndef _C_ENEMY_H_