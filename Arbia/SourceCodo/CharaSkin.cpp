#include "CharaSkin.h"

clsCharaSkin::clsCharaSkin()
{
	m_pModel = NULL;
	m_pAnimCtrl = NULL;
	m_dAnimSpeed = 0.025;
	//m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_fScale = 1.0f;



}

clsCharaSkin::~clsCharaSkin()
{
//	if( m_pModel != NULL ){
//		delete m_pModel;
//		m_pModel = NULL;
//	}
}


//============================================================
//	各種値の更新.
//============================================================
void clsCharaSkin::UpDateModel()
{
	if( m_pModel == NULL ){
		return;
	}

	m_pModel->m_vPos	= m_vPos;

	m_pModel->m_fYaw	= m_vRot.y;
	m_pModel->m_fPitch	= m_vRot.x;
	m_pModel->m_fRoll	= m_vRot.z;

	m_pModel->m_vScale	=
		D3DXVECTOR3( m_fScale, m_fScale, m_fScale );

	m_pModel->m_dAnimSpeed = m_dAnimSpeed;
}


//============================================================
//	ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
//============================================================
void clsCharaSkin::AttachModel( clsD3DXSKINMESH* pModel )
{
	if( pModel == NULL ){
		return;
	}
	//ﾓﾃﾞﾙ設定.
	m_pModel = pModel;
	//ｱﾆﾒｰｼｮﾝ速度.
//	m_dAnimSpeed = m_pModel->GetAnimSpeed();
	//ｽｹｰﾙ設定.
	m_fScale = m_pModel->m_vScale.x;

	//ｱﾆﾒｰｼｮﾝｺﾝﾄﾛｰﾗのｸﾛｰﾝ作成.
	LPD3DXANIMATIONCONTROLLER pAC
		= m_pModel->GetAnimController();
	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(),
		pAC->GetMaxNumAnimationSets(),
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_pAnimCtrl );	//(out)ｱﾆﾒｰｼｮﾝｺﾝﾄﾛｰﾗ.
}
//============================================================
//	ﾓﾃﾞﾙﾃﾞｰﾀ関連付け解除.
//============================================================
void clsCharaSkin::DetatchModel()
{
	if( m_pModel != NULL ){
		m_pModel = NULL;

	
		if( m_pAnimCtrl != NULL ){
			m_pAnimCtrl->Release();
			m_pAnimCtrl = NULL;
		}
	}
}




//============================================================
//	ﾚﾝﾀﾞﾘﾝｸﾞ.
//============================================================
void clsCharaSkin::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye,
	D3DXVECTOR4 &vColor, bool alphaFlg )
{
	if( m_pModel == NULL || m_pAnimCtrl == NULL ){
		return;
	}
	//影.
	if( m_pShadow != NULL ){
		m_pShadow->Render( mView, mProj, vEye );
	}

	UpDateModel();

	m_pModel->Render(
		mView, mProj, vLight, vEye,
		vColor, alphaFlg, m_pAnimCtrl );
}

//============================================================
//	ｱﾆﾒｰｼｮﾝ最大数を取得する.
//============================================================
int clsCharaSkin::GetAnimSetMax()
{
	if( m_pAnimCtrl != NULL ){
		return (int)m_pAnimCtrl->GetMaxNumAnimationSets();
	}
	return -1;
}

//============================================================
//	ｱﾆﾒｰｼｮﾝ切替.
//============================================================
void clsCharaSkin::ChangeAnimSet( int index, double dStatPos )
{
	//ｱﾆﾒｰｼｮﾝの範囲外かﾁｪｯｸする.
	if( index < 0 || index >= GetAnimSetMax() ){
		return;
	}
	m_pModel->
		ChangeAnimSet_StartPos( index, dStatPos, m_pAnimCtrl );
}


////============================================================
//	角度判定.
////============================================================
bool clsCharaSkin::ThetaCheck( double dMyTheta, double dTargTheta,
	int iSarchTheta )
{
	double d_thetaArea_h = (double)iSarchTheta / 180.0 * M_PI / 2.0;
	double thetaSearchR, thetaSearchL;
	thetaSearchR = dMyTheta - d_thetaArea_h;
	thetaSearchL = dMyTheta + d_thetaArea_h;

	//角度が合っていたら.
	if( thetaSearchL > dTargTheta &&
		thetaSearchR < dTargTheta )
	{
		return true;
	}
	////一周対策(0と360の境目).
	else{
		if( dMyTheta < dTargTheta ){
			dTargTheta -= M_PI * 2.0;
			if( thetaSearchL > dTargTheta &&
				thetaSearchR < dTargTheta )
			{
				return true;
			}
		}
		else if( dMyTheta > dTargTheta ){
			dTargTheta += M_PI * 2.0;
			if( thetaSearchL > dTargTheta &&
				thetaSearchR < dTargTheta )
			{
				return true;
			}
		}
	}

	return false;
}


//============================================================
//	一瞬で振り向かない(徐々に振り向く).
//============================================================
void clsCharaSkin::YawSpnToTarg( float& NowYaw, float TarYaw, float TurnSpd, float TurnStop )
{
	//360,0問題解決.
	if( TarYaw - NowYaw > (float)M_PI ){
		TarYaw -= (float)( M_PI * 2.0 );
	}
	else if( TarYaw - NowYaw < (float)( -M_PI ) ){
		TarYaw += (float)( M_PI * 2.0 );
	}

	//角度が近づく.
	if( abs( TarYaw - NowYaw ) > TurnStop ){
		if( NowYaw < TarYaw ){
			NowYaw += TurnSpd;
		}
		else if( NowYaw > TarYaw ){
			NowYaw -= TurnSpd;
		}
	}
}



//==================================================
//	位置更新関数.
//==================================================
void clsCharaSkin::UpdatePos()
{
	D3DXMATRIX mYaw;
	//回転.
	D3DXMatrixRotationY( &mYaw, m_vRot.y );	//Y軸回転.

	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord( 
		&vecAxisZ,	//(out).
		&vecAxisZ,	//.
		&mYaw );	//Y軸回転行列.

	//方向によって進行させる値を設定.
	switch( m_enDir )
	{
	case enDirection_Stop:
		break;
	case enDirection_Foward:	//前進.
		//向かう方向*進める値(0.1f).
		m_vPos -= vecAxisZ * 0.15f * m_fSpd;
		break;
	case enDirection_BackWard:	//後退.
		m_vPos += vecAxisZ * 0.15f * m_fSpd;
		break;
	//case enDirection_LeftTurn:
	//	break;
	//case enDirection_RightTurn:
	//	break;
	//default:
	//	break;
	}
	//m_enDir = enDirection_Stop;//停止.
	//==================================================

}
//==================================================
//	あたり判定情報更新関数.
//==================================================
void clsCharaSkin::UpdateColState()
{
	ColState.vPos = m_vPos;
	ColState.fYaw = m_vRot.y;
}


//==================================================
//	あたり判定の情報のアドレス取得.
//==================================================
COL_STATE* clsCharaSkin::GetPointerCol()
{
	return &ColState;
}
//==================================================
//	あたり判定用.
//==================================================
void clsCharaSkin::SetColPos( D3DXVECTOR3* vPos, float fYaw )
{
	ColState.vPos = *vPos;
	if( fYaw < -1.0f ){
		ColState.fYaw = fYaw;
	}
}


//==================================================
//	.
//==================================================
void clsCharaSkin::SetDropoutFlg( bool flg )
{
	m_bDropout = flg;
}



