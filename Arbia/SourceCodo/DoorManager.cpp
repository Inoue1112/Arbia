#include "Resource.h"
#include "DoorManager.h"

const float fMODEL_SCALE = 1.0f;
const double fANIM_SPD = 0.01;

//----- 音 -----//.
const char sALIAS_NAME[] = "DoorBreak";
const char sFILE_PATH[] = "SE\\300Trap\\900GateBreak.wav";
const int iVOL = 1000;

//----- 蹴られ判定用 -----//.
const float fCOL_RANGE = 1.5f;
const float fCOL_HEIGHT = 3.0f;

//----- 壁の調整 -----//.
const float fSTAGE_WIDTH = 10.0f;
const float fSTAGE_WIDTH_HARF = fSTAGE_WIDTH / 2.0f;
const float fWALL_HEIGHT = 6.0f;
const float fWALL_Z = -0.75f;

const D3DXVECTOR3 vWALL_OFFSET = { -fSTAGE_WIDTH_HARF, fWALL_HEIGHT, fWALL_Z };

//復活時どれだけ手前にいるかZ.
const float fRE_SPAWN_POS_Z = 1.5f;

//----- 透過 ------//
//透過境界線Z.
const float fALPHA_BORDER_Z = 0.5f;

//透過速度.
const float fALPHA_SPD = 1.0f / 16.0f;
//透過を戻す速度.
const float fALPHA_SPD_BACK = 1.0f / 32.0f;

//一番薄くしても.
const float fALPHA_LIMIT = 0.375f;
//一番濃くしても.
const float fALPHA_LIMIT_MAX = 2.0f;

//文字を作らないNoは?.
const int iNOT_CREATE_STRING = 0;

clsDoorMgr::clsDoorMgr()
{
	m_pCheck = nullptr;
}

clsDoorMgr::~clsDoorMgr()
{
	Release();
}

void clsDoorMgr::Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, int iNo)
{
	//門.
	m_pGate = new clsCharaStatic;
	m_pGate->AttachModel(
		clsResource::GetInstance()->GetStaticModels(
		clsResource::enST_MODEL_MON));

	//扉.
	m_pDoor = new clsCharaSkin;

	CD3DXSKINMESH_INIT si;//skin_Init.
	si.hWnd = hWnd;
	si.pDevice = pDevice;
	si.pDeviceContext = pContext;

	m_pDoor->m_pModel = new clsD3DXSKINMESH;
	m_pDoor->m_pModel->Init(&si);

	m_pDoor->AttachModel(
		clsResource::GetInstance()->GetSkinModels(
		clsResource::enSK_MODEL_DOOR));

	m_pDoor->SetScale(fMODEL_SCALE);
	m_pDoor->SetAnimSpeed(fANIM_SPD);

	//壁判定.
	m_pColWall = new clsCharaStatic;
	m_pColWall->AttachModel(
		clsResource::GetInstance()->GetStaticModels(
		clsResource::enST_MODEL_SPIA_WALL));

	//文字( 最初の奴は作らない{ MOVIE用 } ).
	if (iNo != iNOT_CREATE_STRING){
		m_pCheck = new clsCheckPointMgr;
		m_pCheck->Create(hWnd, pDevice, pContext, iNo);
	}

	//蹴られ判定用.
	ColState.fRange = fCOL_RANGE;
	ColState.fHeight = fCOL_HEIGHT;

	//効果音.
	m_pSe = new clsSound;
	m_pSe->SetVolume(0);
	//名前.
	char cAliasName[STR_BUFF_MAX] = "";
	strcat_s(cAliasName, sizeof(cAliasName), sALIAS_NAME);
	//番号.
	char cNumber[] = "  ";
	_itoa_s(iNo, cNumber, 10);
	//名前と番号合体.
	strcat_s(cAliasName, sizeof(cAliasName), cNumber);
	//作成.
	m_pSe->Open(sFILE_PATH, cAliasName, hWnd);
	//最大音量設定.
	m_pSe->SetMaxVolume(iVOL);

	Init();
}
void clsDoorMgr::Init()
{
	ReStart();
	m_bBreak = false;
	SetColPos(m_vPos);
	m_dAnimTimer = 0.0;
	ChangeAnimMode(enANIM_IDLE);

	if (m_pCheck != nullptr){
		m_pCheck->Init();
	}
}
//復活時の初期化.
void clsDoorMgr::ReStart()
{
	m_fAlpha = fALPHA_LIMIT_MAX;
	m_bAlpha = false;
	m_bAlphaChange = false;
}

void clsDoorMgr::Release()
{
	if (m_pSe != NULL){
		delete m_pSe;
		m_pSe = NULL;
	}

	if (m_pCheck != nullptr){
		delete m_pCheck;
		m_pCheck = nullptr;
	}

	if (m_pColWall != NULL){
		delete m_pColWall;
		m_pColWall = NULL;
	}

	if (m_pDoor != NULL){
		delete m_pDoor;
		m_pDoor = NULL;
	}

	if (m_pGate != NULL){
		delete m_pGate;
		m_pGate = NULL;
	}
}

