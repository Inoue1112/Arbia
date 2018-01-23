#ifndef _MY_MACRO_H_	//²Ý¸Ù°ÄÞ¶Þ°ÄÞ.
#define _MY_MACRO_H_


//============================================================
//	Ï¸Û.
//============================================================

//´×°Ò¯¾°¼Þ.
#define ERR_MSG(str,title)	{MessageBox(NULL,str,title,MB_OK);}


//‰ð•ú.
#define SAFE_RELEASE(x)		if(x){(x)->Release();	(x)=NULL;}
#define SAFE_DELETE(p)		{ if(p) { delete (p);   (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p); (p)=NULL; } }

//·°ÎÞ°ÄÞ“ü—Í.
//‰Ÿ‚µ‚Ä‚¢‚éŠÔ.
#define GETKEY_STAY(x) if (GetAsyncKeyState(x) & 0x8000)	
//‰Ÿ‚µ‚½‚Æ‚«‚¾‚¯.
#define GETKEY_DOWN(x) if (GetAsyncKeyState(x) & 0x0001)

#endif//#ifndef _MY_MACRO_H_