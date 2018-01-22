#include "Main.h"
#include <stdio.h>

//====やることリスト==========================================
//PendulumのInit()作る.
//兵士のやられ(アルビアの攻撃).
//============================================================

//ｸﾞﾛｰﾊﾞﾙ変数.
clsMain* g_pClsMain = NULL;

//-----振動-----//.
//アルビアの蹴りヒット.
const unsigned short usVIB_KICK_POWER = INPUT_VIBRATION_MAX;
const int iVIB_KICK_TIME = 15;
//ドア破壊.
const unsigned short usVIB_DOOR_POWER = INPUT_VIBRATION_MAX;
const int iVIB_DOOR_TIME = 30;
//落とし穴ギシギシ.
const unsigned short usVIB_COVER_POWER = INPUT_VIBRATION_MAX / 10;
const int iVIB_COVER_TIME = 82;
const int iVIB_COVER_DEC = 128;

//BGM.
//起動.
#define ALIAS_NAME_BGM_START_UP "StartUp"
#define  FILE_PATH_BGM_START_UP "BGM\\000StartUp\\StartUp.mp3"
const int	  iVOL_BGM_START_UP = 250;

//タイトル.
#define ALIAS_NAME_BGM_TITLE "Title"
#define  FILE_PATH_BGM_TITLE "BGM\\100Title\\100Title.mp3"
const int	  iVOL_BGM_TITLE = 350;

//メイン.
#define ALIAS_NAME_BGM_MAIN "Main"
#define  FILE_PATH_BGM_MAIN "BGM\\200Main\\010Stage.mp3"
const int	  iVOL_BGM_MAIN = 250;

//リザルト.
#define ALIAS_NAME_BGM_RESULT "Result"
#define  FILE_PATH_BGM_RESULT "BGM\\300Result\\200ResultJingle.mp3"
const int	  iVOL_BGM_RESULT = 750;

//クリア.
#define ALIAS_NAME_BGM_CLEAR "Clear"
#define  FILE_PATH_BGM_CLEAR "BGM\\400Clear\\100EndRoll.mp3"
const int	  iVOL_BGM_CLEAR = 200;

//オーバー.
#define ALIAS_NAME_BGM_OVER "Over"
#define  FILE_PATH_BGM_OVER "BGM\\500Over\\100Bye.mp3"
const int	  iVOL_BGM_OVER = 300;

//SE.
//明るいやつ.
#define ALIAS_NAME_SE_ENTER "Enter"
#define  FILE_PATH_SE_ENTER "SE\\700Button\\100Enter.wav"
const int	  iVOL_SE_ENTER = 300;

//暗いやつ.
#define ALIAS_NAME_SE_EXIT "Exit"
#define  FILE_PATH_SE_EXIT "SE\\700Button\\200Exit.wav"
const int	  iVOL_SE_EXIT = 300;

//ライト方向.
const D3DXVECTOR3 vLIGHT_DIR = { 0.0f, 0.01f, 0.02f };

//カメラのより具合.
const float g_Zoom = 4.0f;

//描画限界距離.
const float fRENDER_LIMIT = 150.0f;

//死亡シーン時間.
const int iDEAD_TIME = 90;

//暗転速度.
const float fBLACK_SPD_SLOW = 0.0125f;
const float fBLACK_SPD_FAST = 0.05f;

const int PAT_SMOK_MAX = 500;

//----- 配置用 -----//
const float STAGE_WIDHT = 10.0f;
const float fWALL_HIT_RANGE = STAGE_WIDHT / 2.0f - 0.75f;
//落下死亡扱いする高さ.
const float fDROP_DOWN_Y = -6.0f;

//手前限界.
const float fPLAYER_BACK_LIMIT = 4.0f;
//スタート地点.
const D3DXVECTOR3 START_POSITION = { 0.0f, 0.0f, fPLAYER_BACK_LIMIT };
//カメラが手前によらないZ座標.
const float CAMERA_NOT_BACK = 12.0f;

//カメラの初期ずれ.
const D3DXVECTOR3 vCAMERA_INIT_OFFSET = { 0.0f, 4.9f, -7.0f };
//カメラの視点の位置.
const float fLOOK_OFFSET_Z = 5.0f;
const float fLOOK_OFFSET_Y = 3.0625f;//.
//----- 配置用終わり -----//

//----- あたり判定の判断距離 -----//
const float fONE_BROCK_HIT_AREA = 1.0f;//槍や床、壁.3.0f.
const float fGOAL_TREASURE_HIT_AREA = 32.0f;//ゴールの箱の奥距離.

//ステージ作成用定数.

const float WALL_SPACE = 0.8f;	//壁との限界距離.

const int	STAGEPATARUNSETMAX = 12;

const float PENDPOSY = 12.0f;//Pendulum基点の高さ.

const int	COVEREREA = 12;	//落とし穴の蓋.
const int	ENEMYEREA = 13;	//敵の行動範囲.
const int	ENEMYEREACENTER = 6;	//敵のCsvの設置場所.
const int	CILMOVEEREA = 15;	//丸床の移動範囲.
const int	CILMOVEEREACENTER = 4;	//丸床のCsvの設置場所.

//ステージ作成用定数終了.

#ifdef Inoue
int		g_iPngNum;
vector<string> g_vsFilePath;	//ここにD&Dしたファイルを入れる.
bool	g_bFileActive = false;	//ファイルを入れたかどうか.

bool	g_bStageRead;

int		g_ControlNum = 0;

#endif//Inoue.
//============================================================

//============================================================

//============================================================
//	ﾒｲﾝ関数.
//============================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,		//ｲﾝｽﾀﾝ番号(ｳｨﾝﾄﾞｳの番号).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	g_pClsMain = new clsMain;	//初期化&ｸﾗｽの宣言.

	//ｸﾗｽが存在しているかﾁｪｯｸ.
	if (g_pClsMain != NULL){
		//ｳｨﾝﾄﾞｳ作成成功.
		if (SUCCEEDED(
			g_pClsMain->InitWindow(
			hInstance,
			0, 0,
			WND_W, WND_H,
			WND_TITLE)))
		{
			//Dx11用の初期化
			if (SUCCEEDED(g_pClsMain->InitD3D())){
				//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
				g_pClsMain->Loop();
			}
		}
		//終了.
		g_pClsMain->DestroyD3D();//Direct3Dの解放.

		delete g_pClsMain;		//ｸﾗｽの破棄.
	}

	return 0;
}

//============================================================
//	ｳｨﾝﾄﾞｳﾌﾟﾛｼｰｼﾞｬ.
//============================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	//ﾌﾟﾛｼｰｼﾞｬ.
	return g_pClsMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

//============================================================
//	ﾒｲﾝｸﾗｽ.
//============================================================

//============================================================
//	ｺﾝｽﾄﾗｸﾀ.
//============================================================
clsMain::clsMain()
{
	ZeroMemory(this, sizeof(clsMain));

	m_hWnd = NULL;

	m_pDevice = NULL;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	m_pDeviceContext = NULL;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	m_pSwapChain = NULL;	//ｽﾜｯﾌﾟﾁｪｰﾝ.

	m_pBackBuffer_TexRTV = NULL;//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ.
	m_pBackBuffer_DSTex = NULL;//ﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pBackBuffer_DSTexDSV = NULL;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ.

	//	//ｶﾒﾗ(視点)位置.
	//	m_Camera.vEye	= D3DXVECTOR3( 0.0f, 2.0f, -10.0f );
	//	//注視位置.
	//	m_Camera.vLook	= D3DXVECTOR3( 0.0f, 2.0f, 10.0f );

	//ﾗｲﾄ方向.
	m_vLight = vLIGHT_DIR;

	//ステージ読込全速力.
	g_bStageRead = false;

	m_fAlphaPoint = 1.0f;
}

//============================================================
//	ﾃﾞｽﾄﾗｸﾀ.
//============================================================
clsMain::~clsMain()
{
}

//============================================================
//ｳｨﾝﾄﾞｳ初期化関数.
//============================================================
HRESULT clsMain::InitWindow(
	HINSTANCE hInstance,	//ｲﾝｽﾀﾝｽ.
	INT x, INT y,			//ｳｨﾝﾄﾞｳのx,y座標.
	INT width, INT height,	//ｳｨﾝﾄﾞｳの幅,高さ.
	LPSTR WindowName)		//ｳｨﾝﾄﾞｳ名.
{
	//ｳｨﾝﾄﾞｳの定義.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//初期化.

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APR_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//ｳｨﾝﾄﾞｳｸﾗｽをWindowsに登録.
	if (!RegisterClassEx(&wc)){
		MessageBox(NULL,
			"ｳｨﾝﾄﾞｳｸﾗｽの登録に失敗", "clsMain::InitWindow", MB_OK);
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの作成.
	m_hWnd = CreateWindow(
		APR_NAME,			//ｱﾌﾟﾘ名.
		WindowName,			//ｳｨﾝﾄﾞｳﾀｲﾄﾙ.
		WS_OVERLAPPEDWINDOW,//ｳｨﾝﾄﾞｳ種別.
		0, 0,				//表示座標.
		width, height,		//ｳｨﾝﾄﾞｳの幅,高さ.
		NULL,				//親ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
		NULL,				//ﾒﾆｭｰ設定.
		hInstance,			//ｲﾝｽﾀﾝｽ番号.
		NULL);				//ｳｨﾝﾄﾞｳ作成時に発生するｲﾍﾞﾝﾄに渡すﾃﾞｰﾀ.

	if (!m_hWnd){
		MessageBox(NULL,
			"ｳｨﾝﾄﾞｳ作成に失敗", "clsMain::InitWindow", MB_OK);
		return E_FAIL;
	}
#ifdef Inoue
#ifdef _DEBUG
	DragAcceptFiles(
		m_hWnd,    // 登録するウィンドウ
		true // アクセプトオプション
		);
#endif	//#ifdef _DEBUG

#endif//#ifdef Inoue

	//ｳｨﾝﾄﾞｳの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	//マウスカーソルの非表示.
#ifdef _DEBUG
#else
	ShowCursor(false);
#endif//#ifdef _RELEASE

	return S_OK;
}

//============================================================
//	ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理).
//============================================================
LRESULT clsMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDROP hDrop;
	UINT uFileNo;
	switch (uMsg)
	{
		case WM_DESTROY:	//ｳｨﾝﾄﾞｳ破棄時.
			//ｱﾌﾟﾘｹｰｼｮﾝの終了をWindowsに通知する.
			PostQuitMessage(0);
			break;
		case WM_DROPFILES:
			g_bFileActive = true;
			hDrop = (HDROP)wParam;
			uFileNo = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
			char Path[MAX_PATH];
			for (int i = 0; i < (int)uFileNo; i++){
				DragQueryFile(hDrop, i, Path, sizeof(Path));
				g_vsFilePath.push_back(Path);
			}
			DragFinish(hDrop);
			break;
		case WM_KEYDOWN:	//ｷｰﾎﾞｰﾄﾞが押されたとき.
			//ｷｰ別の処理.
			switch ((char)wParam)
			{
				case VK_ESCAPE:	//ESCｷｰ.
					if (MessageBox(NULL,
						"ｹﾞｰﾑを終了しますか?", "Message",
						MB_YESNO) == IDYES)
					{
						//ｳｨﾝﾄﾞｳを破棄する.
						DestroyWindow(hWnd);
					}
					break;
			}
			break;
	}

	//ﾒｲﾝに返す情報.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//============================================================
//	ﾒｯｾｰｼﾞﾙｰﾌﾟとｱﾌﾟﾘｹｰｼｮﾝ処理の入り口.
//============================================================
void clsMain::Loop()
{
	//BGM作成.
	CreateSound();
	//起動音再生.
	m_pBgm[enBGM_START_UP]->Play();

#ifdef START_LOADING_DISPLAY
	//メッシュやスプライトなどの読込.
	NowLoading();
#endif//#ifdef START_LOADING_DISPLAY.

	m_pXInput = new clsXInput;

	//あたり判定.
	m_pCollision = new clsCollision;
	m_pCollision->CreateSe(m_hWnd);

#ifndef START_LOADING_DISPLAY
	//ﾒｯｼｭ読み込み関数をまとめたもの.
	ReadMesh();
	//メインシーン初期化(仮置き).
	InitMain( true );
#endif//#ifndef START_LOADING_DISPLAY

	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄの初期化.
	m_pText = new clsDebugText;
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(m_pText->Init(
		m_pDeviceContext,
		WND_W, WND_H, 50.0f,
		vColor)))
	{
		MessageBox(NULL, "ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄ作成失敗", "clsMain::Loop", MB_OK);
	}

	//----------------------------------------------------------
	//	ﾌﾚｰﾑﾚｰﾄ.
	//----------------------------------------------------------
	float fRate = 0.0f;	//ﾚｰﾄ.
	float fFPS = 60.0f;//FPS値.
	DWORD sync_old = timeGetTime();	//過去時間.
	DWORD sync_now;
	//時間処理の為、最小単位を1ﾐﾘ秒に変更.
	timeBeginPeriod(1);

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();	//現在時間を取得.

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		fRate = 1000.0f / fFPS;	//理想時間を算出.

		if (sync_now - sync_old >= fRate){
			sync_old = sync_now;	//現在時間に置きかえ.

			//ｱﾌﾟﾘｹｰｼｮﾝ処理はここから飛ぶ.
			AppMain();
		}
	}
	//ｱﾌﾟﾘｹｰｼｮﾝの終了.
	timeEndPeriod(1);	//解除.
}

//============================================================
//	ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
//============================================================
void clsMain::AppMain()
{
	//ｺﾝﾄﾛｰﾗ入力情報更新.
	m_pXInput->UpdateStatus();

#ifdef Inoue
	if (g_bFileActive)
	{
		for (size_t i = 0; i < g_vsFilePath.size(); i++)
		{
			m_pTestData->Init((char*)g_vsFilePath[i].c_str());
			m_pTestData->Read();
			if (m_pTestData->GetExtension() == x)
			{
				m_pResource->ReleaseStaticModel(m_pResource->enST_MODEL_TEST);
				m_pResource->CreateStaticModel((LPSTR)m_pTestData->GetFilePath().c_str(),
					m_pResource->enST_MODEL_TEST);
			}
			else if (m_pTestData->GetExtension() == png)
			{
				m_vsmpSpriteTest.push_back(make_unique<clsSprite2D>());
				m_vsmpSpriteTest[g_iPngNum]->Init(m_pDevice, m_pDeviceContext, (LPSTR)m_pTestData->GetFilePath().c_str());
				m_vsmpSpriteTest[g_iPngNum]->SetPos(WND_W / 2 - m_vsmpSpriteTest[g_iPngNum]->GetSs().Disp.w / 2, WND_H / 2 - m_vsmpSpriteTest[g_iPngNum]->GetSs().Disp.h / 2);
				g_iPngNum++;
			}
		}
		g_vsFilePath.clear();
	}
	g_bFileActive = false;

	switch (m_enScene)
	{
		case enSCENE_TITLE:
			SceneTitle();
			m_smpUiManagar->SceneTitle();
			GETKEY_DOWN('O')
			{
				m_smpUiManagar->BlackStart(0.01f);
				InitMain(true);
			}
			break;
		case enSCENE_MAIN:
			SceneMain();
			m_smpUiManagar->SceneMain();
			GETKEY_DOWN('O')
			{
				m_smpUiManagar->BlackStart(0.01f);
				m_enScene = enSCENE_OVER;
			}
			break;
		case enSCENE_OVER:
			SceneOver();
			m_smpUiManagar->SceneOver();
			GETKEY_DOWN('O')
			{
				m_smpUiManagar->BlackStart(0.01f);
				m_enScene = enSCENE_RESULT;
			}
			break;
		case enSCENE_RESULT:
			SceneResult();
			m_smpUiManagar->SceneResult();
			GETKEY_DOWN('O')
			{
				m_smpUiManagar->BlackStart(0.01f);
				m_enScene = enSCENE_ENDROLL;
			}
			break;
		case enSCENE_ENDROLL:
			SceneEnding();
			m_smpUiManagar->SceneEnding();
			GETKEY_DOWN('O')
			{
				m_smpUiManagar->BlackStart(0.01f);
				m_enScene = enSCENE_TITLE;
			}
			break;
	}

#endif//Inoue.

	if (GetAsyncKeyState('I') & 0x0001){
		m_fAlphaPoint = 1.0f;
		for (size_t i = 0; i < m_vsmpSpriteTest.size(); i++)
		{
			if (m_vsmpSpriteTest[i])
			{
				m_vsmpSpriteTest[i].release();
				m_vsmpSpriteTest[i] = NULL;
			}
		}
		m_vsmpSpriteTest.clear();
		m_SpriteControlNum = 0;
		g_iPngNum = 0;
		InitMain(true);
	}

	GETKEY_DOWN('M'){
		if (ControlMode < 1 && 0 < m_vsmpSpriteTest.size()){
			ControlMode++;
		}
		else{
			ControlMode = 0;
		}
	}

	static char blkZYFlg = 0;
	switch (ControlMode)
	{
		case 0:
			//定規位置の移動.
			GETKEY_STAY('A') 	m_ppBlkAry[0]->AddPositionX(-0.1f);
			GETKEY_STAY('D')	m_ppBlkAry[0]->AddPositionX(0.1f);
			GETKEY_STAY('W')	m_ppBlkAry[0]->AddPositionY(0.1f);
			GETKEY_STAY('S')	m_ppBlkAry[0]->AddPositionY(-0.1f);
			GETKEY_STAY('E')	m_ppBlkAry[0]->AddPositionZ(0.1f);
			GETKEY_STAY('Q')	m_ppBlkAry[0]->AddPositionZ(-0.1f);

			GETKEY_DOWN('B'){
				blkZYFlg++;
				if (blkZYFlg >= 3)	blkZYFlg = 0;
			}
			break;
		case 1:
			//スプライト移動、拡縮.
			if (!m_vsmpSpriteTest.empty())
			{
				GETKEY_STAY('A'){ m_vsmpSpriteTest[m_SpriteControlNum]->AddPosX(-5.0f);	 m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs(); }
				GETKEY_STAY('D'){ m_vsmpSpriteTest[m_SpriteControlNum]->AddPosX(5.0f);	 m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs(); }
				GETKEY_STAY('W'){ m_vsmpSpriteTest[m_SpriteControlNum]->AddPosY(-5.0f);	 m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs(); }
				GETKEY_STAY('S'){ m_vsmpSpriteTest[m_SpriteControlNum]->AddPosY(5.0f);	 m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs(); }
				GETKEY_STAY('Q'){
					m_fAlphaPoint -= 0.01f;
					m_vsmpSpriteTest[m_SpriteControlNum]->MulDisp(m_fAlphaPoint);
					m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs();
				}
				GETKEY_STAY('E'){
					m_fAlphaPoint += 0.01f;
					m_vsmpSpriteTest[m_SpriteControlNum]->MulDisp(m_fAlphaPoint);
					m_vsmpSpriteTest[m_SpriteControlNum]->UpDateSpriteSs();
				}
				GETKEY_DOWN('N'){
					if (m_SpriteControlNum < m_vsmpSpriteTest.size() - 1){
						m_SpriteControlNum++;
					}
					else{
						m_SpriteControlNum = 0;
					}
				}
				GETKEY_DOWN('B'){
					if (m_SpriteControlNum > 0){
						m_SpriteControlNum--;
					}
					else{
						m_SpriteControlNum = m_vsmpSpriteTest.size() - 1;
					}
				}
			}
			break;
		case 2:
			//ライトの方向の操作.
			GETKEY_STAY('A') 	m_vLight.x += 0.01f;
			GETKEY_STAY('D')	m_vLight.x -= 0.01f;
			GETKEY_STAY('W')	m_vLight.y += 0.01f;
			GETKEY_STAY('S')	m_vLight.y -= 0.01f;
			GETKEY_STAY('E')	m_vLight.z += 0.01f;
			GETKEY_STAY('Q')	m_vLight.z -= 0.01f;
			GETKEY_STAY('I')	m_vLight = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;
		default:
			break;
	}
	for (char blk_no = 0; blk_no < m_iBlkAryMax; blk_no++){
		m_ppBlkAry[blk_no]->SetPosition(m_ppBlkAry[0]->GetPosition());
		if (blkZYFlg == 0){
			m_ppBlkAry[blk_no]->AddPositionZ((float)blk_no);
		}
		else if (blkZYFlg == 1){
			m_ppBlkAry[blk_no]->AddPositionY((float)blk_no);
		}
		else{
			m_ppBlkAry[blk_no]->AddPositionX((float)blk_no);
		}

		//ギザギザ.
		if (blkZYFlg <= 1){
			if (blk_no % 2 != 0){
				m_ppBlkAry[blk_no]->AddPositionX(1.0f);
			}
		}
		else if (blkZYFlg == 2){
			if (blk_no % 2 != 0){
				m_ppBlkAry[blk_no]->AddPositionY(1.0f);
			}
		}
	}

	GETKEY_DOWN(VK_F1){
		m_smpStageDataExcel->Open(m_hWnd);
	}
	GETKEY_DOWN(VK_F2) {
		m_smpStageDataReadMe->Open(m_hWnd);
	}

	GETKEY_DOWN('P'){
		StageModelSet();
		InitMain(true);
	}

	//ﾚﾝﾀﾞﾘﾝｸﾞ.
	Render();
}

