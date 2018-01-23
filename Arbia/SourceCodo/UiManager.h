#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_


#include "Mymacro.h"
#include "Sp2DMgrActTxt.h"

#include "Sp2DMgrTitle.h"
#include "Sp2DMgrMain.h"
#include "Sp2DMgrResult.h"
#include "Sp2DMgrOver.h"
#include "Sp2DMgrEnding.h"

#include "SceneManager.h"




using namespace std;


class clsUiManagar
{
public:
	clsUiManagar();
	~clsUiManagar();

	void Init( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void Delete();

	void ChangeTitleInit();
	void SceneTitle();
	void ChangeMainInit();
	void SceneMain();
	void ChangeResultInit();
	void SceneResult();
	void ChangeOverInit();
	void SceneOver();
	void ChangeEndingInit();
	void SceneEnding();



	void RenderTitle();
	void RenderMain();
	void RenderResult();
	void RenderOver();
	void RenderEnding();

	void BlackOut();
	void BlackStart(float ChaAmo, bool BFlg = true);

	//暗転機能が動いているかどうかbyInoue.
	bool GetBlackIdle(){
		if( m_smpBlack->m_BlackMode == Idle &&
			m_smpWhite->m_BlackMode == Idle )
		{
			return true;
		}
		return false;
	}

	//暗転しきった瞬間を知らせるbyTahara.
	bool GetBlackEnd(){
		if( m_smpBlack->m_BlackMode == Change ||
			m_smpWhite->m_BlackMode == Change )
		{
			return true;
		}
		return false;
	}

#ifdef Tahara

	//ラップ.
	void ReSetLives(){
		m_smpMainScene->ReSetLives();
	}
	void ReSetDisc(){
		m_smpMainScene->ReSetDisc();
	}
	void ReSetTimer(){
		m_smpMainScene->ReSetTimer();
	}
	void SetLives( int iHp ){
		m_smpMainScene->SetLives( iHp );
	}
	void SetDisc( int iDisc ){
		m_smpMainScene->SetDisc( iDisc );
	}

	void SetTimerStop( bool bStop ){
		m_smpMainScene->SetTimerStop( bStop );
	}

	//三点セット.
	void AddPosYUiSet( float fMove ){
		m_smpMainScene->AddPosYUiSet( fMove );
	}

	//アクション文字引っ込む.
	void ActTxtOut(){
		m_smpMainScene->ActTxtMoveOut();
	}

	//宝石.
	void SetJewelAppPos( D3DXVECTOR3 vPos ){
		m_smpResult->SetJewelAppPos( vPos );
	}
	D3DXVECTOR3 GetJewelAppPos(){
		return m_smpResult->GetJewelAppPos();
	}

	//リザルトで使う.
	bool isCanBgmRePlay(){
		return m_smpResult->isCanBgmRePlay();
	}
	bool isCanResEnd(){
		return m_smpResult->isCanEnd();
	}
	void ResEndPos(){
		m_smpResult->ResEndPos();
	}

	//ラップ + 時間セット.
	void SetJewelState( UI_STATE_RESULT ResUiState ){
		ResUiState.iTime = m_smpMainScene->GetTime();
		m_smpResult->SetJewelState( ResUiState );
	}

#endif//#ifdef Tahara


	//タイトルの透過開始.//Trueなら透過していくfalseなら何もしない.
	void SetTitleTransFlg(bool TransFlg)
	{
		m_smpTitleScene->SetTransmissionFlg(TransFlg);
	}

	//EndRollの終了確認用関数.
	bool GetEndRollEnd()
	{
		return m_smpEndingScene->GetEndRollEnd();
	}

	//ボタンが押されているか確認用関数.
	void SetPushButtun(bool PushButtun)
	{
		m_smpEndingScene->SetPushButtun(PushButtun);
	}

	//メインの上のテキストを下におろして表示する関数.
	void TopMainTxtMoveStart()
	{
#ifdef Tahara
		const float fUI_IN_SPD = 1.5f;
#endif //Tahara
		m_smpMainScene->MoveStartTopTxt(fUI_IN_SPD);
	}

private:

	float	m_fChangePoint;
	bool	m_bColor;

	void InitPos();


	unique_ptr<clsSprite2D> m_smpBlack;	//暗転用.
	unique_ptr<clsSprite2D> m_smpWhite;	//暗転用.

	//void ActTxtPosChange(bool );


	unique_ptr<clsSprite2D> m_smpXButton;	//Xボタンねぇでやんす.
	unique_ptr<clsSprite2D> m_smpAButton;	//Aボタンねぇでやんす.
	unique_ptr<clsActTxt>	m_smpActTxt;	//メインのジャンプなどの文字.

	//タイトル用.
	unique_ptr<clsSp2dMgrTitle> m_smpTitleScene;

	//メイン用.
	unique_ptr<clsSp2dMgrMain> m_smpMainScene;

	//リザルト用.
	unique_ptr<clsSp2dMgrReslt> m_smpResult;

	//エンディング用.
	unique_ptr<clsSp2dMgrEnding> m_smpEndingScene;	//クレジットねぇでやんす.

	//オーバー用.
	unique_ptr<clsSp2dMgrOver> m_smpOverScene;

	unique_ptr<clsSprite2D> m_pTestSpr2D;


};

#endif //#ifndef _UI_MANAGER_H_