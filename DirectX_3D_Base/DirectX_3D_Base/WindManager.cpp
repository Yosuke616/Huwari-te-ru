#include "WindManager.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2dComponent.h"
#include "imgui.h"
#include "ObjectManager.h"
#include <list>
#include "Camera.h"
#include "ColliderComponent.h"
#include "DrawEffectComponent.h"
#include "TextureManager.h"
#include "WindComponent.h"
#include "GaugeComponent.h"

#include "Sound.h"

//�e�N�X�`�������p
#define UV_DXCHAN_WIDTH		(4)
#define UV_DXCHAN_HEIGHT	(4)

#define CREATE_FRAME		(1)

#define BIG_SOUND			(5)
#define SE_FRAME			(100)

//	������(���m�ł͂Ȃ�)
float squareRoot(float x);
//	2��֐�
float pow(float x);

//===== �ÓI�����o�ϐ� =====
WindManager* WindManager::m_pInstance = nullptr;


//=== �O���[�o���ϐ�
int g_nSE = 0;

// �C���X�^���X����
WindManager* WindManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new WindManager();
	}
	return m_pInstance;
}

// �I������
void WindManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// �R���X�g���N�^
WindManager::WindManager()
	: m_nFrameCnt(0)
	, m_nDeleteCnt(0)
	, m_bDeleteFlg(false)
	, m_bTutorialFlg(false)
	, m_bSpawnWind(false)

{
	m_WindCenterList.resize(0);
	m_WindVectorList.resize(0);

	g_nSE = 0;
}

// �f�X�g���N�^
WindManager::~WindManager()
{
}