//============================================================
//	描画(ﾚﾝﾀﾞﾘﾝｸﾞ).
//	ｼｰﾝ(場面)を画面にﾚﾝﾀﾞﾘﾝｸﾞ(描画).
//	(※DX9MESH内とMain内で2つ存在するので注意).
//============================================================
void clsMain::Render()
{
	//画面のｸﾘｱ.
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//ｸﾘｱ色(RGBA順)(0.0f~1.0f).
	//ｶﾗｰﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor);
	//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	//ｶﾒﾗ関数.
	Camera();
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
	Proj();

	switch (m_enScene)
	{
		case enSCENE_TITLE:
			RenderTitle();
			SetDepth(false);	//Zﾃｽﾄ:OFF.
			m_smpUiManagar->RenderTitle();
			SetDepth(true);	//Zﾃｽﾄ:ON.
			break;
		case enSCENE_MAIN:
			RenderMain();
			SetDepth(false);	//Zﾃｽﾄ:OFF.
			m_smpUiManagar->RenderMain();
			SetDepth(true);	//Zﾃｽﾄ:ON.
			break;
		case enSCENE_OVER:
			RenderOver();
			SetDepth(false);	//Zﾃｽﾄ:OFF.
			m_smpUiManagar->RenderOver();
			SetDepth(true);	//Zﾃｽﾄ:ON.
			break;
		case enSCENE_RESULT:
			RenderResult();
			SetDepth(false);	//Zﾃｽﾄ:OFF.
			m_smpUiManagar->RenderResult();
			SetDepth(true);	//Zﾃｽﾄ:ON.
			break;
		case enSCENE_ENDROLL:
			RenderEnding();
			SetDepth(false);	//Zﾃｽﾄ:OFF.
			m_smpUiManagar->RenderEnding();
			SetDepth(true);	//Zﾃｽﾄ:ON.
			break;
	}

#if _DEBUG
	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄ.
	RenderDebugText();
#endif//#if _DEBUG

	for (char i = 0; i < m_vsmpSpriteTest.size(); i++){
		m_vsmpSpriteTest[i]->Render();
	}
	//ﾚﾝﾀﾞﾘﾝｸﾞされたｲﾒｰｼﾞを表示.
	m_pSwapChain->Present(0, 0);
}

//============================================================
//	DirectX初期化.
//============================================================
HRESULT clsMain::InitD3D()
{
	//---------------------------------------------------
	//	ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝ関係.
	//---------------------------------------------------

	//ｽﾜｯﾌﾟﾁｪｰﾝ構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;		//ﾊﾞｯｸﾊﾞｯﾌｧの数.
	sd.BufferDesc.Width = WND_W;
	sd.BufferDesc.Height = WND_H;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//ﾌｫｰﾏｯﾄ(32ﾋﾞｯﾄｶﾗｰ).
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分子).
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//使い方(表示先).
	sd.OutputWindow = m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	sd.SampleDesc.Count = 1;		//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	sd.SampleDesc.Quality = 0;		//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	sd.Windowed = TRUE;		//ｳｨﾝﾄﾞｳﾓｰﾄﾞ(ﾌﾙｽｸ時はFALSE).

	//作成を試みる機能ﾚﾍﾞﾙの優先を指定.
	//	(GPUがｻﾎﾟｰﾄする機能ｾｯﾄの定義).
	//	D3D_FEATURE_LEVEL列挙体の配列.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1のGPUﾚﾍﾞﾙ.
	D3D_FEATURE_LEVEL	pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL*	pFeatureLevel = NULL;	//配列の要素数.

	//ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成.
	//	ﾊｰﾄﾞｳｪｱ(GPU)ﾃﾞﾊﾞｲｽで作成.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
		NULL,					//ﾋﾞﾃﾞｵｱﾀﾞﾌﾟﾀへのﾎﾟｲﾝﾀ.
		D3D_DRIVER_TYPE_HARDWARE,//作成するﾃﾞﾊﾞｲｽの種類.
		NULL,					//ｿﾌﾄｳｪｱﾗｽﾀﾗｲｻﾞｰを実装するDLLのﾊﾝﾄﾞﾙ.
		0,						//有効にするﾗﾝﾀｲﾑﾚｲﾔｰ.
		&pFeatureLevels,		//作成を試みる機能ﾚﾍﾞﾙの順序を指定する配列.
		1,						//↑の要素数.
		D3D11_SDK_VERSION,		//SDKのﾊﾞｰｼﾞｮﾝ.
		&sd,					//ｽﾜｯﾌﾟﾁｪｰﾝの初期化ﾊﾟﾗﾒｰﾀのﾎﾟｲﾝﾀ.
		&m_pSwapChain,			//(out)ﾚﾝﾀﾞﾘﾝｸﾞに使用されたｽﾜｯﾌﾟﾁｪｰﾝ.
		&m_pDevice,				//(out)作成されたﾃﾞﾊﾞｲｽ.
		pFeatureLevel,			//機能ﾚﾍﾞﾙの配列にある最初の要素を表すﾎﾟｲﾝﾀ.
		&m_pDeviceContext)))	//(out)ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	{
		//WARPﾃﾞﾊﾞｲｽの作成.
		// D3D_FEATURE_LEVEL_9_1 ～ D3D_FEATURE_LEVEL_10_1.
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_WARP,
			NULL, 0, &pFeatureLevels, 1,
			D3D11_SDK_VERSION, &sd,
			&m_pSwapChain, &m_pDevice,
			pFeatureLevel, &m_pDeviceContext)))
		{
			//ﾘﾌｧﾚﾝｽﾃﾞﾊﾞｲｽの作成.
			//	DirectX SDKがｲﾝｽﾄｰﾙされていないと使えない.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd,
				&m_pSwapChain, &m_pDevice,
				pFeatureLevel, &m_pDeviceContext)))
			{
				MessageBox(NULL,
					"ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成に失敗",
					"error(main.cpp)", MB_OK);
				return E_FAIL;
			}
		}
	}

	//各種ﾃｸｽﾁｬｰと、それに付帯する各種ﾋﾞｭｰ(画面)を作成.

	//---------------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備:ｶﾗｰﾊﾞｯﾌｧ設定.
	//---------------------------------------------------

	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬｰを取得(既にあるので作成はない).
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けたGUIDを取得.
		//		   Texture2Dの唯一の物として扱う.
		(LPVOID*)&pBackBuffer_Tex);//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬｰに対しﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice->CreateRenderTargetView(
		pBackBuffer_Tex,
		NULL,
		&m_pBackBuffer_TexRTV);//(out)RTV.
	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬｰを解放.
	SAFE_RELEASE(pBackBuffer_Tex);//ﾏｸﾛ内にｾﾐｺﾛﾝがあるのでいらないが見た目の統一でつけている.

	//---------------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備:ﾃﾞﾌﾟｽ(深度)ｽﾃﾝｼﾙ関係.
	//---------------------------------------------------

	//ﾃﾞﾌﾟｽ(深さor深度)ｽﾃﾝｼﾙﾋﾞｭｰ用のﾃｸｽﾁｬｰを作成.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = WND_W;					//幅.
	descDepth.Height = WND_H;					//高さ.
	descDepth.MipLevels = 1;						//ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize = 1;						//配列数:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;	//32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count = 1;						//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality = 0;						//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage = D3D11_USAGE_DEFAULT;		//使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//深度(ｽﾃﾝｼﾙとして使用).
	descDepth.CPUAccessFlags = 0;						//CPUからｱｸｾｽしない.
	descDepth.MiscFlags = 0;						//その他設定なし.

	m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pBackBuffer_DSTex);//(out)ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対しﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	m_pDevice->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		NULL,
		&m_pBackBuffer_DSTexDSV);//(out)DSV.

	//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//深度ﾃｽﾄ(Zﾃｽﾄ)を有効にする.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc,
		sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;

	if (SUCCEEDED(m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState)))
	{
		m_pDeviceContext->OMSetDepthStencilState(
			m_pDepthStencilState, 1);
	}

	//ﾋﾞｭｰﾎﾟｰﾄの設定.
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;//幅.
	vp.Height = WND_H;//高さ.
	vp.MinDepth = 0.0f;	//最小深度(手前).
	vp.MaxDepth = 1.0f;	//最大深度(奥).
	vp.TopLeftX = 0.0f;	//左上位置x.
	vp.TopLeftY = 0.0f;	//左上位置y.
	m_pDeviceContext->RSSetViewports(1, &vp);

	//Sp2D用.
	SetViewPort10(&vp);

	//ﾗｽﾀﾗｲｽﾞ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;	//塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode = D3D11_CULL_NONE;
	//D3D11_CULL_NONE :ｶﾘﾝｸﾞを切る(正背面を描画する).
	//D3D11_CULL_BACK :背面を描画しない.
	//D3D11_CULL_FRONT:正面を描画しない.
	rdc.FrontCounterClockwise = FALSE;
	//ポリゴンの表裏を決定するﾌﾗｸﾞ.
	//TRUE :左回りなら前向き,右回りなら後ろ向き.
	//FALSE:↑の逆になる.
	rdc.DepthClipEnable = FALSE;	//距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}

//============================================================
//	Direct3D終了処理.
//============================================================
void clsMain::DestroyD3D()
{
	//作ったのとは逆順.

	////Effekseerの破棄.
	//clsEffects::GetInstance()->Destroy();

	//音声ﾌｧｲﾙを閉じる.
	if (m_pBgm != NULL){
		for (int i = 0; i < enBGM_MAX; i++){
			m_pBgm[i]->Close();
			delete m_pBgm[i];
			m_pBgm[i] = NULL;
		}
	}

	//SEを閉じる.
	if (m_pSe != NULL){
		for (int i = 0; i < enSe_MAX; i++){
			m_pSe[i]->Close();
			delete m_pSe[i];
			m_pSe[i] = NULL;
		}
	}

	//定規の削除.
	for (int i = 0; i < m_iBlkAryMax; i++){
		//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
		m_ppBlkAry[i]->DetatchModel();
		//破棄.
		delete m_ppBlkAry[i];
		m_ppBlkAry[i] = NULL;
	}
	//ﾎﾟｲﾝﾀ配列破棄.
	delete[] m_ppBlkAry;
	m_ppBlkAry = NULL;

	//最大数初期化.
	m_iBlkAryMax = 0;

#ifdef Inoue

	//ステージ(壁)の削除.
	if (!m_vsmpWall.empty()){
		for (int i = 0; i < m_vsmpWall.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpWall[i]->DetatchModel();
			//破棄.
			m_vsmpWall[i].reset();
		}
		//vectorの破棄.
		m_vsmpWall.clear();
		m_vsmpWall.shrink_to_fit();
	}
	//ステージ(床)の削除.
	if (!m_vsmpFloor.empty()){
		for (int i = 0; i < m_vsmpFloor.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpFloor[i]->DetatchModel();
			//破棄.
			m_vsmpFloor[i].reset();
		}
		//vectorの破棄.
		m_vsmpFloor.clear();
		m_vsmpFloor.shrink_to_fit();
	}

	//ステージ(動く床○)の削除.
	if (!m_vsmpStepCil.empty()){
		for (int i = 0; i < m_vsmpStepCil.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpStepCil[i]->DetatchModel();
			//破棄.
			m_vsmpStepCil[i].reset();
		}
		//vectorの破棄.
		m_vsmpStepCil.clear();
		m_vsmpStepCil.shrink_to_fit();
	}

	//ステージ(動く床□)の削除.
	if (!m_vsmpStepCil.empty()){
		for (int i = 0; i < m_vsmpStepCil.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpStepBox[i]->DetatchModel();
			//破棄.
			m_vsmpStepBox[i].reset();
		}
		//vectorの破棄.
		m_vsmpStepCil.clear();
		m_vsmpStepCil.shrink_to_fit();
	}

	//ステージ(敵)の削除.
	if (!m_vsmpEnemyMgr.empty()){
		for (int i = 0; i < m_vsmpEnemyMgr.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpEnemyMgr[i]->DetatchModel();
			//破棄.
			m_vsmpEnemyMgr[i].reset();
		}
		//vectorの破棄.
		m_vsmpEnemyMgr.clear();
		m_vsmpEnemyMgr.shrink_to_fit();
	}

	//ステージ(ペンデュラム)の削除.
	if (!m_vsmpPend.empty()){
		for (int i = 0; i < m_vsmpPend.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpPend[i]->DetatchModel();
			//破棄.
			m_vsmpPend[i].reset();
		}
		//vectorの破棄.
		m_vsmpPend.clear();
		m_vsmpPend.shrink_to_fit();
	}
	//ステージ(槍床)の削除.
	if (!m_vsmpSpiaFloorMgr.empty()){
		for (int i = 0; i < m_vsmpSpiaFloorMgr.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpSpiaFloorMgr[i]->DetatchModel();
			//破棄.
			m_vsmpSpiaFloorMgr[i].reset();
		}
		//vectorの破棄.
		m_vsmpSpiaFloorMgr.clear();
		m_vsmpSpiaFloorMgr.shrink_to_fit();
	}

	//ステージ(槍壁)の削除.
	if (!m_vsmpSpiaWallMgr.empty()){
		for (int i = 0; i < m_vsmpSpiaWallMgr.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpSpiaWallMgr[i]->DetatchModel();
			//破棄.
			m_vsmpSpiaWallMgr[i].reset();
		}
		//vectorの破棄.
		m_vsmpSpiaWallMgr.clear();
		m_vsmpSpiaWallMgr.shrink_to_fit();
	}
	//ステージ(落とし穴の蓋)の削除.
	if (m_vsmpCoverMgr.empty()){
		for (int i = 0; i < m_vsmpCoverMgr.size(); i++){
			//ﾓﾃﾞﾙﾃﾞｰﾀの関連付け.
			m_vsmpCoverMgr[i]->DetatchModel();
			//破棄.
			m_vsmpCoverMgr[i].reset();
		}
		//vectorの破棄.
		m_vsmpCoverMgr.clear();
		m_vsmpCoverMgr.shrink_to_fit();
	}

	//スプライトクラス解放.
	if (!m_vsmpSpriteTest.empty()){
		for (size_t i = 0; i < m_vsmpSpriteTest.size(); i++)
		{
			m_vsmpSpriteTest[i].reset();
		}
		m_vsmpSpriteTest.clear();
		m_vsmpSpriteTest.shrink_to_fit();
	}

	//ﾌｧｲﾙ読込ｸﾗｽ解放.
	SAFE_DELETE(m_pFile);

	//ﾌｧｲﾙ読込ｸﾗｽ解放.
	if (m_smpStageData != NULL){
		m_smpStageData.reset();
		m_smpStageData = NULL;
	}

	//Excel.
	if (m_smpStageDataExcel != NULL){
		m_smpStageDataExcel.reset();
		m_smpStageDataExcel = NULL;
	}

	//UiManager.
	if (m_smpUiManagar != NULL){
		m_smpUiManagar.reset();
		m_smpUiManagar = NULL;
	}

#endif //ifdef Inoue.

	//NEW_ARBIA.
	if (m_pPlayer != NULL){
		m_pPlayer->DetatchModel();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	//あたり判定.
	if (m_pCollision != NULL){
		delete m_pCollision;
		m_pCollision = NULL;
	}

	//ｺﾝﾄﾛｰﾗ入力.
	if (m_pXInput != NULL){
		m_pXInput->EndProc();
		XInputEnable(false);
		delete m_pXInput;
		m_pXInput = NULL;
	}

	//Direct3Dｵﾌﾞｼﾞｪｸﾄを解放.
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

////============================================================
//	ｽﾌｨｱ作成.
////============================================================
HRESULT clsMain::InitSphere(clsDX9Mesh* pMesh, float fScale)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;	//頂点ﾊﾞｯﾌｧ.
	void*	pVertices = NULL;	//頂点.
	D3DXVECTOR3	vCenter;		//中心.
	float	fRadius;			//半径.

	//頂点ﾊﾞｯﾌｧを取得.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB))){
		return E_FAIL;
	}

	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0))){
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//ﾒｯｼｭの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),	//頂点の数.
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),	//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius);	//(out)半径.

	//ｱﾝﾛｯｸ.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * fScale;

	return S_OK;
}
////============================================================
//	ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ作成.
////============================================================
HRESULT clsMain::InitBBox(clsDX9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVertices = NULL;
	D3DXVECTOR3 Max, Min;

	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB))){
		MessageBox(NULL, "頂点ﾊﾞｯﾌｧ取得失敗", "clsMain::InitBBox()", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0))){
		MessageBox(NULL, "頂点ﾊﾞｯﾌｧのﾛｯｸに失敗", "clsMain::InitBBox()", MB_OK);
		return E_FAIL;
	}
	//ﾒｯｼｭ内の頂点位置の最大と最小を検索する.
	D3DXComputeBoundingBox(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(
		pMesh->m_pMesh->GetFVF()),
		&Min, &Max);	//(out)最小、最大頂点.
	pVB->Unlock();			//ｱﾝﾛｯｸ.
	SAFE_RELEASE(pVB);	//使用済頂点ﾊﾞｯﾌｧの解放.

	pMesh->m_BBox.vPosMax = Max;
	pMesh->m_BBox.vPosMin = Min;

	//軸ﾍﾞｸﾄﾙ,軸の長さ(この場合ﾎﾞｯｸｽの各半径)を初期化する.
	pMesh->m_BBox.fLengthX = (Max.x - Min.x) / 2.0f;
	pMesh->m_BBox.fLengthY = (Max.y - Min.y) / 2.0f;
	pMesh->m_BBox.fLengthZ = (Max.z - Min.z) / 2.0f;

	return S_OK;
}

