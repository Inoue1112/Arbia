#ifndef _SP_2D_MANAGER_LIVES_H_
#define _SP_2D_MANAGER_LIVES_H_

#include "Sp2DTimerGroup.h"


class clsLives
{
public:
	clsLives();
	virtual ~clsLives();

	//最初の初期化.
	virtual void Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	
	//二回目以降の初期化.
	virtual void ReSet();


	void Update();

	void Render();

	void  Release();

	void SetPos( D3DXVECTOR3 vPos );
	void AddPos( D3DXVECTOR3 vPos );
	void AddPosY( float fPos );

	void SetNum( int iNum  ){
		m_smpLives->SetNum( iNum );
	}


protected:

	D3DXVECTOR3 m_vPos;

	unique_ptr<clsSprite2D> m_smpIcon;
	unique_ptr<clsNumGrp> m_smpLives;		//残機.

};

#endif//#ifndef _SP_2D_MANAGER_LIVES_H_