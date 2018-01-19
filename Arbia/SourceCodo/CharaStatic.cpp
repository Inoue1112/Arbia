#include "CharaStatic.h"



clsCharaStatic::clsCharaStatic()
{
}

clsCharaStatic::~clsCharaStatic()
{
}



//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsCharaStatic::Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
	D3DXVECTOR4 vColor , bool alphaFlg )
{

	//座標更新する.
	UpDateModel();
	
	//ﾒｯｼｭのﾚﾝﾀﾞﾘﾝｸﾞ関数を呼ぶ.
	m_pModel->Render( mView, mProj, vLight, vEye, vColor, alphaFlg );
}

//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け関数clsDX9Mesh
void clsCharaStatic::AttachModel( clsDX9Mesh* pModel )
{
	if( pModel != NULL ){
		m_pModel = pModel;
	}
}

//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除関数.
void clsCharaStatic::DetatchModel()
{
	if( m_pModel != NULL ){
		m_pModel = NULL;
	}
}




//座標や回転値を更新する.
void clsCharaStatic::UpDateModel()
{
	if( m_pModel == NULL ){
		return;
	}

	m_pModel->m_vPos	= m_vPos;
	m_pModel->m_fYaw	= m_vRot.y;
	m_pModel->m_fPitch= m_vRot.x;
	m_pModel->m_fRoll	= m_vRot.z;
	m_pModel->m_fScale= m_fScale;
}


//============================================================
//	位置更新関数.
//============================================================
void clsCharaStatic::UpdatePos()
{
	//ｷｬﾗｸﾀ移動処理(ﾗｼﾞｺﾝ)
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3	vecAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw );	//Y軸回転行列.

	//方向によって進行させる値を設定.
	switch( m_enDir )
	{
	case enDirection_Stop:
		break;
	case enDirection_Foward:	//前進.
		//向かう方向*進める値(0.1f).
		m_vPos -= vecAxisZ * 0.15f * m_fSpd;
//		m_fWalkLangth -= 0.15f * m_fSpd;
		break;
	case enDirection_BackWard:	//後退.
		m_vPos += vecAxisZ * 0.15f * m_fSpd;
//		m_fWalkLangth -= 0.15f * m_fSpd;
		break;
	//case enDirection_LeftTurn:
	//	break;
	//case enDirection_RightTurn:
	//	break;
	//default:
	//	break;
	}
	//m_enDir = enDirection_Stop;//停止.

}

//==================================================
//	位置更新関数.
//==================================================
void clsCharaStatic::UpdateColState()
{
	ColState.vPos = m_vPos;
	ColState.fYaw = m_vRot.y;
}
//==================================================
//	あたり判定用.
//==================================================
void clsCharaStatic::SetColPos( D3DXVECTOR3* vPos )
{
	ColState.vPos = *vPos;
}



//==================================================
//	あたり判定の情報のアドレス取得.
//==================================================
COL_STATE* clsCharaStatic::GetPointerCol()
{
	return &ColState;
}



