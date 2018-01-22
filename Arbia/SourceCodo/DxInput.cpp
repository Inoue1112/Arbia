#include "DxInput.h"

LPDIRECTINPUT8 pDI2 = NULL;			// DxInputオブジェクト.
LPDIRECTINPUTDEVICE8 pPad2 = NULL;	// デバイス(コントローラ)オブジェクト.

//========== ｽﾃｨｯｸの倒し具合と最大値 ==========.
//#define STICK_MAX	1000
//#define STICK_RUN	975
//#define STICK_WALK	250
const LONG STICK_MAX = 1000;
const float STICK_RUN = 975;
const float STICK_WALK = 250;




// ジョイスティック列挙関数.
BOOL CALLBACK EnumJoysticksCallBack( const DIDEVICEINSTANCE *pdidInstance, VOID *pContext )
{
	HRESULT hRlt;	// 関数復帰値.

	// デバイス(コントローラ)の作成.
	hRlt = pDI2->CreateDevice(
		pdidInstance->guidInstance,	// デバイスの番号.
		&pPad2,	// (out)作成されるデバイスオブジェクト.
		NULL );
	if( hRlt != DI_OK ){
		return DIENUM_CONTINUE;	// 次のデバイスを要求.
	}
	return DIENUM_STOP;	// 列挙停止.
}

// オブジェクトの列挙関数.
BOOL CALLBACK EnumObjectsCallBack( const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext )
{
	// 軸(スティック)を持っているか？.
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		// 入力情報の範囲を設定する.
		DIPROPRANGE diprg;	// 範囲設定構造体.

		diprg.diph.dwSize = sizeof( DIPROPRANGE );			// 構造体のサイズ.
		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );	// ヘッダー情報のサイズ.
		diprg.diph.dwObj = pdidoi->dwType;					// 軸(オブジェクト).
		diprg.diph.dwHow = DIPH_BYID;						// dwObjに設定されるものが.
															// オブジェクトの種類か？インスタンス番号か？.

		diprg.lMax = +STICK_MAX;	// 最大値.
		diprg.lMin = -STICK_MAX;	// 最小値.

		// 範囲を設定.
		if( FAILED( pPad2->SetProperty(
					DIPROP_RANGE,	// 範囲.
					&diprg.diph ) ) )// 範囲設定構造体.
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}


//============================================================
//	DxInputｸﾗｽ.
//============================================================


//ｺﾝｽﾄﾗｸﾀ.
clsDxInput::clsDxInput()
	: m_pDI( NULL )
	, m_pPad( NULL )
{
}

//ﾃﾞｽﾄﾗｸﾀ.
clsDxInput::~clsDxInput()
{
}



