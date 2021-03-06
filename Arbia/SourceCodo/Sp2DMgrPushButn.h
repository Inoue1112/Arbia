#ifndef _SP_2D_MANAGER_PUSH_BUTTON_H_
#define _SP_2D_MANAGER_PUSH_BUTTON_H_

#include "Sprite2D.h"

class clsPushTxt
{
public:

	enum enAlphaMode
	{
		Flassing = 0,
		Out
	};

	clsPushTxt();
	~clsPushTxt();

	void Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	void InitSetPos();
	void Render();
	void Release();

	void AddAlpha(float AlphaPoint);
	void SetAlphaMode(enAlphaMode AlphaMode){
		m_enAlphaMode = AlphaMode;
	}

private:
	enAlphaMode m_enAlphaMode;
	float m_fAlphaPoint;//透過する際の変化量.

	unique_ptr<clsSprite2D> m_smpAButton;	//Aボタンねぇでやんす.
	unique_ptr<clsSprite2D> m_smpPushTxt;	//PUSH KETねぇでやんす.
};

#endif//#ifndef _SP_2D_MANAGER_PUSH_BUTTON_H_