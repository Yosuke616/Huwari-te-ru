#include "SpiderWebComponent.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "imgui.h"
#include "DrawMeshComponent.h"
#include "DrawEffectComponent.h"
#include "AnimationComponent.h"
#include "Draw3dComponent.h"
#include "PlayerComponent.h"
#include "Sound.h"
#include "TextureManager.h"
#include "CloverComponent.h"

//***�R���X�g���N�^
CSpiderWeb::CSpiderWeb()
	: m_bUpColFlg(false)
	, m_bDownColFlg(false)
	, m_pTransform(nullptr)
	, m_nWindCnt(0)
	, nCnt(0) //se
{

}
CSpiderWeb::~CSpiderWeb()
{
	// �N���[�o�[�̍S���t���O������
	for (auto&& object : m_BindObjects)
	{
		if (object.first->GetName() == CLOVER_NAME) object.first->GetComponent<CloverComponent>()->Binded(false);
	}

	m_BindObjects.clear();
}
//***������
void CSpiderWeb::Start()
{
	//�e�I�u�W�F�N�g�̃R���|�[�l���g�擾
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();

}

//***�X�V
void CSpiderWeb::Update()
{
	//�㕔���Ɖ������̗����ɕ����ڐG���Ă�����
	if (m_bUpColFlg == true && m_bDownColFlg == true)
	{
		//�N���̑��j��SE�Đ�
		CSound::Play(SE_WEB_DESTROY);

		//�e�I�u�W�F�N�g���폜
		Parent->Delete();
		return;
	}


	for (auto&& Object : m_BindObjects)
	{
		if (Object.first == nullptr) continue;
		if (Object.first->GetDeleteFlag() == true) continue;
		//�ڐG�I�u�W�F�N�g���Œ�
		Object.first->GetComponent<CTransform>()->Pos.x = Object.second.x;
		Object.first->GetComponent<CTransform>()->Pos.y = Object.second.y;

		if (Object.first->GetName() == PLAYER_NAME)
		{
			//SE���[�v�Đ��p
			nCnt--;
			if (nCnt < 0)
			{
				CSound::Play(SE_BIND);
				nCnt = 30;	// �Đ��t���[��
			}

			// �A�j���[�V����
			Object.first->GetComponent<CPlayer>()->SetAnimState(PLAYER_BIND);
		}
	}
}

//***�`��
void CSpiderWeb::Draw()
{

}
// *** �Z�b�^�[�@�S���͈�
void CSpiderWeb::SetBindSize(float x, float y)
{
	m_BindSize.x = x;
	m_BindSize.y = y;
}