////============================================================
//	ﾒｯｼｭ読み込み関数(まとめた).
////============================================================
HRESULT clsMain::ReadMesh()
{
	m_pFile = new clsFile;
	m_pFile->Init("Data\\Text\\power.txt");

	m_pTestData = make_unique<clsFile>();

#ifdef Inoue

	m_smpUiManagar = make_unique<clsUiManagar>();
	m_smpUiManagar->Init(m_hWnd, m_pDevice, m_pDeviceContext);

	m_smpStageDataExcel = make_unique<clsFile>();
	m_smpStageDataExcel->Init("Data\\Text\\StageData.xlsm");

	m_smpStageData = make_unique<clsFile>();
	m_smpStageData->Init("Data\\Text\\StageData.csv");

	m_smpStageDataReadMe = make_unique<clsFile>();
	m_smpStageDataReadMe->Init("Data\\Text\\ReadMe.txt");
#endif //#ifdef Inoue.

	//プレイヤー.
	{
		m_pPlayer = new clsPlayer;
		m_pPlayer->Create(m_hWnd, m_pDevice, m_pDeviceContext);
		m_pPlayer->Init();

		CD3DXSKINMESH_INIT si;//skin_Init.
		si.hWnd = m_hWnd;
		si.pDevice = m_pDevice;
		si.pDeviceContext = m_pDeviceContext;

		m_pPlayer->m_pModel = new clsD3DXSKINMESH;
		m_pPlayer->m_pModel->Init(&si);

		//ﾓﾃﾞﾙ関連付け.
		m_pPlayer->
			AttachModel(m_pResource->GetSkinModels(clsResource::enSK_MODEL_PLAYER));

		m_pPlayer->SetPosition(
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		float LocalScale = 0.1f;
		m_pPlayer->SetScale(LocalScale);

		//		m_pPlayer->SetAnimSpeed( 0.01 );
	}

	//ﾘｿｰｽ定規.
	//ｷｬﾗｸﾀｰｸﾗｽ.
	m_iBlkAryMax = 100;
	m_ppBlkAry = new clsCharaStatic*[m_iBlkAryMax];
	//ｷｬﾗｸﾗｽのﾎﾟｲﾝﾀの配列を作る.
	for (int i = 0; i < m_iBlkAryMax; i++){
		m_ppBlkAry[i] = new clsCharaStatic;

		//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け.
		m_ppBlkAry[i]->AttachModel(
			m_pResource->GetStaticModels(clsResource::enST_MODEL_BLOCK));

		m_ppBlkAry[i]->SetPosition(
			D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	//列挙体のステージの一番最初.
	iWallModelNumZero = m_pResource->EtoI(clsResource::enST_MODEL_WALL_MIN);
	iFloorModelNumZero = m_pResource->EtoI(clsResource::enST_MODEL_FLOOR_MIN);

	////板ﾎﾟﾘｺﾞﾝの初期化.
	//m_pExplosion = new clsSprite;
	//m_pExplosion->Init( m_pDevice, m_pDeviceContext );
	//m_pExplosion->m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_pExplosion->m_bDispFlg = false;

	//ﾊﾟｰﾃｨｸﾙの初期化500個.
	//	m_pParticle = new clsParticle;	//元はこうして使っていたよ.
	m_pParticle = new clsParticle(PAT_SMOK_MAX, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pParticle->Init(m_pDevice, m_pDeviceContext);
	m_pParticle->m_bDispFlg = false;

	//ﾚｲ表示の初期化(垂直).
	m_pRayV = new clsRay;
	m_pRayV->m_Ray.vPoint[0] = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	m_pRayV->m_Ray.vPoint[1] = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	m_pRayV->Init(m_pDevice, m_pDeviceContext);
	//ﾚｲ表示の初期化(前後).
	m_pRayFB = new clsRay;
	m_pRayFB->m_Ray.vPoint[0] = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	m_pRayFB->m_Ray.vPoint[1] = D3DXVECTOR3(0.0f, 0.0f, 5.0f);
	m_pRayFB->Init(m_pDevice, m_pDeviceContext);
	//ﾚｲ表示の初期化(左右).
	m_pRayH = new clsRay;
	m_pRayH->m_Ray.vPoint[0] = D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
	m_pRayH->m_Ray.vPoint[1] = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
	m_pRayH->Init(m_pDevice, m_pDeviceContext);

	//モデルの紐づけ.
	StageModelSet();

	//------------------------------
	//	Effekseerｸﾗｽ.
	//------------------------------
	clsEffects::GetInstance()->Create(m_pDevice, m_pDeviceContext);
	clsEffects::GetInstance()->LoadData();

	return S_OK;
}

//============================================================
//	深度ﾃｽﾄ(Zﾃｽﾄ)　ON/OFF切替.
//============================================================
void clsMain::SetDepth(bool bFlg)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc,
		sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = bFlg;

	m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState);
	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState, 1);
}

////============================================================
//	衝突判定関数.
////============================================================
bool clsMain::Collision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget)
{
	//2つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength
		= pTarget->m_vPos - pAttacker->m_vPos;
	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//2物体間の距離が、2物体の半径を足したものより.
	//小さいということは、ｽﾌｨｱ同士が重なっている.
	//(衝突している)ということ.
	if (Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;//衝突した.
	}
	return false;	//衝突していない.
}
//============================================================
//	ﾎﾞｯｸｽ衝突判定関数.
//============================================================
bool clsMain::BBoxCollision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget)
{
	//攻撃側用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecMaxAtk, vecMinAtk;	//最大、最小頂点.
	vecMaxAtk = pAttacker->m_BBox.vPosMax
		+ pAttacker->m_vPos;
	vecMinAtk = pAttacker->m_BBox.vPosMin
		+ pAttacker->m_vPos;

	//目標側用に2つの頂点を用意し初期化する.
	D3DXVECTOR3 vecMaxTrg, vecMinTrg;	//最大、最小頂点.
	vecMaxTrg = pTarget->m_BBox.vPosMax
		+ pTarget->m_vPos;
	vecMinTrg = pTarget->m_BBox.vPosMin
		+ pTarget->m_vPos;

	//2つの物体それぞれの最大、最小位置という,.
	//4つの情報があれば、衝突を検出できる.
	if (vecMinAtk.x < vecMaxTrg.x  &&  vecMaxAtk.x > vecMinTrg.x &&
		vecMinAtk.y < vecMaxTrg.y  &&  vecMaxAtk.y > vecMinTrg.y &&
		vecMinAtk.z < vecMaxTrg.z  &&  vecMaxAtk.z > vecMinTrg.z)
	{
		return true;//衝突している.
	}

	return false;	//衝突していない.
}

//============================================================
//	ﾎﾞｰﾝの座標をとる.
//============================================================
void clsMain::GetPosFromBone(clsD3DXSKINMESH* skinMesh, char BoneName[], D3DXVECTOR3& Pos)
{
	D3DXVECTOR3 vBonePos;
	if (skinMesh->GetPosFromBone(BoneName, &vBonePos)){
		Pos = vBonePos;
	}
}

//============================================================
//	ﾚｲとﾒｯｼｭのあたり判定.
//============================================================
bool clsMain::Intersect(
	clsDX9Mesh* pAttacker,		//基準のの物体.
	clsDX9Mesh* pTarget,		//対象のの物体.
	float* pfDistance,			//(out)距離.
	D3DXVECTOR3* pvIntersect)	//(out)交差座標.
{
	D3DXMATRIXA16	matRot;	//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, pAttacker->m_fYaw);

	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ;
	//Z軸ﾍﾞｸﾄﾙそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = pAttacker->m_vRay;
	//Attackerの座標に回転座標を合成する.
	vecEnd += vecAxisZ * 1.0f;

	//対象が動いている物体でも、対象のworld行列の、.
	//逆行列を用いれば、正しくﾚｲが当たる.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->m_vPos.x,
		pTarget->m_vPos.y,
		pTarget->m_vPos.z);

	//逆行列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;	//命中ﾌﾗｸﾞ.

	DWORD dwIndex = 0;	//ｲﾝﾃﾞｯｸｽ番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;	//(out)重心ﾋｯﾄ座標.

	//ﾒｯｼｭとﾚｲの交差を調べる.
	D3DXIntersect(
		pTarget->m_pMesh,//対象ﾒｯｼｭ.
		&vecStart,		//開始位置.
		&vecDistance,	//ﾚｲの距離.
		&bHit,			//(out)判定結果.
		&dwIndex,		//(out)bHitがTrue時、ﾚｲの視点に。.
		//最も近くの面のｲﾝﾃﾞｯｸｽ値へのﾎﾟｲﾝﾀ.
		&U, &V,			//(out)重心ﾋｯﾄ座標.
		pfDistance,		//ﾀｰｹﾞｯﾄとの距離.
		NULL, NULL);

	if (bHit){
		//命中したとき.
		FindVerticesOnPoly(
			pTarget->m_pMeshForRay, dwIndex, vVertex);
		//重心座標から交差点を算出.
		//ﾛｰｶﾙ交点pは、 v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		return true;
	}
	return false;
}
//============================================================
//	交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
//		※頂点座標はﾛｰｶﾙ座標.
//============================================================
HRESULT clsMain::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
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
		D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE*	pbVertices = NULL;	//頂点(ﾊﾞｲﾄ型)
	FLOAT*	pfVertices = NULL;	//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;	//頂点ﾊﾞｯﾌｧ.
	pTarget->GetVertexBuffer(&VB);	//頂点情報の取得.

	//頂点ﾊﾞｯﾌｧのﾛｯｸ.
	if (SUCCEEDED(
		VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
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
		for (int i = 0; i < 3; i++){
			pfVertices
				= (FLOAT*)&pbVertices[dwStride * pwPoly[dwPolyIndex * 3 + i]];
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
//	壁のあたり判定関連.
//============================================================
void clsMain::WallJudge(clsDX9Mesh* pAttacker, clsDX9Mesh* pWall)
{
	const char MAX = 4;//.線の数
	const float HitHeight = 0.375f;	//ｱﾙﾋﾞｱの足元からの高さ.

	FLOAT		fDistance[MAX] = { 0.0f, 0.0f, 0.0f, 0.0f };	//距離.
	D3DXVECTOR3	vIntersect[MAX];//交点座標.
	float		fDis, fYaw;		//距離と回転.

	pAttacker->m_vRay = pAttacker->m_vPos;
	pAttacker->m_vRay.y += HitHeight;

#if 0
	//軸ﾍﾞｸﾄﾙの方向前後右左.
	D3DXVECTOR3 Axis[MAX] = { D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f) };
	//ﾚｲの向きにより当たる壁までの距離を求める.
	//軸ﾍﾞｸﾄﾙ　前後右左.
	for (char i = 0; i < MAX; i++){
		pAttacker->m_vAxis = Axis[i];
		Intersect(pAttacker, pWall, &fDistance[i], &vIntersect[i]);
	}
#else
	//軸ﾍﾞｸﾄﾙ　前.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	Intersect(pAttacker, pWall, &fDistance[0], &vIntersect[0]);
	//軸ﾍﾞｸﾄﾙ　後.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	Intersect(pAttacker, pWall, &fDistance[1], &vIntersect[1]);
	//軸ﾍﾞｸﾄﾙ　右.
	pAttacker->m_vAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[2], &vIntersect[2]);
	//軸ﾍﾞｸﾄﾙ　左.
	pAttacker->m_vAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[3], &vIntersect[3]);
#endif

	//絶対値.
	fYaw = fabs(pAttacker->m_fYaw);
	DirOverGuard(&fYaw);

	//前が壁に接近.
	fDis = fDistance[0];
	if (fDis < WALL_SPACE && fDis > 0.01f){
		if (pAttacker->m_fYaw < 0.0f){
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
		}
		else{
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
		}
	}

	//後ろが壁に接近.
	fDis = fDistance[1];
	if (fDis < WALL_SPACE && fDis > 0.01f){
		//時計回り.
		if (pAttacker->m_fYaw < 0.0f){
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else if( fYaw >= 3.925f && fYaw < 5.495f ){	//左から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
		}
		//反時計回り.
		else{
			if( fYaw >= 0.785f && fYaw < 2.355f ){		//右から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
		}
	}

	//右が壁に接近.
	fDis = fDistance[2];
	if (fDis < WALL_SPACE && fDis > 0.01f){
		//時計回り.
		if (pAttacker->m_fYaw < 0.0f){
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
		}
		//反時計回り.
		else{
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
		}
	}

	//左が壁に接近.
	fDis = fDistance[3];
	if (fDis < WALL_SPACE && fDis > 0.01f){
		//時計回り.
		if (pAttacker->m_fYaw < 0.0f){
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
		}
		//反時計回り.
		else{
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->m_vPos.z -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->m_vPos.x -= WALL_SPACE - fDis;
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->m_vPos.z += WALL_SPACE - fDis;
			}
			else{										//奥から.
				pAttacker->m_vPos.x += WALL_SPACE - fDis;
			}
		}
	}
}

//============================================================
//	回転値調整.
//============================================================
void clsMain::DirOverGuard(float* fYaw)
{
	if (*fYaw > (float)(D3DX_PI * 2.0)){
		*fYaw -= (float)(D3DX_PI * 2.0);
	}
	else{
		return;
	}

	//再帰.
	if (*fYaw > (float)(D3DX_PI * 2.0)){
		DirOverGuard(fYaw);
	}
}

//============================================================
//	ｶﾒﾗ関数.
//============================================================
void clsMain::Camera()
{
	////----------------------------.
	////	ｶﾒﾗ追従処理ここから.
	////----------------------------.
	////ｶﾒﾗ位置(自機の背中から)の位置.
	//m_Camera.vEye = m_Camera.vLook = m_pOldPlayer->m_vPos;
	//m_Camera.fYaw = m_pOldPlayer->m_fYaw;
	////Y軸回転行列の作成.
	//D3DXMatrixRotationY( &m_Camera.mRot, m_Camera.fYaw );
	////軸ﾍﾞｸﾄﾙ.
	//D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );
	////Z軸ﾍﾞｸﾄﾙそのものを回転.
	//D3DXVec3TransformCoord( &vecAxisZ, &vecAxisZ, &m_Camera.mRot );
	//m_Camera.vEye	-= vecAxisZ * 4.0f;//自機の背中側.
	//m_Camera.vLook	+= vecAxisZ * 2.0f;//自機の前側.
	//m_Camera.vEye.y		+= 2.0f;
	//m_Camera.vLook.y	+= 0.2f;
	////----------------------------.
	////	ｶﾒﾗ追従処理ここまで.
	////----------------------------.

	//ﾋﾞｭｰ(ｶﾒﾗ)変換.
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);	//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ﾋﾞｭｰ計算結果.
		&m_Camera.vEye, &m_Camera.vLook, &vUpVec);
}
//============================================================
//	ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
//============================================================
void clsMain::Proj()
{
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,			//(out)ﾌﾟﾛｼﾞｪｸｼｮﾝ計算結果.
		D3DX_PI / g_Zoom,	//y方向の視野(ﾗｼﾞｱﾝ指定)数字を大きくしたら視野が狭くなる.
		(FLOAT)WND_W / (FLOAT)WND_H,//ｱｽﾍﾟｸﾄ比(幅/高さ).
		0.1f,				//近いﾋﾞｭｰ平面のz値.
		fRENDER_LIMIT);	//遠いﾋﾞｭｰ平面のz値.100.f
}

