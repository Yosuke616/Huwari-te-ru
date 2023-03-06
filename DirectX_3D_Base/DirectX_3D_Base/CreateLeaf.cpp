#include "CreateLeaf.h"
#include "TransformComponent.h"
#include "Draw2dComponent.h"
#include "ColliderComponent.h"
#include "fallleafComponent.h"
#include "DrawEffectComponent.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "sceneGame.h"
#include "fallleafComponent.h"
#include "SceneSelect.h"
#include "SceneManager.h"
#include "sceneTitle.h"

//	�R���X�g���N�^
CLeaf::CLeaf()
	:cnt(0)
{

}
//	�f�X�g���N�^
CLeaf::~CLeaf()
{
}

//	�Q�[���ł̍X�V
void CLeaf::Update()
{
	//�J�E���g���Z
	cnt++;

	//2022/1/9 Shimizu Yosuke --------------------------------------------------
	switch (SceneGame::GetInstance()->GetSeason()) {
	case SPRING_1:	case SPRING_2:	//�t������10�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case SUMMER_1:	case SUMMER_2:	//�ā�����40�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case FALL_1:	case FALL_2:	//�H������20�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case WINTER_1:	case WINTER_2:	//�~������5�t���[�����Ƃɐ��`��
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
}

// �^�C�g���ł̍X�V
void CLeaf::UpdateTitleScene()
{
	//�J�E���g���Z
	cnt++;
	int nSeason;
	nSeason = SceneTitle::m_nTitleSeason;

	//2022/1/9 Shimizu Yosuke --------------------------------------------------
	switch (nSeason) {
	case 0:
		//�t������10�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 1:
		//�ā�����40�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 2:
		//�H������20�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 3:
		//�~�������t�F�[�h���ђʂ��Ă���
		//5�t���[�����Ƃɐ��`��
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
	//--------------------------------------------------------------------------
}

// �Z���N�g�ł̍X�V
void CLeaf::UpdateSelectScene()
{
	//�J�E���g���Z
	cnt++;

	switch (SceneSelect::m_nCurrentSeason)
	{
	case 0:	//�t������10�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 10)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 1:	//�ā�����40�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 40)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 2:	//�H������20�t���[�����Ƃɂ͂��ς�`��
		if (cnt >= 20)
		{
			CreateLeaf();
			cnt = 0;
		}
		break;
	case 3:	//�~������5�t���[�����Ƃɐ��`��
		if (cnt >= 1) {
			CreateLeaf();
			cnt = 0;
		}
		break;
	}
}

//	�t���ϐ���
void CLeaf::CreateLeaf()
{
	switch (SceneManager::Instance()->GetScene())
	{
	case SCENE_TITLE:	//	�^�C�g�����
		//��ԏ�
	//�����ł��t�ďH�~�ŕς����悤�ɂ�����
	//2022/1/7 Shimizu Yosuke -------------------------------------------------
		int nSeason;
		nSeason = SceneTitle::m_nTitleSeason;

		//�~�ȊO
		if (nSeason != 3)
		{
			//�����_���ŗt���ς̏o���ʒu�̌���
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	��]���x�������_���ɂ���
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//�I�u�W�F�N�g�̐���
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//�`��
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pLeaf);

			//����������̃I�u�W�F�N�g����낤�Ƃ�����i�I�u�W�F�N�g����������ɓ\��j
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//�����ɗ����̃I�u�W�F�N�g���쐬����
			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//�R���|�[�l���g�̒ǉ�
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//�g�����X�t�H�[��
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//�`��
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //�t���ς̗���	 
			//�I�u�W�F�N�g�̐ݒ�
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			//--------------------------------------------------------------------------------------------------------

			//�I�u�W�F�N�g����������ɓ\��
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//������ύX�ł���悤�ɂ���i�e�N�X�`���������ŕύX����j
			switch (nSeason) {
				//�t
			case 0:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//��
			case 1:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//�H
			case 2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
			//---------------------------------------------------------------------------------------------------------

		}
		//�~�̂Ƃ�
		else
		{
			//===== ��ver

			//�����_���̏ꏊ����o��悤�ɂ���
			//��ׂŎʂ��Ă���ꏊ�̃��[���h���W���o��
			XMFLOAT2 WorldSize = CalcWorldSize();
			//�ڂ��Ă���ꏊ�ɂ����~�点��悤�ɂ���
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//��ׂ̈ڂ��Ă���͈͂̃G�b�N�X���W�Ɉڂ�
			//���̎��̂܂܂ł͉�ʂ̉E�������o�Ă��Ȃ�
			//���͉E�����ɂ͏o�����邯�Ǎ��ɏo�Ȃ�
			float RandLeafX = rand() % ((int)WorldSize.x) + CameraScroll.x - 200.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//��]�������͂���Ȃ�

			//�I�u�W�F�N�g�̍쐬
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transSnow = pSnow->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//�`��
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//�e�N�X�`���̐ݒ�
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//���[�v�t���O���I���ɂ���
			drawSnow->SetLoop(true);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pSnow);
		}

		break;

	case SCENE_GAME:	//	�Q�[�����
		//��ԏ�
	//�����ł��t�ďH�~�ŕς����悤�ɂ�����
	//2022/1/7 Shimizu Yosuke -------------------------------------------------
		Season Season;
		Season = SceneGame::GetInstance()->GetSeason();

		//�t�ďH�Ɠ~�ō����̂𕪂���(if (Season != WINTER_1 && Season != WINTER_2) ��ǉ����Ăق����擪��)
		if (Season != WINTER_1 && Season != WINTER_2)
		{
			//�����_���ŗt���ς̏o���ʒu�̌���
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	��]���x�������_���ɂ���
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//�I�u�W�F�N�g�̐���
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//�`��
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pLeaf);

			//����������̃I�u�W�F�N�g����낤�Ƃ�����i�I�u�W�F�N�g����������ɓ\��j
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//�����ɗ����̃I�u�W�F�N�g���쐬����
			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//�R���|�[�l���g�̒ǉ�
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//�g�����X�t�H�[��
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//�`��
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //�t���ς̗���	 
			//�I�u�W�F�N�g�̐ݒ�
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			//--------------------------------------------------------------------------------------------------------

			//�I�u�W�F�N�g����������ɓ\��
			//2022/1/7 Shimizu Yosuke --------------------------------------------------------------------------------
			//������ύX�ł���悤�ɂ���i�e�N�X�`���������ŕύX����j
			switch (Season) {
				//�t
			case SPRING_1:
			case SPRING_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//��
			case SUMMER_1:
			case SUMMER_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//�H
			case FALL_1:
			case FALL_2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
			//---------------------------------------------------------------------------------------------------------

		}
		else
		{
			//===== ��ver

			//�����_���̏ꏊ����o��悤�ɂ���
			//��ׂŎʂ��Ă���ꏊ�̃��[���h���W���o��
			XMFLOAT2 WorldSize = CalcWorldSize();
			//�ڂ��Ă���ꏊ�ɂ����~�点��悤�ɂ���
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//��ׂ̈ڂ��Ă���͈͂̃G�b�N�X���W�Ɉڂ�
			//���̎��̂܂܂ł͉�ʂ̉E�������o�Ă��Ȃ�
			//���͉E�����ɂ͏o�����邯�Ǎ��ɏo�Ȃ�
			float RandLeafX = rand() % ((int)WorldSize.x)*2 + CameraScroll.x - 300.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//��]�������͂���Ȃ�

			//�I�u�W�F�N�g�̍쐬
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transSnow = pSnow->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//�`��
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//�e�N�X�`���̐ݒ�
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//���[�v�t���O���I���ɂ���
			drawSnow->SetLoop(true);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pSnow);
		}
		//---------------------------------------------------------------------------------------------

		break;

	case SCENE_SELECT:
		nSeason = SceneSelect::m_nCurrentSeason;

		//�~�ȊO
		if (nSeason != 3)
		{
			//�����_���ŗt���ς̏o���ʒu�̌���
			XMFLOAT2 WorldSize = CalcWorldSize();
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();
			float RandLeafX = rand() % ((int)WorldSize.x - (int)WorldSize.x / 2) + CameraScroll.x;
			float RandLeafZ = rand() % 400 - 200.0f;

			//	��]���x�������_���ɂ���
			float RotationVel_X = rand() % 5 + -2.5f;
			float RotationVel_Y = rand() % 3 + -1.5f;
			float RotationVel_Z = rand() % 4 + -2.0f;

			//�I�u�W�F�N�g�̐���
			Object* pLeaf = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transLeaf = pLeaf->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawLeaf = pLeaf->AddComponent<CDrawEffect>();			//�`��
			auto Leaf = pLeaf->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transLeaf->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf->SetSize(10.0f, 10.0f);
			drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
			drawLeaf->SetLoop(true);
			Leaf->SetRotVelocity(RotationVel_X, RotationVel_Y, RotationVel_Z);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pLeaf);

			Object* pLeaf_back = new Object(LEAF_NAME, UPDATE_UI, DRAW_UI);
			//�R���|�[�l���g�̒ǉ�
			auto transLeaf_back = pLeaf_back->AddComponent<CTransform>();//�g�����X�t�H�[��
			auto drawLeaf_back = pLeaf_back->AddComponent<CDrawEffect>();//�`��
			auto Leaf_back = pLeaf_back->AddComponent<CFallLeaf>();		 //�t���ς̗���	 
			//�I�u�W�F�N�g�̐ݒ�
			transLeaf_back->SetRotate(0.0f, 180.0f, 0.0f);
			transLeaf_back->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawLeaf_back->SetSize(10.0f, 10.0f);
			drawLeaf_back->SetNormal(0.0f, 1.0f, 0.0f);

			switch (nSeason) {
				//�t
			case 0:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				drawLeaf_back->SetTexture(TextureManager::GetInstance()->GetTexture(LEAF_SPRING_NUM));
				break;
				//��
			case 1:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(SUMMER_LEAF_TEX_NUM));
				break;
				//�H
			case 2:
				drawLeaf->SetTexture(TextureManager::GetInstance()->GetTexture(FALL_LEAF_TEX_NUM));
				break;
			}
		}
		//�~�̂Ƃ�
		else
		{
			//===== ��ver

			//�����_���̏ꏊ����o��悤�ɂ���
			//��ׂŎʂ��Ă���ꏊ�̃��[���h���W���o��
			XMFLOAT2 WorldSize = CalcWorldSize();
			//�ڂ��Ă���ꏊ�ɂ����~�点��悤�ɂ���
			XMFLOAT3 CameraScroll = CCamera::Get()->GetPos();

			//��ׂ̈ڂ��Ă���͈͂̃G�b�N�X���W�Ɉڂ�
			//���̎��̂܂܂ł͉�ʂ̉E�������o�Ă��Ȃ�
			//���͉E�����ɂ͏o�����邯�Ǎ��ɏo�Ȃ�
			float RandLeafX = rand() % ((int)WorldSize.x) + CameraScroll.x-200.0f;
			float RandLeafZ = rand() % 400 - 200.0f;

			//��]�������͂���Ȃ�

			//�I�u�W�F�N�g�̍쐬
			Object* pSnow = new Object(SNOW_NAME, UPDATE_UI, DRAW_UI);
			//	�R���|�[�l���g�̒ǉ�
			auto transSnow = pSnow->AddComponent<CTransform>();			//�g�����X�t�H�[��
			auto drawSnow = pSnow->AddComponent<CDrawEffect>();			//�`��
			auto Snow = pSnow->AddComponent<CFallLeaf>();				//�t���ς̗���
			//	�I�u�W�F�N�g�̐ݒ�
			transSnow->SetPosition(RandLeafX, WorldSize.y, RandLeafZ);
			drawSnow->SetSize(5.0f, 5.0f);

			//�e�N�X�`���̐ݒ�
			drawSnow->SetTexture(TextureManager::GetInstance()->GetTexture(SNOW_TEX_NUM));

			//���[�v�t���O���I���ɂ���
			drawSnow->SetLoop(true);

			//���X�g�ɒǉ�
			ObjectManager::GetInstance()->AddObject(pSnow);
		}

		break;

	default: break;
	}
}


