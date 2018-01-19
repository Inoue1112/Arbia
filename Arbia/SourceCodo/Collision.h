#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Global.h"

#include "Sound.h"


class clsCharaSkin;
class clsCharaStatic;

class clsCollisionRay;






//あたり判定用ｽﾃｰﾀｽ.
struct COL_STATE
{
	//共通.
	D3DXVECTOR3 vPos;

	//円関係用.
	float		fYaw;
	float		fRange;
	float		fHeight;
	int			iSarchTheta;

	//BOX用.
	D3DXVECTOR3 vRangeHalf;	//中心からの長さ.
	float		fCenterY;	//Yの中心への距離(主人公なら足元からはんぶんまで).
	float		fCenterX;	//Xの中心への距離(壁槍なら槍の穂先の根元から半分);

	COL_STATE(){
		ZeroMemory( this, sizeof( COL_STATE ) );
	}
};



class clsCollision
{
public:
	clsCollision();
	~clsCollision();

	void CreateSe( HWND hWnd );

	void Release();


	//BOXのあたり判定.
	bool HitBox( COL_STATE* Attker, COL_STATE* Target );


	//対象との距離を算出する(円としても使える).
	float LengthComp( D3DXVECTOR3 Attker, D3DXVECTOR3 Target );

	//円柱.
	bool HitCylind( COL_STATE* Attacker, COL_STATE* Target );

	//目標との角度.
	float SetTarYaw( D3DXVECTOR3 Attker, D3DXVECTOR3 Target);//OpponentDirect

	//角度判定.
	bool ThetaCheck( double dMyTheta, double dTargTheta, int iSarchTheta );

	//扇形ﾁｪｯｸ.
	bool HitSect( COL_STATE* Attacker, COL_STATE* Target );






	//壁のあたり判定関連.
	void WallJudge( clsCharaSkin* pWalker, clsCharaStatic* pWall );


	//床との判定.
	bool FloorJudge( 
		clsCharaSkin* pWalker, 
		float fRange,
		float& fLandY,//下に床があるとtrueが返ってくる.
		clsCharaStatic* pFloor,
		bool& bFlg );

	//天井との判定.
	bool CeilingJudge( 
		clsCharaSkin* pWalker, 
		float fWalkerHeight,
		clsCharaStatic* pFloor );



	//----- 接触効果音 -----///.
	//音の種類.
	enum enSound
	{
		enSOUND_ARBIA_KICK = 0,
		enSOUND_ARBIA_JATK,

		enSOUND_ENEMY,
		enSOUND_ENEMY_SRASH,

		enSOUND_SPIA,
		enSOUND_PEND,

		enSOUND_DROP_DOWN,

		enSOUND_MAX,	//最大数.
	};


	//音再生.
	void PlaySe( enSound enSe );



private:
	////360～0に納める.
	void ThetaOverGuard( float& theta );
	void ThetaOverGuard( double& theta );

	clsCollisionRay*	m_pRay;



	//効果音.
	clsSound**	m_ppSe;
};

#endif//#ifndef _COLLISION_H_