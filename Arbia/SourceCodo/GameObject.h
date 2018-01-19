#ifndef _C_GAME_OBJ_
#define _C_GAME_OBJ_


#define _USE_MATH_DEFINES
#include <math.h>


#include "Global.h"








class clsGameObject
{
public:
	clsGameObject();
	virtual ~clsGameObject();

	virtual void Init() = 0;

	//ÓÃŞÙÃŞ°ÀŠÖ˜A•t‚¯.
	virtual void AttachModel(){};
	//ÓÃŞÙÃŞ°ÀŠÖ˜A•t‚¯‰ğœ.
	virtual void DetatchModel(){};

	//•`‰æ.
	virtual void Render(){};


	//ˆÊ’uİ’è.
	virtual void SetPosition( D3DXVECTOR3 vPos );//SpiaMge‚Åvirtual.
	void SetPositionX( float fPosX );
	void SetPositionY( float fPosY );
	void SetPositionZ( float fPosZ );
	//æ“¾.
	D3DXVECTOR3 GetPosition();
	float GetPositionX();
	float GetPositionY();
	float GetPositionZ();
	//‘«‚µ‚±‚İ.
	void AddPosition( D3DXVECTOR3 vMove );
	void AddPositionX( float fMoveX );
	void AddPositionY( float fMoveY );
	void AddPositionZ( float fMoveZ );


	//‰ñ“]İ’è.
	//İ’è.
	void SetRotation( D3DXVECTOR3 vRot );
	void SetRotationY( float fRotY );
	void SetRotationZ( float fRotZ );
	//æ“¾.
	D3DXVECTOR3 GetRotation();
	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	//‘«‚µ‚±‚İ.
	void AddRotation( D3DXVECTOR3 vMove );
	void AddRotationY( float fMoveY );
	void AddRotationZ( float fMoveZ );


	//Šgkİ’è.
	void SetScale( float fScale );
	float GetScale();


	//‹——£‚Å‰¹—Ê‚ğ•Ï‚¦‚é.
	int ChangeVolumeDistance( float fEarPosZ, float fThisPosZ );


protected:

	//À•W‚â‰ñ“]’l‚ğXV‚·‚é.
	virtual void UpDateModel() = 0;
	//360`0‚É”[‚ß‚é.
	void ThetaOverGuard( float& theta );
	void ThetaOverGuard( double& theta );

//public:
	D3DXVECTOR3		m_vPos;		//ˆÊ’u.
	D3DXVECTOR3		m_vRot;		//‰ñ“].
	float			m_fScale;	//Šgk.

private:


};



#endif//#ifndef _C_GAME_OBJ_