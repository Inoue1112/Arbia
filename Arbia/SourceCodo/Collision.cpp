#define _USE_MATH_DEFINES
#include <math.h>

#include "Collision.h"
#include "CollisionRay.h"


//----- âπ -----//.
//----- ÉAÉãÉrÉA.
//ÉLÉbÉN.
#define ALIAS_NAME_ARBIA_KICK "ColArbiaKick"
#define  FILE_PATH_ARBIA_KICK "SE\\500Collider\\100ArbiaKickHit.wav"
const	  int iVOL_ARBIA_KICK = 1000;

//ÉWÉÉÉìÉvçUåÇ.
#define ALIAS_NAME_ARBIA_JATK "ColArbiaJumpAtk"
#define  FILE_PATH_ARBIA_JATK "SE\\500Collider\\110ArbiaJumpAtkHit.wav"
const	  int iVOL_ARBIA_JATK = 1000;

//è’ìÀ.
#define ALIAS_NAME_ENEMY_HIT "ColEnemyHit"
#define  FILE_PATH_ENEMY_HIT "SE\\500Collider\\200EnemyHit3.wav"
const	  int iVOL_ENEMY_HIT = 1000;

//åï.
#define ALIAS_NAME_ENEMY_SRASH "ColEnemySrash"
#define  FILE_PATH_ENEMY_SRASH "SE\\500Collider\\210EnemySrashHit.wav"
const	  int iVOL_ENEMY_SRASH = 1000;

//ëÑ.
#define ALIAS_NAME_SPIA "ColSpia"
#define  FILE_PATH_SPIA "SE\\500Collider\\300SpiaHit2.wav"
const	  int iVOL_SPIA = 1000;

//ÉyÉìÉfÉÖÉâÉÄ.
#define ALIAS_NAME_PEND "ColPendulum"
#define  FILE_PATH_PEND "SE\\500Collider\\400PendHit.wav"
const	  int iVOL_PEND = 1000;

//óéâ∫.
#define ALIAS_NAME_DROP_DOWN "ColDropDown"
#define  FILE_PATH_DROP_DOWN "SE\\500Collider\\900DropDown.wav"
const	  int iVOL_DROP_DOWN = 250;//250.







//”√ﬁŸÇÃìsçá.
const float fMODEL_BACK_TURN = (float)M_PI;//TestPlusTheta




clsCollision::clsCollision()
{
	m_pRay = new clsCollisionRay;
}

clsCollision::~clsCollision()
{
	Release();
}

void clsCollision::Release()
{
	if( m_ppSe != NULL ){
		for( int i=0; i<enSOUND_MAX; i++ ){
			delete m_ppSe[i];
			m_ppSe[i] = NULL;
		}
		delete[] m_ppSe;
		m_ppSe = NULL;
	}


	if( m_pRay != NULL ){
		delete m_pRay;
		m_pRay = NULL;
	}
}




//BOXÇÃÇ†ÇΩÇËîªíË.
bool clsCollision::HitBox( COL_STATE* Attker, COL_STATE* Target )
{
	if( Attker->vPos.z + Attker->vRangeHalf.z > Target->vPos.z - Target->vRangeHalf.z &&
		Attker->vPos.z - Attker->vRangeHalf.z < Target->vPos.z + Target->vRangeHalf.z )
	{
		float attkerPosY = Attker->vPos.y + Attker->fCenterY;
		float TargetPosY = Target->vPos.y + Target->fCenterY;
		if(	attkerPosY + Attker->vRangeHalf.y > TargetPosY - Target->vRangeHalf.y &&
			attkerPosY - Attker->vRangeHalf.y < TargetPosY + Target->vRangeHalf.y )
		{
			float attkerPosX = Attker->vPos.x + Attker->fCenterX;
			float TargetPosX = Target->vPos.x + Target->fCenterX;
			if( attkerPosX + Attker->vRangeHalf.x > TargetPosX - Target->vRangeHalf.x &&
				attkerPosX - Attker->vRangeHalf.x < TargetPosX + Target->vRangeHalf.x )
			{
				return true;
			}
		}
	}

	return false;
}



