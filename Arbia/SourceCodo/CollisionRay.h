#ifndef _COLLISION_RAY_H_
#define _COLLISION_RAY_H_

#include "Global.h"


#include "CharaStatic.h"
#include "CharaSkin.h"




class clsCollisionRay
{
public:
	clsCollisionRay();
	~clsCollisionRay();


	//壁のあたり判定関連.
	void WallJudge( clsCharaSkin* pWalker, clsCharaStatic* pWall );

	//床との判定.
	bool FloorJudge( 
		clsCharaSkin* pWalker, 
		float fRange,	//半径.
		float& fLandY,	//下に床があるとtrueが返ってくる.
		clsCharaStatic* pFloor,
		bool& bGround );//接地フラグ.

	//天井との判定.
	bool CeilingJudge( 
		clsCharaSkin* pWalker, 
		float fWalkerHeight,
		clsCharaStatic* pFloor );

private:
	bool Intersect(
		clsCharaSkin* pAttacker, clsCharaStatic* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect );
	//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices );
	//回転値調整.
	void DirOverGuard( float* fYaw );	
};

#endif//#ifndef _COLLISION_RAY_H_.