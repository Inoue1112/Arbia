#define _CRT_SECURE_NO_WARNINGS
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )
//============================================================
//	ｲﾝｸﾙｰﾄﾞ.
//============================================================
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>

#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "Global.h"
#include "MyMacro.h"

//#include "DX9Mesh.h"	//Dx9を使用するｸﾗｽ.
//#include "D3DXSKINMESH.h"//ｽｷﾝﾒｯｼｭｸﾗｽ.
//#include "Sprite.h"		//板ﾎﾟﾘｺﾞﾝ(ｽﾌﾟﾗｲﾄ)ｸﾗｽ.
//#include "Sprite2D.h"	//ｽﾌﾟﾗｲﾄ2Dｸﾗｽ.
#include "DebugText.h"
#include "Particle.h"
#include "Ray.h"		//ﾚｲ表示ｸﾗｽ.

#include "Sound.h"		//ｻｳﾝﾄﾞｸﾗｽ.

#include "Effects.h"//Effekseerを管理するｸﾗｽ.

#include "Player.h"

#include "UiManager.h"

#include "Collision.h"

#include "Resource.h"

#include "File.h"

#include "Wall.h"
#include "Floor.h"
#include "StepCylinder.h"
#include "StepBox.h"
#include "DoorManager.h"
#include "Pendulum.h"
#include "SpiaFloorManager.h"
#include "SpiaWallManager.h"
#include "CoverManager.h"
#include "EnemyManager.h"
#include "GoalManager.h"

#include "GageSprite2D.h"

#include "SceneManager.h"

//============================================================
//	ﾗｲﾌﾞﾗﾘ.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.

//============================================================
//	定数.
//============================================================
#define WND_TITLE	"怪盗アルビア"
#define APR_NAME	"Arbia"

//============================================================
//	構造体.
//============================================================
//ｶﾒﾗ情報.
struct CAMERA
{
	D3DXVECTOR3 vEye;	//ｶﾒﾗ位置.
	D3DXVECTOR3 vLook;	//注視位置.
	float		fYaw;	//回転値.
	D3DXMATRIX	mRot;	//回転行列.
};

struct StagePatarunSet
{
	//基本壁あり.
	int  iWall;		// 0:壁無し1:通常壁.

	//↓二つは同時に出ない.
	int  iFloor;		// 0:床無し1:通常床 2:半分床右側.
	// 3:半分床左側 4:ペンデュラム床.

	//絶対に床あり.
	int   iEnemy;		// 0:何もないよ 1～:敵の設置.

	//絶対に床無し.
	int	 iStepCil;	// 縦移動の床 0:何もないよ 1以上:動く床を出すのとｘ座標をかねている.
	int	 iStepBox;   // 横移動の床 0:何もないよ 1以上:動く床を出すのとｘ座標をかねている.

	//下三つは床ありを前提にする.
	bool bDoor;		// 0:何もないよ 1:扉設置.
	//絶対に壁を変えるように.
	int iPend;		// 0:何もないよ 1:ペンデュラム設置.
	bool bSpiaFloor;	// 0:何もないよ 1:槍床設置.
	//床無しでもおｋ.
	int iSpiaWall;	// 0:何もないよ 1:槍壁設置.

	//絶対に床無し.
	bool bCover;		// 0:何もないよ 1:落とし穴蓋の設置.

	//絶対に床無し.
	bool bGoal;		// 0:何もないよ 1:ゴールの設置.

	//テストのモデル.
	bool bTest;
};

//============================================================
//	ﾒｲﾝｸﾗｽ.
//============================================================
class clsMain
{
public:
	clsMain();	//ｺﾝｽﾄﾗｸﾀ.
	~clsMain();	//ﾃﾞｽﾄﾗｸﾀ.

	//ｳｨﾝﾄﾞｳ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	void Loop();
	//ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
	void AppMain();
	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
	void Render();

	//DirectX初期化.
	HRESULT InitD3D();
	//Direct3D終了処理.
	void DestroyD3D();

