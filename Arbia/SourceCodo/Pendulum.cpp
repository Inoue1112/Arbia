#include <math.h>
#include "Pendulum.h"


//��.
const char ALIAS_NAME[] = "PendSe";
const char FILE_PATH[] = "SE\\300Trap\\200Pendulum.wav";
const int VOL = 1000;


//===== �����蔻�� =====//.
//===== Box =====//.
const float COL_PEND_BOX_X = 6.0f / 2.0f;
const float COL_PEND_BOX_Y = 4.0f / 2.0f;
const float COL_PEND_BOX_Z = 0.875f / 2.0f;
const float COL_PEND_BOX_CENTER_Y = 0.0f;
//===== �����蔻�� �I��� =====//.




//const float fPEND_SPN_POWER_MAX = 0.05f;
//const float fPEND_SPN_POWER_UP = 0.001f;
//const float fPEND_YAW_MAX = 0.0f;
const float fPEND_SPN_POWER_MAX = 0.025f;
const float fPEND_SPN_POWER_UP = 0.0005f;
const float fPEND_YAW_MAX = 0.0f;




//----- ���� ------//
//���ߋ��E��Z.
const float fALPHA_BORDER_Z = 0.5f;

//���ߑ��x.
const float fALPHA_SPD = 1.0f / 64.0f;

//��Ԕ������Ă�.
const float fALPHA_LIMIT = 0.25f;


//.


clsPend::clsPend()
{
}

clsPend::~clsPend()
{
	if( m_pSe != NULL ){
		m_pSe->Stop();
		m_pSe->Close();
		delete m_pSe;
		m_pSe = NULL;
	}
}

void clsPend::Create( HWND hWnd, int iNo, int iPower  )
{
	Init();

	//�����蔻���ݒ�( ���肦�Ȃ����ɓ���Ȃ���1�t���[���ڂ� 0, 0, 0 �ɗ��� ).
	float initPos = -100.0f;
	ColState.vPos = { initPos, initPos, initPos };
	//Box�p.
	ColState.vRangeHalf.x = COL_PEND_BOX_X;
	ColState.vRangeHalf.y = COL_PEND_BOX_Y;
	ColState.vRangeHalf.z = COL_PEND_BOX_Z;
	ColState.fCenterY = COL_PEND_BOX_CENTER_Y;
	ColState.fCenterX = 0.0f;


	//���N���X.
	m_pSe = new clsSound;
	//�����ʏ�����.
	m_pSe->SetVolume( 0 );
	//���O.
	char cAliasName[STR_BUFF_MAX] = "";
	strcat_s( cAliasName, sizeof( cAliasName ), ALIAS_NAME );
	//�ԍ�.
	char cNumber[] = "  ";
	_itoa_s( iNo, cNumber, 10 );
	//���O�Ɣԍ�����.
	strcat_s( cAliasName, sizeof( cAliasName ), cNumber );
	//�쐬.
	m_pSe->Open( FILE_PATH, cAliasName, hWnd );
	//�ő剹�ʐݒ�.
	m_pSe->SetMaxVolume( VOL );

	//�������t���O�Z�b�g.
	if( iPower <= 0 ){
		iPower = 1;
	}
	 m_iInitFlg = iPower;
}


void clsPend::Init()
{
	//�G�N�Z���ɓ����Ă��鐔�����傫���ƍŏ��̗h�ꂪ�x���.
	m_vRot.z = 0.0f;
	//��̎��i�ŏ��͉E�ɍs���j.
	if( m_iInitFlg % 2 ){
		m_fMovePower = fPEND_SPN_POWER_MAX;
		m_bRigtFlg = true;
		m_vRot.z -= (float)( m_iInitFlg * 0.01 );
	}
	//�����̎��i�ŏ��͍��ɍs���j.
	else{
		m_fMovePower = -fPEND_SPN_POWER_MAX;
		m_bRigtFlg = false;
		m_vRot.z += (float)( ( m_iInitFlg - 1 ) * 0.01 );
	}


	m_fAlpha = 1.0f;
	m_bAlpha = false;
	m_bAlphaChange = false;
}