//============================================================
//	ëŒè€Ç∆ÇÃãóó£ÇéZèoÇ∑ÇÈ.
//============================================================
float clsCollision::LengthComp( D3DXVECTOR3 Attker, D3DXVECTOR3 Target )
{
	float vX, vZ, LengthA_T;

	vX = Target.x - Attker.x;
	vZ = Target.z - Attker.z;

	vX *= vX;
	vZ *= vZ;

	LengthA_T = sqrtf( vX + vZ );

	return LengthA_T;
}

////============================================================
//	â~íå.
////============================================================
bool clsCollision::HitCylind( COL_STATE* Attacker, COL_STATE* Target )
{
	//çÇÇ≥Ç™ìØÇ∂ëOíÒ.
	if( Attacker->vPos.y					 < Target->vPos.y + Target->fHeight &&
		Attacker->vPos.y + Attacker->fHeight > Target->vPos.y )
	{
		//â~.
		if( LengthComp( Attacker->vPos, Target->vPos ) <= Attacker->fRange + Target->fRange ){
			return true;
		}
	}
	return false;
}

//============================================================
//	ñ⁄ïWÇ∆ÇÃäpìx.
//============================================================
float clsCollision::SetTarYaw( D3DXVECTOR3 Attker, D3DXVECTOR3 Target)
{
	float vX, vZ;

	vX = Target.x - Attker.x;
	vZ = Target.z - Attker.z;


	double thetaTar = atan2( (double)-vZ, (double)vX ) + M_PI_2 - fMODEL_BACK_TURN;	//ëäéËÇ∆ÇÃäpìx.
	ThetaOverGuard( thetaTar );
	return (float)thetaTar;
}

