#include "Resource.h"
#include "EnemyManager.h"

const float MODEL_SCALE = 1.0f / 20.0f;//初期化用.

const float WALK_RANGE_MAX = 6.0f / 2.0f;//歩く幅(中心からの距離).
const float WALK_RANGE_MIN = 0.5f / 2.0f;//歩く幅(中心からの距離).

const float TWINS_OFFSET = 4.0f / 2.0f;//並ぶ二人の中央からの距離(二倍すると二人の距離).


const int iDOR_DEAD_TIME = 24;//ドア死亡時差.

clsEnemyMgr::clsEnemyMgr()
{
}

clsEnemyMgr::~clsEnemyMgr()
{
	Release();
}




//==================================================
//	.
//==================================================
void clsEnemyMgr::UpDateModel(){}


//==================================================
//	.
//==================================================
void clsEnemyMgr::Release()
{
	if( m_ppEnemy != NULL ){
		for( int i=0; i<m_iEnemyMax; i++ ){
			m_ppEnemy[i]->DetatchModel();
			delete m_ppEnemy[i];
			m_ppEnemy[i] = NULL;
		}
		delete[] m_ppEnemy;
		m_ppEnemy = NULL;
		m_iEnemyMax = 0;
	}
}



//==================================================
//	ﾓﾃﾞﾙ読込.
//==================================================
void clsEnemyMgr::CreateEnemy( HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		 int mode, int iMgrID )
{
	m_enMode = enEMM_1X;
	if( mode < enEMM_MAX ){
		m_enMode = (enENEMY_MGR_MODE)mode;
	}

	//敵の数確定.
	if( enEMM_1X <= m_enMode && m_enMode <= enEMM_1SQ_R ){
		m_iEnemyMax = 1;
	}
	else if( enEMM_2X <= m_enMode && m_enMode <= enEMM_2SQ_R ){
		m_iEnemyMax = 2;
	}
	else if( m_enMode == enEMM_DOOR ){
		m_iEnemyMax = 1;
	}
	else{
		m_iEnemyMax = 0;
	}

	m_ppEnemy = new clsEnemy*[m_iEnemyMax];
	for( int j=0; j<m_iEnemyMax; j++ ){
		m_ppEnemy[j] = new clsEnemy;
		m_ppEnemy[j]->Create( hWnd, pDevice, pContext, iMgrID, j );
		m_ppEnemy[j]->Init( GetPositionZ() );

		CD3DXSKINMESH_INIT si;//skin_Init.
		si.hWnd = hWnd;
		si.pDevice = pDevice;
		si.pDeviceContext = pContext;

		m_ppEnemy[j]->m_pModel = new clsD3DXSKINMESH;
		m_ppEnemy[j]->m_pModel->Init( &si );

		m_ppEnemy[j]->AttachModel(
			clsResource::GetInstance()->GetSkinModels(
				clsResource::enSK_MODEL_ENEMY ) );

		m_ppEnemy[j]->SetScale( MODEL_SCALE );
//		m_ppEnemy[j]->SetAnimSpeed( 0.01 );
	}

	Init( true );
}


//==================================================
//	.
//==================================================
void clsEnemyMgr::Init( bool bFirst )
{
	//ステージの開始でないなら ドアで死んだ奴は 初期化しない.
	if( !bFirst && m_bDoor ){
		return;
	}

	for( int i=0; i<m_iEnemyMax; i++ ){
		m_ppEnemy[i]->Init( GetPositionZ() );
	}

	//敵の位置を初期化.
	SetPosition( GetPosition() );

	m_bDoor = false;
	m_iDoorTimer = 0;
}


//==================================================
//	.
//==================================================
void clsEnemyMgr::SetPosition( D3DXVECTOR3 vPos )
{
	m_vPos = vPos;

	//敵の座標.
	for( int i=0; i<m_iEnemyMax; i++ ){
		m_ppEnemy[i]->SetPosition( GetPosition() );
		//移動の角ｾｯﾄ.
		SetTurnPos( m_enMode, i );
	}
}


