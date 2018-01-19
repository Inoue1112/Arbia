#ifndef _C_SOUND_H_
#define _C_SOUND_H_

#include <Windows.h>

//ﾗｲﾌﾞﾗﾘ.
#pragma comment( lib, "winmm.lib" )



//文字列ﾊﾞｯﾌｧ.
const int STR_BUFF_MAX = 256;


	//-----  -----//


//============================================================
//	ｻｳﾝﾄﾞｸﾗｽ.
//		midi, mp3, wav形式ﾌｧｲﾙの再生・停止等を行う.
//============================================================
class clsSound
{
public:
	clsSound();
	~clsSound();

	//サウンド構造体.
	struct SOUND_DATA
	{
		char sAlias[STR_BUFF_MAX];	//エイリアス名.
		char sPath[STR_BUFF_MAX];	//ファイルパス (「BGM」or「SE」からでよい ).
		int  iMaxVolume;			//最大音量.
	};


	//音声ﾌｧｲﾙを開く.
	bool Open( LPCTSTR sFName, const char *sAlias, HWND hWnd );

	//音声ﾌｧｲﾙを閉じる.
	bool Close();

	//再生関数.
	bool Play( bool bNotify = false );

	//停止関数.
	bool Stop();

//	//一時停止.
//	bool Pause();
//	//一時停止の解除.
//	bool Resume();


	//巻き戻し関数(再生位置初期化).
	bool SeekToStart();

	//巻き戻して再生.
	void SeekPlay( bool bNotify = false ){
		SeekToStart();
		Play( bNotify );
	}

	//状態取得関数.
	bool GetStatus( char *sStatus );

	//音の停止を確認する関数.
	bool IsStopped();

	//音の再生中を確認する関数.
	bool IsPlaying();


	//音量を設定する.
	bool SetVolume( int iVolume );
	//最大音量を設定する.
	void SetMaxVolume( int iVolume = 1000 ){
		m_iMaxVol = iVolume;
	}

	//その音の最大音量と最大音量(1000)の割合を返す.
	void GetMaxVolRate( int& iVol ){
		float fRate = (float)m_iMaxVol / 1000.0f;
		iVol = (int)( (float)iVol * fRate );
	}



private:
	//初期設定.
	void	SetInitParam( const char *sAlias, HWND hWnd );

	HWND	m_hWnd;					//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	char	m_sAlias[STR_BUFF_MAX];	//ｴｲﾘｱｽ.


	int		m_iMaxVol;//最大音量.

};

#endif//#define _C_SOUND_H_