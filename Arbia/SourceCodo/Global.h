#ifndef _GLOBAL_H_	//ｲﾝｸﾙｰﾄﾞｶﾞｰﾄﾞ.
#define _GLOBAL_H_


//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//井上KyoStar.
#define Inoue
//ロード画面.
#define START_LOADING_DISPLAY

//太原.
#define Tahara


//============================================================
//	ｲﾝｸﾙｰﾄﾞ.
//============================================================
#include	<Windows.h>

#include	<D3DX11.h>
#include	<D3D11.h>

#include	<D3DX10.h>//「D3DX～」の定義使用時に必要.
#include	<D3D10.h>

#include	"MyMacro.h"


//============================================================
//	ﾗｲﾌﾞﾗﾘ.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.



//============================================================
//	定数.
//============================================================
#define WND_W 1280	//ｳｨﾝﾄﾞｳ幅.
#define WND_H 720	//ｳｨﾝﾄﾞｳ高さ.



//方向列挙体.
enum enDirection
{
	enDirection_Stop	= 0,//停止.
	enDirection_Foward,		//前進.
	enDirection_BackWard,	//後退.
	enDirection_LeftTurn,	//左回転.
	enDirection_RightTurn	//右回転.
};



#endif//#ifndef _GLOBAL_H_