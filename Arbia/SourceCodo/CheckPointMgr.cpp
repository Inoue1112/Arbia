#include "CheckPointMgr.h"

//音.
const char ALIAS_NAME[] = "CheckPoint";
const char FILE_PATH[] = "SE\\600Result\\010JewelUp.wav";
const int VOL = 1000;

//文字数.
const char cSTRING_MAX = 8;

const LPSTR sFILE_PATH[cSTRING_MAX] =
{
	{ "Data\\Image\\CheckPointSet\\Chi.png" },
	{ "Data\\Image\\CheckPointSet\\Le.png" },
	{ "Data\\Image\\CheckPointSet\\Ltu.png" },
	{ "Data\\Image\\CheckPointSet\\Ku.png" },
	{ "Data\\Image\\CheckPointSet\\Po.png" },
	{ "Data\\Image\\CheckPointSet\\I.png" },
	{ "Data\\Image\\CheckPointSet\\NN.png" },
	{ "Data\\Image\\CheckPointSet\\To.png" },
};

const D3DXVECTOR3 vINIT_POS = { 0.0f, -100.0f, 0.0f };
const D3DXVECTOR3 vSTART_POS = { 10.0f, 4.0f, 0.0f };

//文字サイズ.
const float fSIZE = 0.5f;

//文字と文字の間隔.
const float fOFFSET = 0.5f;

//文字と文字の奥行間隔.
const float fOFFSET_Z = 0.25f * 0.25f * 0.25f * 0.25f;

//文字の移動速度.
const float fSTRING_SPD = -1.5f;

//文字が止まるX座標.
const float fSTOP_POS_X[cSTRING_MAX] =
{
	{ -(fOFFSET * 4 - (fOFFSET / 2.0f)) },
	{ -(fOFFSET * 3 - (fOFFSET / 2.0f)) },
	{ -(fOFFSET * 2 - (fOFFSET / 2.0f)) },
	{ -(fOFFSET * 1 - (fOFFSET / 2.0f)) },
	{ (fOFFSET * 1 - (fOFFSET / 2.0f)) },
	{ (fOFFSET * 2 - (fOFFSET / 2.0f)) },
	{ (fOFFSET * 3 - (fOFFSET / 2.0f)) },
	{ (fOFFSET * 4 - (fOFFSET / 2.0f)) },
};

clsCheckPointMgr::clsCheckPointMgr(){
	m_ppModel = nullptr;
	m_ppSe = nullptr;
}

clsCheckPointMgr::~clsCheckPointMgr()
{
	Release();
}
void clsCheckPointMgr::Release()
{
	if (m_ppSe != nullptr){
		for (char i = 0; i < cSTRING_MAX; i++){
			m_ppSe[i]->Stop();
			m_ppSe[i]->Close();
			delete m_ppSe[i];
			m_ppSe[i] = nullptr;
		}
		delete[] m_ppSe;
		m_ppSe = nullptr;
	}

	if (m_ppModel != nullptr){
		for (char i = 0; i < cSTRING_MAX; i++){
			delete m_ppModel[i];
			m_ppModel[i] = nullptr;
		}
		delete[] m_ppModel;
		m_ppModel = nullptr;
	}
}

void clsCheckPointMgr::Create(HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, int iNo)
{
	m_ppModel = new clsCheckPoint*[cSTRING_MAX];
	for (char i = 0; i < cSTRING_MAX; i++){
		m_ppModel[i] = new clsCheckPoint;
		m_ppModel[i]->Create(pDevice11, pContext11, sFILE_PATH[i]);
		m_ppModel[i]->SetScale(fSIZE);
	}

	//音クラス.
	m_ppSe = new clsSound*[cSTRING_MAX];
	for (char i = 0; i < cSTRING_MAX; i++){
		m_ppSe[i] = new clsSound;
		//名前.
		char cAliasName[STR_BUFF_MAX] = "";
		strcat_s(cAliasName, sizeof(cAliasName), ALIAS_NAME);
		//番号.
		char cNumberDoor[] = "  ";
		_itoa_s(iNo, cNumberDoor, 10);
		//名前と番号合体.
		strcat_s(cAliasName, sizeof(cAliasName), cNumberDoor);
		//番号.
		char cNumberString[] = "  ";
		_itoa_s(i, cNumberString, 10);
		//名前と番号合体.
		strcat_s(cAliasName, sizeof(cAliasName), cNumberString);
		//作成.
		m_ppSe[i]->Open(FILE_PATH, cAliasName, hWnd);
		//最大音量設定.
		m_ppSe[i]->SetMaxVolume(VOL);
		//現音量初期化.
		m_ppSe[i]->SetVolume(VOL);
	}

	Init();
}