//============================================================
//============================================================
//				シーン初期化シリーズ.
//============================================================
//============================================================
//============================================================
//	タイトル用初期化.
//============================================================
void clsMain::InitTitle()
{
	InitMain(true);

	//初期化カメラ位置.
	const D3DXVECTOR3 vCAM_INIT_TITLE_POS =
	{ 0.7f, 2.4f, 6.7f };
	const D3DXVECTOR3 vLOOK_INIT_TITLE_POS =
	{ -1.4f, 1.55f, 9.6f };
	m_Camera.vEye = vCAM_INIT_TITLE_POS;
	m_Camera.vLook = vLOOK_INIT_TITLE_POS;

	//シーン.
	m_enScene = enSCENE_TITLE;
	m_enTitleSceneMode = en_TSM_IDLE;
	//UI.
	m_smpUiManagar->ChangeTitleInit();

	m_ResUiState.Init();

	//発見情報更新.
	m_smpUiManagar->SetDisc(m_ResUiState.iDisc);
	m_smpUiManagar->SetLives(m_ResUiState.iLives);

	//BGM.
	m_pBgm[enBGM_MAIN]->Stop();
	m_pBgm[enBGM_ENDROLL]->Stop();
	m_pBgm[enBGM_OVER]->Stop();

	m_pBgm[enBGM_TITLE]->SeekToStart();
	m_pBgm[enBGM_TITLE]->Play(true);

	//プレイヤー.
	m_pPlayer->InitTitleScene();

	//ドア.
	for (int i = 0; i < m_vsmpDoorMgr.size(); i++){
		m_vsmpDoorMgr[i]->Init();
	}
}
//============================================================
//	ステージ開始用初期化.
//============================================================
void clsMain::InitMain(bool bFirstFlg)
{
	//初回の初期化.
	if (bFirstFlg){
		//シーン.
		m_enScene = enSCENE_MAIN;
		m_smpUiManagar->ChangeMainInit();
		//BGM.
		m_pBgm[enBGM_TITLE]->Stop();
		m_pBgm[enBGM_MAIN]->Stop();
		m_pBgm[enBGM_MAIN]->SeekToStart();
		m_pBgm[enBGM_MAIN]->SetVolume(iVOL_BGM_MAIN);
		m_pBgm[enBGM_MAIN]->Play(true);
		//プレイヤー.
		m_pPlayer->Spawn();
		//ドア(0はタイトル).
		for (int i = 1; i < m_vsmpDoorMgr.size(); i++){
			m_vsmpDoorMgr[i]->Init();
		}
		//時間.
		m_smpUiManagar->ReSetTimer();
		m_smpUiManagar->SetTimerStop(true);
		//発見数.
		m_smpUiManagar->ReSetDisc();
		//残機.
		m_smpUiManagar->ReSetLives();
	}
	//復活時の初期化.
	else{
		//プレイヤー.
		m_pPlayer->ReSpawn();
		//ドア(0はタイトル).
		for (int i = 0; i < m_vsmpDoorMgr.size(); i++){
			m_vsmpDoorMgr[i]->ReStart();
		}
		//タイマ動かす.
		m_smpUiManagar->SetTimerStop(false);
	}
	m_iRespawnTimer = 0;

	//残機.
	m_smpUiManagar->SetLives(m_pPlayer->GetHp());

	//カメラ.
	D3DXVECTOR3 vCamInitPos = m_pPlayer->GetPosition() + vCAMERA_INIT_OFFSET;
	m_Camera.vEye = m_vCameraTarget = vCamInitPos;
	m_fLookOffset = 0.0f;

	//丸床.
	for (int i = 0; i < m_vsmpStepCil.size(); i++){
		m_vsmpStepCil[i]->Init();
	}
	//四角床.
	for (int i = 0; i < m_vsmpStepBox.size(); i++){
		m_vsmpStepBox[i]->Init();
	}

	//敵.
	for (int i = 0; i < m_vsmpEnemyMgr.size(); i++){
		m_vsmpEnemyMgr[i]->Init(bFirstFlg);
	}

	//床槍.
	for (int i = 0; i < m_vsmpSpiaFloorMgr.size(); i++){
		m_vsmpSpiaFloorMgr[i]->Init();
	}
	//壁槍.
	for (int i = 0; i < m_vsmpSpiaWallMgr.size(); i++){
		m_vsmpSpiaWallMgr[i]->Init();
	}

	//振り子.
	for (int i = 0; i < m_vsmpPend.size(); i++){
		m_vsmpPend[i]->Init();
	}

	//落とし穴.
	for (int i = 0; i < m_vsmpCoverMgr.size(); i++){
		m_vsmpCoverMgr[i]->Init();
	}

	//ゴール.
	for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
		m_vsmpGoalMgr[i]->Init();
	}
}
//============================================================
//	ゲームオーバー用初期化.
//============================================================
void clsMain::InitOver()
{
	//カメラ初期化位置.
	const D3DXVECTOR3 vCAM_INIT_OVER_POS = { 0.55f, 3.619998f, -4.024998f };
	const D3DXVECTOR3 vLOOK_INIT_OVER_POS = { 0.3f, 1.762501f, 0.375f };

	//シーン.
	m_enScene = enSCENE_OVER;
	//UI.
	m_smpUiManagar->ChangeOverInit();
	//BGM.
	m_pBgm[enBGM_MAIN]->Stop();
	m_pBgm[enBGM_OVER]->SeekToStart();
	m_pBgm[enBGM_OVER]->Play(true);

	m_pPlayer->InitOverScene();

	m_Camera.vEye = vCAM_INIT_OVER_POS;
	m_Camera.vLook = vLOOK_INIT_OVER_POS;
}
//============================================================
//	結果用初期化.
//============================================================
void clsMain::InitResult()
{
	//シーン.
	m_enScene = enSCENE_RESULT;
	m_enResultSceneMode = enRSM_SILENCE;

	//宝箱の画面上の座標を宝石の出現位置にする.
	m_smpUiManagar->SetJewelAppPos(
		ConvDimPos(
		m_smpUiManagar->GetJewelAppPos(),
		m_vsmpGoalMgr[0]->GetBoxPos()));

	//リザルト情報セット.
	UpDateJewelState();
	SetJewelState();

	m_smpUiManagar->ChangeResultInit();

	m_smpUiManagar->SetTimerStop(true);

	//BGM.
	m_iResultTimer = 0;
	m_pBgm[enBGM_MAIN]->Stop();
	//	m_pBgm[enBGM_RESULT]->SeekToStart();
	//	m_pBgm[enBGM_RESULT]->Play( true );
}
//============================================================
//	クリア用初期化.
//============================================================
void clsMain::InitEndroll()
{
	//シーン.
	m_enScene = enSCENE_ENDROLL;
	//UI.
	m_smpUiManagar->ChangeEndingInit();
	//BGM.
	m_pBgm[enBGM_RESULT]->Stop();
	m_pBgm[enBGM_MAIN]->Stop();
	m_pBgm[enBGM_ENDROLL]->SeekToStart();
	m_pBgm[enBGM_ENDROLL]->Play(true);
}

//============================================================
//	ｶﾒﾗ.
//============================================================
void clsMain::CameraMgr()
{
	if (m_pPlayer != NULL){
		CameraTargetSet();
		Stoker(m_Camera.vEye, m_vCameraTarget);
		CameraLookSet();
	}
}
void clsMain::CameraTargetSet()
{
	float CamTarStopPointN, CamTarStopPointS;
	CamTarStopPointN = 7.0f;
	CamTarStopPointS = 15.0f;
	const float fCAM_SOUTH_OFFSET = CamTarStopPointS - CamTarStopPointN;//北向きに比べて南へはどれだけ下がるか.
	const float fCAMERA_OFFSET_MOVE_Y = 0.05f; //前後切り替え時のカメラ移動量.

	const float fCAMERA_OFFSET_MOVE_BACK_LIMIT = 0.125f; //一番手前時のカメラ移動量.

	//基本位置.
	m_vCameraTarget.z = m_pPlayer->GetPositionZ() - CamTarStopPointN;
	//向きによるオフセット.
	m_vCameraTarget.z += m_fLookOffset;

	//一番手前.
	if (m_pPlayer->GetPositionZ() <= CAMERA_NOT_BACK)
	{
		m_fLookOffset += fCAMERA_OFFSET_MOVE_BACK_LIMIT;
		if (m_fLookOffset > 0){
			m_fLookOffset = 0.0f;
		}
	}
	//北向き.
	else if (m_pPlayer->GetDirNorth())
	{
		m_fLookOffset += fCAMERA_OFFSET_MOVE_Y;
		if (m_fLookOffset > 0){
			m_fLookOffset = 0.0f;
		}
	}
	//南向き.
	else{
		m_fLookOffset -= fCAMERA_OFFSET_MOVE_Y;
		if (m_fLookOffset < -fCAM_SOUTH_OFFSET){
			m_fLookOffset = -fCAM_SOUTH_OFFSET;
		}
	}

	//	//ステージの一番手前では寄る.
	//	if( m_pPlayer->GetPositionZ() <= CAMERA_NOT_BACK ){
	//		m_vCameraTarget.z += CamTarMovSpdN;
	//		if( m_vCameraTarget.z > player_z - CamTarStopPointN ){
	//			m_vCameraTarget.z = player_z - CamTarStopPointN;
	//		}
	//	}
}
bool clsMain::Stoker(D3DXVECTOR3& Pos, D3DXVECTOR3 TargetPos)
{
	//ｶﾒﾗがﾀｰｹﾞｯﾄを追いかける.
	const float CamMoveStop = 0.0078125f;	//停止距離.
	const float CamMoveStep = 0.05f;	//小さくするとｶﾒﾗ移動が遅くなる.

	//たどり着いた?フラグ.
	bool bX, bY, bZ;
	bX = bY = bZ = false;

	//距離.
	float lCamMove = CameraLengthComp(Pos.x, TargetPos.x);
	//距離を計算してｶﾒﾗ追従.
	if (abs(lCamMove) > CamMoveStop){
		Pos.x += lCamMove * CamMoveStep;
	}
	else{
		bX = true;
	}
	lCamMove = CameraLengthComp(Pos.z, TargetPos.z);
	if (abs(lCamMove) > CamMoveStop){
		Pos.z += lCamMove * CamMoveStep;
	}
	else{
		bY = true;
	}
	lCamMove = CameraLengthComp(Pos.y, TargetPos.y);
	if (abs(lCamMove) > CamMoveStop){
		Pos.y += lCamMove * CamMoveStep;
	}
	else{
		bZ = true;
	}

	return bX & bY & bZ;
}

void clsMain::CameraLookSet()
{
	const float LOOK_JUMP_OFFSET_Y_RATE = 4.0f;

	//穴の上用処理.
	if (m_pPlayer->m_fFloorY >= 0.0f){
		m_fCameraLookFloorOld = m_fCameraLookFloor;
		if (m_fCameraLookFloorOld > 0.0f) m_fCameraLookFloorOld = 0.0f;
	}
	if (m_fCameraLookFloorOld < 0.0f){
		m_fCameraLookFloorOld = m_pPlayer->m_fFloorY;
	}

	//視点奥行.
	m_vLookTarget = m_Camera.vEye;
	m_vLookTarget.z += fLOOK_OFFSET_Z;

	//見る高さﾍﾞｰｽ.
	m_fCameraLookFloor = m_pPlayer->m_fFloorY;
	m_vLookTarget.y = m_fCameraLookFloorOld;

	//見る高さ　足元からの高さ.
	m_vLookTarget.y += fLOOK_OFFSET_Y;

	//ｼﾞｬﾝﾌﾟｱﾀｯｸで見上げる.
	if (m_pPlayer->isCamLookUp()){
		if (m_pPlayer->GetPositionY() > m_vLookTarget.y){
			float look_offset_y = m_pPlayer->GetPositionY()
				- m_vLookTarget.y;
			look_offset_y /= LOOK_JUMP_OFFSET_Y_RATE;
			m_vLookTarget.y += look_offset_y;
		}
	}

	m_Camera.vLook = m_vLookTarget;
}
float clsMain::CameraLengthComp(float Attker, float Target)
{
	float Langth = Target - Attker;

	return Langth;
}

//============================================================
//	一瞬で振り向かない(徐々に振り向く).
//============================================================
void clsMain::YawSpnToTarg(float& NowYaw, float TarYaw, float TurnSpd, float TurnStop)
{
	//3600問題解決.
	if (TarYaw - NowYaw > (float)M_PI){
		TarYaw -= (float)(M_PI * 2.0);
	}
	else if (TarYaw - NowYaw < (float)(-M_PI)){
		TarYaw += (float)(M_PI * 2.0);
	}

	//角度が近づく.
	if (abs(TarYaw - NowYaw) > TurnStop){
		if (NowYaw < TarYaw){
			NowYaw += TurnSpd;
		}
		else if (NowYaw > TarYaw){
			NowYaw -= TurnSpd;
		}
	}
}

//============================================================
//	360～0に納める.
//============================================================
void clsMain::ThetaOverGuard_M(float& theta)
{
	//		if( m_pOldPlayer->m_fYaw < 0.0f )m_pOldPlayer->m_fYaw += (float)( M_PI * 2.0 );
#define THETA_MAX (float)( M_PI * 2.0 )
	if (theta > THETA_MAX){
		theta -= THETA_MAX;
	}
	else if (theta < 0.0f){
		theta += THETA_MAX;
	}
}
void clsMain::ThetaOverGuard_M(double& theta)
{
#define THETA_MAX ( M_PI * 2.0 )
	if (theta > THETA_MAX){
		theta -= THETA_MAX;
	}
	else if (theta < 0.0){
		theta += THETA_MAX;
	}
}

void clsMain::CreateSound()
{
	//BGM.
	{
		//サウンド構造体.
		clsSound::SOUND_DATA tmpSData[enBGM_MAX] =
		{
			{ ALIAS_NAME_BGM_START_UP, FILE_PATH_BGM_START_UP, iVOL_BGM_START_UP },
			{ ALIAS_NAME_BGM_TITLE, FILE_PATH_BGM_TITLE, iVOL_BGM_TITLE },
			{ ALIAS_NAME_BGM_MAIN, FILE_PATH_BGM_MAIN, iVOL_BGM_MAIN },
			{ ALIAS_NAME_BGM_RESULT, FILE_PATH_BGM_RESULT, iVOL_BGM_RESULT },
			{ ALIAS_NAME_BGM_CLEAR, FILE_PATH_BGM_CLEAR, iVOL_BGM_CLEAR },
			{ ALIAS_NAME_BGM_OVER, FILE_PATH_BGM_OVER, iVOL_BGM_OVER },
		};
		for (int i = 0; i < enBGM_MAX; i++){
			m_pBgm[i] = new clsSound;
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s(cAliasName, sizeof(cAliasName), tmpSData[i].sAlias);
			//作成.
			m_pBgm[i]->Open(tmpSData[i].sPath, cAliasName, m_hWnd);
			//		//最大音量設定.
			//		m_pBgm[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
			//現音量初期化.
			m_pBgm[i]->SetVolume(tmpSData[i].iMaxVolume);
		}
	}

	//SE.
	{
		//サウンド構造体.
		clsSound::SOUND_DATA tmpSData[enSe_MAX] =
		{
			{ ALIAS_NAME_SE_ENTER, FILE_PATH_SE_ENTER, iVOL_SE_ENTER },
			{ ALIAS_NAME_SE_EXIT, FILE_PATH_SE_EXIT, iVOL_SE_EXIT },
		};
		for (int i = 0; i < enSe_MAX; i++){
			m_pSe[i] = new clsSound;
			//名前.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s(cAliasName, sizeof(cAliasName), tmpSData[i].sAlias);
			//作成.
			m_pSe[i]->Open(tmpSData[i].sPath, cAliasName, m_hWnd);
			//		//最大音量設定.
			//		m_pBgm[i]->SetMaxVolume( tmpSData[i].iMaxVolume );
			//現音量初期化.
			m_pSe[i]->SetVolume(tmpSData[i].iMaxVolume);
		}
	}
}