	//ﾒｯｼｭ読み込み関数(まとめた).
	HRESULT ReadMesh();

	//ｽﾌｨｱ作成.
	HRESULT InitSphere(clsDX9Mesh* pMesh, float fScale = 0.7f);
	//ｽﾌｨｱ衝突判定関数.
	bool Collision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);

	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ作成.
	HRESULT InitBBox(clsDX9Mesh* pMesh);
	//ﾎﾞｯｸｽ衝突判定関数.
	bool BBoxCollision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);

	//ﾚｲとﾒｯｼｭのあたり判定.
	bool Intersect(
		clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);
	//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);
	//壁のあたり判定関連.
	void WallJudge(clsDX9Mesh* pAttacker, clsDX9Mesh* pWall);
	//回転値調整.
	void DirOverGuard(float* fYaw);

	//深度ﾃｽﾄ(Zﾃｽﾄ)　ON/OFF切替.
	void SetDepth(bool bFlg);

	//ﾎﾞｰﾝの座標をとる(ﾗｯﾌﾟ関数).
	void GetPosFromBone(clsD3DXSKINMESH* skinMesh, char BoneName[], D3DXVECTOR3& Pos);

	//ｶﾒﾗ関数.
	void Camera();
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
	void Proj();

	//ｼﾞｮｲｽﾃｨｯｸ.
	clsXInput* m_pXInput;

	//あたり判定.
	clsCollision*	m_pCollision;

#ifdef Inoue

#ifdef START_LOADING_DISPLAY
	/*/ ﾛｰﾄﾞ画面処理 /*/
	void NowLoading();
	/*/ ﾛｰﾄﾞ画面初期化 /*/
	void LoadSpriteInit();
	/*/ ﾛｰﾄﾞ画面表示 /*/
	void LoadRender();

	/*=*=*=*=*=*=*=*=| ｽﾌﾟﾗｲﾄ2D |=*=*=*=*=*=*=*=*/
	clsSprite2D* m_pLoadTxt;

	unique_ptr<clsSprite2D> m_pLoadBack;

	clsGageSprite2D* m_pLoadGage;
	GAGE_SPRITE_STATE ss_LoadGage;
	unique_ptr<clsSprite2D> m_pLoadGageBack;
	/*=*=*=*=*=*=*=*=| ｽﾌﾟﾗｲﾄ2D |=*=*=*=*=*=*=*=*/
	int LoadStep;
	int LoadGage;
	float LoadGageUVTarget;
#endif//#ifdef START_LOADING_DISPLAY

	unique_ptr<clsFile>	m_pTestData;

	unique_ptr<clsFile>	m_smpStageData;
	unique_ptr<clsFile>	m_smpStageDataExcel;
	unique_ptr<clsFile>	m_smpStageDataReadMe;

	unique_ptr<clsUiManagar>	m_smpUiManagar;

	int			m_iStageDataMax;
	int			m_iStageDataMaxMax;

	int			ControlMode;

	vector<unique_ptr<clsWall>>			m_vsmpWall;		//(壁)表示.
	vector<unique_ptr<clsFloor>>		m_vsmpFloor;		//(床)表示.
	vector<unique_ptr<clsStepCyl>>		m_vsmpStepCil;	//(縦床移動)表示.
	vector<unique_ptr<clsStepBox>>		m_vsmpStepBox;	//(横床移動)表示.
	vector<unique_ptr<clsEnemyMgr>>		m_vsmpEnemyMgr;	//(敵)表示.
	vector<unique_ptr<clsDoorMgr>>		m_vsmpDoorMgr;	//(ドア)表示.
	vector<unique_ptr<clsPend>>			m_vsmpPend;		//(ペンデュラム)表示.
	vector<unique_ptr<clsSpiaFlorMgr>>	m_vsmpSpiaFloorMgr;	//(槍床)表示.
	vector<unique_ptr<clsSpiaWallMgr>>	m_vsmpSpiaWallMgr;	//(槍壁)表示.
	vector<unique_ptr<clsCoverMgr>>		m_vsmpCoverMgr;	//(落とし穴蓋)表示.
	vector<unique_ptr<clsGoalMgr>>		m_vsmpGoalMgr;	//(ゴール)表示.

	vector<unique_ptr<clsCharaStatic>>	m_vsmpCeiling;	//(屋根)表示.

	vector<unique_ptr<clsCharaStatic>>	m_vsmpTest;		//(モデルテスト)表示.

	vector<StagePatarunSet> m_vStageDataPatarnSet;	//配列1個でステージ１マス分
	vector<unique_ptr<clsSprite2D>>  m_vsmpSpriteTest;
	int	m_SpriteControlNum;

	inline void StageSet();
	inline void StageModelSet();
	inline void StageModelSetErrMsg(int StageNum, const char* ERR_MSG);
	inline void StageDetatchModelSet();

	void SceneTitle();
	void SceneMain();
	void SceneResult();
	void SceneOver();
	void SceneEnding();

	void RenderTitle();
	void RenderMain();
	void RenderResult();
	void RenderOver();
	void RenderEnding();

	void RenderDebugText();

	int	TestDataCnt;

	int TestDataNum;

	int	 iWallModelNumZero;
	int	 iFloorModelNumZero;

	enScene m_enScene;