void clsCheckPointMgr::Init()
{
	for (char i = 0; i < cSTRING_MAX; i++){
		m_ppModel[i]->Init();
		m_ppModel[i]->m_vPos = D3DXVECTOR3(vINIT_POS);
		m_ppModel[i]->m_enMode = clsCheckPoint::enM_AHEAD;
	}
}

void clsCheckPointMgr::Start(float fPosZ)
{
	if (m_ppModel[0]->m_enMode != clsCheckPoint::enM_AHEAD){
		return;
	}
	m_ppModel[0]->m_enMode = clsCheckPoint::enM_IN;

	for (char i = 0; i < cSTRING_MAX; i++){
		m_ppModel[i]->m_vPos = vSTART_POS;
		m_ppModel[i]->m_vPos.z = fPosZ;
		//重なり防止.
		m_ppModel[i]->m_vPos.z -= static_cast<float>(i)* fOFFSET_Z;
	}

	m_ppModel[0]->SetMove(true);

	m_ppSe[0]->SeekToStart();
	m_ppSe[0]->Play();
}

void clsCheckPointMgr::Update()
{
	//if( m_enMode == clsCheckPoint::enM_AHEAD || m_enMode == clsCheckPoint::enM_END ){
	//	return;
	//}
	for (char i = 0; i < cSTRING_MAX; i++){
		//動ける奴だけ.
		if (m_ppModel[i]->GetMove()){
			switch (m_ppModel[i]->m_enMode)
			{
				case clsCheckPoint::enM_IN:
					//動かす.
					m_ppModel[i]->m_vPos.x += fSTRING_SPD;
					//動き終わり.
					if (m_ppModel[i]->m_vPos.x <= fSTOP_POS_X[i]){
						m_ppModel[i]->m_enMode = clsCheckPoint::enM_STAY;
						m_ppModel[i]->m_vPos.x = fSTOP_POS_X[i];
						//次の始動.fNEXT_MOVE_START_POS_X
						//オーバーしない.
						if (i + 1 < cSTRING_MAX){
							//次の奴がまだ動いてないなら.
							if (!m_ppModel[i + 1]->GetMove()){
								//次のやつを動かす.
								m_ppModel[i + 1]->SetMove(true);
								m_ppModel[i + 1]->m_enMode = clsCheckPoint::enM_IN;
								m_ppSe[i + 1]->Stop();
								m_ppSe[i + 1]->SeekToStart();
								m_ppSe[i + 1]->Play();
							}
						}
					}
					break;

				case clsCheckPoint::enM_STAY:
					//動かす　終わり際に音鳴らす.
					if (m_ppModel[i]->Stay()){
						m_ppSe[i]->Stop();
						m_ppSe[i]->SeekToStart();
						m_ppSe[i]->Play();
					}
					break;

				case clsCheckPoint::enM_OUT:
					m_ppModel[i]->m_vPos.x += fSTRING_SPD;
					if (m_ppModel[i]->m_vPos.x <= -vSTART_POS.x){
						m_ppModel[i]->m_enMode = clsCheckPoint::enM_END;
						m_ppModel[i]->SetMove(false);
					}
					break;
			}
		}
	}
}

void clsCheckPointMgr::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3 &vEye)
{
	for (char i = 0; i < cSTRING_MAX; i++){
		m_ppModel[i]->Render(mView, mProj, vEye);
	}
}