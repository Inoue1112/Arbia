#ifndef _COVOR_MANAGER_H_
#define _COVOR_MANAGER_H_

//#include "Covor.h"
#include "CharaStatic.h"

#include "Sound.h"


const char cCOVER_MAX = 2; 




class clsCoverMgr
	: public clsGameObject
{
public:
	clsCoverMgr();
	~clsCoverMgr();

	enum enCoverMove
	{
		enCM_FLOOR,	//閉じている（不動）.
		enCM_VIB,	//揺れる.
		enCM_OPEN,	//開く（動作）.
		enCM_HOLL,	//穴（開いている）.
		enCM_CLOSE,	//閉じる（動作）.
	};


	void CreateCover( HWND hWnd, int iNo );
	void Init();
	void Release();
	void UpDateModel(){};



	virtual void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye );



	//動き.
	void Move( float fEarZ );
	//踏む.
	void StmpCovor( float fEarZ );
	//強く踏む.
	void StmpCovorStrong( float fEarZ );




	//Mainであたり判定の判断で使う.
	enCoverMove GetCoverMove(){
		return m_enMove;
	}
	char GetCoverMax(){
		return cCOVER_MAX;
	}
	clsCharaStatic* GetCoverPointer( char j ){
		return m_ppCover[j];
	}

	//乗れる?.
	bool isCanStmp(){
		if( m_enMove == enCM_FLOOR ||
			m_enMove == enCM_VIB )
		{
			return true;
		}
		return false;
	}

private:

	//音の種類.
	enum enSound
	{
		enSOUND_VIB = 0,
		enSOUND_OPEN,
		  
		enSOUND_MAX,	//最大数.
	};

	//効果音.
	void PlaySe( enSound enSe, float fEarZ );
	clsSound***	m_pppSe;

	clsCharaStatic** m_ppCover;//ppだけど左右の二つだけ.
	enCoverMove m_enMove;
	int			m_iTimer;

};

#endif//#ifndef _COVOR_MANAGER_H_