//***�����蔻��
void CSpiderWeb::OnCollisionEnter(Object* pObject)
{

	//�e�̓����蔻���������Ă���
	auto Owner = Parent->GetComponent<CCollider>();
	//�����蔻��̃|�C���^����T�C�Y�������Ă���
	auto OwnerSize = Owner->GetColliderSize();
	//�����蔻��̒��S���i�[����ϐ�
	auto OwnerPos = Owner->GetCenterPos();

	//�����̃I�u�W�F�N�g�̏Փ˔���R���|�[�l���g�������Ă���
	auto Other = pObject->GetComponent<CCollider>();
	//�����̃g�����X�t�H�[���R���|�[�l���g�������Ă���
	auto OtherTransform = pObject->GetComponent<CTransform>();
	//�����̏Փ˔���̒��S�������Ă���
	auto OtherPos = Other->GetCenterPos();
	//�����̓����蔻��̒u�����������Ă���
	auto OtherSize = Other->GetColliderSize();



	//�v���C���[�ƐڐG
	if (pObject->GetName() == PLAYER_NAME && m_BindFlags[pObject] == false)//m_bPlayerBindFlg == false)
	{
		//�S���͈͂Ƃ̐ڐG
		if (OwnerPos.x - m_BindSize.x / 2 < Other->GetCenterPos().x + OtherSize.x / 2 &&		// 2�̍��[ < 1�̉E�[
			Other->GetCenterPos().x - OtherSize.x / 2 < OwnerPos.x + m_BindSize.x / 2)		// 1�̍��[ < 2�̉E�[
		{
			if (OwnerPos.y - m_BindSize.y / 2 < Other->GetCenterPos().y + OtherSize.y / 2 &&	// 2�̏�[ < 1�̉��[
				Other->GetCenterPos().y - OtherSize.y / 2 < OwnerPos.y + m_BindSize.y / 2)	// 1�̏�[ < 2�̉��[

			{
				//�ڐG�I�u�W�F�N�g���Œ肷����W��ݒ�
				m_BindObjects[pObject] = XMFLOAT2(OtherTransform->Pos.x, OtherTransform->Pos.y);
				//�S���t���O��ON
				m_BindFlags[pObject] = true;

				//��A�������̓����蔻��t���O���ꎞ�I��false��
				m_bUpColFlg = false;
				m_bDownColFlg = false;
				//m_bPlayerBindFlg = true;
			}
		}
	}

	//���ƐڐG(�S�����)
	if (pObject->GetName() == WIND_NAME && m_BindFlags.size() != 0)//&& m_bPlayerBindFlg == true)
	{
		m_nWindCnt++;			//�������������񐔃J�E���^

		if (m_nWindCnt >= WIND_ATTACK_NUM)	//�������񂩓���������
		{
			//�N���̑��j��SE�Đ�
			CSound::Play(SE_WEB_DESTROY);

			//�e�I�u�W�F�N�g���폜
			Parent->Delete();

			for (auto&& player : m_BindObjects)
			{
				if (player.first->GetName() == PLAYER_NAME)
				{
					// �A�j���[�V����
					player.first->GetComponent<CPlayer>()->SetAnimState(PLAYER_IDLE);
				}
			}

			return;
		}
	}

	//���ƐڐG(�S����Ԃł͂Ȃ�)
	if (pObject->GetName() == WIND_NAME && m_BindFlags.size() == 0)//m_bPlayerBindFlg == false)
	{
		//�㕔���̓����蔻����W������
		m_UpColliderPos = XMFLOAT2(m_pCollider->GetCenterPos().x, m_pCollider->GetCenterPos().y + m_pCollider->GetColliderSize().y / 2 * m_pTransform->Scale.y);
		//�������̓����蔻����W������
		m_DownColliderPos = XMFLOAT2(m_pCollider->GetCenterPos().x, m_pCollider->GetCenterPos().y - m_pCollider->GetColliderSize().y / 2 * m_pTransform->Scale.y);

		//�㕔���ƐڐG
		if (m_bUpColFlg == false &&
			m_UpColliderPos.x <= Other->GetCenterPos().x + OtherSize.x / 2 &&
			m_UpColliderPos.x >= Other->GetCenterPos().x - OtherSize.x / 2 &&
			m_UpColliderPos.y <= Other->GetCenterPos().y + OtherSize.y / 2 &&
			m_UpColliderPos.y >= Other->GetCenterPos().y - OtherSize.y / 2)
		{
			//�ڐG�t���O��ON
			m_bUpColFlg = true;
			//�摜�؂�ւ��i�オ���Ă�N���̑�)
			Parent->GetComponent<CDrawEffect>()->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_TWO_TEX_NUM));

			//�N���̑��j��SE�Đ�
			CSound::Play(SE_WEB_DESTROY);
		}

		//�������ƐڐG
		else if (m_bDownColFlg == false &&
			m_DownColliderPos.x <= Other->GetCenterPos().x + OtherSize.x / 2 &&
			m_DownColliderPos.x >= Other->GetCenterPos().x - OtherSize.x / 2 &&
			m_DownColliderPos.y <= Other->GetCenterPos().y + OtherSize.y / 2 &&
			m_DownColliderPos.y >= Other->GetCenterPos().y - OtherSize.y / 2)
		{
			//�ڐG�t���O��ON
			m_bDownColFlg = true;
			//�摜�؂�ւ��i�������Ă�N���̑��j
			Parent->GetComponent<CDrawEffect>()->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_THREE_TEX_NUM));
			//�N���̑��j��SE�Đ�
			CSound::Play(SE_WEB_DESTROY);
		}
	}

	// *** �N���[�o�[�ƐڐG
	if (pObject->GetName() == CLOVER_NAME && m_BindFlags[pObject] == false)//m_bBindFlg == false)
	{
		//�S���͈͂Ƃ̐ڐG
		if (OwnerPos.x - m_BindSize.x / 2 < Other->GetCenterPos().x + OtherSize.x / 2 &&		// 2�̍��[ < 1�̉E�[
			Other->GetCenterPos().x - OtherSize.x / 2 < OwnerPos.x + m_BindSize.x / 2)			// 1�̍��[ < 2�̉E�[
		{
			if (OwnerPos.y - m_BindSize.y / 2 < Other->GetCenterPos().y + OtherSize.y / 2 &&	// 2�̏�[ < 1�̉��[
				Other->GetCenterPos().y - OtherSize.y / 2 < OwnerPos.y + m_BindSize.y / 2)		// 1�̏�[ < 2�̉��[

			{
				//�ڐG�I�u�W�F�N�g���Œ肷����W��ݒ�
				m_BindObjects[pObject] = XMFLOAT2(OtherTransform->Pos.x, OtherTransform->Pos.y);
				//�S���t���O��ON
				m_BindFlags[pObject] = true;

				// �N���̑��ɍS������Ă��܂�
				pObject->GetComponent<CloverComponent>()->Binded(true);
			}

		}
	}
}