//============================================================
//	DirectInputの初期設定。.
//============================================================
bool clsDxInput::initDI( HWND hWnd )
{
	HRESULT hRlt;	// 関数復帰値.
	hRlt = DirectInput8Create(
			GetModuleHandle(NULL),	// DxInputを作成するアプリのハンドル.
			DIRECTINPUT_VERSION,	// 固定：DxInputのバージョン.
			IID_IDirectInput8,		// 固定：UnicodeとAnsiの区別用.
			(VOID**)&m_pDI,			// (out)DxInputオブジェクト.
			NULL);					// NULL固定.
	if( hRlt != DI_OK ){
		MessageBox(NULL, "DxInputの作成に失敗",	"clsDxInput::initDI", MB_OK);
		return false;
	}

	pDI2 = m_pDI;

	// 利用可能なコントローラを探す(列挙する).
	hRlt = (*m_pDI).EnumDevices(
			DI8DEVCLASS_GAMECTRL,	// 全てのゲームコントローラ.
			EnumJoysticksCallBack,	// コントローラの列挙関数.
			NULL,					// コールバック関数からの値.
			DIEDFL_ATTACHEDONLY);	// 繋がっているﾓﾉのみ.
	if( hRlt != DI_OK ){
		MessageBox(NULL, "ｺﾝﾄﾛｰﾗの確認に失敗", "clsDxInput::initDI", MB_OK);
	}

	m_pPad = pPad2;

	// コントローラの接続確認.
	if( pPad2 == NULL ){
		//MessageBox( NULL, "ｺﾝﾄﾛｰﾗが接続されていません", "clsDxInput::initDI", MB_OK );
	}
	else {
		// コントローラ構造体のデータフォーマットを作成.
		hRlt = pPad2->SetDataFormat(
				&c_dfDIJoystick2);	//固定.
		if( hRlt != DI_OK ){
			MessageBox( NULL, "ﾃﾞｰﾀﾌｫｰﾏｯﾄの作成失敗", "clsDxInput::initDI", MB_OK );
		}
		// (他のデバイスとの)協調レベルの設定.
		hRlt = pPad2->SetCooperativeLevel(
				hWnd,
				DISCL_EXCLUSIVE |	// 排他アクセス.
				DISCL_FOREGROUND );	// フォアグラウンドアクセス権.
		if( hRlt != DI_OK ){
			MessageBox( NULL, "協調ﾚﾍﾞﾙの設定失敗", "clsDxInput::initDI", MB_OK );
		}
		// 使用可能なオブジェクト(ボタンなど)の列挙.
		hRlt = pPad2->EnumObjects(
			EnumObjectsCallBack,	// オブジェクト列挙関数.
			(VOID*)hWnd,			// コールバック関数に送る情報.
			DIDFT_ALL );			// 全てのオブジェクト.
		if( hRlt != DI_OK ){
			MessageBox( NULL, "ｵﾌﾞｼﾞｪｸﾄの列挙に失敗", "clsDxInput::initDI", MB_OK );
		}
	}

	return true;
}


//============================================================
//	入力情報更新関数.
//============================================================
HRESULT clsDxInput::UpdateInputState()
{
	HRESULT hRslt;	//関数復帰値.
	DIJOYSTATE2 js;	//ｼﾞｮｲｽﾃｨｯｸ情報構造体.

	//1frem前の情報.
	m_uInputStateOld = m_uInputState;

	//入力情報の初期化.
	InitInputState();

	//ｼﾞｮｲｽﾃｨｯｸの接続確認.
	if( m_pPad == NULL ){
		return E_FAIL;
	}

	//ｼﾞｮｲｽﾃｨｯｸが変更されているかを確認.
	hRslt = m_pPad->Poll();
	if( hRslt != DI_OK && hRslt != DI_NOEFFECT )//DI_NOEFFECT:確認不要なﾃﾞﾊﾞｲｽ.
	{
		//ｺﾝﾄﾛｰﾗへのｱｸｾｽ権を取得する.
		hRslt = m_pPad->Acquire();
		while( hRslt == DIERR_INPUTLOST ){
			hRslt = m_pPad->Acquire();
		}
		return S_OK;
	}

	//ｼﾞｮｲｽﾃｨｯｸから直接ﾃﾞｰﾀを取得.
	hRslt = m_pPad->GetDeviceState(
				sizeof( DIJOYSTATE2 ),	//取得ｻｲｽﾞ.
				&js );					//(out)取得ﾃﾞｰﾀ.
	if( hRslt != DI_OK ){
		return hRslt;
	}

//****************************************************//

	////左ｱﾅﾛｸﾞｽﾃｨｯｸ(ｽﾃｨｯｸの傾き具合(遊び)の値を600,-600として考える).
	//if( js.lX > STICK_PLAY ){
	//	//右ｷｰ.
	//	AddInputState( enPKey_Right );
	//}
	//else if( js.lX < -STICK_PLAY ){
	//	//左ｷｰ.
	//	AddInputState( enPKey_Left );
	//}
	//if( js.lY < -STICK_PLAY ){
	//	//上ｷｰ.
	//	AddInputState( enPKey_Up );
	//}
	//else if( js.lY > STICK_PLAY ){
	//	//下ｷｰ.
	//	AddInputState( enPKey_Down );
	//}

	//ｽﾃｨｯｸ角度.
	m_fStickTheta = GetStickTheta( js.lY, js.lX );
	//ｽﾃｨｯｸの傾き.
	m_fStickSlope = GetStickSlope( js.lY, js.lX );
	//ｽﾃｨｯｸの傾きから倒し具合を判定する.
	m_enStickSlope = GetStickSlopeMode( m_fStickSlope );


	//十字ｷｰ.
	switch( js.rgdwPOV[0] )
	{
	case 4500://右上.
		AddInputState( enPKey_Up );
		AddInputState( enPKey_Right );
		break;
	case 13500://右下.
		AddInputState( enPKey_Down );
		AddInputState( enPKey_Right );
		break;
	case 22500://左下.
		AddInputState( enPKey_Down );
		AddInputState( enPKey_Left );
		break;
	case 31500://左上.
		AddInputState( enPKey_Up );
		AddInputState( enPKey_Left );
		break;
	case 0://上.
		AddInputState( enPKey_Up );
		break;
	case 9000://右.
		AddInputState( enPKey_Right );
		break;
	case 18000://下.
		AddInputState( enPKey_Down );
		break;
	case 27000://左.
		AddInputState( enPKey_Left );
		break;
	}




	//ﾎﾞﾀﾝ(列挙体のﾎﾞﾀﾝの数が増えても対応が楽な書き方).
	//※ﾙｰﾌﾟ開始位置をﾁｪｯｸ対象のﾎﾞﾀﾝに設定する.
	for( int iKey=enPKey_A; iKey<enPKey_Max; iKey++ ){
		if( js.rgbButtons[ iKey - enPKey_A ] & 0x80 ){
			AddInputState( (enPKey)iKey );
		}
	}



	return S_OK;
}


