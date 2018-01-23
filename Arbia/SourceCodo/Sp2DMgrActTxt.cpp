#include "Sp2DMgrActTxt.h"
//Jacky is here now!
//Jacky Was HERE!!!!!!!
clsActTxt::clsActTxt()
{
}

clsActTxt::~clsActTxt()
{
}

void clsActTxt::Move(float PlusSize)
{
	switch (m_enState)
	{
		case clsActTxt::In:
			if (m_vPos.x < 0.0f)
			{
				m_vPos.x += 10.0f;
			}
			else{
				m_enState = Idle;
			}
			break;
		case clsActTxt::Out:
			if (m_vPos.x > -(m_SState.Disp.w + PlusSize))
			{
				m_vPos.x -= 10.0f;
			}
			else{
				m_enState = Idle;
			}
			break;
		default:
			break;
	}

}
void clsActTxt::InStart()
{
	if (m_enState == Idle)
		m_enState = In;
}
void clsActTxt::OutStart()
{
	if (m_enState == Idle)
		m_enState = Out;
}
