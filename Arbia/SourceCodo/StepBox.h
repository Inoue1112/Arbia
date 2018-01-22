#ifndef _STEP_BOX_H_
#define _STEP_BOX_H_

#include "CharaStatic.h"

class clsStepBox
	: public clsCharaStatic
{
public:
	clsStepBox();
	~clsStepBox();

	//動きの確定.
	void CreateMove( bool bRight, bool bEast );

	virtual void Init();

	//動作.
	void Move();

	//キャラが乗っている時に動かす量を返す.
	D3DXVECTOR3 GetCarryRange(){
		return D3DXVECTOR3( m_fMovePower, 0.0f, 0.0f );
	}
	float GetCarryRangeX(){
		return m_fMovePower;
	}


private:

	float	m_fMovePower;//移動量.
	bool	m_bRightFlg;//true : 右側.
	bool	m_bEastFlg;//true : 東向き.
	bool	m_bInitFlg;	//どう初期化するかのフラグ.

	D3DXVECTOR3 m_vStartPos;

};

#endif//#ifndef _STEP_BOX_H_