// �X�V
void WindManager::Update()
{
	InputManager* Input = InputManager::Instance();

	// se����
	if (g_nSE > 0)
	{
		g_nSE--;
	}

	if (m_bSpawnWind == false) return;
	
	//	L�}�E�X�v���X
	if (Input->GetMouseButton(MOUSE_L))
		{
			//	���Q�[�W��0�̏ꍇ���Ȃ�
			if (CGauge::GetCurrentGauge(m_pWindGauge) <= 0) return;

			//	L�}�E�X�������Ă�t���OON
			m_bDeleteFlg = false;

			//	�}�E�X�̍��W���擾����
			m_P = Input->GetMousePos();

			//���t���[�����ƂɃ}�E�X���W���擾����
			if (m_nFrameCnt >= CREATE_FRAME)
			{
				//	�}�E�X���W��ۑ�
				m_WindCenterList.push_back(m_P);

				//	//��ȏ�̍��W���������Ƃ��̏ꍇ
				if (m_WindCenterList.size() >= 2)
				{
					// ������Ԃ̋������v�Z����
					std::list<XMFLOAT2>::iterator itrPoint = m_WindCenterList.end();
					itrPoint--;	itrPoint--;		//	���W���X�g�̌�납���Ԗ�
					m_WindPower.x = m_P.x - (*itrPoint).x;				// ���W���X�g��Ԍ�� - ���W���X�g�̌�납���Ԗ�
					m_WindPower.y = m_P.y - (*itrPoint).y;				// ���W���X�g��Ԍ�� - ���W���X�g�̌�납���Ԗ�
					float fDistance = squareRoot(pow(m_WindPower.x) + pow(m_WindPower.y));

					// ���ȏ�̋����ȏ㗣��Ă����瑱����
					if (fDistance < WIND_CREATE_DIFF)
					{
						// ����Ă��Ȃ�������ŐV�̃}�E�X���W���|�b�v����
						m_WindCenterList.pop_back();
						return;
					}

					//---- ������邱�Ƃ��m�� -----

					// ���̈��̋����~n�ȏ㗣��Ă�����n�̕������

					
					//--- ���̃x�N�g���v�Z
					// ���̃p���[��P�ʃx�N�g���ɒ����ĕۑ�
					XMVECTOR xmvWindNorm = XMLoadFloat2(&m_WindPower);	//	XMFLOAT2��XMVECTOR�^�ɂ�ϊ�
					xmvWindNorm = XMVector2Normalize(xmvWindNorm);		//	�x�N�g���̐��K��
					XMFLOAT2 vWindNorm;									//	���K�����ꂽXMVECTOR�^��XMFLOAT2�^�ɓ���锠�̗p��
					DirectX::XMStoreFloat2(&vWindNorm, xmvWindNorm);	//	XMVECTOR��XMFLOAT2�ɕϊ�
					m_WindVectorList.push_back(vWindNorm);				// �ۑ�

					//	���̐����J�E���g����
					CGauge::CalcGauge(m_pWindGauge, -1);
				}
				m_nFrameCnt = 0;	//	�ۑ�������t���[���������Z�b�g
			}

			//�t���[�������J�E���g
			m_nFrameCnt++;
		}

	//	L�}�E�X�����[�X
	if (Input->GetMouseRelease(MOUSE_L))
	{
		if (m_WindVectorList.size() > 0 && g_nSE <= 0)
		{
			// ���������̉�
			CSound::Play(SE_WIND);

			g_nSE = SE_FRAME;
		}
		m_bDeleteFlg = true;		//	L�}�E�X�������Ă�t���OOFF
	}

	//	�����[�X������
	if (m_bDeleteFlg)
	{
		if (m_nDeleteCnt >= CREATE_FRAME)
		{
			//���̔����ʒu
			std::list<XMFLOAT2>::iterator itrPos = m_WindCenterList.begin();
			//���̌���
			std::list<XMFLOAT2>::iterator itrPower = m_WindVectorList.begin();

			//	���̐������X�g�̃T�C�Y��0�̏ꍇ�Areturn
			if (m_WindVectorList.size() <= 0)
			{
				if (m_WindCenterList.size())
				{
					m_WindCenterList.pop_front();
				}
				return;
			}

			//	���̃^�C�~���O�ŕ������
			WindManager::CreateWind((*itrPos), (*itrPower));

			//	�}�E�X���W�̃��X�g�̐擪��������Ă���
			m_WindCenterList.pop_front();
			//	���̃x�N�g�����X�g������
			if (m_WindVectorList.size())
			{
				m_WindVectorList.pop_front();
			}

			//	�J�E���g��0�ɂ���
			m_nDeleteCnt = 0;
		}
		m_nDeleteCnt++;

		//	���ׂĂ̕��𐶐�������t���O��OFF
		if (!m_WindCenterList.size())
		{
			m_bDeleteFlg = false;
		}
	}

	//�I�u�W�F�N�g���X�g��S�����ĕ�������������false�ɕς���
	//m_bTutorialFlg���I����������������������
	if (m_bTutorialFlg) {
		Object* pWind = ObjectManager::GetInstance()->GetGameObject(WIND_NAME);

		if (!pWind) {
			//�`���[�g���A���t���O���I�t�ɂ���
			m_bTutorialFlg = false;
		}
	}
}

// �`��
void WindManager::Draw()
{
//#ifdef _DEBUG
//	using namespace ImGui;
//
//	//�}�E�X�̍��W�̑傫��
//	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	Begin("StorePosition");			//�^�C�g���o�[�ƈ�v�����炻���ɒǉ�����
//	Text("MousePosList : %d", m_WindCenterList.size());
//	//�}�E�X�̍��W�̕\��
//	for (auto&& Pos : m_WindCenterList)
//	{
//		Text("MousePos_X_Y     : (%.0f , %.0f) ", Pos.x, Pos.y);
//	}
//	End();
//
//	////	���̃p���[
//	//SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
//	//SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
//	//Begin("StoreVector");
//	//Text("WindPowerList : %d", m_WindVectorList.size());
//	//for (auto&& Vector : m_WindVectorList)
//	//{
//	//	Text("WindVector   : (%.0f , %.0f) ", Vector.x, Vector.y);
//	//}
//	//End();
//
//#endif // _DEBUG
}


