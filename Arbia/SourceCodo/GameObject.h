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

	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け.
	virtual void AttachModel(){};
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除.
	virtual void DetatchModel(){};

	//描画.
	virtual void Render(){};


	//位置設定.
	virtual void SetPosition( D3DXVECTOR3 vPos );//SpiaMgeでvirtual.
	void SetPositionX( float fPosX );
	void SetPositionY( float fPosY );
	void SetPositionZ( float fPosZ );
	//取得.
	D3DXVECTOR3 GetPosition();
	float GetPositionX();
	float GetPositionY();
	float GetPositionZ();
	//足しこみ.
	void AddPosition( D3DXVECTOR3 vMove );
	void AddPositionX( float fMoveX );
	void AddPositionY( float fMoveY );
	void AddPositionZ( float fMoveZ );


	//回転設定.
	//設定.
	void SetRotation( D3DXVECTOR3 vRot );
	void SetRotationY( float fRotY );
	void SetRotationZ( float fRotZ );
	//取得.
	D3DXVECTOR3 GetRotation();
	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	//足しこみ.
	void AddRotation( D3DXVECTOR3 vMove );
	void AddRotationY( float fMoveY );
	void AddRotationZ( float fMoveZ );


	//拡縮設定.
	void SetScale( float fScale );
	float GetScale();


	//距離で音量を変える.
	int ChangeVolumeDistance( float fEarPosZ, float fThisPosZ );


protected:

	//座標や回転値を更新する.
	virtual void UpDateModel() = 0;
	//360〜0に納める.
	void ThetaOverGuard( float& theta );
	void ThetaOverGuard( double& theta );

//public:
	D3DXVECTOR3		m_vPos;		//位置.
	D3DXVECTOR3		m_vRot;		//回転.
	float			m_fScale;	//拡縮.

private:


};



#endif//#ifndef _C_GAME_OBJ_