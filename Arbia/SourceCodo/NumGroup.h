#ifndef _NUM_GROUP_H_
#define _NUM_GROUP_H_

#include "Sprite2D.h"
#include "Mymacro.h"
#include "Common.h"//共通ｸﾗｽ.
#include <memory>


using namespace std;



class clsNumGrp
{
public:
	clsNumGrp();
	~clsNumGrp();

	//最初の初期化( クラスを作った直後 ).
	//一ケタ目の速度 二ケタ目の速度 タイマーか否か.
	virtual void Create( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	//二回目以降の初期化.
	virtual void ReSet();

	void SetPos( D3DXVECTOR3 vPos );
	void AddPos( D3DXVECTOR3 vPos );

	void Move();

	void Render();

	//数を増やす.
	virtual void PlusNum();
	//数を減らす.
	void MinusNum();
	//数を決め打ち.
	void SetNum( int iNum );


	int GetTime(){
		return m_iRealNum;
	}

protected:


	//数セット : Set Parameter.
	void SetParam( int iRealNum );

	//実数値を座標地に変換.
	float ConvNum( float fRendNum, bool bSixFlg = false );//conversion : 変換.

	D3DXVECTOR3 m_vPos;

	//二桁固定.
	clsSprite2D*	NumImage[2];
	int				m_iRealNum;//実数値.
	float			m_fRendNum[2];//描画( 画面に見える )の数値( m_iRealNumを一ケタ当たりに分解されたもの ).
	float			m_fNumU[2];//座標値.


	//ターゲットに実数値を近づける : approach_to_target.
	void	ApproachTargetNum();
	int		m_iDiffNum;//あとどれくらいで目的達成する?.

	bool	 m_bSixDecimal;//6進数?.

	int		m_iMax;//最大値.
};


#endif//#ifndef _NUM_GROUP_H_