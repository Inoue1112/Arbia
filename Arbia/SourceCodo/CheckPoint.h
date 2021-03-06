#ifndef _CHECK_POINT_H_
#define _CHECK_POINT_H_

#include "Sprite.h"

class clsCheckPoint
	: public clsSprite
{
public:
	
		enum enMode
	{
		enM_AHEAD = 0,//待機.
		enM_IN,
		enM_STAY,
		enM_OUT,
		enM_END,

		enM_MAX
	};

	clsCheckPoint();
	~clsCheckPoint();

	
	//初期化.
	HRESULT Create( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPSTR sFileName );

	void Init();


	bool Stay();


	void SetMove( bool bMove ){
		m_bMobe = bMove;
	}
	bool GetMove(){
		return m_bMobe;
	}


	enMode m_enMode;


private:

	////ﾓﾃﾞﾙ作成.
	virtual HRESULT InitModel();


	LPSTR	m_sFileName;

	bool	m_bMobe;
	int		m_iTimer;

	float	m_fFloat;//ふわふわ.
	bool	m_bAcc;

};

#endif //#ifndef _CHECK_POINT_H_