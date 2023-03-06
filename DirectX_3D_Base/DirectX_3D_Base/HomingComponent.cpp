#include "HomingComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2dComponent.h"
#include "TextureManager.h"
#include "Objectmanager.h"
#include "EffectManager.h"
#include "Camera.h"

//===== �}�N����` =====
#define CREATE_EFFECT_TIME		(20)
#define CREATE_EFFECT_DIFF		(30)

#define CURSOR_EFFECT_SIZE_X	(100.0f)
#define CURSOR_EFFECT_SIZE_Y	(100.0f)
#define CURSOR_EFFECT_SPLIT_X	(5)
#define CURSOR_EFFECT_SPLIT_Y	(2)
#define CURSOR_EFFECT_FRAME		(3)

InputManager* g_Input = nullptr;
EffectManager* g_Effect = nullptr;

//	������(���m�ł͂Ȃ�)
float squareRoot_Cursor(float x);
//	2��֐�
float pow_Cursor(float x);

/*****

�^�C�g���A���[�h��ʁA�Z���N�g��ʁ@�c�@CURSOR_EFFECT_NUM(���F)

�t�@�c�@CURSOR_EFFECT_SPRING_NUM(���F)

�ā@�c�@CURSOR_EFFECT_SUMMER_NUM(�ΐF)

�H�@�c�@CURSOR_EFFECT_AUTUMN_NUM(���F)

�~�@�c�@CURSOR_EFFECT_WINTER_NUM(���F)

*****/

//	***�R���X�g���N�^
CHoming::CHoming()
	: m_pTransform(nullptr)
	, m_vCmpMousePos(0.0f, 0.0f)
	, m_nStopTime(0)
	, m_nMoveTime(0)
	, m_nEffectID(CURSOR_EFFECT_NUM)
{
}
//	***�f�X�g���N�^
CHoming::~CHoming()
{
}
//	***������
void CHoming::Start()
{
	m_pTransform = Parent->GetComponent<CTransform>();
	g_Input = INPUT;
	g_Effect = EffectManager::GetInstance();
}
//	***�X�V
void CHoming::Update()
{
	// ���W�擾
	XMFLOAT2 MousePos = g_Input->GetMousePos();														// �}�E�X���W�擾
	CCamera* pCamera = CCamera::Get();																// �J�����擾
	XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
	XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
	XMFLOAT3 vWorldPos;
	CalcScreenToXY(&vWorldPos, MousePos.x, MousePos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);		// ���[���h���W����X�N���[�����W��
	// �J�[�\���G�t�F�N�g�̍��W
	vWorldPos.z = CURSOR_POS_Z;


	if (Parent->GetName() == ENDING_CLOVER_NAME)
	{
		vWorldPos = Parent->GetComponent<CTransform>()->Pos;
	}
	else
	{
		// ���݂̍��W��ۑ�
		m_pTransform->Pos.x = MousePos.x;
		m_pTransform->Pos.y = MousePos.y;
	}

	// ���ԍX�V
	m_nStopTime++;				// �~�܂��Ă���Ƃ��̎��ԍX�V
	m_nMoveTime++;				// �����Ă���Ƃ��̎��ԍX�V

	// �}�E�X�𓮂����Ă��Ȃ�������
	if (m_pTransform->Pos.x - m_vCmpMousePos.x == 0 &&
		m_pTransform->Pos.y - m_vCmpMousePos.y == 0)
	{
		// �����Ԋu
		if (m_nStopTime > CREATE_EFFECT_TIME)
		{
			// �J�[�\���G�t�F�N�g�Đ�
			//g_Effect->Play(m_nEffectID, vWorldPos, XMFLOAT3(3.0f, 3.0f, 3.0f));
			m_nStopTime = 0;														// ���ԕ␳
		}
		return;
	}

	// �}�E�X�������Ă�����
	float fDistance = squareRoot_Cursor(pow_Cursor(m_pTransform->Pos.x - m_vCmpMousePos.x) + pow_Cursor(m_pTransform->Pos.y - m_vCmpMousePos.y));

	// �����Ԋu
	if (m_nMoveTime > 1)
	{
		// �J�[�\���G�t�F�N�g�Đ�
		g_Effect->Play(m_nEffectID, vWorldPos, XMFLOAT3(3.0f, 3.0f, 3.0f));
		m_nMoveTime = 0;														// ���ԕ␳
	}

	// �ߋ����W�X�V
	m_vCmpMousePos = XMFLOAT2(m_pTransform->Pos.x, m_pTransform->Pos.y);
}
//	***�`��
void CHoming::Draw()
{
}
//	***�Փˏ���
void CHoming::OnCollisionEnter(Object* pObject)
{
}

void CHoming::SetEffectNum(int nEffectNum)
{
	m_nEffectID = nEffectNum;
}

//	������(���m�ł͂Ȃ�)
float squareRoot_Cursor(float x)
{
	unsigned int i = *(unsigned int*)&x;

	// adjust bias
	i += 127 << 23;
	// approximation of square root
	i >>= 1;

	return *(float*)&i;
}

//	2��֐�
float pow_Cursor(float x)
{
	return x * x;
}