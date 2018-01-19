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

	const float	MoveTopTxtPoint = 0.7f;	//��̃e�L�X�g�����ɂ��낷�Ƃ��̕ω���.
}


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

	//���b�v.
	void ReSetTimer(){
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

	void MoveStartTopTxt()
	{
		m_iMoveTopTxtPoint = 0.0f;
		m_bMoveTopTxt = true;
	}

	void MoveTopTxt()
	{
		if (m_bMoveTopTxt)
		{
			m_smpTimerGrp->AddPos( D3DXVECTOR3( 0.0f, MainScene::MoveTopTxtPoint, 0.0f ) );
			m_smpDisc->AddPos( D3DXVECTOR3( 0.0f, MainScene::MoveTopTxtPoint, 0.0f ) );
			m_smpLives->AddPos( D3DXVECTOR3( 0.0f,MainScene::MoveTopTxtPoint, 0.0f ) );
			if (m_iMoveTopTxtPoint < MainScene::ICONSIZE.h)
			{
				m_iMoveTopTxtPoint+=MainScene::MoveTopTxtPoint;
			}
			else
			{
				m_bMoveTopTxt = false;
			}
		}
	}

#endif//#ifdef Tahara

private:
	unique_ptr<clsSprite2D> m_smpXButton;	//X�{�^���˂��ł��.
	unique_ptr<clsSprite2D> m_smpAButton;	//A�{�^���˂��ł��.
	unique_ptr<clsActTxt>	m_smpActTxt;	//���C���̃W�����v�Ȃǂ̕���.

	bool  m_bMoveTopTxt;	 //true�Ȃ�ړ�����.
	float m_iMoveTopTxtPoint;//�ω��ʊm�F.
#ifdef Tahara
	unique_ptr<clsTimerGrp> m_smpTimerGrp;	//����.
	unique_ptr<clsDiscover> m_smpDisc;		//������.
	unique_ptr<clsLives> m_smpLives;		//�c�@.
#endif//#ifdef Tahara

};
#endif //#ifndef _SP_2D_MANAGER_MAIN_SCENE_H_