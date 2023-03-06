//===== �C���N���[�h�@=====
#include "Load.h"

#include "TextureManager.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "LotationComponent.h"
#include "EffectManager.h"
#include "InputManager.h"
#include "Camera.h"
#include "HomingComponent.h"

//2022/1/31/Load_Tutorial Shimizu Yosuke ------------------------------------
#include "ObjectInfo.h"
#include "TutorialComponent.h"
//---------------------------------------------------------------------------

#include <process.h>

//===== �}�N���� =====
#define LOAD_FRAME		(1)		// �V�[���̏���������u�ŏI������ꍇ�҂��Ă��炤�b��

#define WIDTH_WHEEL		(200.0f)
#define HEIGHT_WHEEL	(200.0f)
#define POS_X_WHEEL		(500.0f)
#define POS_Y_WHEEL		(-250.0f)

//=== �ÓI�����o�ϐ�
HANDLE Load::m_handle;			// �X���b�h�n���h��
bool Load::m_bLoading = false;	// ���[�h��
int Load::m_nLoadCount = 0;		// ���[�h���Ԃ̃J�E���g
std::vector<Object*> Load::m_LoadScreenObjects;	// ���[�h��ʂ̃I�u�W�F�N�g���X�g


// �R���X�g���N�^
Load::Load(){
}
// �f�X�g���N�^
Load::~Load(){
}

// ���[�h��ʂ̏�����
void Load::Init()
{
	CCamera::Get()->Init();

	// �e�N�X�`���ǂݍ���
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_STOP_GAME, STOP_GAME_TEX_NUM);


#pragma region ---�w�i
	//	�I�u�W�F�N�g����
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//	�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();				// ���W
	auto Draw_BG = objBG->AddComponent<CDraw2D>();	// �`��
	Draw_BG->SetColor(0.0f, 0.0f, 0.0f);			// �F
	Draw_BG->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);	// �T�C�Y
	//	���X�g�ɒǉ�
	m_LoadScreenObjects.push_back(objBG);
#pragma endregion

#pragma region ---���[�f�B���O�V���t�B
	//	�I�u�W�F�N�g����
	Object* wheel = new Object("wheel", UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto Trans = wheel->AddComponent<CTransform>();				//	���W
	auto pDraw = wheel->AddComponent<CDrawEffect2D>();			//	�`��
	//auto pLotate = wheel->AddComponent<CLotation>();			//	�`��
	//	�I�u�W�F�N�g�̐ݒ�
	Trans->SetPosition(POS_X_WHEEL, POS_Y_WHEEL);				//	���W�̐ݒ�
	pDraw->SetTexture(pTexManager->GetTexture(LOAD_SILFY_TEX_NUM));	//	�e�N�X�`���̐ݒ�
	pDraw->SetSize(WIDTH_WHEEL, HEIGHT_WHEEL);					//	�T�C�Y�̐ݒ�
	pDraw->SetLoop(true);
	pDraw->SetAnimSplit(8, 8);
	pDraw->SetSwapFrame(3);
	pDraw->SetColor(0.4f, 1.0f, 0.4f);
	//	���X�g�ɒǉ�
	m_LoadScreenObjects.push_back(wheel);
#pragma endregion

#pragma region	---�J�[�\��
	//	�I�u�W�F�N�g����
	Object* pCursor = new Object(CURSOR_NAME, UPDATE_UI, DRAW_UI);
	//	�R���|�[�l���g�̒ǉ�
	auto transCursor = pCursor->AddComponent<CTransform>();	//	�g�����X�t�H�[��
	auto homingCursor = pCursor->AddComponent<CHoming>();		//	�}�E�X�Ǐ]
	auto drawCursor = pCursor->AddComponent<CDraw2D>();		// �`��
	pCursor->AddComponent<CLotation>();				// ��]

	drawCursor->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));
	drawCursor->SetSize(CURSOR_WIDTH, CURSOR_HEIGHT);
	drawCursor->SetColor(1.0f, 1.0f, 1.0f);
	//	�I�u�W�F�N�g�̐ݒ�
	transCursor->SetPosition(0.0f, 0.0f);						//	���W
	homingCursor->SetEffectNum(CURSOR_EFFECT_NUM);
	// ���X�g�ɒǉ�
	m_LoadScreenObjects.push_back(pCursor);
#pragma endregion


	//2022/1/31/Load_Tutorial Shimizu Yosuke -----------------------------