#endif //Inoue

	//タイトルの状態.
	enum enTitleSceneMode
	{
		en_TSM_IDLE = 0,	//入力受付.
		en_TSM_BREAK_DOOR,	//ドア破壊しつつカメラ移動.
		en_TSM_GO_MAIN,

		en_TSM_MAX,
	};
	enTitleSceneMode m_enTitleSceneMode;

	//リザルトの状態.
	enum enResultSceneMode
	{
		enRSM_SILENCE = 0,
		enRSM_JINGLE,
		enRSM_LOOP,

		enRSM_MAX
	};
	enResultSceneMode m_enResultSceneMode;
	//リザルトで数える.
	int m_iResultTimer;

	//シーンの初期化.
	void InitTitle();
	//メインシーン(ステージ)の初期化.
	void InitMain(bool bFirstFlg = false);//bFirstFlg = 一回目(Mainシーン中で使う場合はfalse, タイトルから来た場合はtrue).
	void InitResult();
	void InitEndroll();
	void InitOver();

	//ｶﾒﾗ.
	void CameraMgr();
	bool Stoker(D3DXVECTOR3& Pos, D3DXVECTOR3 TargetPos);
	void CameraTargetSet();
	void CameraLookSet();
	float CameraLengthComp(float Attker, float Target);
	float m_fCameraLookFloor;	//見る高さの保存用.
	float m_fCameraLookFloorOld;//足場が真下にないときに使う時.

	//一瞬で振り向かない(徐々に振り向く).
	void YawSpnToTarg(
		float& NowYaw, float TarYaw,
		float TurnSpd, float TurnStop = 0.1f);

	////360～0に納める.
	void ThetaOverGuard_M(float& theta);
	void ThetaOverGuard_M(double& theta);

private:
	HWND	m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//↓ｱﾌﾟﾘに一つ.
	ID3D11Device*			m_pDevice;			//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	IDXGISwapChain*			m_pSwapChain;		//ｽﾜｯﾌﾟﾁｪｰﾝ.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//ﾊﾞｯｸﾊﾞｯﾌｧ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ.

	D3DXVECTOR3		m_vLight;	//ﾗｲﾄの方向.

	D3DXMATRIX		m_mView;	//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	D3DXMATRIX		m_mProj;	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.

	CAMERA			m_Camera;//ｶﾒﾗ情報.

	D3DXVECTOR3		m_vCameraTarget;//ｶﾒﾗがいるべき位置.
	D3DXVECTOR3		m_vLookTarget;	//注視位置があるべき位置.

	float 			m_fLookOffset;

#ifdef Inoue
	//PngTest用変数.
	float m_fAlphaPoint;