//============================================================
//	for文の処理の無駄を減らす.
//============================================================
//あたり判定.
bool clsMain::isCutOutForHit(float fPlayerZ, float fObjZ, float fBack, float fOpposite){
	float fOffsetBack = 17.0f;	//後(手前)にどれだけ判定するか.
	float fOffsetOpposite = fOffsetBack;	//前(奥)にどれだけ判定するか.
#if 0
	//指定距離以内なら.
	if (abs(fPlayerZ - fObjZ) <= fOffset){
		return true;
	}
#else
	if (fBack > 0.0f){
		fOffsetBack = fBack;
	}
	if (fOpposite > 0.0f){
		fOffsetOpposite = fOpposite;
	}

	if (-fOffsetBack < (fObjZ - fPlayerZ)){
		if ((fPlayerZ - fObjZ) < fOffsetOpposite){
			return true;
		}
	}
#endif
	return false;
}
//動き.
bool clsMain::isCutOutForMove(float fPlayerZ, float fObjZ, float fBack, float fOpposite){
	float fOffsetBack = 32.0f;	//後にどれだけ判定するか.
	float fOffsetOpposite = fRENDER_LIMIT;	//前にどれだけ判定するか.
	if (fBack){
		fOffsetBack = fBack;
	}
	if (fOpposite){
		fOffsetOpposite = fOpposite;
	}

	if (-fOffsetBack < (fObjZ - fPlayerZ)){
		if ((fObjZ - fPlayerZ) < fOffsetOpposite){
			return true;
		}
	}

	return false;
}

void clsMain::StageSet()
{
	StagePatarunSet tmp;
	if (!m_vStageDataPatarnSet.empty())
	{
		m_vStageDataPatarnSet.clear();
	}
	for (int i = 0; i < m_smpStageData->GetDataArrayNumMax(); i += STAGEPATARUNSETMAX)
	{
		m_smpStageData->GetDataArray( &tmp.iWall,		i );
		m_smpStageData->GetDataArray( &tmp.iFloor,		i + 1 );
		m_smpStageData->GetDataArray( &tmp.iEnemy,		i + 2 );
		m_smpStageData->GetDataArray( &tmp.iStepCil,	i + 3 );
		m_smpStageData->GetDataArray(&tmp.iStepBox, i + 4);
		m_smpStageData->GetDataArray(&tmp.bDoor, i + 5);
		m_smpStageData->GetDataArray(&tmp.iPend, i + 6);
		m_smpStageData->GetDataArray(&tmp.bSpiaFloor, i + 7);
		m_smpStageData->GetDataArray(&tmp.iSpiaWall, i + 8);
		m_smpStageData->GetDataArray(&tmp.bCover, i + 9);
		m_smpStageData->GetDataArray(&tmp.bGoal, i + 10);
		m_smpStageData->GetDataArray(&tmp.bTest, i + 11);

		m_vStageDataPatarnSet.push_back(tmp);
}
}

void clsMain::StageModelSet()
{
	//Csvのデータ読込.
	m_smpStageData->Read();

	//csvからのデータを構造体に.
	StageSet();

	//パーツの番号振り用.
	int StageWallNum = 0;
	int StageFloorNum = 0;
	int StageEnemyNum = 0;
	int StageMoveFloorViticulNum = 0;
	int StageMoveFloorSideNum = 0;
	int StageDoarNum = 0;
	int StagePendNum = 0;
	int StageSpearFloorNum = 0;
	int StageSpearWallNum = 0;
	int StageCoverNum = 0;
	int StageGoalNum = 0;
	int	CeilingNum = 0;
	int	StageTestStNum = 0;
	int	StageTestSkNum = 0;

#ifdef Tahara
	bool bCilBig = true;
#endif//#ifdef Tahara

	StageDetatchModelSet();

	//リソースステージ.
	m_iStageDataMax = m_smpStageData->GetDataArrayNumMax() / STAGEPATARUNSETMAX;
	m_iStageDataMaxMax = m_iStageDataMax;

	//ステージのﾎﾟｲﾝﾀの配列を作る.
	for (int StageNum = 0; StageNum < m_iStageDataMaxMax; StageNum++)
	{
		//規定のステージでのポジと大きさ.
		D3DXVECTOR3 SetPos(0.0f, 0.0f, 1.0f + (StageNum * 1.0f));
		float SetScale = 1.0f;

		//落とし穴の蓋の作成.
		if (m_vStageDataPatarnSet[StageNum].bCover)
		{
			m_vsmpCoverMgr.push_back(make_unique<clsCoverMgr>());
			m_vsmpCoverMgr[StageCoverNum]->CreateCover(m_hWnd, StageCoverNum);
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpCoverMgr[StageCoverNum]->SetPosition(SetPos);
			m_vsmpCoverMgr[StageCoverNum]->SetScale(SetScale);
			m_vsmpCoverMgr[StageCoverNum]->Init();
			StageCoverNum++;
		}

		//敵作成.
		if (m_vStageDataPatarnSet[StageNum].iEnemy > 0)
		{
			m_vsmpEnemyMgr.push_back(make_unique<clsEnemyMgr>());
			m_vsmpEnemyMgr[StageEnemyNum]->CreateEnemy(m_hWnd, m_pDevice, m_pDeviceContext,
				m_vStageDataPatarnSet[StageNum].iEnemy, StageEnemyNum);
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpEnemyMgr[StageEnemyNum]->SetPosition(SetPos);
			m_vsmpEnemyMgr[StageEnemyNum]->SetScale(SetScale);
			StageEnemyNum++;
		}

		//縦移動の丸床作成.
		if (m_vStageDataPatarnSet[StageNum].iStepCil != 0)
		{
			{
#ifdef Tahara
				//動きの初期化用.
				bool bInitCil = true;
				if (m_vStageDataPatarnSet[StageNum].iStepCil < 0){
					bInitCil = false;
					m_vStageDataPatarnSet[StageNum].iStepCil *= -1;//逆向きの証のマイナスはここで外す.
				}
#endif//#ifdef Tahara
				m_vsmpStepCil.push_back(make_unique<clsStepCyl>());
				m_vsmpStepCil[StageMoveFloorViticulNum]->SetPosition(SetPos);
#ifdef Tahara
				if (bCilBig){
					m_vsmpStepCil[StageMoveFloorViticulNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_STEP_CIL_BIG));
					bCilBig = false;
				}
				else{
					m_vsmpStepCil[StageMoveFloorViticulNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_STEP_CIL));
				}
#else
				m_vsmpStepCil[StageMoveFloorViticulNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_STEP_CIL));
#endif//#ifdef Tahara
				m_vsmpStepCil[StageMoveFloorViticulNum]->SetPositionX((float)m_vStageDataPatarnSet[StageNum].iStepCil - STAGE_WIDHT / 2.0f);
				m_vsmpStepCil[StageMoveFloorViticulNum]->SetScale(SetScale);
				m_vsmpStepCil[StageMoveFloorViticulNum]->CreateMove(bInitCil);
				StageMoveFloorViticulNum++;
			}
		}
		if (m_vStageDataPatarnSet[StageNum].iWall > 0)
		{
			m_vsmpWall.push_back(make_unique<clsWall>());
			int ResauseModelNum;
			ResauseModelNum = m_vStageDataPatarnSet[StageNum].iWall;
			ResauseModelNum = ResauseModelNum + iWallModelNumZero;
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け.
			m_vsmpWall[StageWallNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->ItoE(ResauseModelNum)));
			m_vsmpWall[StageWallNum]->SetPosition(SetPos);
			m_vsmpWall[StageWallNum]->SetScale(SetScale);
			StageWallNum++;
		}

		//床作成.
		if (m_vStageDataPatarnSet[StageNum].iFloor > 0)//csvから～.
		{
			m_vsmpFloor.push_back(make_unique<clsFloor>());
			int ResauseModelNum;
			ResauseModelNum = m_vStageDataPatarnSet[StageNum].iFloor;
			ResauseModelNum = ResauseModelNum + iFloorModelNumZero;
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付け.
			m_vsmpFloor[StageFloorNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->ItoE(ResauseModelNum)));
			m_vsmpFloor[StageFloorNum]->SetPosition(SetPos);
			m_vsmpFloor[StageFloorNum]->SetScale(SetScale);
			StageFloorNum++;
		}

		//横移動の四角床作成.
		if (m_vStageDataPatarnSet[StageNum].iStepBox > 0)
		{
			//右か左か、西か東か.
			bool bRight = false;
			bool bEast = false;
			//2より大きいならなら左.
			if (m_vStageDataPatarnSet[StageNum].iStepBox > 2){
				bRight = true;
			}
			//奇数なら.//東へ.
			if (m_vStageDataPatarnSet[StageNum].iStepBox % 2){
				bEast = true;
			}
			m_vsmpStepBox.push_back(make_unique<clsStepBox>());
			m_vsmpStepBox[StageMoveFloorSideNum]->SetPosition(SetPos);
			m_vsmpStepBox[StageMoveFloorSideNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_STEP_BOX));
			m_vsmpStepBox[StageMoveFloorSideNum]->CreateMove(bRight, bEast);
			m_vsmpStepBox[StageMoveFloorSideNum]->SetScale(SetScale);
			StageMoveFloorSideNum++;
		}

		//ドアの作成.
		if (m_vStageDataPatarnSet[StageNum].bDoor)
		{
			m_vsmpDoorMgr.push_back(make_unique<clsDoorMgr>());
			m_vsmpDoorMgr[StageDoarNum]->Create(m_hWnd, m_pDevice, m_pDeviceContext, StageDoarNum);
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpDoorMgr[StageDoarNum]->SetPosition(SetPos);
			m_vsmpDoorMgr[StageDoarNum]->SetScale(SetScale);
			StageDoarNum++;
		}

		//ペンデュラムの作成.
		if (m_vStageDataPatarnSet[StageNum].iPend > 0)
		{
			m_vsmpPend.push_back(make_unique<clsPend>());
			m_vsmpPend[StagePendNum]->Create(m_hWnd, StagePendNum, m_vStageDataPatarnSet[StageNum].iPend);
			m_vsmpPend[StagePendNum]->AttachModel(m_pResource->GetSkinModels(m_pResource->enSK_MODEL_PENDULUM));
			m_vsmpPend[StagePendNum]->Init();
			m_vsmpPend[StagePendNum]->SetPosition( SetPos );
			m_vsmpPend[StagePendNum]->SetPositionY( PENDPOSY );
			m_vsmpPend[StagePendNum]->SetScale(SetScale);
			StagePendNum++;
		}

		//槍床の作成.
		if (m_vStageDataPatarnSet[StageNum].bSpiaFloor)
		{
			m_vsmpSpiaFloorMgr.push_back(make_unique<clsSpiaFlorMgr>());
			m_vsmpSpiaFloorMgr[StageSpearFloorNum]->CreateSpia(m_hWnd, StageSpearFloorNum);
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpSpiaFloorMgr[StageSpearFloorNum]->SetPosition(SetPos);
			m_vsmpSpiaFloorMgr[StageSpearFloorNum]->SetScale(SetScale);
			StageSpearFloorNum++;
		}

		//槍壁の作成.
		if (m_vStageDataPatarnSet[StageNum].iSpiaWall)
		{
			m_vsmpSpiaWallMgr.push_back(make_unique<clsSpiaWallMgr>());
			m_vsmpSpiaWallMgr[StageSpearWallNum]->CreateSpia(m_hWnd, StageSpearWallNum, m_vStageDataPatarnSet[StageNum].iSpiaWall);
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpSpiaWallMgr[StageSpearWallNum]->SetPosition(SetPos);
			m_vsmpSpiaWallMgr[StageSpearWallNum]->SetScale(SetScale);
			StageSpearWallNum++;
		}

		//ゴールの作成.
		if (m_vStageDataPatarnSet[StageNum].bGoal)
		{
			m_vsmpGoalMgr.push_back(make_unique<clsGoalMgr>());
			//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
			m_vsmpGoalMgr[StageGoalNum]->Create(m_hWnd, m_pDevice, m_pDeviceContext);
			m_vsmpGoalMgr[StageGoalNum]->SetPosition(SetPos);
			m_vsmpGoalMgr[StageGoalNum]->SetScale(SetScale);
			StageGoalNum++;
		}

		//屋根の作成.
		m_vsmpCeiling.push_back(make_unique<clsCharaStatic>());
		//ﾓﾃﾞﾙﾃﾞｰﾀ関連付けはマネージャー側でやるのでフヨウラ.
		m_vsmpCeiling[CeilingNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_CEILING));
		m_vsmpCeiling[CeilingNum]->SetPosition(SetPos);
		m_vsmpCeiling[CeilingNum]->AddPositionY(7.0f);	//壁と合わせるため.
		m_vsmpCeiling[CeilingNum]->SetScale(SetScale);
		CeilingNum++;

		//テストモデル用.
		if (m_vStageDataPatarnSet[StageNum].bTest)
		{
			m_vsmpTest.push_back(make_unique<clsCharaStatic>());
			m_vsmpTest[StageTestStNum]->AttachModel(m_pResource->GetStaticModels(m_pResource->enST_MODEL_TEST));
			m_vsmpTest[StageTestStNum]->SetPosition(SetPos);
			m_vsmpTest[StageTestStNum]->SetScale(SetScale);
			StageTestStNum++;
		}
	}
}

void clsMain::StageDetatchModelSet()
{
	//壁紐づけ解除.
	if (!m_vsmpWall.empty())
	{
		for (int i = 0; i < m_vsmpWall.size(); i++)
		{
			m_vsmpWall[i]->DetatchModel();
		}
		m_vsmpWall.clear();
	}
	//床紐づけ解除.
	if (!m_vsmpFloor.empty())
	{
		for (int i = 0; i < m_vsmpFloor.size(); i++)
		{
			m_vsmpFloor[i]->DetatchModel();
		}
		m_vsmpFloor.clear();
	}
	//敵紐づけ解除.
	if (!m_vsmpEnemyMgr.empty())
	{
		for (int i = 0; i < m_vsmpEnemyMgr.size(); i++)
		{
			m_vsmpEnemyMgr[i]->DetatchModel();
		}
		m_vsmpEnemyMgr.clear();
	}
	//移動床丸紐づけ解除.
	if (!m_vsmpStepCil.empty())
	{
		for (int i = 0; i < m_vsmpStepCil.size(); i++)
		{
			m_vsmpStepCil[i]->DetatchModel();
		}
		m_vsmpStepCil.clear();
	}
	//移動床四角紐づけ解除.
	if (!m_vsmpStepBox.empty())
	{
		for (int i = 0; i < m_vsmpStepBox.size(); i++)
		{
			m_vsmpStepBox[i]->DetatchModel();
		}
		m_vsmpStepBox.clear();
	}
	//ペンデュラム紐づけ解除.
	if (!m_vsmpPend.empty())
	{
		for (int i = 0; i < m_vsmpPend.size(); i++)
		{
			m_vsmpPend[i]->DetatchModel();
		}
		m_vsmpPend.clear();
	}
	//ドア紐づけ解除.
	if (!m_vsmpDoorMgr.empty())
	{
		for (int i = 0; i < m_vsmpDoorMgr.size(); i++)
		{
			m_vsmpDoorMgr[i]->DetatchModel();
		}
		m_vsmpDoorMgr.clear();
	}
	//床槍紐づけ解除.
	if (!m_vsmpSpiaFloorMgr.empty())
	{
		for (int i = 0; i < m_vsmpSpiaFloorMgr.size(); i++)
		{
			m_vsmpSpiaFloorMgr[i]->DetatchModel();
		}
		m_vsmpSpiaFloorMgr.clear();
	}
	//壁槍紐づけ解除.
	if (!m_vsmpSpiaWallMgr.empty())
	{
		for (int i = 0; i < m_vsmpSpiaWallMgr.size(); i++)
		{
			m_vsmpSpiaWallMgr[i]->DetatchModel();
		}
		m_vsmpSpiaWallMgr.clear();
	}
	//落とし穴の蓋紐づけ解除.
	if (!m_vsmpCoverMgr.empty())
	{
		for (int i = 0; i < m_vsmpCoverMgr.size(); i++)
		{
			m_vsmpCoverMgr[i]->DetatchModel();
		}
		m_vsmpCoverMgr.clear();
	}
	//ゴール紐づけ解除.
	if (!m_vsmpGoalMgr.empty())
	{
		for (int i = 0; i < m_vsmpGoalMgr.size(); i++)
		{
			m_vsmpGoalMgr[i]->DetatchModel();
		}
		m_vsmpGoalMgr.clear();
	}

	//屋根紐づけ解除.
	if (!m_vsmpCeiling.empty())
	{
		for (int i = 0; i < m_vsmpCeiling.size(); i++)
		{
			m_vsmpCeiling[i]->DetatchModel();
		}
		m_vsmpCeiling.clear();
	}

	//モデルテスト紐づけ解除.
	if (!m_vsmpTest.empty())
	{
		for (int i = 0; i < m_vsmpTest.size(); i++)
		{
			m_vsmpTest[i]->DetatchModel();
		}
		m_vsmpTest.clear();
	}
}

