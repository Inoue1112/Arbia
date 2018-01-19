#ifndef _SP_2D_MANAGER_ACT_TXT_H_
#define _SP_2D_MANAGER_ACT_TXT_H_

#include"Sprite2D.h"

class clsActTxt
	: public clsSprite2D
{
public:
	clsActTxt();
	~clsActTxt();

	enum en_State
	{
		Idle = 0,
		In,
		Out,
	};
	void Move( float PlusSize = 0.0f );
	void InStart();
	void OutStart();
	en_State GetMoveState(){
		return m_enState;
	}
private:

	en_State m_enState;

};
#endif//#ifndef _SP_2D_MANAGER_ACT_TXT_H_