#endif //Inoue

	//板ﾎﾟﾘｺﾞﾝ用.
	clsSprite*		m_pExplosion;//爆発用.

	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄｸﾗｽ.
	clsDebugText*	m_pText;

	//ﾊﾟｰﾃｨｸﾙ用.
	clsParticle*	m_pParticle;//ﾊﾟｰﾃｨｸﾙ.

	//深度(Z)ﾃｽﾄ設定.
	ID3D11DepthStencilState* m_pDepthStencilState;

	//ﾚｲ表示ｸﾗｽ.
	clsRay*			m_pRayV;	//垂直.
	clsRay*			m_pRayFB;	//前後.
	clsRay*			m_pRayH;	//左右.

	//ﾌﾟﾚｲﾔｰ.
	clsPlayer*			m_pPlayer;
	int					m_iRespawnTimer;

	//ゲームオーバー用敵.
	//	clsEnemy*			m_pOverEnemy[4];

	//定規.
	int					m_iBlkAryMax;
	clsCharaStatic**	m_ppBlkAry;

	//ﾘｿｰｽｸﾗｽ.
	clsResource*		m_pResource;

	//ﾌｧｲﾙ読込.
	clsFile*	m_pFile;

	//ｻｳﾝﾄﾞｸﾗｽ.
	enum enBgm
	{
		enBGM_START_UP = 0,
		enBGM_TITLE,
		enBGM_MAIN,
		enBGM_RESULT,
		enBGM_ENDROLL,
		enBGM_OVER,

		enBGM_MAX,
	};
	clsSound*	m_pBgm[enBGM_MAX];
	enum enSe
	{
		enSe_ENTER,
		enSe_EXIT,

		enSe_MAX,
	};
	clsSound*	m_pSe[enSe_MAX];
	void CreateSound();

	//効果音の音量を決めるセット.
	float m_fEar;	//耳(音量基準)の位置.
	void SetEar(float player, float cam){
		const float fOffset = 2.0f;
		m_fEar = (player + cam) / 2.0f + fOffset;
	}
	float GetEar(){
		return m_fEar;
	}

	//for文の処理の無駄を減らす.
	//あたり判定計算する?.//Used.
	bool isCutOutForHit(float fPlayerZ, float fObjZ, float fBack = -1.0f, float fOpposite = -1.0f);
	//動き計算する?.//まだ使っていない.
	bool isCutOutForMove(float fPlayerZ, float fObjZ, float fBack = -1.0f, float fOpposite = -1.0f);

#ifdef Tahara
	//ConvDimPosの事前準備.
	void SetViewPort10(D3D11_VIEWPORT* Vp){
		m_ViewPort.TopLeftX = Vp->TopLeftX;
		m_ViewPort.TopLeftY = Vp->TopLeftY;
		m_ViewPort.MaxDepth = Vp->MaxDepth;
		m_ViewPort.MinDepth = Vp->MinDepth;
		m_ViewPort.Width = Vp->Width;
		m_ViewPort.Height = Vp->Height;
	};

	//3D座標のスクリーン( 2D )座標変換.dimensions(次元) conversion(変換)
	D3DXVECTOR3 ConvDimPos(D3DXVECTOR3 v2DPos, D3DXVECTOR3 v3DPos){
		D3DXMATRIX mWorld;
		D3DXMatrixIdentity(&mWorld);
		D3DXVec3Project(&v2DPos, &v3DPos, &m_ViewPort, &m_mProj, &m_mView, &mWorld);
		return v2DPos;
	}

	D3D10_VIEWPORT m_ViewPort;//2DSp用.

	//リザルトの宝石用.
	UI_STATE_RESULT m_ResUiState;//リザルトに必要な情報.
	//ラップ.
	void SetJewelState(){
		m_smpUiManagar->SetJewelState(m_ResUiState);
	}
	//リザルトの宝石用情報を渡せるように加工する.
	void UpDateJewelState(){
		m_ResUiState.iLives = m_pPlayer->GetHp();
	}

#endif//#ifdef Tahara
};