#pragma region ---�`���[�g���A��
	//�I�u�W�F�N�g�̐���
	Object* pLoad_Tutorial = new Object(LOAD_TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto TransTutorial = pLoad_Tutorial->AddComponent<CTransform>();			//�g�����X�t�H�[��
	auto DrawTutorial = pLoad_Tutorial->AddComponent<CDraw2D>();				//2D�`��
	//������������̃R���|�[�l���g�̒ǉ����K�v
	pLoad_Tutorial->AddComponent<CTutorial>();	//�`���[�g���A��
	//�I�u�W�F�N�g�̐ݒ�
	TransTutorial->SetPosition(-75.0f, 75.0f);										//���W�̐ݒ�
	DrawTutorial->SetTexture(pTexManager->GetTexture(TUTORIAL_UI_TEX_NUM));			//�e�N�X�`���̐ݒ�
	DrawTutorial->SetSize(TUTORIAL_SIZE_X - 40.0f, TUTORIAL_SIZE_Y - 40.0f);			//�傫��
	//���X�g�ɒǉ�
	m_LoadScreenObjects.push_back(pLoad_Tutorial);

#pragma endregion

#pragma region ---���邭��
	//�I�u�W�F�N�g�̐���
	Object* pLoad_Cursor = new Object(LOAD_TUTORIAL_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto TransTutorial2 = pLoad_Cursor->AddComponent<CTransform>();			//�g�����X�t�H�[��
	auto DrawTutorial2 = pLoad_Cursor->AddComponent<CDraw2D>();				//2D�`��
	//������������̃R���|�[�l���g�̒ǉ����K�v
	pLoad_Cursor->AddComponent<CTutorial>();								//�`���[�g���A��
	pLoad_Cursor->AddComponent<CLotation>();
	//�I�u�W�F�N�g�̐ݒ�
	TransTutorial2->SetPosition(-120.0f, 130.0f);										//���W�̐ݒ�
	DrawTutorial2->SetTexture(pTexManager->GetTexture(STOP_GAME_TEX_NUM));			//�e�N�X�`���̐ݒ�
	DrawTutorial2->SetSize(STOP_GAME_SIZE_X, STOP_GAME_SIZE_Y);					//�傫��
	DrawTutorial2->SetColor(0.2f, 0.2f, 0.6f);
	//���X�g�ɒǉ�
	m_LoadScreenObjects.push_back(pLoad_Cursor);
#pragma endregion
	//--------------------------------------------------------------------


	// �S�I�u�W�F�N�g�̏�����
	for (auto object : m_LoadScreenObjects)
		object->Start();
}

// ���[�h��ʂ̃I�u�W�F�N�g�I������
void Load::Uninit()
{
	// �I�u�W�F�N�g�̊J��
	for (auto object : m_LoadScreenObjects)
		delete object;

	// ���X�g����ɂ���
	m_LoadScreenObjects.clear();
}

// ���[�h��ʂ̊J�n
void Load::Begin()
{
	// ���[�h��ʏ�����
	Load::Init();

	// ���[�h��ʊJ�n�̂��m�点
	m_bLoading = true;

	// ���[�h��ʂ̂̃X���b�h�����
	m_handle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Load::LoadScreen, NULL, 0, NULL);
}

// ���[�h��ʂ̏I��
void Load::End()
{
	// ���[�h�������I����Ă��w�肳�ꂽ�t���[�����̓��[�h��ʂ�\������
	while (m_nLoadCount < LOAD_FRAME * 60)
	{
		Sleep(1);	// �ҋ@
	}

	// ���[�h��ʏI���̂��m�点
	m_bLoading = false;

	// ���[�h��ʂ̃X���b�h���I������܂ő҂�
	WaitForSingleObject(m_handle, INFINITE);

	// �n���h�������
	CloseHandle(m_handle);

	// ���[�h��ʂ̃I�u�W�F�N�g���X�g�̊J��
	Load::Uninit();
}

// ���[�h��ʂ̍X�V���`��
unsigned __stdcall Load::LoadScreen()
{
	// �t���[������p
	DWORD dwExecLastTime = timeGetTime();	//	�Ō�Ɏ��s���������iexecutive�����s�j
	DWORD dwCurrentTime	 = 0;				//	���ݎ���

	// ���[�h���Ԃ̃J�E���^�̏�����
	m_nLoadCount = 0;

	// ���[�h��ʏI���̂��m�点������܂Ń��[�v
	while (m_bLoading)
	{
		//	�V�X�e���������~���b�P�ʂŎ擾
		dwCurrentTime = timeGetTime();
		
		// �t���[������(1/60�b���ƂɎ��s)
		if (dwCurrentTime - dwExecLastTime >= 1000 / 60)
		{
			dwExecLastTime = dwCurrentTime;	//	�Ō�Ɏ��s�������� = ���ݎ���

			//--- �X�V
			InputManager::Instance()->Update();		// ����
			CCamera::Get()->Update();				// �J����
			for (auto object : m_LoadScreenObjects)	// �I�u�W�F�N�g
				object->Update();

			// �X�N���[�����W���烏�[���h���W�ɕϊ�
			InputManager* Input = InputManager::Instance();
			CCamera* pCamera = CCamera::Get();
			XMFLOAT2 vPos = Input->GetMousePos();
			XMMATRIX view = DirectX::XMLoadFloat4x4(&pCamera->GetViewMatrix());
			XMMATRIX prj = DirectX::XMLoadFloat4x4(&pCamera->GetProjMatrix());
			XMFLOAT3 vWorldPos;
			CalcScreenToXY(&vWorldPos, vPos.x, vPos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view, prj);
			// �J�[�\���G�t�F�N�g�̍��W
			vWorldPos.z = CURSOR_POS_Z;
			EffectManager::GetInstance()->SetPosition(CURSOR_EFFECT_NUM, vWorldPos);
			// �G�t�F�N�V�A�X�V
			EffectManager::GetInstance()->Update();

			// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
			float ClearColor[4] = { 0.117647f, 0.254902f, 0.352941f, 1.0f };
			GetDeviceContext()->ClearRenderTargetView(GetRenderTargetView(), ClearColor);
			GetDeviceContext()->ClearDepthStencilView(GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//--- �`��
			for (auto object : m_LoadScreenObjects)	// �I�u�W�F�N�g
				object->Draw();
			EffectManager::GetInstance()->Draw();	// �G�t�F�N�V�A

			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
			GetSwapChain()->Present(0, 0);

			// ���[�h���Ԃ��J�E���g
			m_nLoadCount++;
		}
	}

	// �X���b�h�I��
	_endthreadex(0);

	return 0;
}

// ���[�h��ʂ̃t���[�����擾
int Load::GetLoadCount()
{
	return m_nLoadCount;
}