//============================================================
//	入力情報を追加する関数.add:加える.
//============================================================
void clsDxInput::AddInputState( enPKey enKey )
{
	// << ｼﾌﾄ演算子:左にｼﾌﾄ.
	m_uInputState |= 1 << enKey;
	//m_uInputState = m_uInputState | ( 1 << enKey );　と同じ.

}

//============================================================
//	入力情報を初期化する関数.
//============================================================
void clsDxInput::InitInputState()
{
	m_uInputState = 0;
}



//============================================================
//	ｽﾃｨｯｸの角度.
//============================================================
float clsDxInput::GetStickTheta( LONG lY, LONG lX )
{
	double Theta = 0.0;
	Theta = atan2( (double)lY, (double)lX );

	Theta -= M_PI_2;

	return (float)Theta;
}


//============================================================
//	ｽﾃｨｯｸの傾き.
//============================================================
float clsDxInput::GetStickSlope( LONG lY, LONG lX )
{
	lY *= lY;
	lX *= lX;

	float slope = sqrtf( (float)lY + (float)lX );

	return slope;
}

//============================================================
//	ｽﾃｨｯｸの傾きから倒し具合を判定する.
//============================================================
enPStickSlopeMode clsDxInput::GetStickSlopeMode( float slope )
{
	enPStickSlopeMode mode = enPSSM_Stand;
	
	if( slope > abs( STICK_RUN ) ){
		mode = enPSSM_High;
	}
	else if( slope > abs( STICK_WALK ) ){
		mode = enPSSM_Low;
	}
	return mode;
}


//============================================================
//	入力ﾁｪｯｸする関数(単押し).
//		※UpdateInputState()関数で情報が更新されていることが前提.
//============================================================
bool clsDxInput::IsPressKeyEnter( enPKey enKey )
{
	if( ( m_uInputState >> enKey ) & 1 ){
		if( !( ( m_uInputStateOld >> enKey ) & 1 ) ){
				return true;
		}
	}
	return false;
}
//============================================================
//	入力ﾁｪｯｸする関数(長押し).
//		※UpdateInputState()関数で情報が更新されていることが前提.
//============================================================
bool clsDxInput::IsPressKeyStay( enPKey enKey )
{
	if( ( m_uInputState >> enKey ) & 1 ){
		return true;
	}
	return false;
}
//============================================================
//	入力ﾁｪｯｸする関数(離し).
//		※UpdateInputState()関数で情報が更新されていることが前提.
//============================================================
bool clsDxInput::IsPressKeyExit( enPKey enKey )
{
	if( ( m_uInputStateOld >> enKey ) & 1 ){
		if( !( ( m_uInputState >> enKey ) & 1 ) ){
				return true;
		}
	}
	return false;
}


