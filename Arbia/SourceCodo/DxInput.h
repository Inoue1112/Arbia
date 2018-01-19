#ifndef _DX_INPUT_H_
#define _DX_INPUT_H_

//========================================================================================
//
//	※[dinput.h]を読込む際にしなければいけないこと。.
//
//		[ﾂｰﾙ]→[ｵﾌﾟｼｮﾝ]→[ﾌﾟﾛｼﾞｪｸﾄおよびｿﾘｭｰｼｮﾝ]→.
//			[全般]→[VC++ ﾃﾞｨﾚｸﾄﾘ]→[ﾃﾞｨﾚｸﾄﾘを表示するﾌﾟﾛｼﾞｪｸﾄ]→.
//				[ｲﾝｸﾙｰﾄﾞ ﾌｧｲﾙ].
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
//				[ﾗｲﾌﾞﾗﾘ ﾌｧｲﾙ].
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86
//
//		[ﾌﾟﾛｼﾞｪｸﾄ]→[○○のﾌﾟﾛﾊﾟﾃｨ]→[構成ﾌﾟﾛﾊﾟﾃｨ]→.
//			[ﾘﾝｶ]→[入力]→[追加の依存ﾌｧｲﾙ].
//					dxguid.lib dinput8.lib			←DirectInput で使用する必要最低限。.
//
//		追加依存ﾌｧｲﾙは、下記の #pragma comment( lib, "○○.lib") で代用可能。.
//		他のプロジェクトにコードを移植することを考えるとこちらの方が楽でよい。.
//
//========================================================================================
#define _USE_MATH_DEFINES
#include <math.h>

#include <dinput.h>	// DirectInput用.

// DirectX 関係。.
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )	// コントローラ操作で使用。.

//列挙体宣言.
//ｷｰ情報.
enum enPKey
{
	enPKey_Up,
	enPKey_Down,
	enPKey_Left,
	enPKey_Right,

	enPKey_A,
	enPKey_B,
	enPKey_X,
	enPKey_Y,
	enPKey_LB,
	enPKey_RB,
	enPKey_BACK,
	enPKey_START,
	enPKey_LSTICK,
	enPKey_RSTICK,

	enPKey_Max	//ｷｰ最大数.
};

//ｽﾃｨｯｸの倒され具合による動作の変化.
enum enPStickSlopeMode
{
	enPSSM_Stand = 0,
	enPSSM_Low,
	enPSSM_High
};



class clsDxInput
{
public:
	clsDxInput();
	~clsDxInput();

	//DirectInput初期設定.
	bool initDI( HWND hWnd );

	//入力情報更新関数.
	HRESULT UpdateInputState();

	//入力情報を追加する関数.add:加える.
	void AddInputState( enPKey enKey );

	//入力情報を初期化する関数.
	void InitInputState();

	//入力ﾁｪｯｸする関数.
	bool IsPressKeyEnter( enPKey enKey );
	bool IsPressKeyStay( enPKey enKey );
	bool IsPressKeyExit( enPKey enKey );

	//ｽﾃｨｯｸの角度.
	float GetStickTheta( LONG lY, LONG lX );
	//ｽﾃｨｯｸの傾き.
	float GetStickSlope( LONG lY, LONG lX );

	//ｽﾃｨｯｸの傾きから倒し具合を判定する.
	enPStickSlopeMode	GetStickSlopeMode( float slope );


	float				m_fStickTheta;//ﾒｲﾝに渡す角度.
	enPStickSlopeMode	m_enStickSlope;//ﾒｲﾝに渡すｽﾃｨｯｸの倒し具合.

private:
	LPDIRECTINPUT8		 m_pDI; //DxInputｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECTINPUTDEVICE8 m_pPad;//ﾃﾞﾊﾞｲｽ(ｺﾝﾄﾛｰﾗ)ｵﾌﾞｼﾞｪｸﾄ.

	unsigned int	m_uInputState;//入力情報.
	unsigned int	m_uInputStateOld;//入力情報.
	float m_fStickSlope;//ｽﾃｨｯｸ距離.

};


#endif//#ifndef _DX_INPUT_H_
