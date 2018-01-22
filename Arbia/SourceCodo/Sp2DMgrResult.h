#ifndef _SP_2D_MANAGER_RESULT_SCENE_H_
#define _SP_2D_MANAGER_RESULT_SCENE_H_

#include "Sp2DMgrPushButn.h"
#include "Sp2DJewelSet.h"

#include "Sound.h"


const char cSTEAL_RESULT_MAX = 4;//結果の項目の数.
const char cJEWEL_NUM_PARTS = 3;//項目ごとの成果の最大数.
//宝石の合計数 : 項目数 * 項目当たりの最大数 + 制作者タイムボーナスの1.
const short sJEWEL_NUM_MAX = ( cSTEAL_RESULT_MAX * cJEWEL_NUM_PARTS ) - 2;


//UIに表示するために必要.
struct UI_STATE_RESULT
{
public:
	int iLives;
	int iTime;
	int iDisc;


	UI_STATE_RESULT(){
		iINIT_NUM = 1001;
		Init();
	}
	void Init(){
		iLives = 0;
		iTime = iDisc = iINIT_NUM;
	}

private:
	int iINIT_NUM;

};


class clsSp2dMgrReslt
{
public:
	clsSp2dMgrReslt();
	~clsSp2dMgrReslt();



	enum enResultMode
	{
		enM_BLACK = 0,
		enM_STEAL,
		enM_OTHER,
		enM_JEWEL,

		enM_PARF,//パーフェクト.

		enM_FINAL_SET,//最終位置.

		enM_END,//動かない.


		enM_MAX
	};



	void Init( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	void InitSetPos();

	//シーンのはじめの初期化.
	void StartSet();

	void UpDate();

	void Render();
	void Release();

	void SetJewelAppPos( D3DXVECTOR3 vPos ){
		m_vJewelAppPos = vPos;
	}
	D3DXVECTOR3 GetJewelAppPos(){
		return m_vJewelAppPos;
	}

	//宝石必要かの判断に必要な情報を持ってくる.
	void SetJewelState( UI_STATE_RESULT UiState ){
		m_UiState = UiState;
	}

	//BGM再再生許可フラグ関数.
	bool isCanBgmRePlay(){
		return m_bCanBgmRePlay;
	}

	//決定押したら終わっていい?.
	bool isCanEnd(){
		return m_bEndFlg;
	}

	//強制的に最終状態にする.
	void ResEndPos(){
		m_enMode = enM_FINAL_SET;
	};

private:

	//宝石のMoveとAccを求める. Parb -> parabola : 放物線.
	void MakeParbPower(
		D3DXVECTOR3 &vMove, float &fAcc,
		D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos );


	//動き.
	void BlackMove();
	void StealMove();
	void OtherMove();
	void JewelMove();
	void ParfeMove();//パーフェクト.

	void FinalPos( bool bInit = false );

	void NextMove(){
		m_enMode = static_cast<enResultMode>( m_enMode + 1 );
	}


	//パーフェクト中のフラグ.
	enum enParMode
	{
		enPM_SOUND = 0,
		enPM_PARFECT,
		enPM_JEWEL,
		enPM_END,
	};
	enParMode m_enParFlg;

	//宝石初期化.
	void JewelSetReSet();

	//宝石出すかの判断.
	void JewelJudge();

	//次の宝石を動かす準備.
	void NextJewerSet();

	//OtherMove()で使う.
	bool m_bOtherMove[cSTEAL_RESULT_MAX - 1];


	//アクションシーンを半ば隠す暗転（半透明）.
	unique_ptr<clsSprite2D> m_smpBlack;

	//「盗んだ成果」.
	unique_ptr<clsSprite2D> m_smpSteal;
	//盗んだ成果の項目.
	unique_ptr<clsSprite2D> m_smpResult[cSTEAL_RESULT_MAX];//for( char i=0; i<cSTEAL_RESULT_MAX; i++ ){
	//宝石 : 項目数 * 項目当たりの最大数 + 制作者タイムボーナスの1.
	unique_ptr<clsJewerSet> m_smpJewel[sJEWEL_NUM_MAX];	//for( char i=0; i<sJEWEL_NUM_MAX; i++ ){

	//ボタン指示.
	unique_ptr<clsPushTxt> m_smpPushTxt;


	enResultMode m_enMode;

	D3DXVECTOR3 m_vJewelAppPos;//宝石出現位置(宝箱と重ねる).
	int			m_iJewelCnt;


	UI_STATE_RESULT m_UiState;

	bool m_bEndFlg;//決定押したら終わっていい?.

	bool m_bCanBgmRePlay;

	//音の種類.
	enum enSound
	{
		enS_HIT = 0,
		enS_OPEN,

		enS_STEAL,
		enS_OTHER,

		enS_LAST_JEWEL,

		enS_MAX,	//最大数.
	};
	//効果音.
	clsSound*	m_pSeBox[enS_MAX];
	void SetSe( HWND hWnd );


};
#endif //#ifndef _SP_2D_MANAGER_RESULT_SCENE_H_
