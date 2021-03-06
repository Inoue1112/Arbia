#ifndef _SP_2D_MANAGER_MAIN_SCENE_H_
#define _SP_2D_MANAGER_MAIN_SCENE_H_

#include "Sp2DMgrActTxt.h"
#include "NumGroup.h"
#include "Sp2DTimerGroup.h"
#include "Sp2DMgrLives.h"
#include "Sp2DMgrDiscover.h"

namespace MainScene{
	const WHSIZE_FLOAT ICONPOS = { 1050.0f, 5.0f };
	const WHSIZE_FLOAT ICONSIZE = { 108.0f, 108.0f };

	const WHSIZE_FLOAT CENTER_POS = { WND_W / 2, WND_H / 2 };
}


//初期化用.
const float fINIT_MAIN_UI_TRIO_ADD_POS_Y = MainScene::ICONSIZE.h * 2.0f;

class clsSp2dMgrMain
{
public:
	clsSp2dMgrMain();
	~clsSp2dMgrMain();

	void Init( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void InitSetPos();
	void Render();
	void ActTxtMove();
	void ActTxtMoveIn();
	void ActTxtMoveOut();
	void Release();

#ifdef Tahara
	int GetTime(){
		return m_smpTimerGrp->GetTime();
	}

	//ラップ.
	void ReSetTimer(){
		m_smpTimerGrp->SetPos( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
		m_smpTimerGrp->ReSet();
	}

	void ReSetLives(){
		m_smpLives->ReSet();
	}
	void SetLives( int iHp ){
		m_smpLives->SetNum( iHp );
	}

	void ReSetDisc(){
		m_smpDisc->ReSet();
	}
	void SetDisc( int iDisc ){
		m_smpDisc->SetNum( iDisc );
	}

	void SetTimerStop( bool bStop ){
		m_smpTimerGrp->SetStop( bStop );
	}


	void AddPosYUiSet( float fMove ){
		m_smpTimerGrp->AddPosY( fMove );
		m_smpDisc->AddPosY( fMove );
		m_smpLives->AddPosY( fMove );
	}

#endif//#ifdef Tahara


	//変化量.
	void MoveStartTopTxt(float fMovePoint )
	{
		m_iMoveTopTxtPoint = 0.0f;
		m_bMoveTopTxt      = true;
		m_fMoveTopTxtPoint = fMovePoint;
	}

	void MoveTopTxt()
	{
		if (m_bMoveTopTxt)
		{
			AddPosYUiSet( m_fMoveTopTxtPoint );
			if (m_iMoveTopTxtPoint < fINIT_MAIN_UI_TRIO_ADD_POS_Y )
			{
				m_iMoveTopTxtPoint+=m_fMoveTopTxtPoint;
			}
			else
			{
				m_bMoveTopTxt = false;
				//セット.
				ReSetTimer();
				SetTimerStop( true );
				ReSetLives();
				ReSetDisc();
			}
		}
	}


private:
	unique_ptr<clsSprite2D> m_smpXButton;	//Xボタンねぇでやんす.
	unique_ptr<clsSprite2D> m_smpAButton;	//Aボタンねぇでやんす.
	unique_ptr<clsActTxt>	m_smpActTxt;	//メインのジャンプなどの文字.

	bool  m_bMoveTopTxt;	 //trueなら移動する.
	float m_iMoveTopTxtPoint;//変化量確認.
	float	m_fMoveTopTxtPoint = 0.7f;	//上のテキストを下におろすときの変化量.


#ifdef Tahara
	unique_ptr<clsTimerGrp> m_smpTimerGrp;	//時間.
	unique_ptr<clsDiscover> m_smpDisc;		//発見数.
	unique_ptr<clsLives> m_smpLives;		//残機.
#endif//#ifdef Tahara

};
#endif //#ifndef _SP_2D_MANAGER_MAIN_SCENE_H_