void clsMain::StageModelSetErrMsg(int StageNum, const char* ERR_MSG)
{
	string ErrMsg;
	stringstream ss;	//変換用.
	ErrMsg = ERR_MSG;
	ErrMsg += "\n";
	ss << StageNum + 1;
	ErrMsg += ss.str();
	ErrMsg += "行目";
	ERR_MSG(ErrMsg.c_str(), "Csvの配置エラー");
	ss.str(""); // バッファをクリアする.
	ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない.
}

void clsMain::SceneTitle()
{
	//カメラの最終地点.
	const D3DXVECTOR3 vCAM_GO_MAIN_TARGET = m_pPlayer->GetPosition() + vCAMERA_INIT_OFFSET;
	const float fLOOK_GO_MAIN_TARGET_Y = 3.0625f;
	const D3DXVECTOR3 vLOOK_GO_MAIN_TARGET = { vCAM_GO_MAIN_TARGET.x,
		fLOOK_GO_MAIN_TARGET_Y,
		vCAM_GO_MAIN_TARGET.z + fLOOK_OFFSET_Z };

	//ループ.
	if (m_pBgm[enBGM_TITLE]->IsStopped()){
		m_pBgm[enBGM_TITLE]->SeekToStart();
		m_pBgm[enBGM_TITLE]->Play(true);
	}

	//	//テスト.
	//		if( GetAsyncKeyState( 'Y' ) & 0x8000 ) m_Camera.vLook.y += 0.05f;
	//		if( GetAsyncKeyState( 'H' ) & 0x8000 ) m_Camera.vLook.y -= 0.05f;
	//		if( GetAsyncKeyState( 'J' ) & 0x8000 ) m_Camera.vLook.x += 0.05f;
	//		if( GetAsyncKeyState( 'G' ) & 0x8000 ) m_Camera.vLook.x -= 0.05f;
	//		if( GetAsyncKeyState( 'U' ) & 0x8000 ) m_Camera.vLook.z += 0.05f;
	//		if( GetAsyncKeyState( 'T' ) & 0x8000 ) m_Camera.vLook.z -= 0.05f;
	//
	//		if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_Camera.vEye.y += 0.05f;
	//		if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_Camera.vEye.y -= 0.05f;
	//		if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_Camera.vEye.x += 0.05f;
	//		if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_Camera.vEye.x -= 0.05f;
	//		if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_Camera.vEye.z += 0.05f;
	//		if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_Camera.vEye.z -= 0.05f;
	//		if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_Camera.vLook.y += 0.05f;
	//		if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_Camera.vLook.y -= 0.05f;
	//		if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_Camera.vLook.x += 0.05f;
	//		if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_Camera.vLook.x -= 0.05f;
	//		if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_Camera.vLook.z += 0.05f;
	//		if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_Camera.vLook.z -= 0.05f;

	bool bPushEnter = false;
	if (m_pXInput->IsPressEnter(XINPUT_GAMEPAD_A))bPushEnter = true;
	GETKEY_DOWN('Z') bPushEnter = true;
	GETKEY_DOWN('A') bPushEnter = true;

	//モード.
	switch (m_enTitleSceneMode)
	{
		case en_TSM_IDLE:
			//暗転中ではない.
			if (m_smpUiManagar->GetBlackIdle()){
				//破壊開始.
				if (bPushEnter){
					m_enTitleSceneMode = en_TSM_BREAK_DOOR;
					m_pSe[enSe_EXIT]->SeekPlay();
				}
			}
			break;
		case en_TSM_BREAK_DOOR:
		{
			//カメラ追いかける.
			if (Stoker(m_Camera.vEye, vCAM_GO_MAIN_TARGET) &&
				Stoker(m_Camera.vLook, vLOOK_GO_MAIN_TARGET))
			{
				//追いかけ終わったら.
				m_enTitleSceneMode = en_TSM_GO_MAIN;
			}
			//タイトルの動きをする.
			m_pPlayer->MoveTitleScene();

			//ドア蹴破り.
			if (m_pPlayer->GetAttackImpact()){
				//壊れてないとき.
				if (!m_vsmpDoorMgr[0]->GetDoorBroken()){
					//タイトル透過.
					m_smpUiManagar->SetTitleTransFlg(true);
					//キックの範囲.
					if (m_pCollision->HitSect(
						m_pPlayer->GetPointerSubCol(),
						m_vsmpDoorMgr[0]->GetDoorHitColPointer()))
					{
						//メインの上のテキストを下に落とす.
						m_smpUiManagar->TopMainTxtMoveStart();
						//ドアの破壊と復活位置更新.
						m_pPlayer->UpdateReSpawnPos(m_vsmpDoorMgr[0]->DoorBreak());
						//振動.
						m_pXInput->SetVibPowerL(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
						m_pXInput->SetVibPowerR(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
						break;
					}
				}
			}
			//床.
			bool bGroundFlg = false;
			m_vsmpDoorMgr[0]->Move(m_pPlayer->GetPositionZ());
			for (int i = 0; i < m_vsmpFloor.size(); i++){
				if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpFloor[i]->GetPositionZ())){
					if (m_pCollision->FloorJudge(
						m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
						m_vsmpFloor[i].get(),
						bGroundFlg))
					{
						//breakしないと別の床の情報に上書きされる.
						break;
					}
				}
			}
			m_pPlayer->SetGroundFlg(bGroundFlg);
		}
		break;
		case en_TSM_GO_MAIN:
			//メインへ.
			InitMain(true);
			m_enTitleSceneMode = en_TSM_IDLE;
			break;
		default:
			m_enTitleSceneMode = en_TSM_GO_MAIN;
			break;
	}
}

