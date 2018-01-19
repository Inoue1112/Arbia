#ifndef _STEP_CYLINDER_H_
#define _STEP_CYLINDER_H_

#include "CharaStatic.h"


class clsStepCyl
	: public clsCharaStatic
{
public:
	clsStepCyl();
	~clsStepCyl();

	//動きの確定.
	void CreateMove( bool bNorth = true );

	virtual void Init();

	//動作.
	void Move();

	//キャラが乗っている時に動かす量を返す.
	D3DXVECTOR3 GetCarryRange(){
		return D3DXVECTOR3( 0.0f, 0.0f, m_fMovePower );
	}
	float GetCarryRangeZ(){
		return m_fMovePower;
	}

private:

	float	m_fMovePower;//移動量.
	bool	m_bNorthFlg;//true : 北向き.
	bool	m_bInitFlg;	//どう初期化するかのフラグ.

	D3DXVECTOR3 m_vStartPos;
	float		m_fGoalPosZ;

};


#endif//#ifndef _STEP_CYLINDER_H_