#ifndef _C_CHARA_SKIN_H_
#define _C_CHARA_SKIN_H_

#include "GameObject.h"
#include "D3DXSKINMESH.h"//ｽｷﾝﾒｯｼｭｸﾗｽ.

#include "Collision.h"


#include "SpShadow.h"

const float NOT_YAW_UPDATE = -999.0f;

//アニメーション一フレーム食い込み問題のごり押しの解決.
const double dANIM_ONE_FRAME_OVER_SOLUTION = 0.001;


class clsCharaSkin
	: public clsGameObject
{
public:
	clsCharaSkin();
	virtual ~clsCharaSkin();

	virtual void Init(){}

	//ﾚﾝﾀﾞﾘﾝｸﾞ.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye,
		D3DXVECTOR4 &vColor = D3DXVECTOR4( 1.0f, 1.0f, 1.0f ,1.0f ),
		bool alphaFlg = false );

	//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
	virtual void AttachModel( clsD3DXSKINMESH* pModel );
	//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除.
	virtual void DetatchModel();


	//ｱﾆﾒｰｼｮﾝ最大数を取得.
	int GetAnimSetMax();
	//ｱﾆﾒｰｼｮﾝ切替.
	void ChangeAnimSet( int index, double dStatPos = 0.0 );
	//ｱﾆﾒｰｼｮﾝ速度設定.
	void SetAnimSpeed( double dSpeed ){ m_dAnimSpeed = dSpeed; }



	//位置更新関数.
	void UpdatePos();
	enDirection		m_enDir;	//方向.
	float	m_fSpd;
	float	m_fYawTarget;
	float	m_fFloorY;	//着地する高さ.


	//ｽﾋﾟｰﾄﾞ変更.
	void SetSpeed(){
		m_pModel->m_fSpd = m_fSpd;
	}
	void AddSpeed( float fPlusSpd ){
		m_pModel->m_fSpd = m_fSpd + fPlusSpd;
	}


	//角度判定.
	bool ThetaCheck( double dMyTheta, double dTargTheta, int iSarchTheta );

	//あたり判定用.
	COL_STATE* GetPointerCol();


	clsD3DXSKINMESH*			m_pModel;//ｽｷﾝﾒｯｼｭﾓﾃﾞﾙ.



	D3DXVECTOR3	m_vRay;			//ﾚｲの位置.
	D3DXVECTOR3	m_vAxis;		//回転軸.
//	LPD3DXMESH	m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.


	//落下フラグセット.
	void SetDropoutFlg( bool flg );

	//あたり判定用.
	void SetColPos( D3DXVECTOR3* vPos, float fYaw = NOT_YAW_UPDATE );


protected:

	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;//ｱﾆﾒｰｼｮﾝ制御用.
	double						m_dAnimSpeed;//ｱﾆﾒｰｼｮﾝ速度.
	double						m_dAnimTimer;//アニメーション時間.

	//一瞬で振り向かない(徐々に振り向く).
	void YawSpnToTarg( 
		float& NowYaw, float TarYaw, 
		float TurnSpd, float TurnStop = 0.1f );


	//座標や回転値を更新する.
	virtual void UpDateModel();

	//あたり判定用.
	COL_STATE ColState;
	virtual void UpdateColState();//ﾑｰﾌﾞの最後に使う.
//	float	m_fHeight;
//	int		m_iSarchTheta;


	bool		m_bDropout;	//落下フラグ.

	clsShadow* m_pShadow;


private:

};



#endif//#ifndef _C_CHARA_SKIN_H_