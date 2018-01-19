#include "Sound.h"
#include <stdio.h>



//ｺﾝｽﾄﾗｸﾀ.
clsSound::clsSound()
{
	ZeroMemory( this, sizeof( clsSound ) );
}

//ﾃﾞｽﾄﾗｸﾀ.
clsSound::~clsSound()
{
}


//音声ﾌｧｲﾙを開く.
bool clsSound::Open( LPCTSTR sFName, const char *sAlias, HWND hWnd )
{
	//初期設定.
	SetInitParam( sAlias, hWnd );

	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "open Data\\Sound\\";//面倒なﾊﾟｽを引数から省略.

	strcat_s( command, sizeof( command ), sFName );		//ﾌｧｲﾙ名を結合.
	strcat_s( command, sizeof( command ), " type mpegvideo alias " );//「 type mpegvideo 」: こいつを入れることで、wavをmp3と同じように開く(扱える)ので、音量の変化を受け付けるようになる.
	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合 : ｴｲﾘｱｽ(ﾆｯｸﾈｰﾑのようなもの).

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//音声ﾌｧｲﾙを閉じる.
bool clsSound::Close()
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "close ";
	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
													//ｴｲﾘｱｽ(ﾆｯｸﾈｰﾑのようなもの).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//再生関数.
bool clsSound::Play( bool bNotify )
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "play ";
	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.

	//	ﾙｰﾌﾟする場合ﾒｯｾｰｼﾞを送る.
	if( bNotify ){
		strcat_s( command, sizeof( command ), " notify" );
		//notify: 処理が終了したときにhWndにMM_MCINOTIFYﾒｯｾｰｼﾞをﾎﾟｽﾄする.
	}

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//停止関数.
bool clsSound::Stop()
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "stop ";
	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
													//ｴｲﾘｱｽ(ﾆｯｸﾈｰﾑのようなもの).
	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		SeekToStart();
		return true;
	}
	return false;
}

////一時停止.
//bool clsSound::Pause()
//{
//	//ｺﾏﾝﾄﾞ.
//	char command[STR_BUFF_MAX] = "pause ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
//													//ｴｲﾘｱｽ(ﾆｯｸﾈｰﾑのようなもの).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}
//
////一時停止の解除.
//bool clsSound::Resume()
//{
//	//ｺﾏﾝﾄﾞ.
//	char command[STR_BUFF_MAX] = "resume ";
//	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
//													//ｴｲﾘｱｽ(ﾆｯｸﾈｰﾑのようなもの).
//	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
//		SeekToStart();
//		return true;
//	}
//	return false;
//}



//巻き戻し関数(再生位置初期化).
bool clsSound::SeekToStart()
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "seek ";

	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
	strcat_s( command, sizeof( command ), " to start" );

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//状態取得関数.
//	sStatus の配列は256以下にすること.
//※ただし、状態を取得する場合は、再生時に「notify」を設定し、.
//	ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙのﾒｯｾｰｼﾞを送っておく必要がある.
bool clsSound::GetStatus( char *sStatus )
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "status ";
	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
	strcat_s( command, sizeof( command ), " mode" );

	if( mciSendString( command, sStatus, ( STR_BUFF_MAX - 1 ), m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//音の停止を確認する関数.
bool clsSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "stopped" ) == 0 ){
			return true;
		}
	}
	return false;
}

//音の再生中を確認する関数.
bool clsSound::IsPlaying()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "playing" ) == 0 ){
			return true;
		}
	}
	return false;
}

//============================================================
//	音量を設定する.
//============================================================
bool clsSound::SetVolume( int ivolume )
{
	//数値の丸め.
	if( ivolume > 1000 )	{ ivolume = 1000; }
	else if( ivolume < 0 )	{ ivolume = 0	; }
	
	//音量を文字列に変換.
	char sVolume[STR_BUFF_MAX] = "";
	sprintf_s( sVolume, sizeof( sVolume ), "%d", ivolume );

	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "setaudio ";

	strcat_s( command, sizeof( command ), m_sAlias );	//ｴｲﾘｱｽを結合.
	strcat_s( command, sizeof( command ), " volume to " );
	strcat_s( command, sizeof( command ), sVolume );//音量を結合.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}



//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*


//============================================================
//初期設定.
//============================================================
void clsSound::SetInitParam( const char *sAlias, HWND hWnd )
{
	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙを渡す.
	m_hWnd = hWnd;

	//ｴｲﾘｱｽ.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}