//描画.
void clsDoorMgr::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye)
{
	//.
	//	m_pColWall->Render( mView, mProj, vLight, vEye );
	//扉.
	m_pDoor->Render(mView, mProj, vLight, vEye);
	//門.
	m_pGate->Render(mView, mProj, vLight, vEye,
		D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_fAlpha), m_bAlpha);

	//文字.
	if (m_pCheck != nullptr){
		m_pCheck->Render(mView, mProj, vEye);
	}
}

void clsDoorMgr::SetPosition(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;

	//子分.
	m_pGate->SetPosition(m_vPos);
	m_pDoor->SetPosition(m_vPos);

	m_pColWall->SetPosition(m_vPos);
	m_pColWall->AddPosition(vWALL_OFFSET);

	Init();
}

void clsDoorMgr::Move(float fEarZ)
{
	m_fEarZ = fEarZ;
	Animation();
	SetAlpha();

	if (m_pCheck != nullptr){
		m_pCheck->Update();
	}
}

//============================================================
//	透過値の設定.
//============================================================
void clsDoorMgr::SetAlphaFlg(float fPlayerZ)
{
	//透過中.
	if (m_bAlphaChange){
		//透過フラグON.
		m_bAlpha = true;
		//プレイヤーが奥にいるなら.
		if (fPlayerZ > m_vPos.z + fALPHA_BORDER_Z){
			return;
		}
		//実体化開始.
		m_bAlphaChange = false;
	}
	//透過中ではない.
	else{
		//プレイヤーが手前にいるなら.
		if (fPlayerZ < m_vPos.z + fALPHA_BORDER_Z){
			return;
		}
		//透過開始.
		m_bAlphaChange = true;
	}
}

//============================================================
//	透過値の設定.
//============================================================
void clsDoorMgr::SetAlpha()
{
	//透過中.
	if (m_bAlphaChange){
		m_fAlpha -= fALPHA_SPD;
		if (m_fAlpha < fALPHA_LIMIT){
			m_fAlpha = fALPHA_LIMIT;
		}
	}
	//実体化中.
	else{
		m_fAlpha += fALPHA_SPD_BACK;
		if (m_fAlpha > fALPHA_LIMIT_MAX){
			m_fAlpha = fALPHA_LIMIT_MAX;
			m_bAlpha = false;
		}
	}
}

//============================================================
//	蹴られたとき.
//============================================================
D3DXVECTOR3 clsDoorMgr::DoorBreak()
{
	m_bBreak = true;

	PlaySe();

	D3DXVECTOR3 vReSpawnPos = m_vPos;
	vReSpawnPos.z -= fRE_SPAWN_POS_Z;

	ChangeAnimMode(enANIM_BREAK);

	if (m_pCheck != nullptr){
		m_pCheck->Start(vReSpawnPos.z);
	}

	return vReSpawnPos;
}

//==================================================
//	あたり判定用.
//==================================================
void clsDoorMgr::SetColPos(D3DXVECTOR3 vPos)
{
	ColState.vPos = vPos;
}

//==================================================
//	効果音再生.
//==================================================
void clsDoorMgr::PlaySe(/* enSound enSe*/)
{
	//再生する距離なら.
	int vol = ChangeVolumeDistance(m_fEarZ, m_vPos.z);
	vol = 1;
	if (vol){
		//		m_ppSe[enSe]->GetMaxVolRate( vol );
		//		m_ppSe[enSe]->SeekToStart();
		//		m_ppSe[enSe]->SetVolume( vol );
		//		m_ppSe[enSe]->Play();
		m_pSe->GetMaxVolRate(vol);
		m_pSe->SeekToStart();
		m_pSe->SetVolume(vol);
		m_pSe->SetVolume(iVOL);
		m_pSe->Play();
	}
	//再生しない距離なら.
	else{
		//		m_ppSe[enSe]->SetVolume( 0 );
		m_pSe->SetVolume(0);
	}
}

//============================================================
//	ドアのアニメ.
//============================================================
void clsDoorMgr::Animation()
{
	//ループしないアニメ.
	if (m_enAnimNo == enANIM_BREAK){
		m_dAnimTimer += m_pDoor->m_pModel->GetAnimSpeed();

		const double dRATE = 5.0;//何倍飛ばす?.

		//現在のアニメーションを終えたら.
		if (m_pDoor->m_pModel->GetAnimPeriod(m_enAnimNo) - (dANIM_ONE_FRAME_OVER_SOLUTION * dRATE) <= m_dAnimTimer){
			//動かなくなる.
			ChangeAnimMode(enANIM_DEAD);
		}
	}
}

//============================================================
//	アニメーションモードを変更.
//============================================================
void clsDoorMgr::ChangeAnimMode(enAnimation anim){
	m_enAnimNo = anim;
	m_pDoor->ChangeAnimSet(m_enAnimNo);//アニメセット.
	m_dAnimTimer = 0.0;
}