void clsMain::SceneMain()
{
	//ループ.
	if (m_pBgm[enBGM_MAIN]->IsStopped()){
		m_pBgm[enBGM_MAIN]->SeekToStart();
		m_pBgm[enBGM_MAIN]->Play(true);
	}

	//----- 自機 -----//.
	m_pPlayer->Input(m_pXInput);

	m_pPlayer->Move(GetEar());

	//手前ガード.
	if (m_pPlayer->GetPositionZ() < fPLAYER_BACK_LIMIT){
		m_pPlayer->SetPositionZ(fPLAYER_BACK_LIMIT);
	}
	//壁.
	if (m_pPlayer->GetPositionX() > fWALL_HIT_RANGE){
		m_pPlayer->SetPositionX(fWALL_HIT_RANGE);
	}
	else if (m_pPlayer->GetPositionX() < -fWALL_HIT_RANGE){
		m_pPlayer->SetPositionX(-fWALL_HIT_RANGE);
	}

	//----- ｶﾒﾗ -----//.
	CameraMgr();

	//----- 耳の位置 -----//.
	SetEar(m_pPlayer->GetPositionZ(), m_Camera.vEye.z);

	//----- 敵 -----//.
	for (int i = 0; i < m_vsmpEnemyMgr.size(); i++){
		//動く.
		m_vsmpEnemyMgr[i]->Move(GetEar());

		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpEnemyMgr[i]->GetPositionZ())){
			//ﾌﾟﾚｲﾔｰのPosをﾀｰｹﾞｯﾄにｾｯﾄ.
			m_vsmpEnemyMgr[i]->GetSarchTarget(m_pPlayer->GetPosition());

			//敵のあたり判定.
			for (int j = 0; j < m_vsmpEnemyMgr[i]->GetEnemyMax(); j++)
			{
				//敵がやられていたら判定しない.
				if (m_vsmpEnemyMgr[i]->GetEnemyMove(j) != clsEnemy::enEM_DEATH){
					//自機が生きてるとき.
					if (!m_pPlayer->GetDead()){
						//===== いつでも動いているあたり判定 =====//.
						//自機から敵への攻撃.
						//攻撃中.
						if (m_pPlayer->GetAttackImpact()){
							//かかと落とし中.
							if (m_pPlayer->isJumpAtk()){
								//降下中.
								if (m_pPlayer->GetJumpAtkImpact()){
									//キックの範囲.
									if (m_pCollision->HitCylind(
										m_pPlayer->GetPointerSubCol(),
										m_vsmpEnemyMgr[i]->GetPointerEnemyCol(j)))
									{
										m_vsmpEnemyMgr[i]->Dead(j);
										m_pCollision->PlaySe(clsCollision::enSOUND_ARBIA_KICK);
										m_pXInput->SetVibPowerR(usVIB_KICK_POWER, iVIB_KICK_TIME);
										break;
									}
								}
							}
							//通常キック.
							else{
								//キックの範囲.
								if (m_pCollision->HitSect(
									m_pPlayer->GetPointerSubCol(),
									m_vsmpEnemyMgr[i]->GetPointerEnemyCol(j)))
								{
									m_vsmpEnemyMgr[i]->Dead(j);
									m_pCollision->PlaySe(clsCollision::enSOUND_ARBIA_KICK);
									m_pXInput->SetVibPowerR(usVIB_KICK_POWER, iVIB_KICK_TIME);
									break;
								}
							}
							//敵に接触で敵が死ぬ.
							if (m_pCollision->HitCylind(
								m_pPlayer->GetPointerCol(),
								m_vsmpEnemyMgr[i]->GetPointerEnemyCol(j)))
							{
								m_vsmpEnemyMgr[i]->Dead(j);
								m_pCollision->PlaySe(clsCollision::enSOUND_ARBIA_KICK);
								m_pXInput->SetVibPowerR(usVIB_KICK_POWER, iVIB_KICK_TIME);
								break;
							}
						}
						//自機が攻撃中以外.
						else{
							//敵に接触で自機が死ぬ.
							if (m_pCollision->HitCylind(
								m_pPlayer->GetPointerCol(),
								m_vsmpEnemyMgr[i]->GetPointerEnemyCol(j)))
							{
								m_pPlayer->Dead();
								m_vsmpEnemyMgr[i]->Lost(j);
								m_pCollision->PlaySe(clsCollision::enSOUND_ENEMY);
								break;
							}
						}

						//===== モード別あたり判定 =====//.
						switch (m_vsmpEnemyMgr[i]->GetEnemyMove(j))
						{
							case clsEnemy::enEM_WALK:
								//自機発見.
								//範囲内.
								if (m_pCollision->HitSect(
									m_vsmpEnemyMgr[i]->GetPointerEnemySubCol(j),
									m_pPlayer->GetPointerCol()))
								{
									//敵が見つけれる状態で見つかった時.
									if (m_vsmpEnemyMgr[i]->Discover(j)){
										//発見情報更新.
										m_ResUiState.iDisc++;
										m_smpUiManagar->SetDisc(m_ResUiState.iDisc);
									}
								}
								break;
							case clsEnemy::enEM_RUN:
								//攻撃開始範囲.
								if (m_pCollision->HitSect(
									m_vsmpEnemyMgr[i]->GetPointerEnemySubCol(j),
									m_pPlayer->GetPointerCol()))
								{
									m_vsmpEnemyMgr[i]->Attack(j);
								}
								break;
							case clsEnemy::enEM_ATK:
								//攻撃のインパクト(判定のある時間か?).
								if (m_vsmpEnemyMgr[i]->GetAttackImpact(j)){
									//攻撃範囲.
									if (m_pCollision->HitSect(
										m_vsmpEnemyMgr[i]->GetPointerEnemySubCol(j),
										m_pPlayer->GetPointerCol()))
									{
										m_pPlayer->Dead();
										m_vsmpEnemyMgr[i]->Win(j);
										m_pCollision->PlaySe(clsCollision::enSOUND_ENEMY_SRASH);
									}
								}
								break;
						}
					}
				}
			}
		}
	}
	//----- 敵 終わり -----//.

	//----- 床槍 -----//.
	for (int i = 0; i < m_vsmpSpiaFloorMgr.size(); i++){
		//動き.
		m_vsmpSpiaFloorMgr[i]->Move(GetEar());

		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpSpiaFloorMgr[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
			//生きてるとき.
			if (!m_pPlayer->GetDead()){
				//槍とのあたり判定.
				for (int j = 0; j < m_vsmpSpiaFloorMgr[i]->GetSpiaMax(); j++)
				{
					if (m_pCollision->HitCylind(
						m_vsmpSpiaFloorMgr[i]->GetPointerSpiaCol(j),
						m_pPlayer->GetPointerCol()))
					{
						m_pPlayer->Dead();
						m_pCollision->PlaySe(clsCollision::enSOUND_SPIA);
						break;
					}
				}
			}
			//自機と槍壁のあたり判定.
			m_pCollision->WallJudge(m_pPlayer, m_vsmpSpiaFloorMgr[i]->GetWallPointer());
		}
	}

	//----- 壁槍 -----//.
	for (int i = 0; i < m_vsmpSpiaWallMgr.size(); i++){
		//動き.
		m_vsmpSpiaWallMgr[i]->Move(GetEar());

		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpSpiaWallMgr[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
			//生きてるとき.
			if (!m_pPlayer->GetDead()){
				//槍とのあたり判定.
				for (int j = 0; j < m_vsmpSpiaWallMgr[i]->GetSpiaMax(); j++)
				{
					//左側.
					if (m_pCollision->HitBox(
						m_vsmpSpiaWallMgr[i]->GetPointerSpiaCol(false, j),
						m_pPlayer->GetPointerCol()))
					{
						m_pPlayer->Dead();
						m_pCollision->PlaySe(clsCollision::enSOUND_SPIA);
						break;
					}
					//右側.
					if (m_pCollision->HitBox(
						m_vsmpSpiaWallMgr[i]->GetPointerSpiaCol(true, j),
						m_pPlayer->GetPointerCol()))
					{
						m_pPlayer->Dead();
						m_pCollision->PlaySe(clsCollision::enSOUND_SPIA);
						break;
					}
				}
			}

			//壁.
			m_pCollision->WallJudge(m_pPlayer, m_vsmpSpiaWallMgr[i]->GetWallLCol());
			m_pCollision->WallJudge(m_pPlayer, m_vsmpSpiaWallMgr[i]->GetWallRCol());
		}
	}

	//----- 振り子 -----//.
	char* cPendBoneName = (char*)PEND_BONE_NAME;//ﾎﾞｰﾝ名.
	for (int i = 0; i < m_vsmpPend.size(); i++){
		//振り子動き.
		m_vsmpPend[i]->Move(GetEar());
		//透過基準設定.
		m_vsmpPend[i]->SetAlphaFlg(m_pPlayer->GetPositionZ());

		//振り子のあたり判定.
		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpPend[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
			//座標ｾｯﾄはﾚﾝﾀﾞｰでやっている（1ﾌﾚｰﾑ遅れるけど許してね）.
			if (!m_pPlayer->GetDead()){
				//ﾋｯﾄ.
				if (m_pCollision->HitBox(
					m_vsmpPend[i]->GetPointerCol(),
					m_pPlayer->GetPointerCol()))
				{
					m_pPlayer->Dead();
					m_pCollision->PlaySe(clsCollision::enSOUND_PEND);
					break;
				}
			}
		}
	}

	//========== 地形とのあたり判定 ==========//.

	//	//----- 自機と壁の壁あたり判定 -----//.
	//	for( int i=0; i<m_vpWall.size(); i++ ){
	//		if( isCutOutForHit( m_pPlayer->GetPositionZ(), m_vpWall[i]->GetPositionZ() ) ){
	//			m_pCollision->WallJudge( m_pPlayer, m_vpWall[i] );
	//		}
	//	}
	//----- 自機と床との壁あたり判定 -----//.
	for (int i = 0; i < m_vsmpFloor.size(); i++){
		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpFloor[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
			m_pCollision->WallJudge(m_pPlayer, m_vsmpFloor[i].get());
		}
	}
	//----- 自機と丸足場の壁あたり判定 -----//.
	for (int i = 0; i < m_vsmpStepCil.size(); i++){
		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpStepCil[i]->GetPositionZ())){
			m_pCollision->WallJudge(m_pPlayer, m_vsmpStepCil[i].get());
		}
	}
	//----- 自機と四角足場の壁あたり判定 -----//.
	for (int i = 0; i < m_vsmpStepBox.size(); i++){
		if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpStepBox[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
			m_pCollision->WallJudge(m_pPlayer, m_vsmpStepBox[i].get());
		}
	}
	//----- ゴールの壁あたり判定と動作 -----//.
	for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
		if ( /*isCutOutForHit( m_pPlayer->GetPositionZ(), m_vsmpGoalMgr[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fGOAL_TREASURE_HIT_AREA )*/ 1){
			m_pCollision->WallJudge(m_pPlayer, m_vsmpGoalMgr[i]->GetFloorColPointer());
			m_pCollision->WallJudge(m_pPlayer, m_vsmpGoalMgr[i]->GetTrBoxColPointer());
			//蹴破られ.
			if (m_pPlayer->GetAttackImpact()){
				//壊れてないとき.
				if (!m_vsmpGoalMgr[i]->GetBoxOpened()){
					//キックの範囲.
					if (m_pCollision->HitSect(
						m_pPlayer->GetPointerSubCol(),
						m_vsmpGoalMgr[i]->GetBoxHitColPointer()))
					{
						//箱を開ける.
						m_vsmpGoalMgr[i]->BoxBreak();

						m_pXInput->SetVibPowerL(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
						m_pXInput->SetVibPowerR(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
						break;
					}
				}
			}
			m_vsmpGoalMgr[i]->Move(GetEar());
		}
	}

	//----- 自機と門との壁あたり判定と動作 -----//.
	for (int i = 0; i < m_vsmpDoorMgr.size(); i++){
		//壊れてないとき.
		if (!m_vsmpDoorMgr[i]->GetDoorBroken()){
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpDoorMgr[i]->GetPositionZ())){
				//扉の壁判定.
				m_pCollision->WallJudge(m_pPlayer, m_vsmpDoorMgr[i]->GetWallColPointer());
				//蹴破られ.
				if (m_pPlayer->GetAttackImpact()){
					//かかと落とし中.
					if (m_pPlayer->isJumpAtk()){
						//降下中.
						if (m_pPlayer->GetJumpAtkImpact()){
							//キックの範囲.
							if (m_pCollision->HitSect(
								m_pPlayer->GetPointerSubCol(),
								m_vsmpDoorMgr[i]->GetDoorHitColPointer()))
							{
								//ドアの破壊と復活位置更新.
								m_pPlayer->UpdateReSpawnPos(m_vsmpDoorMgr[i]->DoorBreak());
								//ドアを破って兵士を倒す.
								for (int j = 0; j < m_vsmpEnemyMgr.size(); j++){
									//ドアで死ぬ人.
									if (m_vsmpEnemyMgr[j]->GetDoorDeader()){
										if (isCutOutForHit(m_vsmpEnemyMgr[j]->GetPositionZ(), m_vsmpDoorMgr[i]->GetPositionZ())){
											m_vsmpEnemyMgr[j]->DoorTimerStart();
											m_pCollision->PlaySe(clsCollision::enSOUND_ARBIA_KICK);
										}
									}
								}
								m_pXInput->SetVibPowerL(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
								m_pXInput->SetVibPowerR(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
								break;
							}
						}
					}
					//通常攻撃.
					else{
						//キックの範囲.
						if (m_pCollision->HitSect(
							m_pPlayer->GetPointerSubCol(),
							m_vsmpDoorMgr[i]->GetDoorHitColPointer()))
						{
							//ドアの破壊と復活位置更新.
							m_pPlayer->UpdateReSpawnPos(m_vsmpDoorMgr[i]->DoorBreak());
							//ドアを破って兵士を倒す.
							for (int j = 0; j < m_vsmpEnemyMgr.size(); j++){
								//ドアで死ぬ人.
								if (m_vsmpEnemyMgr[j]->GetDoorDeader()){
									if (isCutOutForHit(m_vsmpEnemyMgr[j]->GetPositionZ(), m_vsmpDoorMgr[i]->GetPositionZ())){
										m_vsmpEnemyMgr[j]->DoorTimerStart();
										m_pCollision->PlaySe(clsCollision::enSOUND_ARBIA_KICK);
									}
								}
							}
							m_pXInput->SetVibPowerL(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
							m_pXInput->SetVibPowerR(usVIB_DOOR_POWER, iVIB_DOOR_TIME);
							break;
						}
					}
				}
			}
		}
		//壊れている時.
		else{
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpDoorMgr[i]->GetPositionZ())){
				//最初の門をくぐるとタイマーが動き出す.
				if (!i){
					if (m_pPlayer->GetPositionZ() > m_vsmpDoorMgr[i]->GetPositionZ()){
						m_smpUiManagar->SetTimerStop(false);
					}
				}
				//二つめの門をくぐるとアクション説明が引っ込む.
				else if (i == 1){
					if (m_pPlayer->GetPositionZ() > m_vsmpDoorMgr[i]->GetPositionZ()){
						m_smpUiManagar->ActTxtOut();
					}
				}

				//壁判定.
				m_pCollision->WallJudge(m_pPlayer, m_vsmpDoorMgr[i]->GetGateColPointer());
				//天井に頭を打つとプレイヤーの上下移動量反転.
				if (m_pCollision->CeilingJudge(
					m_pPlayer,
					m_pPlayer->GetHeight(),
					m_vsmpDoorMgr[i]->GetGateColPointer()))
				{
					m_pPlayer->HeadHitToCeiling();
				}
			}
			//透過.
			m_vsmpDoorMgr[i]->SetAlphaFlg(GetEar());
			//動作.
			m_vsmpDoorMgr[i]->Move(GetEar());
		}
	}

	//========== 自機と床のあたり判定 ==========//.
	m_pPlayer->m_fFloorY = 0.0f;//地面の高さ初期化.
	bool bGroundFlg = false;
	bool bCutGroundCheck = false;	//地面の上にいるならそれ以上のチェックはいらないので、そのためのフラグ.
	m_pPlayer->SetGroundFlg(bGroundFlg);

	//----- 落とし穴の蓋 -----//.//落とし穴は最初に判定しないとダメ.
	//宙に浮いてるとき.
	if (!bCutGroundCheck){
		//落とし穴の数分回す.
		for (int i = 0; i < m_vsmpCoverMgr.size(); i++){
			//動作.
			m_vsmpCoverMgr[i]->Move(GetEar());
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpCoverMgr[i]->GetPositionZ())){
				//開いている時は乗れない.
				if (m_vsmpCoverMgr[i]->isCanStmp())
				{
					for (char j = 0; j < m_vsmpCoverMgr[i]->GetCoverMax(); j++){
						if (m_pCollision->FloorJudge(
							m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
							m_vsmpCoverMgr[i]->GetCoverPointer(j),
							bGroundFlg))
						{
							//着地時のみ.
							if (bGroundFlg){
								//かかと落としで乗る.
								if (m_pPlayer->isJumpAtk()){
									//急に開く.
									m_vsmpCoverMgr[i]->StmpCovorStrong(GetEar());
								}
								//普通に乗る.
								else{
									//ぎしぎし.
									m_vsmpCoverMgr[i]->StmpCovor(GetEar());
									m_pXInput->SetVibPowerL(usVIB_COVER_POWER, iVIB_COVER_TIME, iVIB_COVER_DEC);
								}
							}
							bCutGroundCheck = true;
							//2重for文の脱出用（これがあるのでbreakはいらない｛ 他では普通にbreakしている理由は、可読性がいいからです ｝）.
							i = m_vsmpCoverMgr.size() - 1;
							j = m_vsmpCoverMgr[i]->GetCoverMax() - 1;
						}
					}
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//----- 普通床 -----//.
	if (!bCutGroundCheck){
		for (int i = 0; i < m_vsmpFloor.size(); i++){
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpFloor[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpFloor[i].get(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//breakしないと別の床の情報に上書きされる.
					break;
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//----- 動く丸足場 -----//.
	for (int i = 0; i < m_vsmpStepCil.size(); i++){
		m_vsmpStepCil[i]->Move();	//こいつは外に出しておかないとbreakのせいで乗っている足場以外が止まってしまう.
	}
	if (!bCutGroundCheck){
		for (int i = 0; i < m_vsmpStepCil.size(); i++){
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpStepCil[i]->GetPositionZ())){
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpStepCil[i].get(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//乗っていたら運ぶ.
					if (bGroundFlg){
						//足場の移動量を更新.
						m_pPlayer->AddPositionZ(m_vsmpStepCil[i]->GetCarryRangeZ());
					}
					//breakしないと別の床の情報に上書きされる.
					break;
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//----- 動く四角足場 -----//.
	for (int i = 0; i < m_vsmpStepBox.size(); i++){
		m_vsmpStepBox[i]->Move();	//こいつは外に出しておかないとbreakのせいで乗っている足場以外が止まってしまう.
	}
	if (!bCutGroundCheck){
		for (int i = 0; i < m_vsmpStepBox.size(); i++){
			if (isCutOutForHit(m_pPlayer->GetPositionZ(), m_vsmpStepBox[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fONE_BROCK_HIT_AREA)){
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpStepBox[i].get(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//乗っていたら運ぶ.
					if (bGroundFlg){
						//足場の移動量を更新.
						m_pPlayer->AddPositionX(m_vsmpStepBox[i]->GetCarryRangeX());
					}
					//breakしないと別の床の情報に上書きされる.
					break;
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//----- ゴール床 -----//.
	if (!bCutGroundCheck){
		for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
			if ( /*isCutOutForHit( m_pPlayer->GetPositionZ(), m_vsmpGoalMgr[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fGOAL_TREASURE_HIT_AREA )*/ 1){
				//宝箱.
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpGoalMgr[i]->GetTrBoxColPointer(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//breakしないと別の床の情報に上書きされる.
					break;
				}
				//床判定.
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpGoalMgr[i]->GetFloorColPointer(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//breakしないと別の床の情報に上書きされる.
					break;
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//死んだら戻ってくる.
	if (m_pPlayer->GetDead()){
		//タイマーとめる.
		m_smpUiManagar->SetTimerStop(true);
		m_iRespawnTimer++;
		if (m_iRespawnTimer > iDEAD_TIME){
			//シーン初期化(リスポーン).
			if (m_pPlayer->GetHp() > 0){
				if (m_smpUiManagar->GetBlackIdle()){
					//暗転;
					m_smpUiManagar->BlackStart(fBLACK_SPD_FAST);
				}
				else if (m_smpUiManagar->GetBlackEnd()){
					InitMain();
				}
			}
			//ゲームオーバー.
			else{
				if (m_smpUiManagar->GetBlackIdle()){
					//暗転;
					m_smpUiManagar->BlackStart(fBLACK_SPD_SLOW);
				}
				else if (m_smpUiManagar->GetBlackEnd()){
					InitOver();
				}
			}
		}
	}
	//生きている時に.
	else{
		//穴に落ちたら.
		if (m_pPlayer->GetPositionY() < fDROP_DOWN_Y){
			m_pPlayer->Dead();
			//			m_pCollision->PlaySe( clsCollision::enSOUND_DROP_DOWN );
		}

		//ゴール.
		if (!m_vsmpGoalMgr.empty())
		{
			if (m_vsmpGoalMgr[0]->GetBoxOpened()){
				InitResult();
				//				//暗転開始.
				//				m_smpUiManagar->BlackStart( fBLACK_SPD_SLOW, false );
				//				//暗転終了.
				//				if( m_smpUiManagar->GetBlackEnd() ){
				//					InitResult();
				//				}
			}
		}
	}
}
void clsMain::SceneResult()
{
	const int iSILENCE_TIME = 30;

	switch (m_enResultSceneMode)
	{
		case enRSM_SILENCE://静かになる.
			//リザルトBGM進行.
			m_iResultTimer++;
			if (m_iResultTimer > iSILENCE_TIME){
				//ジングル再生.
				m_pBgm[enBGM_RESULT]->SeekToStart();
				m_pBgm[enBGM_RESULT]->Play(true);

				m_iResultTimer = 0;
				m_enResultSceneMode = enRSM_JINGLE;
			}
			break;

		case enRSM_JINGLE:
			//ジングルが止まったら.
			if (m_pBgm[enBGM_RESULT]->IsStopped()){
				//UIの用が終わっていたら.
				if (m_smpUiManagar->isCanBgmRePlay()){
					m_pBgm[enBGM_RESULT]->Stop();
					m_pBgm[enBGM_MAIN]->Play(true);
					m_enResultSceneMode = enRSM_LOOP;
				}
			}
			break;

		case enRSM_LOOP:
			//ループ.
			if (m_pBgm[enBGM_MAIN]->IsStopped()){
				m_pBgm[enBGM_MAIN]->SeekToStart();
				m_pBgm[enBGM_MAIN]->Play(true);
			}
			break;

		default:
			m_enResultSceneMode = enRSM_SILENCE;
			break;
	}

	m_pPlayer->Move(GetEar());

	//----- ｶﾒﾗ -----//.
	CameraMgr();

	//----- 耳の位置 -----//.
	SetEar(m_pPlayer->GetPositionZ(), m_Camera.vEye.z);

	//----- ゴールの壁あたり判定と動作 -----//.
	for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
		m_pCollision->WallJudge(m_pPlayer, m_vsmpGoalMgr[i]->GetFloorColPointer());
		m_pCollision->WallJudge(m_pPlayer, m_vsmpGoalMgr[i]->GetTrBoxColPointer());
		m_vsmpGoalMgr[i]->Move(GetEar());
	}
	//----- ゴール床 -----//.
	bool bGroundFlg = false;
	bool bCutGroundCheck = false;	//地面の上にいるならそれ以上のチェックはいらないので、そのためのフラグ.
	if (!bCutGroundCheck){
		for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
			if ( /*isCutOutForHit( m_pPlayer->GetPositionZ(), m_vsmpGoalMgr[i]->GetPositionZ(), fONE_BROCK_HIT_AREA, fGOAL_TREASURE_HIT_AREA )*/ 1){
				//宝箱.
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpGoalMgr[i]->GetTrBoxColPointer(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//breakしないと別の床の情報に上書きされる.
					break;
				}
				//床判定.
				if (m_pCollision->FloorJudge(
					m_pPlayer, m_pPlayer->GetColToFloorRange(), m_pPlayer->m_fFloorY,
					m_vsmpGoalMgr[i]->GetFloorColPointer(),
					bGroundFlg))
				{
					bCutGroundCheck = true;
					//breakしないと別の床の情報に上書きされる.
					break;
				}
			}
		}
		m_pPlayer->SetGroundFlg(bGroundFlg);
	}

	//宝箱の画面上の座標を宝石の出現位置にする.
	m_smpUiManagar->SetJewelAppPos(
		ConvDimPos(
		m_smpUiManagar->GetJewelAppPos(),
		m_vsmpGoalMgr[0]->GetBoxPos()));

	//決定ボタン.
	bool bPushEnter = false;
	if (m_pXInput->IsPressEnter(XINPUT_GAMEPAD_A))bPushEnter = true;
	GETKEY_DOWN('Z') bPushEnter = true;
	GETKEY_DOWN('A') bPushEnter = true;

	if (m_smpUiManagar->GetBlackIdle()){
		//タイトルへ.
		if (bPushEnter){
			//
			m_smpUiManagar->BlackStart(fBLACK_SPD_SLOW);
			m_pSe[enSe_EXIT]->SeekPlay();
		}
	}
	//暗転終了.
	else if (m_smpUiManagar->GetBlackEnd()){
		InitEndroll();
	}
}
void clsMain::SceneEnding()
{
	//ループ.
	if (m_pBgm[enBGM_ENDROLL]->IsStopped()){
		m_pBgm[enBGM_ENDROLL]->SeekToStart();
		m_pBgm[enBGM_ENDROLL]->Play(true);
	}

	//短押し.
	bool bPushEnterEnter = false;
	if (m_pXInput->IsPressEnter(XINPUT_GAMEPAD_A))bPushEnterEnter = true;
	GETKEY_DOWN('Z') bPushEnterEnter = true;
	GETKEY_DOWN('A') bPushEnterEnter = true;

	//長押し.
	bool bPushEnterStay = false;
	if (m_pXInput->IsPressStay(XINPUT_GAMEPAD_A))bPushEnterStay = true;
	GETKEY_STAY('Z') bPushEnterStay = true;
	GETKEY_STAY('A') bPushEnterStay = true;

	//暗転が動いていない.
	if (m_smpUiManagar->GetBlackIdle()){
		//加速.
		m_smpUiManagar->SetPushButtun(bPushEnterStay);
		//スタッフロールが終わったら.
		if (m_smpUiManagar->GetEndRollEnd()){
			//終了.
			if (bPushEnterEnter){
				//タイトルへ.
				m_smpUiManagar->BlackStart(fBLACK_SPD_SLOW, false);
				m_pSe[enSe_ENTER]->SeekPlay();
			}
		}
	}
	//暗転終了.
	else if (m_smpUiManagar->GetBlackEnd()){
		InitTitle();
	}
}
void clsMain::SceneOver()
{
	m_pPlayer->MoveOverScene();

	bool bPushEnter = false;
	if (m_pXInput->IsPressEnter(XINPUT_GAMEPAD_A))bPushEnter = true;
	GETKEY_DOWN('Z') bPushEnter = true;
	GETKEY_DOWN('A') bPushEnter = true;

	if (m_smpUiManagar->GetBlackIdle()){
		//タイトルへ.
		if (bPushEnter){
			//
			m_smpUiManagar->BlackStart(fBLACK_SPD_SLOW);
			m_pSe[enSe_ENTER]->SeekPlay();
		}
	}
	//暗転終了.
	else if (m_smpUiManagar->GetBlackEnd()){
		InitTitle();
	}
}

//==========
void clsMain::RenderTitle()
{
	//壁.
	if (!m_vsmpWall.empty()){
		for (int i = 0; i < m_vsmpWall.size(); i++){
			m_vsmpWall[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//床.
	if (!m_vsmpFloor.empty()){
		for (int i = 0; i < m_vsmpFloor.size(); i++){
			m_vsmpFloor[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}

	//プレイヤー(アルビア)表示.
	m_pPlayer->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);

	//ドア.
	if (!m_vsmpDoorMgr.empty()){
		for (int i = 0; i < m_vsmpDoorMgr.size(); i++){
			m_vsmpDoorMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
}
void clsMain::RenderMain()
{
#ifdef Inoue
	//壁.
	if (!m_vsmpWall.empty()){
		for (int i = 0; i < m_vsmpWall.size(); i++){
			m_vsmpWall[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//床.
	if (!m_vsmpFloor.empty()){
		for (int i = 0; i < m_vsmpFloor.size(); i++){
			m_vsmpFloor[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//足場丸型.
	if (!m_vsmpStepCil.empty()){
		for (int i = 0; i < m_vsmpStepCil.size(); i++){
			m_vsmpStepCil[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//足場四角型.
	if (!m_vsmpStepBox.empty()){
		for (int i = 0; i < m_vsmpStepBox.size(); i++){
			m_vsmpStepBox[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//落とし穴.
	if (!m_vsmpCoverMgr.empty()){
		for (int i = 0; i < m_vsmpCoverMgr.size(); i++){
			m_vsmpCoverMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//ゴール.
	if (!m_vsmpGoalMgr.empty()){
		for (int i = 0; i < m_vsmpGoalMgr.size(); i++){
			m_vsmpGoalMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}

#endif //#ifdef Inoue.

	//プレイヤー(アルビア)表示.
	m_pPlayer->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);

#ifdef Inoue
	//敵.
	if (!m_vsmpEnemyMgr.empty()){
		for (int i = 0; i < m_vsmpEnemyMgr.size(); i++){
			m_vsmpEnemyMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//床槍.
	if (!m_vsmpSpiaFloorMgr.empty()){
		for (int i = 0; i < m_vsmpSpiaFloorMgr.size(); i++){
			m_vsmpSpiaFloorMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//壁槍.
	if (!m_vsmpSpiaWallMgr.empty()){
		for (int i = 0; i < m_vsmpSpiaWallMgr.size(); i++){
			m_vsmpSpiaWallMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//屋根.
	if (!m_vsmpCeiling.empty()){
		for (int i = 0; i < m_vsmpCeiling.size(); i++)
		{
			m_vsmpCeiling[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
	//ペンデュラム(逆順なのは透過の都合).
	if (!m_vsmpPend.empty()){
		for (int i = m_vsmpPend.size() - 1; i >= 0; i--){
			m_vsmpPend[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
			m_vsmpPend[i]->GetBonePos(PEND_BONE_NAME);
		}
	}
	//ドア(逆順なのは透過の都合).
	if (!m_vsmpDoorMgr.empty()){
		for (int i = m_vsmpDoorMgr.size() - 1; i >= 0; i--){
			m_vsmpDoorMgr[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}

	if (!m_vsmpTest.empty()){
		for (int i = 0; i < m_vsmpTest.size(); i++){
			m_vsmpTest[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
		}
	}
#endif //#ifdef Inoue.

	//	//ﾊﾟｰﾃｨｸﾙの表示.
	//	if( m_pParticle->m_bDispFlg ){
	//		SetDepth( false );	//Zﾃｽﾄ:OFF.
	//		//※ﾊﾟｰﾃｨｸﾙ1粒をﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄ1枚として500枚描画.
	//		int particleMax = m_pParticle->GetMaxParticle();
	//		for( int i=0; i<particleMax; i++ ){
	//			//現在のi番目の粒の位置を取得.
	//			D3DXVECTOR3 vParticlePos = m_pParticle->GetParticlePos( i );
	//			//i番目の粒を表示.
	//			m_pParticle->Render( m_mView, m_mProj, m_Camera.vEye, vParticlePos );
	//		}
	//		//ﾊﾟｰﾃｨｸﾙの移動処理.
	//		m_pParticle->Run();
	//		SetDepth( true );	//Zﾃｽﾄ:ON.
	//	}

	////------------------------------
	////	Effekseerｸﾗｽのﾚﾝﾀﾞﾘﾝｸﾞ.
	////------------------------------
	//clsEffects* pEffects = clsEffects::GetInstance();
	//if( pEffects ){
	//	//確認用に変数を宣言.
	//	static ::Effekseer::Handle handle = -1;
	//	if( GetAsyncKeyState( 'R' ) & 0x1 ){
	//		for( int i=0; i<clsEffects::enEfcType_Max; i++ ){
	//			//ｴﾌｪｸﾄの再生.
	//			handle =
	//				pEffects->Play( (clsEffects::enEfcType)i ,
	//				D3DXVECTOR3( -2.0f + (float)(i*2), 1.0f, 5.0f ) );
	//			//回転.
	//			pEffects->SetRotation( handle,
	//				D3DXVECTOR3( 0.0f, D3DXToRadian( 90.0 ), 0.0f ) );
	//			//ｽｹｰﾙ.
	//			pEffects->SetScale( handle,
	//				D3DXVECTOR3( 0.5f, 0.5f, 0.5f ) );
	//		}
	//	}
	//	pEffects->Render( m_mView, m_mProj, m_Camera.vEye );
	//}

#ifdef _DEBUG
	//定規.
	for (int i = 0; i < m_iBlkAryMax; i++){
		m_ppBlkAry[i]->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
	}

	//ﾚｲの表示(垂直).
	m_pRayV->m_Ray.vPos = m_pPlayer->GetPosition();
	//回転値を自機に合わせる.
	m_pRayV->m_Ray.fYaw = m_pPlayer->GetRotationY();
	m_pRayV->Render(m_mView, m_mProj);

	//ﾚｲの表示(前後).
	m_pRayFB->m_Ray.vPos = m_pPlayer->GetPosition();
	//回転値を自機に合わせる.
	m_pRayFB->m_Ray.fYaw = m_pPlayer->GetRotationY();
	m_pRayFB->Render(m_mView, m_mProj);

	//ﾚｲの表示(左右).
	m_pRayH->m_Ray.vPos = m_pPlayer->GetPosition();
	//回転値を自機に合わせる.
	m_pRayH->m_Ray.fYaw = m_pPlayer->GetRotationY();
	m_pRayH->Render(m_mView, m_mProj);

#endif//#if _DEBUG
}
void clsMain::RenderOver()
{
	//プレイヤー(アルビア)表示.
	m_pPlayer->Render(m_mView, m_mProj, m_vLight, m_Camera.vEye);
}
void clsMain::RenderResult()
{
	RenderMain();
}
void clsMain::RenderEnding()
{
}

void clsMain::RenderDebugText()
{
	//NULLﾁｪｯｸ.
	if (m_pText != NULL){
		char strDbgTxt[256];
		int dbgtxty = 50;
		int dbgTxtX = 20;

		int YajirusiY;

		sprintf_s(strDbgTxt,
			"Ply:x[%f],y[%f],z[%f],",
			m_pPlayer->GetPositionX(),
			m_pPlayer->GetPositionY(),
			m_pPlayer->GetPositionZ());

		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;

		sprintf_s(strDbgTxt,
			"PlyRotY:[%f]",
			m_pPlayer->GetRotationY());

		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;

		//どちらを更新する?.
		sprintf_s(strDbgTxt, "Control Mode");
		m_pText->Render(strDbgTxt, dbgtxty, dbgtxty);
		dbgtxty += 10;

		YajirusiY = dbgtxty;
		sprintf_s(strDbgTxt, "jyougiu  Mode");
		m_pText->Render(strDbgTxt, dbgtxty, dbgtxty);
		dbgtxty += 10;

		sprintf_s(strDbgTxt, "Png Mode");
		m_pText->Render(strDbgTxt, dbgtxty, dbgtxty);
		dbgtxty += 10;

		//矢印.
		switch (ControlMode){
			case 0:
				break;
			case 1:
				YajirusiY += 10;
				break;
		}
		sprintf_s(strDbgTxt, "->");
		m_pText->Render(strDbgTxt, dbgTxtX, YajirusiY);

		if (!m_vsmpSpriteTest.empty())
		{
			//PngDisp.
			sprintf_s(strDbgTxt,
				"PngDispScale[%d]:W[%f],H[%f]",
				m_SpriteControlNum,
				m_vsmpSpriteTest[m_SpriteControlNum]->GetSs().Disp.w,
				m_vsmpSpriteTest[m_SpriteControlNum]->GetSs().Disp.h);

			m_pText->Render(strDbgTxt, dbgTxtX, dbgtxty);
			dbgtxty += 10;

			//PngDisp.
			sprintf_s(strDbgTxt,
				"PngPos[%d]:X[%f],Y[%f]",
				m_SpriteControlNum,
				m_vsmpSpriteTest[m_SpriteControlNum]->GetPos().x,
				m_vsmpSpriteTest[m_SpriteControlNum]->GetPos().y);

			m_pText->Render(strDbgTxt, dbgTxtX, dbgtxty);
			dbgtxty += 10;
		}

		sprintf_s(strDbgTxt,
			"camera:x[%f],y[%f],z[%f],",
			m_Camera.vEye.x,
			m_Camera.vEye.y,
			m_Camera.vEye.z);

		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;
		sprintf_s(strDbgTxt,
			"look:x[%f],y[%f],z[%f],",
			m_Camera.vLook.x,
			m_Camera.vLook.y,
			m_Camera.vLook.z);

		m_pText->Render(strDbgTxt, 0, dbgtxty);
		dbgtxty += 10;

		//dbgtxty += 10;
		//if( m_pBgm[0]->IsStopped() ){
		//	sprintf_s( strDbgTxt, "Stopped" );
		//	m_pText->Render( strDbgTxt, 0, dbgtxty );
		//}
		//if( m_pBgm[0]->IsPlaying() ){
		//	sprintf_s( strDbgTxt, "Playingn" );
		//	m_pText->Render( strDbgTxt, 0, dbgtxty );
		//}
	}
}

#ifdef START_LOADING_DISPLAY
//========/ ﾛｰﾄﾞ画面 /========//
void clsMain::NowLoading()
{
	LoadSpriteInit();
	//ﾘｿｰｽｸﾗｽ.
	m_pResource = clsResource::GetInstance();

	for (LoadStep = 0; LoadStep <= 6; LoadStep++)
	{
		m_pLoadGage->SetPos(D3DXVECTOR3(WND_W / 2.0f - (ss_LoadGage.Disp.w / 2.0f), 453.0f, -2.0f));
		LoadGageUVTarget = (0.5f - (0.5f / 100.0f) * LoadGage);
		m_pLoadGage->SetPatarnU(LoadGageUVTarget);

		LoadRender();
		switch (LoadStep)
		{
			case 0:
				m_pResource->InitStaticModel(m_hWnd, m_pDevice, m_pDeviceContext);
				m_pResource->InitSkinModel(m_hWnd, m_pDevice, m_pDeviceContext);
				LoadGage += 10;
				break;
			case 1:
				m_pResource->CreateStaticModel(
					"Data\\Test\\Test.x",
					clsResource::enST_MODEL_Plane);
				//通常壁.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\kabe.X",
					clsResource::enST_MODEL_WALL);

				//壁面無し壁.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\kabe_2.X",
					clsResource::enST_MODEL_WALL_NOT_WALL);

				//壁面無し通常床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_2.X",
					clsResource::enST_MODEL_FLOOR_NOT_WALL_NORMAL);

				//壁面無し左の半分床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_hanbun_L_2.X",
					clsResource::enST_MODEL_FLOOR_NOT_WALL_HALF_L);

				//壁面無し右の半分床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_hanbun_R_2.X",
					clsResource::enST_MODEL_FLOOR_NOT_WALL_HALF_R);

				//壁面無しペンデュラム床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_kizu_2.X",
					clsResource::enST_MODEL_FLOOR_NOT_WALL_PEND);

				//通常床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka.X",
					clsResource::enST_MODEL_FLOOR_NORMAL);

				//左の半分床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_hanbun_L.X",
					clsResource::enST_MODEL_FLOOR_HALF_L);

				//右の半分床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_hanbun_R.X",
					clsResource::enST_MODEL_FLOOR_HALF_R);
				//ペンデュラム床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\yuka_kizu.X",
					clsResource::enST_MODEL_FLOOR_PEND);
				//縦移動床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\StepCylinder.X",
					clsResource::enST_MODEL_STEP_CIL);
				m_pResource->CreateStaticModel(
					"Data\\Stage\\StepCylinder_Big.X",
					clsResource::enST_MODEL_STEP_CIL_BIG);
				//横移動床.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\StepBox.X",
					clsResource::enST_MODEL_STEP_BOX);

				//ドアモデルを確認する.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\mon.X",
					clsResource::enST_MODEL_MON);
				LoadGage += 50;
				break;
			case 2:
				//ゴールモデルを確認する.
				m_pResource->CreateStaticModel(
					"Data\\TrRoom\\gool.X",
					clsResource::enST_MODEL_GOOL);
				//宝箱のあたり判定
				m_pResource->CreateStaticModel(
					"Data\\TrRoom\\TrBoxCol.X",
					clsResource::enST_MODEL_TR_BOX_COL);

				//普通槍.
				m_pResource->CreateStaticModel(
					"Data\\Trap\\Spia.x",
					clsResource::enST_MODEL_SPIA);
				//壁槍.
				m_pResource->CreateStaticModel(
					"Data\\Trap\\SpiaOfWall.x",
					clsResource::enST_MODEL_SPIA_OF_WALL);

				//槍の柱部分の壁判定用.
				m_pResource->CreateStaticModel(
					"Data\\Trap\\SpiaWallHit.x",
					clsResource::enST_MODEL_SPIA_WALL);

				//左側の落とし穴の蓋.
				m_pResource->CreateStaticModel(
					"Data\\Trap\\otosiana_L.x",
					clsResource::enST_MODEL_COVER_L);

				//右側の落とし穴の蓋.
				m_pResource->CreateStaticModel(
					"Data\\Trap\\otosiana_R.x",
					clsResource::enST_MODEL_COVER_R);
				//定規用のブロック.
				m_pResource->CreateStaticModel(
					"Data\\Jikken\\1_1_1_kijyun.x",
					clsResource::enST_MODEL_BLOCK);

				//天井.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\Ceiling.X",
					clsResource::enST_MODEL_CEILING);

				//テスト用.
				m_pResource->CreateStaticModel(
					"Data\\Stage\\tobira.X",
					clsResource::enST_MODEL_TEST);

				//=====Skin Model=====.
				//アルビア.
				m_pResource->CreateSkinModel(
					"Data\\Player\\Arbia.X",
					clsResource::enSK_MODEL_PLAYER);

				//ドア.
				m_pResource->CreateSkinModel(
					"Data\\Stage\\tobira.X",
					clsResource::enSK_MODEL_DOOR);

				//敵.
				m_pResource->CreateSkinModel(
					"Data\\Enemy\\Enemy.X",
					clsResource::enSK_MODEL_ENEMY);
				//ペンデュラム.
				m_pResource->CreateSkinModel(
					"Data\\Trap\\Pendulum.X",
					clsResource::enSK_MODEL_PENDULUM);
				//宝箱.
				m_pResource->CreateSkinModel(
					"Data\\TrRoom\\TrRoomBox.X",
					clsResource::enSK_MODEL_TR_BOX);
				LoadGage += 10;
				break;
			case 3:
				LoadGage += 10;
				break;
			case 4:
				LoadGage += 10;
				break;
			case 5:
				ReadMesh();
				LoadGage += 10;
				break;
			case 6:
				//タイトル用の初期化を入れる.
				InitTitle();
				m_pBgm[enBGM_START_UP]->Stop();
				LoadGage += 10;
				break;
			default:
				break;
		}
	}
}

// ﾛｰﾄﾞ画面初期化 //
void clsMain::LoadSpriteInit()
{
	m_pLoadBack = make_unique<clsSprite2D>();
	m_pLoadBack->Init(m_pDevice, m_pDeviceContext, "Data\\Load\\Black.png");
	m_pLoadBack->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pLoadBack->SetPatarnU(0.0f);
	m_pLoadBack->SetPatarnV(0.0f);
	m_pLoadBack->SetAlpha(1.0f);

	m_pLoadTxt = new clsSprite2D;
	m_pLoadTxt->Init(m_pDevice, m_pDeviceContext, "Data\\Load\\LoadBack.png");
	m_pLoadTxt->SetDispW(m_pLoadTxt->GetSs().Base.w / 2);
	m_pLoadTxt->SetDispH(m_pLoadTxt->GetSs().Base.h / 2);
	m_pLoadTxt->UpDateSpriteSs();
	m_pLoadTxt->SetPos(WND_W / 2 - m_pLoadTxt->GetSs().Disp.w / 2, m_pLoadTxt->GetSs().Disp.h);
	m_pLoadTxt->SetPatarnU(0.0f);
	m_pLoadTxt->SetPatarnV(0.0f);
	m_pLoadTxt->SetAlpha(1.0f);

	ss_LoadGage.Disp.w = 960.0f;
	ss_LoadGage.Disp.h = 50.0f;
	ss_LoadGage.Stride.w = ss_LoadGage.Disp.w / 2;
	ss_LoadGage.Stride.h = ss_LoadGage.Disp.h;
	m_pLoadGage = new clsGageSprite2D;
	m_pLoadGage->Init(m_pDevice, m_pDeviceContext, "Data\\Load\\LoadGarge2.png", ss_LoadGage);
	m_pLoadGage->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pLoadGage->SetPatarnU(0.0f);
	m_pLoadGage->SetPatarnV(0.0f);
	m_pLoadGage->SetAlpha(1.0f);

	m_pLoadGageBack = make_unique<clsSprite2D>();
	m_pLoadGageBack->Init(m_pDevice, m_pDeviceContext, "Data\\Load\\LoadGargeType.png");
	m_pLoadGageBack->SetDispW(960.0f);
	m_pLoadGageBack->SetDispH(50.0f);
	m_pLoadGageBack->UpDateSpriteSs();
	m_pLoadGageBack->SetPos(D3DXVECTOR3(WND_W / 2.0f - (ss_LoadGage.Disp.w / 2.0f), 453.0f, -2.0f));
	m_pLoadGageBack->SetAlpha(1.0f);

	LoadGage = 0;
	LoadGageUVTarget = 0.0f;
}

// ﾛｰﾄﾞ画面表示 //
void clsMain::LoadRender()
{
	//画面のｸﾘｱ.
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };//ｸﾘｱ色(RGBA順:255の比率で出す)
	//ｶﾗｰﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);
	//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	SetDepth(false);
	m_pLoadBack->Render();

	m_pLoadGageBack->Render();

	m_pLoadGage->Render();

	m_pLoadTxt->Render();

	SetDepth(true);

	//ﾚﾝﾀﾞﾘﾝｸﾞされたｲﾒｰｼﾞを表示.
	m_pSwapChain->Present(0, 0);
}
#endif //#ifdef START_LOADING_DISPLAY