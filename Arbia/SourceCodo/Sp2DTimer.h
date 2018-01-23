#ifndef _SP_2D_TIMER_H_
#define _SP_2D_TIMER_H_

#include "NumGroup.h"


class clsTimer
	: public clsNumGrp
{
public:

	//一ケタ目の速度 二ケタ目の速度 タイマーか否か.
	virtual void Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	virtual void ReSet();

	//数を増やす.
	virtual void PlusNum();

	void AddPosX( float fMove );

	//桁上がり( 繰り上がり )確認.
	bool Carrying();

	//上限設定用.
	void Rimit();
private:

	bool m_bCarrying;//桁上がりフラグ.

};




#endif//#ifndef _SP_2D_TIMER_H_