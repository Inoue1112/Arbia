#ifndef _SP2D_MANAGER_TITLE_SCENE_H_
#define _SP2D_MANAGER_TITLE_SCENE_H_

#include "Sp2DMgrPushButn.h"
#include "MyMacro.h"

class clsSp2dMgrTitle
{
public:
	clsSp2dMgrTitle();
	~clsSp2dMgrTitle();
	void Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void InitSetPos();
	void Render();
	void Release();
	void Transmission();
	void SetTransmissionFlg(bool TransmissionFlg)
	{
		m_bTransmissionFlg = TransmissionFlg;
	}
private:
	bool m_bTransmissionFlg;//Trueなら透過していくfalseなら何もしない.
	//タイトル用.
	unique_ptr<clsSprite2D> m_smpTitleLogo;	//タイトルロゴねぇでやんす.
	unique_ptr<clsPushTxt> m_smpPushTxt;	//PUSH KETねぇでやんす.
};
#endif //#ifndef _SP2D_MANAGER_TITLE_SCENE_H_
