#ifndef _C_XINPUT_H_
#define _C_XINPUT_H_

#include <Windows.h>
#include <Xinput.h>

#pragma comment( lib, "xinput.lib" )

//値.
#define INPUT_TRIGGER_MIN	(0)
#define INPUT_TRIGGER_MAX	(255)
#define INPUT_THUMB_MIN		(-32768)
#define INPUT_THUMB_MAX		(32767)
#define INPUT_VIBRATION_MIN	(0)
#define INPUT_VIBRATION_MAX	(65535)



//キー.
#define XINPUT_UP		( XINPUT_GAMEPAD_DPAD_UP )
#define XINPUT_DOWN		( XINPUT_GAMEPAD_DPAD_DOWN )
#define XINPUT_LEFT		( XINPUT_GAMEPAD_DPAD_LEFT )
#define XINPUT_RIGHT	( XINPUT_GAMEPAD_DPAD_RIGHT )
#define XINPUT_START	( XINPUT_GAMEPAD_START )
#define XINPUT_BACK		( XINPUT_GAMEPAD_BACK )
#define XINPUT_LSTICK	( XINPUT_GAMEPAD_LEFT_THUMB )
#define XINPUT_RSTICK	( XINPUT_GAMEPAD_RIGHT_THUMB )
#define XINPUT_LB		( XINPUT_GAMEPAD_LEFT_SHOULDER )
#define XINPUT_RB		( XINPUT_GAMEPAD_RIGHT_SHOULDER )
#define XINPUT_A		( XINPUT_GAMEPAD_A )
#define XINPUT_B		( XINPUT_GAMEPAD_B )
#define XINPUT_X		( XINPUT_GAMEPAD_X )
#define XINPUT_Y		( XINPUT_GAMEPAD_Y )


class clsXInput
{
public:
	//ｽﾃｨｯｸの倒され具合による動作の変化.
	enum enSTICK_SLOPE
	{
		enSS_NOTHING = 0,
		enSS_LOW,
		enSS_HIGH
	};

	clsXInput(){
		ZeroMemory( this, sizeof( clsXInput ) );
	}
	~clsXInput(){}

	bool UpdateStatus();
	bool UpdateKeyStatus();

	bool IsPressEnter( const WORD _padKey );
	bool IsPressStay( const WORD _padKey );
	bool IsPressExit( const WORD _padKey );


	void EndProc(){
		SetVibration( 0, 0 );
//		XInputEnable( false );
	}

	BYTE GetLTrigger(){
		return m_state.Gamepad.bLeftTrigger;
	}
	BYTE GetRTrigger(){
		return m_state.Gamepad.bRightTrigger;
	}
	SHORT GetLThumbX(){
		return m_state.Gamepad.sThumbLX;
	}
	SHORT GetLThumbY(){
		return m_state.Gamepad.sThumbLY;
	}
	SHORT GetRThumbX(){
		return m_state.Gamepad.sThumbRX;
	}
	SHORT GetRThumbY(){
		return m_state.Gamepad.sThumbRY;
	}


	//左スティックの角度.
	float GetLStickTheta();
	//右スティックの角度.
	float GetRStickTheta();

	//各スティックの倒し具合.
	enSTICK_SLOPE GetLStickSlope();
	enSTICK_SLOPE GetRStickSlope();


	//振動を与えよう.
	void SetVibPowerL( int iVibL, int iTime, int iVibDecL = 0 );
	void SetVibPowerR( int iVibR, int iTime, int iVibDecR = 0 );

private:
	//振動.
	bool SetVibration( WORD LMotorSpd, WORD RMotorSpd );

	//スティックの角度.
	float GetStickTheta( SHORT lY, SHORT lX );
	//スティックの傾き.
	float GetStickSlope( SHORT lY, SHORT lX );


	DWORD				m_padId;
	XINPUT_STATE		m_state;
	XINPUT_STATE		m_stateOld;//判断用.
	XINPUT_KEYSTROKE	m_keystroke;
	XINPUT_VIBRATION	m_vibration;


	//振動補助.
	void VibSafe( int &iVibPower, int &iVibDec );//範囲内に収める.
	void VibDecrease( int &iVibPower, int iDecPower );//減衰.
	void VibTimerCnt();//タイマーカウント.
	int		m_iVibL;//振動力.
	int		m_iVibR;
	int		m_iVibDecL;//減衰する力.
	int		m_iVibDecR;
	int		m_iVibTimerL;//収まる時間.
	int		m_iVibTimerR;
	

};

#endif//#ifndef _C_XINPUT_H_


