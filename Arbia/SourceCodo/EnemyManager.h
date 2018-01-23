#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "Enemy.h"
//壁も持つ.




class clsEnemyMgr
	: public clsGameObject
{
public:

	enum enENEMY_MGR_MODE
	{
		enEMM_1X = 1,	//1人でX軸移動.
		enEMM_1Z,
		enEMM_1SQ_L,	//一人で四角移動左回転.
		enEMM_1SQ_R,	

		enEMM_2X,		//二人がX軸で並ぶ.
		enEMM_2X_RE,	//二人がX軸で互い違い.
		enEMM_2Z,
		enEMM_2Z_RE,
		enEMM_2SQ_L,	//二人で四角移動左回転.
		enEMM_2SQ_R,

		enEMM_DOOR,		//最後に登場するドアで死ぬ.

		enEMM_MAX,
	};//m_enMode.

	clsEnemyMgr();
	~clsEnemyMgr();

	void CreateEnemy( HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int mode, int iMgrID );
	virtual void Init(){};//引数が必要になったのでいらない.
	void Init( bool bFirst );
	void UpDateModel();


	virtual void SetPosition( D3DXVECTOR3 vPos );
	COL_STATE* GetPointerEnemyCol( int i );
	COL_STATE* GetPointerEnemySubCol( int i );




	void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				 D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );

	void Move( float fEarZ );



	int GetEnemyMax(){
		return m_iEnemyMax;
	}

	//状態取得.
	clsEnemy::enENEMY_MOVE GetEnemyMove( int j ){
		return m_ppEnemy[j]->GetMove();
	}


	//見失いで使う.
	D3DXVECTOR3 GetEnemyPos( int j ){
		return m_ppEnemy[j]->GetPosition();
	}
	//索敵半径を持ち出す.
	float GetSarchRange( int j ){
		return m_ppEnemy[j]->m_colSub.fRange;
	}

	//ﾗｯﾌﾟ.
	//発見.
	bool Discover( int j ){
		return m_ppEnemy[j]->Discover();
	}
	//見失い.
	void Lost( int j ){
		m_ppEnemy[j]->Lost();
	}
	//攻撃.
	void Attack( int j ){
		m_ppEnemy[j]->Attack();
	}
	//死亡.
	void Dead( int j ){
		m_ppEnemy[j]->Dead();
	}

	//勝利.
	void Win( int j ){
		m_ppEnemy[j]->Win();
	}

	//ドア死亡開始.
	void DoorTimerStart(){
		m_bDoor = true;
	}
	//ドアで死ぬ人?.
	bool GetDoorDeader(){
		if( m_enMode == enEMM_DOOR ){
			return true;
		}
		return false;
	}


	//clsEnemy::追いかけるときに必要のﾗｯﾌﾟ.
	void GetSarchTarget( D3DXVECTOR3 vPos ){
		for( int i=0; i<m_iEnemyMax; i++ ){
			m_ppEnemy[i]->GetSarchTarget( vPos );
		}
	}



	//攻撃の瞬間フラグを返す.
	bool GetAttackImpact( int j ){
		return m_ppEnemy[j]->GetAttackImpact();
	};


	//井上君へ.
	int GetWalkRangeMax();


private:
	clsEnemy**	m_ppEnemy;

	void SetTurnPos( enENEMY_MGR_MODE mode, int i );//移動の角ｾｯﾄ.


	void Release();

	int			m_iEnemyMax;//敵の数.
//	clsEnemy**	m_ppEnemy;


	enENEMY_MGR_MODE m_enMode;

	D3DXVECTOR3 m_vTurnPos[4];	//角.



	//ドア.
	bool	m_bDoor;
	int		m_iDoorTimer;

};


#endif//#ifndef _ENEMY_MANAGER_H_