//============================================================
//	�U��q����.
//============================================================
void clsPend::Move( float fEarZ )
{
	//�U���.
	m_vRot.z += m_fMovePower;

	//�E�ɐU���.
	if( m_bRigtFlg ){
		//����.
		m_fMovePower += fPEND_SPN_POWER_UP;
		//�����Ȃ肷���Ȃ�.
		if( m_fMovePower > fPEND_SPN_POWER_MAX ){
			m_fMovePower = fPEND_SPN_POWER_MAX;
			//���].
			if( m_vRot.z > fPEND_YAW_MAX ){
				m_bRigtFlg = false;
				PlaySe( fEarZ );
			}
		}
	}
	//���ɐU���.
	else{
		m_fMovePower -= fPEND_SPN_POWER_UP;
		if( m_fMovePower < -fPEND_SPN_POWER_MAX ){
			m_fMovePower = -fPEND_SPN_POWER_MAX;
			if( m_vRot.z < -fPEND_YAW_MAX ){
				m_bRigtFlg = true;
				PlaySe( fEarZ );
			}
		}
	}
	//���ߒl�ݒ�.
	SetAlpha();
}




//============================================================
//	�����ݸ�.
//============================================================
void clsPend::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye )
{
	if( m_pModel == NULL || m_pAnimCtrl == NULL ){
		return;
	}

	UpDateModel();

	m_pModel->Render(
		mView, mProj, vLight, vEye,
		D3DXVECTOR4( 1.0f, 1.0f, 1.0f, m_fAlpha ), m_bAlpha, m_pAnimCtrl );
}


//============================================================
//	���ߒl�̐ݒ�.
//============================================================
void clsPend::SetAlphaFlg( float fPlayerZ )
{
	//���ߒ�.
	if( m_bAlphaChange ){
		//���߃t���OON.
		m_bAlpha = true;
		//�v���C���[�����ɂ���Ȃ�.
		if( fPlayerZ > m_vPos.z + fALPHA_BORDER_Z ){
			return;
		}
		//���̉��J�n.
		m_bAlphaChange = false;
	}
	//���ߒ��ł͂Ȃ�.
	else{
		//�v���C���[����O�ɂ���Ȃ�.
		if( fPlayerZ < m_vPos.z + fALPHA_BORDER_Z ){
			return;
		}
		//���ߊJ�n.
		m_bAlphaChange = true;
	}
}

//============================================================
//	���ߒl�̐ݒ�.
//============================================================
void clsPend::SetAlpha()
{
	//���ߒ�.
	if( m_bAlphaChange ){
		m_fAlpha -= fALPHA_SPD;
		if( m_fAlpha < fALPHA_LIMIT ){
			m_fAlpha = fALPHA_LIMIT;
		}
	}
	//���̉���.
	else{
		m_fAlpha += fALPHA_SPD;
		if( m_fAlpha > 1.0f ){
			m_fAlpha = 1.0f;
			m_bAlpha = false;
		}
	}
}




//============================================================
//	�{�[��pos.
//============================================================
void clsPend::GetBonePos( const char* cBoneName, D3DXVECTOR3 vBonePos )
{
	//�{�[�����W.
	char* boneName =  (char*)cBoneName;
	if( m_pModel->GetPosFromBone( boneName, &vBonePos ) ){
		SetColPos( &vBonePos );
	}
}



//============================================================
//	���ʉ�.
//============================================================
void clsPend::PlaySe( float fEarZ )
{
	//�Đ����鋗���Ȃ�.
	int vol = ChangeVolumeDistance( fEarZ, m_vPos.z );
	if( vol ){
		m_pSe->GetMaxVolRate( vol );
		m_pSe->SeekToStart();
		m_pSe->SetVolume( vol );
		m_pSe->Play();
	}
	//�Đ����Ȃ������Ȃ�.
	else{
		m_pSe->SetVolume( 0 );
	}
}