#ifndef _SP2D_TIMER_GROUP_H_
#define _SP2D_TIMER_GROUP_H_

#include "Sp2DTimer.h"
#include "Mymacro.h"
#include "Common.h"//共通ｸﾗｽ.
#include <memory>

#include "Sound.h"

class clsTimerGrp
{
public:
	clsTimerGrp();
	~clsTimerGrp();

	//最初の初期化.
	void Create( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	
	//二回目以降の初期化.
	virtual void ReSet();


	void Move();

	void Render();


	void SetPos( D3DXVECTOR3 vPos );
	void AddPos( D3DXVECTOR3 vPos );
	void AddPosY( float fPos );

	void SetStop( bool bStop ){
		m_bStop = bStop;
	}

	//4桁の整数で返す( 上二ケタは分, 下二桁は秒{ コンマ秒は無視 } ).
	int GetTime();

private:

	D3DXVECTOR3 m_vPos;

	clsTimer*		m_Timer[3];
	clsSprite2D*	m_Colon[2];//:( コロン ).

	bool m_bRimitFlg;//trueになるとカウントしない.

	bool m_bStop;//trueでカウントしない.
	bool m_bStopOld;




	clsSound* m_pSe;

};

#endif//#ifndef _SP2D_TIMER_GROUP_H_