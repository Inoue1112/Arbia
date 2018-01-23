#ifndef _C_CHARA_STATIC_H_
#define _C_CHARA_STATIC_H_


#include "GameObject.h"
#include "DX9Mesh.h"

#include "Collision.h"

class clsCharaStatic
	: public clsGameObject
{
public:
	clsCharaStatic();
	virtual ~clsCharaStatic();

	virtual void Init(){};



	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け関数clsDX9Mesh
	virtual void AttachModel( clsDX9Mesh* pModel );

	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除関数.
	virtual void DetatchModel();

	//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
	virtual void Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
				D3DXVECTOR4 vColor = D3DXVECTOR4( 1.0f,1.0f,1.0f,1.0f ),
				bool alphaFlg = false );

	//位置更新関数.
	void UpdatePos();
	enDirection		m_enDir;	//方向.
	float			m_fSpd;

	//あたり判定用.
	COL_STATE* GetPointerCol();

	clsDX9Mesh*		m_pModel;

//	LPD3DXMESH	m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.


	//あたり判定用.
	void SetColPos( D3DXVECTOR3* vPos );




protected:

	//あたり判定用.
	COL_STATE ColState;
	virtual void UpdateColState();//ﾑｰﾌﾞの最後に使う.



	//座標や回転値を更新する.
	virtual void UpDateModel();

};


#endif//#ifndef _C_CHARA_STATIC_H_