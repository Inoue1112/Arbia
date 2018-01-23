#ifndef _SP_2D_MANAGER_DISCOVER_H_
#define _SP_2D_MANAGER_DISCOVER_H_

#include "Sp2DMgrLives.h"

class clsDiscover
	: public clsLives
{
public:
	clsDiscover();
	~clsDiscover();

	//最初の初期化.
	virtual void Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	//二回目以降の初期化.
	virtual void ReSet();


private:

};

#endif//#ifndef _SP_2D_MANAGER_DISCOVER_H_