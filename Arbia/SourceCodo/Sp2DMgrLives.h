#ifndef _SP_2D_MANAGER_LIVES_H_
#define _SP_2D_MANAGER_LIVES_H_

#include "Sp2DTimerGroup.h"


class clsLives
{
public:
	clsLives();
	virtual ~clsLives();

	//Å‰‚Ì‰Šú‰».
	virtual void Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	
	//“ñ‰ñ–ÚˆÈ~‚Ì‰Šú‰».
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
	unique_ptr<clsNumGrp> m_smpLives;		//Žc‹@.

};

#endif//#ifndef _SP_2D_MANAGER_LIVES_H_