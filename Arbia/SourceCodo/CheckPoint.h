#ifndef _CHECK_POINT_H_
#define _CHECK_POINT_H_

#include "Sprite.h"

class clsCheckPoint
	: public clsSprite
{
public:
	
		enum enMode
	{
		enM_AHEAD = 0,//ë“ã@.
		enM_IN,
		enM_STAY,
		enM_OUT,
		enM_END,

		enM_MAX
	};

	clsCheckPoint();
	~clsCheckPoint();

	
	//èâä˙âª.
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

	////”√ﬁŸçÏê¨.
	virtual HRESULT InitModel();


	LPSTR	m_sFileName;

	bool	m_bMobe;
	int		m_iTimer;

	float	m_fFloat;//Ç”ÇÌÇ”ÇÌ.
	bool	m_bAcc;

};

#endif //#ifndef _CHECK_POINT_H_