/**
 * @file	EffekseerManager.cpp
 * @brief	�G�t�F�N�V�A�̃G�t�F�N�g���Ǘ�
 * @author	�u���Č�
 * @date	2021/12/18		�쐬
 */

//====== �C���N���[�h =====
#include "EffectManager.h"
#include "Camera.h"


//====== �}�N����` =====



//===== �ÓI�����o�ϐ� =====
EffectManager* EffectManager::m_pInstance = nullptr;


/**
 * @fn			EffectManager::EffectManager
 * @brief		�R���X�g���N�^
*/
EffectManager::EffectManager(){
}

/**
 * @fn			EffectManager::~EffectManager
 * @brief		�f�X�g���N�^
*/
EffectManager::~EffectManager(){
}

/**
 * @fn			EffectManager::GetInstance
 * @brief		�C���X�^���X����
*/
EffectManager* EffectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new EffectManager();
	}
	return m_pInstance;
}

/**
 * @fn			EffectManager::Destroy
 * @brief		�C���X�^���X�j��
*/
void EffectManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
 * @fn			EffectManager::Start
 * @brief		Effekseer������
*/
void EffectManager::Start()
{
	// Effekseer������
	m_renderer = EffekseerRendererDX11::Renderer::Create(GetDevice(), GetDeviceContext(), 8000);
	m_manager = Effekseer::Manager::Create(8000);
	// �`�惂�W���[���̐ݒ�
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

/**
 * @fn			EffectManager::Uninit
 * @brief		effekseer�I������
*/
void EffectManager::Uninit()
{
	m_manager.Reset();
	m_renderer.Reset();
}

/**
 * @fn			EffectManager::Update
 * @brief		Effekseer�X�V
*/
void EffectManager::Update()
{
	CCamera* pCamera = CCamera::Get();

	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		pCamera->GetFOV(),
		pCamera->GetAspect(),
		pCamera->GetNearClip(),
		pCamera->GetFarClip()));
	
	// �J�����s���ݒ�
	DirectX::XMFLOAT3 cameraPos  = pCamera->GetPos();
	DirectX::XMFLOAT3 cameraLook = pCamera->GetLook();
	DirectX::XMFLOAT3 cameraUp   = pCamera->GetUp();
	Effekseer::Vector3D eye  = ::Effekseer::Vector3D(cameraPos.x,  cameraPos.y,  cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up   = ::Effekseer::Vector3D(cameraUp.x,   cameraUp.y,   cameraUp.z);
	m_renderer->SetCameraMatrix(::Effekseer::Matrix44().LookAtLH(eye, look, up));
}

/**
 * @fn			EffectManager::Draw
 * @brief		�`��
*/
void EffectManager::Draw()
{
	// Effekseer
	static int time = 0;
	m_manager->Update();					// �`�摬�x���ς�����
	m_renderer->SetTime(time++ / 60.0f);
	m_renderer->BeginRendering();
	m_manager->Draw();
	m_renderer->EndRendering();

}


/**
 * @fn			EffectManager::AddEffect
 * @brief		�G�t�F�N�g�̒ǉ�
 * @param[in]	const char16_t*	�t�@�C����
 * @param[in]	effectID		�G�t�F�N�g�R�t����L�[
 * @return		������ �C���X�^���X��Ԃ�, ���s�� nullptr
*/
Effekseer::EffectRef EffectManager::AddEffect(const char16_t* pszFilname, int effectID)
{
	//*** ���łɂ�������Ԃ�
	if (m_EffectList[effectID] != nullptr) return m_EffectList[effectID];

	//*** map�ɒǉ�
	m_EffectList[effectID] = Effekseer::Effect::Create(m_manager, pszFilname);

	if (m_EffectList[effectID] == nullptr)
	{
		//*** �ǂݍ��܂�Ȃ������ꍇ
		MessageBoxA(GetMainWnd(), "�G�t�F�N�g�ǂݍ��݃G���[", "AddEffect", MB_OK);
		return nullptr;
	}

	// �ǉ������G�t�F�N�g�̃C���X�^���X��Ԃ�
	return m_EffectList[effectID];
}

/**
 * @fn			EffectManager::GetEffect
 * @brief		�G�t�F�N�g�̃C���X�^���X�擾
 * @param[in]	effectID		�G�t�F�N�g�R�t����L�[
 * @return		������ �C���X�^���X��Ԃ�
*/
Effekseer::EffectRef EffectManager::GetEffect(int effectID)
{
	return m_EffectList[effectID];
}


/**
 * @fn			EffectManager::Play
 * @brief		�G�t�F�N�g�Đ�
 * @param[in]	effectID		�G�t�F�N�g�ƕR�t����L�[
 * @param[in]	pos				�G�t�F�N�g�̍��W
 * @param[in]	scale			�G�t�F�N�g�̊g�嗦
*/
void EffectManager::Play(int effectID, XMFLOAT3 pos, XMFLOAT3 scale)
{
	if (m_EffectList[effectID] == nullptr) return;

	// �G�t�F�N�g�Đ�
	m_handle[effectID] = m_manager->Play(m_EffectList[effectID], pos.x, pos.y, pos.z);

	// �X�P�[��
	m_manager->SetScale(m_handle[effectID], scale.x, scale.y, scale.z);
}


/**
 * @fn			EffectManager::UpdatePosition
 * @brief		�w�肵���G�t�F�N�g�̍��W�̐ݒ�B
 * @param[in]	effectID		�G�t�F�N�g�ƕR�t����L�[
 * @param[in]	pos				�G�t�F�N�g�̍��W
*/
void EffectManager::SetPosition(int effectID, XMFLOAT3 pos)
{
	m_manager->SetLocation(m_handle[effectID], pos.x, pos.y, pos.z);
}

/**
 * @fn			EffectManager::SetRotation
 * @brief		�w�肵���G�t�F�N�g�̉�]�̐ݒ�B
 * @param[in]	effectID		�G�t�F�N�g�ƕR�t����L�[
 * @param[in]	rot				�G�t�F�N�g�̍��W
*/
void EffectManager::SetRotation(int effectID, XMFLOAT3 rot)
{
	m_manager->SetRotation(m_handle[effectID], rot.x, rot.y, rot.z);
}

/**
 * @fn			EffectManager::SetScale
 * @brief		�w�肵���G�t�F�N�g�̊g��k������B
 * @param[in]	effectID		�G�t�F�N�g�ƕR�t����L�[
 * @param[in]	rot				�G�t�F�N�g�̍��W
*/
void EffectManager::SetScale(int effectID, XMFLOAT3 scale)
{
	m_manager->SetScale(m_handle[effectID], scale.x, scale.y, scale.z);
}

/**
 * @fn			EffectManager::SetPaused
 * @brief		�w�肵���G�t�F�N�g�̃p�[�e�B�N�����ꎞ��~�A�������͍ĊJ����B
 * @param[in]	effectID		�G�t�F�N�g�ƕR�t����L�[
 * @param[in]	bool			true �ꎞ��~, false �ĊJ
*/
void EffectManager::SetPaused(int effectID, bool pause)
{
	m_manager->SetPaused(m_handle[effectID], pause);
}