//==================================================
//	移動の角ｾｯﾄ.
//==================================================
void clsEnemyMgr::SetTurnPos( enENEMY_MGR_MODE mode, int i )
{
	float x, z;
	x = z = 1.0f;
	float fSecondX, fSecondZ;
	fSecondX = fSecondZ = 0.0f;

	bool bReverse = false;

	//X.                   
	if( mode == enEMM_1X	||
		mode == enEMM_1SQ_L ||
		mode == enEMM_2X_RE	||
		mode == enEMM_2SQ_L  )
	{
		x = WALK_RANGE_MAX;
	}
	else if( 
		mode == enEMM_1SQ_R ||
		mode == enEMM_2SQ_R )
	{
		//逆回転(R).
		x = -WALK_RANGE_MAX;
		bReverse = true;
	}
	else if( mode == enEMM_2X ){
		if( i == 0 )x = WALK_RANGE_MAX;
		else		x = -WALK_RANGE_MAX;
	}
	else if( mode == enEMM_1Z ){
		x = WALK_RANGE_MIN;
	}
	else if( 
		mode == enEMM_2Z ||
		mode == enEMM_2Z_RE )
	{
		x = WALK_RANGE_MIN;
		if( i == 0 )fSecondX = +TWINS_OFFSET;
		else		fSecondX = -TWINS_OFFSET;
	}


	//Z.                   
	if( mode == enEMM_1Z	||
		mode == enEMM_1SQ_L	||
		mode == enEMM_2Z_RE	||
		mode == enEMM_2SQ_L )
	{
		z = WALK_RANGE_MAX;
	}
	else if( 
		mode == enEMM_1SQ_R ||
		mode == enEMM_2SQ_R )
	{
		//逆回転(R).
		z = -WALK_RANGE_MAX;
		bReverse = true;
	}
	else if( mode == enEMM_2Z ){
		if( i == 0 )z = WALK_RANGE_MAX;
		else		z = -WALK_RANGE_MAX;
	}
	else if( mode == enEMM_1X ){
		z = WALK_RANGE_MIN;
	}
	else if( 
		mode == enEMM_2X	||
		mode == enEMM_2X_RE )
	{
		z = WALK_RANGE_MIN;
		if( i == 0 )fSecondZ = +TWINS_OFFSET;
		else		fSecondZ = -TWINS_OFFSET;
	}


	//二人目は対角を目指す.
	bool bSecond = false;
	if( i != 0 ){
		bSecond = true;
	}



//	//ドアで死ぬ.
//	if( mode == enEMM_DOOR ){
//	
//	}




	//ｾｯﾄ.
	m_ppEnemy[i]->SetTurnPos( x, z,fSecondX, fSecondZ, bReverse, bSecond );

}








//==================================================
//	.
//==================================================
COL_STATE* clsEnemyMgr::GetPointerEnemyCol( int i )
{
	return m_ppEnemy[i]->GetPointerCol();
}

//==================================================
//	.
//==================================================
COL_STATE* clsEnemyMgr::GetPointerEnemySubCol( int i )
{
	return &m_ppEnemy[i]->m_colSub;
}



//==================================================
//	.
//==================================================
void clsEnemyMgr::Render( D3DXMATRIX &mView, D3DXMATRIX &mProj,
				D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye )
{
	for( int i=0; i<m_iEnemyMax; i++ ){
		m_ppEnemy[i]->Render( mView, mProj, vLight, vEye );
	}
}


//==================================================
//	.
//==================================================
void clsEnemyMgr::Move( float fEarZ )
{
	for( int i=0; i<m_iEnemyMax; i++ ){
		m_ppEnemy[i]->Move( fEarZ );
	}

	//ドアで死ぬ人.
	if( m_bDoor ){
		//時差.
		if( m_iDoorTimer <= iDOR_DEAD_TIME ){
			//一回だけ.
			if( m_iDoorTimer == iDOR_DEAD_TIME ){
				//念のため全員回す.
				for( int i=0; i<m_iEnemyMax; i++ ){
					m_ppEnemy[i]->Dead();
				}
			}
			m_iDoorTimer ++;
		}
	}
	//こんな書き方をしているのはオーバーライド防止とフラグ増やすのが嫌だったから.
}










//==================================================
//	井上君へ.
//==================================================
int clsEnemyMgr::GetWalkRangeMax()
{
	return (int)( WALK_RANGE_MAX * 2.0f );//定数は中心からの距離なので全長を出すために二倍する.
}
