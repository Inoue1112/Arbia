#ifndef _C_PENDULUM_H_
#define _C_PENDULUM_H_


#include "CharaSkin.h"
#include "Sound.h"

//Ҳ݂Ŏg����.
const char PEND_BONE_NAME[] = "PendHitCenter"; 




class clsPend
	: public clsCharaSkin
{
public:
	clsPend();
	virtual ~clsPend();


	void Create( HWND hWnd, int iNo, int iPower );

	virtual void Init();

	//�����ݸ�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye );


	//����(�����).
	void Move( float fEarZ );

	//���߃t���O�̐ݒ�.
	void SetAlphaFlg( float fPlayerZ );


	
	//�{�[��pos.
	void GetBonePos( 
		const char* cBoneName,
		D3DXVECTOR3 vBonePos = D3DXVECTOR3( -100.0f, -100.0f, -100.0f ) );

private:

	//���ʉ�.
	void PlaySe( float fEarZ );
	clsSound*	m_pSe;


	float m_fMovePower;
	bool m_bRigtFlg;

	int		m_iInitFlg;//�������t���O.


	//���ߒl�̐ݒ�.
	void SetAlpha();
	//����.
	float	m_fAlpha;
	bool	m_bAlpha;//���f����Render()�ɓn������.
	bool	m_bAlphaChange;//���ߒl���ǂ���ɕς��Ă��邩.

};



#endif//#ifndef _C_PENDULUM_H_