////============================================================
//	äpìxîªíË.
////============================================================
bool clsCollision::ThetaCheck( double dMyTheta, double dTargTheta,
								int iSarchTheta )
{
	double d_thetaArea_h = (double)iSarchTheta / 180.0 * M_PI / 2.0;
	double thetaSearchR, thetaSearchL;
	thetaSearchR = dMyTheta - d_thetaArea_h;
	thetaSearchL = dMyTheta + d_thetaArea_h;

	//äpìxÇ™çáÇ¡ÇƒÇ¢ÇΩÇÁ.
	if( thetaSearchL > dTargTheta &&
		thetaSearchR < dTargTheta )
	{
		return true;
	}
	////àÍé¸ëŒçÙ(0Ç∆360ÇÃã´ñ⁄).
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

////============================================================
//	êÓå`¡™Ø∏.
////============================================================
bool clsCollision::HitSect( COL_STATE* Attacker, COL_STATE* Target )
{
	float vX, vZ, R;

	vX = Target->vPos.x - Attacker->vPos.x;
	vZ = Target->vPos.z - Attacker->vPos.z;


	R = Attacker->fRange;


	//â~íå.
	if( HitCylind( Attacker, Target ) )
	{
		//äpìxÇÃäÑÇËèoÇµ.
		double thetaAtt = (double)Attacker->fYaw - fMODEL_BACK_TURN;	//çUåÇé“ÇÃäpìx.
		ThetaOverGuard( thetaAtt );

		double thetaTar = atan2( (double)( -vZ ), (double)vX ) + M_PI_2;	//ëäéËÇ∆ÇÃäpìx.
		ThetaOverGuard( thetaTar );

		//äpìxîªíË.
		if( ThetaCheck( thetaAtt, thetaTar, Attacker->iSarchTheta ) ){
			return true;
		}//äpìxîªíËèIÇÌÇË.
	}//â~îªíËèIÇÌÇË.

	return false;
}




//============================================================
//	360Å`0Ç…î[ÇﬂÇÈ.
//============================================================
void clsCollision::ThetaOverGuard( float& theta )
{
#define THETA_MAX (float)( M_PI * 2.0 )
	if( theta > THETA_MAX ){
		theta -= THETA_MAX;
	}
	else if( theta < 0.0f ){
		theta += THETA_MAX;
	}
}
void clsCollision::ThetaOverGuard( double& theta )
{
#define THETA_MAX ( M_PI * 2.0 )
	if( theta > THETA_MAX ){
		theta -= THETA_MAX;
	}
	else if( theta < 0.0 ){
		theta += THETA_MAX;
	}
}


//============================================================
//	ï«ÇÃÇ†ÇΩÇËîªíË◊ØÃﬂ.
//============================================================
void clsCollision::WallJudge( clsCharaSkin* pWalker, clsCharaStatic* pWall )
{
	m_pRay->WallJudge( pWalker, pWall );
}



//============================================================
//	è∞ÇÃÇ†ÇΩÇËîªíË◊ØÃﬂ.
//============================================================
bool clsCollision::FloorJudge( 
	clsCharaSkin* pWalker, 
	float fRange,
	float& fLandY,
	clsCharaStatic* pFloor,
	bool& bFlg )
{
	bool bJudgeflg;
	bool bGround;
	bJudgeflg = m_pRay->FloorJudge( pWalker, fRange, fLandY, pFloor, bGround );
	bFlg = bGround;
	return bJudgeflg;
}

//============================================================
//	ìVà‰Ç∆ÇÃîªíË.
//============================================================
bool clsCollision::CeilingJudge( 
	clsCharaSkin* pWalker, 
	float fWalkerHeight,
	clsCharaStatic* pFloor )
{
	bool bReturn = m_pRay->CeilingJudge( pWalker, fWalkerHeight, pFloor );
	return bReturn;
}


//å¯â âπçƒê∂.
void clsCollision::PlaySe( enSound enSe )
{
	if( m_ppSe == NULL ){
		return;
	}
	int vol = 1000;
	m_ppSe[enSe]->GetMaxVolRate( vol );
	m_ppSe[enSe]->SeekToStart();
	m_ppSe[enSe]->SetVolume( vol );
	m_ppSe[enSe]->Play();
}


void clsCollision::CreateSe( HWND hWnd )
{
	clsSound::SOUND_DATA tmpSData[enSOUND_MAX] =
	{
		{ ALIAS_NAME_ARBIA_KICK,	FILE_PATH_ARBIA_KICK,	iVOL_ARBIA_KICK	},
		{ ALIAS_NAME_ARBIA_JATK,	FILE_PATH_ARBIA_JATK,	iVOL_ARBIA_JATK	},
		{ ALIAS_NAME_ENEMY_HIT,		FILE_PATH_ENEMY_HIT,	iVOL_ENEMY_HIT	},
		{ ALIAS_NAME_ENEMY_SRASH,	FILE_PATH_ENEMY_SRASH,	iVOL_ENEMY_SRASH},
		{ ALIAS_NAME_SPIA,			FILE_PATH_SPIA,			iVOL_SPIA		},
		{ ALIAS_NAME_PEND,			FILE_PATH_PEND,			iVOL_PEND		},
		{ ALIAS_NAME_DROP_DOWN,		FILE_PATH_DROP_DOWN,	iVOL_DROP_DOWN	},
	};
	m_ppSe = new clsSound*[enSOUND_MAX];
	for( int i=0; i<enSOUND_MAX; i++ ){
		m_ppSe[i] = new clsSound;
		//åªâπó èâä˙âª.
		m_ppSe[i]->SetVolume( 0 );
		//ñºëO.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s( cAliasName, sizeof( cAliasName ), tmpSData[i].sAlias );
//		//î‘çÜ.
//		char cNumber[] = "  ";
//		_itoa_s( iNo, cNumber, 10 );
//		//ñºëOÇ∆î‘çÜçáëÃ.
//		strcat_s( cAliasName, sizeof( cAliasName ), cNumber );
		//çÏê¨.
		m_ppSe[i]->Open( tmpSData[i].sPath, cAliasName, hWnd );
		//ç≈ëÂâπó ê›íË.
		m_ppSe[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
	}
}