/**
* @fn		CreateBlock
* @brief	���̐���
* @param	(XMFLOAT2) pos	 �X�N���[�����Wxy
* @param	(XMFLOAT2) power ���̗̗͂�
*/
void WindManager::CreateWind(XMFLOAT2 pos, XMFLOAT2 power)
{
	TextureManager* pTex = TextureManager::GetInstance();

	//	�X�N���[�����W�����[���h���W�ɕϊ�
	XMMATRIX view = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
	XMFLOAT3 worldPos;
	CalcScreenToXY(&worldPos, pos.x, pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view,	prj);
	
	//	���[���h���W�p�̑傫���ɕϊ�
	XMFLOAT2 WindSize(WIND_WIDTH , WIND_HEIGHT);

	//	���W�A���Ŋp�x���o���Ă����
	float fRad = atan2f(power.y, power.x);			//	���W�A��	(�΂̕�)
	//360�x�ɕϊ����Ă����(�ŏI�I�ȃS�[��)
	float fAngle = XMConvertToDegrees(fRad);		//	��		(360���̕�)

	//	�I�u�W�F�N�g����
	Object* pObjBlock = new Object(WIND_NAME, UPDATE_EFFECT, DRAW_EFFECT);
	//	�R���|�[�l���g�̒ǉ�
	auto TransformBlock = pObjBlock->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto DrawBlock = pObjBlock->AddComponent<CDrawEffect>();		//	�G�t�F�N�g�`��
	auto CollisionBlock = pObjBlock->AddComponent<CCollider>();		//	�����蔻��
	auto Wind = pObjBlock->AddComponent<CWind>();					//	��
	//	�I�u�W�F�N�g�̐ݒ�
	TransformBlock->SetPosition(worldPos.x, worldPos.y);			//	���W
	DrawBlock->SetSize(WindSize.x, WindSize.y);						//	�T�C�Y
	DrawBlock->SetTexture(pTex->GetTexture(WIND_TEX_NUM));			//	�e�N�X�`��
	DrawBlock->SetAnimSplit(WIND_SPLIT_X, WIND_SPLIT_Y);			//	������
	DrawBlock->SetSwapFrame(SWAP_FRAME);							//	�A�j���[�V�����؂�ւ�
	CollisionBlock->SetCollisionSize(WindSize.x, WindSize.y);		//	�����蔻��̐ݒ�
	TransformBlock->SetRotate(0.0f, 0.0f, fAngle);					//	��]
	Wind->SetWindVector(power);										//	���̗�(�P�ʃx�N�g���̗�)
	//	���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pObjBlock);
}


//	������(���m�ł͂Ȃ�)
float squareRoot(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2��֐�
float pow(float x)
{
	return x * x;
}


// ���Q�[�W�̐ݒ�
void WindManager::SetWindGauge(CGauge* pGauge)
{
	m_pWindGauge = pGauge;
}

// ���Q�[�W�̎擾
int WindManager::GetWindGauge()
{
	if (m_pWindGauge)	return CGauge::GetCurrentGauge(m_pWindGauge);
	else				return 0;
}

//2022/1/21/Tutorial Shimizu Yosuke ----------------------------

//�@���j���[���J���邩�ǂ����̃t���O�𑗂�
bool WindManager::GetTutorial() {
	return m_bTutorialFlg;
}

//�@���j���[���J���邩�ǂ������Z�b�g�Ō��߂�
void WindManager::SetTutorial(bool bTutorial) {
	m_bTutorialFlg = bTutorial;
}

//---------------------------------------------------------------


//2022/1/26/Touch_Tutorial Shimizu Yosuke -----------------------
/**
* @fn		SetSpawnWind
* @brief	���𐶐��ł���悤�ɂ���Z�b�^�[
* @param	(bool) bSpawnWind �����o���邩�ǂ����̐^�U
*/
void WindManager::SetSpawnWind(bool bSpawnWind) {
	m_bSpawnWind = bSpawnWind;
}
//---------------------------------------------------------------