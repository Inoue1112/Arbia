#include "CollisionRay.h"


const float WALL_HIT	= 1.01f;	//壁との衝突距離.//0.8f.
const float WALL_SPACE	= WALL_HIT;	//壁との壁と開ける距離.//0.8f.



clsCollisionRay::clsCollisionRay()
{
}

clsCollisionRay::~clsCollisionRay()
{
}


//============================================================
//	壁のあたり判定関連.
//============================================================
void clsCollisionRay::WallJudge( clsCharaSkin* pWalker, clsCharaStatic* pWall )// clsDX9Mesh* pWalker, clsDX9Mesh* pWall .
{
	const char MAX = 4;//.線の数
	const float HitHeight = 0.375f;	//ｱﾙﾋﾞｱの足元からの高さ.

	FLOAT		fDistance[MAX] = { 0.0f, 0.0f, 0.0f, 0.0f };	//距離.
	D3DXVECTOR3	vIntersect[MAX];//交点座標.
	float		fDis, fYaw;		//距離と回転.


	pWalker->m_vRay = pWalker->GetPosition();
	pWalker->m_vRay.y += HitHeight;

#if 0
	//軸ﾍﾞｸﾄﾙの方向前後右左.
	D3DXVECTOR3 Axis[MAX] ={D3DXVECTOR3( 0.0f, 0.0f, 1.0f ),
							D3DXVECTOR3( 0.0f, 0.0f,-1.0f ),
							D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
							D3DXVECTOR3(-1.0f, 0.0f, 0.0f ) };
	//ﾚｲの向きにより当たる壁までの距離を求める.
	//軸ﾍﾞｸﾄﾙ　前後右左.
	for( char i=0; i<MAX; i++ ){
		pWalker->m_vAxis = Axis[i];
		Intersect( pWalker, pWall, &fDistance[i], &vIntersect[i] );
	}
#else
	//軸ﾍﾞｸﾄﾙ　前.
	pWalker->m_vAxis = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	Intersect( pWalker, pWall, &fDistance[0], &vIntersect[0] );
	//軸ﾍﾞｸﾄﾙ　後.
	pWalker->m_vAxis = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
	Intersect( pWalker, pWall, &fDistance[1], &vIntersect[1] );
	//軸ﾍﾞｸﾄﾙ　右.
	pWalker->m_vAxis = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	Intersect( pWalker, pWall, &fDistance[2], &vIntersect[2] );
	//軸ﾍﾞｸﾄﾙ　左.
	pWalker->m_vAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
	Intersect( pWalker, pWall, &fDistance[3], &vIntersect[3] );
#endif

	//絶対値.
	fYaw = fabs( pWalker->GetRotationY() );	
	DirOverGuard( &fYaw );


	//前が壁に接近.
	fDis = fDistance[0];
	if( fDis < WALL_HIT && fDis > 0.01f ){
		if( pWalker->GetRotationY() < 0.0f ){
			//時計回り.
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else{										//奥から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
		}
		else{
			//反時計回り.
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else{										//奥から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
		}
	}

	//後ろが壁に接近.
	fDis = fDistance[1];
	if( fDis < WALL_HIT && fDis > 0.01f ){
		//時計回り.
		if( pWalker->GetRotationY() < 0.0f ){
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else{										//奥から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
		}
		//反時計回り.
		else{
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else{										//奥から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
		}
	}

	//右が壁に接近.
	fDis = fDistance[2];
	if( fDis < WALL_HIT && fDis > 0.01f ){
		//時計回り.
		if( pWalker->GetRotationY() < 0.0f ){
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else{										//奥から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
		}
		//反時計回り.
		else{
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else{										//奥から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
		}
	}

	//左が壁に接近.
	fDis = fDistance[3];
	if( fDis < WALL_HIT && fDis > 0.01f ){
		//時計回り.
		if( pWalker->GetRotationY() < 0.0f ){
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else{										//奥から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
		}
		//反時計回り.
		else{
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pWalker->AddPositionZ( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 2.355f && fYaw < 3.925f ){	//前から.
				pWalker->AddPositionX( -( WALL_SPACE - fDis ) );
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pWalker->AddPositionZ( WALL_SPACE - fDis );
			}
			else{										//奥から.
				pWalker->AddPositionX( WALL_SPACE - fDis );
			}
		}
	}
}




//============================================================
//	ﾚｲとﾒｯｼｭのあたり判定.
//============================================================
bool clsCollisionRay::Intersect(
	clsCharaSkin* pWalker,		//基準のの物体.
	clsCharaStatic* pTarget,		//対象のの物体.
	float* pfDistance,			//(out)距離.
	D3DXVECTOR3* pvIntersect )	//(out)交差座標.
{
	D3DXMATRIXA16	matRot;	//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY( &matRot, pWalker->GetRotationY() );

	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ;
	//Z軸ﾍﾞｸﾄﾙそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pWalker->m_vAxis, &matRot );

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = pWalker->m_vRay;
	//Attackerの座標に回転座標を合成する.
	vecEnd += vecAxisZ * 1.0f;

	//対象が動いている物体でも、対象のworld行列の、.
	//逆行列を用いれば、正しくﾚｲが当たる.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->GetPositionX(),
		pTarget->GetPositionY(),
		pTarget->GetPositionZ() );

	//逆行列を求める.
	D3DXMatrixInverse( &matWorld, NULL, &matWorld );
	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld );
	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld );

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;	//命中ﾌﾗｸﾞ.

	DWORD dwIndex = 0;	//ｲﾝﾃﾞｯｸｽ番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;	//(out)重心ﾋｯﾄ座標.

	//ﾒｯｼｭとﾚｲの交差を調べる.
	D3DXIntersect(
		pTarget->m_pModel->m_pMesh,//対象ﾒｯｼｭ.
		&vecStart,		//開始位置.
		&vecDistance,	//ﾚｲの距離.
		&bHit,			//(out)判定結果.
		&dwIndex,		//(out)bHitがTrue時、ﾚｲの視点に。.
						//		最も近くの面のｲﾝﾃﾞｯｸｽ値へのﾎﾟｲﾝﾀ.
		&U, &V,			//(out)重心ﾋｯﾄ座標.
		pfDistance,		//ﾀｰｹﾞｯﾄとの距離.
		NULL, NULL );

	if( bHit ){
		//命中したとき.
		FindVerticesOnPoly(
			pTarget->m_pModel->m_pMeshForRay, dwIndex, vVertex );
		//重心座標から交差点を算出.
		//ﾛｰｶﾙ交点pは、 v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * ( vVertex[1] - vVertex[0] )
			+ V * ( vVertex[2] - vVertex[0] );

		return true;
	}
	return false;
}



//============================================================
//	交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
//		※頂点座標はﾛｰｶﾙ座標.
//============================================================
HRESULT clsCollisionRay::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices )
{
	//頂点毎のバﾊﾞｲﾄ数を取得.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAmt = pTarget->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをﾛｯｸ(読込ﾓｰﾄﾞ).
	pTarget->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly );
	BYTE*	pbVertices = NULL;	//頂点(ﾊﾞｲﾄ型)
	FLOAT*	pfVertices = NULL;	//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;	//頂点ﾊﾞｯﾌｧ.
	pTarget->GetVertexBuffer( &VB );	//頂点情報の取得.

	//頂点ﾊﾞｯﾌｧのﾛｯｸ.
	if( SUCCEEDED(
		VB->Lock( 0, 0, (VOID**)&pbVertices, 0 ) ) )
	{
		////ﾎﾟﾘｺﾞﾝの頂点の1つ目を取得.
		//pfVertices
		//	= (FLOAT*)&pbVertices[ dwStride * pwPoly[ dwPolyIndex * 3 ] ];
		//pVecVertices[0].x = pfVertices[0];
		//pVecVertices[0].y = pfVertices[1];
		//pVecVertices[0].z = pfVertices[2];
		////ﾎﾟﾘｺﾞﾝの頂点の2つ目を取得.
		//pfVertices
		//	= (FLOAT*)&pbVertices[ dwStride * pwPoly[ dwPolyIndex * 3 + 1 ] ];
		//pVecVertices[1].x = pfVertices[0];
		//pVecVertices[1].y = pfVertices[1];
		//pVecVertices[1].z = pfVertices[2];
		////ﾎﾟﾘｺﾞﾝの頂点の3つ目を取得.
		//pfVertices
		//	= (FLOAT*)&pbVertices[ dwStride * pwPoly[ dwPolyIndex * 3 + 2 ] ];
		//pVecVertices[2].x = pfVertices[0];
		//pVecVertices[2].y = pfVertices[1];
		//pVecVertices[2].z = pfVertices[2];

		////ﾎﾟﾘｺﾞﾝの頂点の1～3つ目を取得.
		for( int i=0; i<3; i++ ){
			pfVertices
				= (FLOAT*)&pbVertices[ dwStride * pwPoly[ dwPolyIndex * 3 + i ] ];
			pVecVertices[i].y = pfVertices[1];
			pVecVertices[i].z = pfVertices[2];
			pVecVertices[i].x = pfVertices[0];
		}

		//ﾛｯｸ解除.
		pTarget->UnlockIndexBuffer();
		VB->Unlock();
	}
	VB->Release();

	return S_OK;
}


//============================================================
//	回転値調整.
//============================================================
void clsCollisionRay::DirOverGuard( float* fYaw )
{
	if( *fYaw > (float)( D3DX_PI * 2.0 ) ){
		*fYaw -= (float)( D3DX_PI * 2.0 );
	}
	else{
		return;
	}

	//再帰.
	if( *fYaw > (float)( D3DX_PI * 2.0 ) ){
		DirOverGuard( fYaw );
	}
}


//============================================================
//	床のあたり判定関連.
//============================================================
bool clsCollisionRay::FloorJudge( 
	clsCharaSkin* pWalker, 
	float fRange,
	float& fLandY,
	clsCharaStatic* pFloor,
	bool& bGround )//接地フラグ.
{
	bGround = false;

	//周囲4つ.
	D3DXVECTOR3 vOffsetWidht[4];
	vOffsetWidht[0] = { 0.0f, 0.0f, fRange };
	vOffsetWidht[1] = { 0.0f, 0.0f,-fRange };
	vOffsetWidht[2] = { fRange, 0.0f, 0.0f };
	vOffsetWidht[3] = {-fRange, 0.0f, 0.0f };

	for( int i=0; i<4; i++ ){
		//地面とのﾚｲ.
		FLOAT		fDistance;	//距離.
		D3DXVECTOR3 vIntersect;	//交差座標.
		const float fOffsetFoot = 0.03125f;
		//現在の位置をｺﾋﾟｰ.
		pWalker->m_vRay = pWalker->GetPosition();
		pWalker->m_vRay += vOffsetWidht[i];
		//ﾚｲの高さを自機より上にする.
		pWalker->m_vRay.y += fOffsetFoot;
		//軸ﾍﾞｸﾄﾙは垂直で下向き.
		pWalker->m_vAxis = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );

		bool bHit = false;
		bHit = Intersect(
			pWalker, pFloor, &fDistance, &vIntersect );

		if( bHit ){
	//		SetWindowTextA( m_hWnd, "下に地面があります" );
			//着地するべき高さ、影の描画高さ.
			fLandY = vIntersect.y;

			//地面に触れたら.
			if( pWalker->GetPositionY() - vIntersect.y <= /*-0.03125f*/0.0f ){
				//交点の座標からy座標を自機のy座標としてｾｯﾄ.
				vIntersect.y += 0.0f;
				pWalker->SetPositionY( vIntersect.y );
				//落下フラグセット.
				pWalker->SetDropoutFlg( false );

				bGround = true;//接地フラグ.
			}
			return true;
		}
		else{
	//		SetWindowTextA( m_hWnd, "nasi" );
			fLandY = -100;//奈落.
			//落下フラグセット.
			pWalker->SetDropoutFlg( true );
		}
	}

	return false;
}


//天井との判定.
bool clsCollisionRay::CeilingJudge( 
	clsCharaSkin* pWalker, 
	float fWalkerHeight,
	clsCharaStatic* pFloor )
{
	//地面とのﾚｲ.
	FLOAT		fDistance;	//距離.
	D3DXVECTOR3 vIntersect;	//交差座標.
	//現在の位置をｺﾋﾟｰ.
	pWalker->m_vRay = pWalker->GetPosition();
	//軸ﾍﾞｸﾄﾙは垂直で上向き.
	pWalker->m_vAxis = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	bool bHit = false;
	bHit = Intersect(
		pWalker, pFloor, &fDistance, &vIntersect );

	if( bHit ){
		//頭が天井に触れたら.
		if( vIntersect.y - ( pWalker->GetPositionY() + fWalkerHeight ) <= 0.0f ){
			//交点の座標からy座標を自機のy座標としてｾｯﾄ.
			vIntersect.y += 0.0f;
			pWalker->SetPositionY( vIntersect.y - fWalkerHeight );
			return true;
		}
	}

	return false;
}
