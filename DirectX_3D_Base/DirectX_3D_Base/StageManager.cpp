//===== �C���N���[�h
#include "StageManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "Object.h"
#include "ObjectInfo.h"
#include "imgui.h"
#include "Camera.h"
// �R���|�[�l���g
#include "TransformComponent.h"
#include "BillboardComponent.h"
#include "ColliderComponent.h"
#include "DrawEffectComponent.h"
#include "WindComponent.h"
#include "Draw3dComponent.h"
#include "Draw2dComponent.h"
#include "ModelManager.h"
#include "WindManager.h"
#include "SeedComponent.h"
#include "CloverComponent.h"
#include "GravityComponent.h"
#include "InformationComponent.h"
#include "FollowCompomemt.h"
#include "PostCollisionComponent.h"
#include "IcicleComponent.h"
#include "SpiderWebComponent.h"
#include "FallenLeavesComponent.h"
#include "BillboardComponent.h"
#include "DeleteTimerCom.h"
#include "FallComponent.h"
#include "BudComponent.h"
#include "IceBlockCom.h"
#include "OutRangeCom.h"
#include "MenuComponent.h"
#include "TutorialComponent.h"
#include "TagCom.h"

//===== �񋓑̐錾
enum MAP_ARRAY
{
	N = -1,					// �`��Ȃ�-1
	B = 0,					// �u���b�N0
	CLOVER,					// �N���[�o�[1
	GOAL,					// �A�ؔ�(�S�[��2
	KEY,					// ��(�L�[3
	ICICLE,					// ���4
	WEB,					// �N���̑�5
	LEAVES,					// �����t6
	ICE_BLOCK,				// �X7
	CLOVER_AND_LEAVES,		// �����t�ƃN���[�o�[8
	CLOVER_AND_ICE_BLOCK,	// �X�ƃN���[�o�[9
	BLOCK_LOW,				// �u���b�N�̉��̕�10
	

	MAX_CHIP
};

//=== �ÓI�����o�ϐ�
StageManager* StageManager::m_pInstance = nullptr;	//	�C���X�^���X

#ifdef _DEBUG
//ResourceCSV::IntRow g_editorStage;
#endif // _DEBUG

// �V���O���g��-------------------------------------
/**
 * @fn			StageManager::GetInstance
 * @brief		�C���X�^���X�擾
*/
StageManager* StageManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new StageManager();
	}

	return m_pInstance;
}

/**
 * @fn			StageManager::Destroy
 * @brief		�C���X�^���X�j��
*/
void StageManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//-----------------------------------------------


// load save-------------------------------------
/**
 * @fn			StageManager::Save
 * @brief		�X�e�[�W�̏�������
 * @param[in]	int		�G��
 * @detail		�����̋G�߂ɂ���ď������ݐ��csv�����߂�
*/
bool StageManager::Save(int Season)
{
	// �ۑ�����CSV�t�@�C�������߂�
	std::string file;
	switch (Season)
	{
	case SPRING_1: file =SPRING_1_STAGE_CSV; break;	// �t1
	case SPRING_2: file =SPRING_2_STAGE_CSV; break;	// �t2
	case SUMMER_1: file =SUMMER_1_STAGE_CSV; break;	// ��1
	case SUMMER_2: file =SUMMER_2_STAGE_CSV; break;	// ��2
	case FALL_1:   file =FALL_1_STAGE_CSV;   break;	// �H1
	case FALL_2:   file =FALL_2_STAGE_CSV;   break;	// �H2
	case WINTER_1: file =WINTER_1_STAGE_CSV; break;	// �~1
	case WINTER_2: file =WINTER_2_STAGE_CSV; break;	// �~2
	default: break;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
	if (CSV == nullptr) return false;

	// save
	if (CSV->Save(file, m_StageGrid) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("stage_save"), NULL, MB_OK);
		return false;
	}
	else
	{
		switch (Season)
		{
		case SPRING_1:	MessageBox(GetMainWnd(), L"spring_1.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// �t1
		case SPRING_2:	MessageBox(GetMainWnd(), L"spring_2.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// �t2
		case SUMMER_1:	MessageBox(GetMainWnd(), L"summer_1.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// ��1
		case SUMMER_2:	MessageBox(GetMainWnd(), L"summer_2.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// ��2
		case FALL_1:	MessageBox(GetMainWnd(), L"fall_1.csv�ɕۑ����܂���",   _T("����"), MB_OK); break;	// �H1
		case FALL_2:	MessageBox(GetMainWnd(), L"fall_2.csv�ɕۑ����܂���",   _T("����"), MB_OK); break;	// �H2
		case WINTER_1:	MessageBox(GetMainWnd(), L"winter_1.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// �~1
		case WINTER_2:	MessageBox(GetMainWnd(), L"winter_2.csv�ɕۑ����܂���", _T("����"), MB_OK); break;	// �~2
		default: break;
		}
	}

	return true;
}

/**
 * @fn			StageManager::Load
 * @brief		�X�e�[�W�̓ǂݍ���
 * @param[in]	string	�t�@�C����
*/
bool StageManager::Load(std::string file)
{
	// load
	if (FileManager::GetInstance()->Load(file) == false)
	{
		// �G���[���b�Z�[�W
		MessageBox(GetMainWnd(), _T("stage_load"), NULL, MB_OK);
		return false;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);

	//	�Q�[���p�̔z��Ɋi�[����
	m_StageGrid.resize(CSV->GetRowSize());				// �s�̐���ݒ�
	for (int y = 0; y < CSV->GetRowSize(); ++y)
	{
		m_StageGrid[y].resize(CSV->GetColumnSize(y));	// ��̐���ݒ�

		for (int x = 0; x < CSV->GetColumnSize(y); x++)
		{
			// �v�f���i�[����
			m_StageGrid[y][x] = CSV->GetInt(x, y);
		}
	}

#ifdef _DEBUG
	//g_editorStage.resize(CSV->GetRowSize());				// �s�̐���ݒ�
	//for (int y = 0; y < CSV->GetRowSize(); ++y)
	//{
	//	g_editorStage[y].resize(CSV->GetColumnSize(y));	// ��̐���ݒ�
	//
	//	for (int x = 0; x < CSV->GetColumnSize(y); x++)
	//	{
	//		// �v�f���i�[����
	//		g_editorStage[y][x] = CSV->GetInt(x, y);
	//	}
	//}
#endif // _DEBUG

	return true;
}
//-----------------------------------------------



// �������A�I���A�X�V�A�`��----------------------------
/**
 * @fn			StageManager::StageManager
 * @brief		������
*/
StageManager::StageManager()
	: m_block_num_x(0), m_block_num_y(0), m_nMap_Sta(N)
	, m_pShowObject(nullptr)
{
	//��ʂɉf���Ă��郏�[���h�̉����ƍ������v�Z
	m_WorldSize = CalcWorldSize();

	//	�X�e�[�W�`��̋N�_
	m_BasePos.x = -(m_WorldSize.x - MAPCHIP_WIDTH)  * 0.5f - MAPCHIP_WIDTH;
	m_BasePos.y =  (m_WorldSize.y - MAPCHIP_HEIGHT) * 0.5f;
}

/**
 * @fn			StageManager::~StageManager
 * @brief		�I��
*/
StageManager::~StageManager()
{
	if (m_pShowObject)
	{
		delete m_pShowObject;
		m_pShowObject = nullptr;
	}
}

/**
 * @fn			StageManager::Update
 * @brief		�X�V
 * @dtail		��ʊO�̃u���b�N�������A��ʓ��ɓ����Ă����u���b�N�����
*/
void StageManager::Update()
{
	// �ҏW���[�h
	if (SceneGame::GetInstance()->m_bEditMode)
	{
		//2021/12/29 Shimizu Yosuke -------------------------------------------------------------------------------
		//�}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����
		InputManager* Input = INPUT;
		XMFLOAT2 Mouse_Pos = Input->GetMousePos();
		XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
		XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
		XMFLOAT3 worldPos;		// �Z�o���ʂ��i�[����
		CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);
		
		// �z�u����I�u�W�F�N�g�̑I��
		if (Input->GetKeyTrigger(DIK_0))	m_nMap_Sta = N;						// �z�u���Ȃ�
		if (Input->GetKeyTrigger(DIK_1))	m_nMap_Sta = B;						// �u���b�N(�㑤)
		if (Input->GetKeyTrigger(DIK_2))	m_nMap_Sta = CLOVER;				// �N���[�o�[
		if (Input->GetKeyTrigger(DIK_3))	m_nMap_Sta = GOAL;					// �A�ؔ�
		if (Input->GetKeyTrigger(DIK_4))	m_nMap_Sta = KEY;					// ��
		if (Input->GetKeyTrigger(DIK_5))	m_nMap_Sta = ICICLE;				// ���
		if (Input->GetKeyTrigger(DIK_6))	m_nMap_Sta = WEB;					// �N���̑�
		if (Input->GetKeyTrigger(DIK_7))	m_nMap_Sta = LEAVES;				// �����t
		if (Input->GetKeyTrigger(DIK_8))	m_nMap_Sta = ICE_BLOCK;				// �X
		if (Input->GetKeyTrigger(DIK_9))	m_nMap_Sta = CLOVER_AND_LEAVES;		// �N���[�o�[�������t
		if (Input->GetKeyTrigger(DIK_W))	m_nMap_Sta = CLOVER_AND_ICE_BLOCK;	//�@�X�������t
		if (Input->GetKeyTrigger(DIK_Q))	m_nMap_Sta = BLOCK_LOW;				// �u���b�N(����
	
		// �v�f�����o�����߂̌v�Z������
		m_ElementNumX = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));
		m_ElementNumY = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));
		// �������W���v�Z
		float CreatePosX = m_BasePos.x + (m_ElementNumX) * MAPCHIP_WIDTH;
		float CreatePosY = m_BasePos.y - (m_ElementNumY) * MAPCHIP_HEIGHT;

		// �J�[�\���̍X�V
		this->UpdateShowObject(CreatePosX, CreatePosY);

		// �N���b�N�Ŕz�u
		if (Input->GetMouseTrigger(MOUSE_L))
		{
			// ��������ꏊ�̃u���b�NID���v�Z�iy�̗v�f�T�C�Y �~ x�̗v�f�T�C�Y + x�̗v�f�j
			int nMapChipID = (m_ElementNumY * m_block_num_x) + m_ElementNumX;
			
			// �����ꏊ�ɃI�u�W�F�N�g�����݂���ꍇ�͍폜���ĐV�����z�u������
			if (m_StageGrid[m_ElementNumY][m_ElementNumX] != -1)
			{
				// �I�u�W�F�N�g����
				Object* pDeleteObject = CTag::Find(nMapChipID);
				if(pDeleteObject)
				{
					pDeleteObject->Delete();
				}
			}

			// �X�e�[�W�u���b�N�̃X�e�[�^�X�̍X�V
			m_StageGrid[m_ElementNumY][m_ElementNumX] = m_nMap_Sta;

			// �I�u�W�F�N�g�̐���
			CreateBlock(CreatePosX, CreatePosY, m_nMap_Sta, nMapChipID);

			//�I�u�W�F�N�g�����(�������Wx, �������Wy, �X�e�[�W�z��)
			//CleateBlock_Deb(CreatePosX, CreatePosY, nMapChipID, m_ElementNumX, m_ElementNumY, m_nMap_Sta);
			//2021/12/30 narita
			// �t�@�C����������
			//std::string file;
			//switch (SceneGame::GetInstance()->GetSeason())
			//{
			//case SPRING_1:	file = SPRING_1_STAGE_CSV; break;	// �t1
			//case SPRING_2:	file = SPRING_2_STAGE_CSV; break;	// �t2
			//case SUMMER_1:	file = SUMMER_1_STAGE_CSV; break;	// ��1
			//case SUMMER_2:	file = SUMMER_2_STAGE_CSV; break;	// ��2
			//case FALL_1:	file = FALL_1_STAGE_CSV;   break;	// �H1
			//case FALL_2:	file = FALL_2_STAGE_CSV;   break;	// �H2
			//case WINTER_1:	file = WINTER_1_STAGE_CSV; break;	// �~1
			//case WINTER_2:	file = WINTER_2_STAGE_CSV; break;	// �~2
			//default: break;
			//}
			//ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
			//CSV->SetInt(m_ElementNumX, m_ElementNumY, m_nMap_Sta);
		}
	
		// �X�e�[�W��csv�ɕۑ�
		if (Input->GetKeyPress(DIK_LCONTROL) && Input->GetKeyTrigger(DIK_S))
		{
			this->Save(SceneGame::GetInstance()->GetSeason());
		}
	
		// �J�[�\���I�u�W�F�N�g�̍X�V
		if(m_pShowObject)	m_pShowObject->Update();
		//-----------------------------------------------------------------------------------------------------
	}
	else
	{
		// �ҏW���[�h�̏I���
		if (m_pShowObject)
		{
			delete m_pShowObject;
			m_pShowObject = nullptr;
		}
	}
}

/**
 * @fn			StageManager::Draw
 * @brief		debug�`��
*/
void StageManager::Draw()
{
	// �J�[�\���I�u�W�F�N�g�̕`��
	if (m_pShowObject)	m_pShowObject->Draw();

#ifdef _DEBUG
	using namespace ImGui;
	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Stage");
	for (int y = 0; y < m_StageGrid.size(); ++y)
	{
		NewLine();
		for (int x = 0; x < m_StageGrid[y].size(); x++)
		{
			Text("%d ", m_StageGrid[y][x]);
			SameLine();
		}
	}
	End();

#endif // _DEBUG
}
//-----------------------------------------------



// ����-------------------------------------------
/**
 * @fn			StageManager::CreateStage
 * @brief		�X�e�[�W�S�̂̐���
 * @param[in]	stage_state		�G��
*/
void StageManager::CreateStage(int stage_state)
{

	// �X�e�[�W�ǂݍ���
	switch (stage_state)	// �G�߂ɂ���ēǂݍ���csv��ς���
	{
	case SPRING_1:	this->Load(SPRING_1_STAGE_CSV); break;	// �t1
	case SPRING_2:	this->Load(SPRING_2_STAGE_CSV); break;	// �t2
	case SUMMER_1:	this->Load(SUMMER_1_STAGE_CSV); break;	// ��1
	case SUMMER_2:	this->Load(SUMMER_2_STAGE_CSV); break;	// ��2
	case FALL_1:	this->Load(FALL_1_STAGE_CSV);   break;	// �H1
	case FALL_2:	this->Load(FALL_2_STAGE_CSV);   break;	// �H2
	case WINTER_1:	this->Load(WINTER_1_STAGE_CSV); break;	// �~1
	case WINTER_2:	this->Load(WINTER_2_STAGE_CSV); break;	// �~2
	default: break;
	}

	float fBaseX = m_BasePos.x;
	float fBaseY = m_BasePos.y;
	int BlockID = 0;
	// 2022/01/26
	std::list<Object*> Clovers;	// �N���[�o�[�ɔԍ�������U�邽�߂̈ꎞ�I�Ȕz��

	//	�}�b�v�̔z�񂾂����[�v
	for (int y = 0; y < (int)m_StageGrid.size(); y++)
	{
		for (int x = 0; x < (int)m_StageGrid[y].size(); x++)
		{
			Object* pObject;
			// �u���b�N����
			pObject = CreateBlock(fBaseX, fBaseY, m_StageGrid[y][x], BlockID);

			// 2022/01/26 �N���[�o�[�𐶐�����I�u�W�F�N�g�������ꍇ�z��ɒǉ����Ă���
			if (m_StageGrid[y][x] == CLOVER ||
				m_StageGrid[y][x] == CLOVER_AND_LEAVES ||
				m_StageGrid[y][x] == CLOVER_AND_ICE_BLOCK)
			{
				Clovers.push_back(pObject);
			}

			fBaseX += MAPCHIP_WIDTH;	//	���̃u���b�N�̍��W��
			BlockID++;					//	���̃u���b�NID��
		}

		fBaseX = m_BasePos.x;			//	X���W���Z�b�g
		fBaseY -= MAPCHIP_HEIGHT;		//	���̃u���b�N�̍��W��
	}

	// �X�e�[�W�̗v�f�����i�[
	m_block_num_y = (int)m_StageGrid.size();	// y�̗v�f��
	for (auto row : m_StageGrid)
	{
		// 1�ԃT�C�Y���傫����𔽉f
		if (m_block_num_x < (int)row.size())
			m_block_num_x = (int)row.size();	// x�̗v�f��
	}

	// �X�e�[�W�̃T�C�Y���i�[
	m_vStageSize.x = m_block_num_x * MAPCHIP_WIDTH;		// �v�f�� x �u���b�N�̑傫��
	m_vStageSize.y = m_block_num_y * MAPCHIP_HEIGHT;

	/*	2022/01/22	�J�����̈ړ����E�̐ݒ�
		�N���A���ɐ������ʊO�̋�����Ԃ��f��Ώ� �X�N���[���͈͊O�ɍ��[�ɂP�s�A�E�[�ɂU�s���̃u���b�N������
	*/
	CCamera::Get()->SetLimit(XMFLOAT2(0.0f, m_vStageSize.x - m_WorldSize.x - (MAPCHIP_WIDTH * 7)));

	// �N���[�o�[�ɔԍ�������U��
	SetCloverNumber(Clovers);
}

/**
 * @fn			StageManager::SetCloverNumber
 * @brief		�N���[�o�[�ɔԍ�������U��
*/
void StageManager::SetCloverNumber(std::list<Object*>& Clovers)
{
	/*	2022/01/25	�N���[�o�[�ɔԍ�������U��
		�����珇�Ԃ�0, 1, 2, 3
	*/
	// ���W�ɂ���ĕ��ёւ���
	Clovers.sort([=](Object* A, Object* B)
		{
			// ���W���擾����
			auto PosA = A->GetComponent<CTransform>()->Pos;
			auto PosB = B->GetComponent<CTransform>()->Pos;

			// ���W���r
			return PosA.x < PosB.x;
		});

	// �ԍ�������U��
	int number = 0;
	for (auto&& clover : Clovers)
	{
		// �N���[�΁[�I�u�W�F�N�g�������ꍇ
		auto ComClover = clover->GetComponent<CloverComponent>();
		if (ComClover)
		{
			// �ԍ�������U��
			ComClover->SetNumber(number);
		}
		// �N���[�o�[�𐶐����闎���t�������ꍇ
		auto ComFallLeaves = clover->GetComponent<CFallenLeaves>();
		if (ComFallLeaves)
		{
			// �ԍ�������U��
			ComFallLeaves->SetNumber(number);
		}
		// �N���[�o�[�𐶐�����X�u���b�N�������ꍇ
		auto ComIceBlock = clover->GetComponent<CIceBlock>();
		if (ComIceBlock)
		{
			// �ԍ�������U��
			ComIceBlock->SetNumber(number);
		}

		// �ԍ����Z
		number++;
	}

	// �N���[�o�[�擾�󋵂̊m�F
	ResourceCSV* CloverCSV = (ResourceCSV*)FileManager::GetInstance()->Get(CLOVER_DATA_CSV);
	int nStage = SceneGame::GetInstance()->GetSeason() * 4;
	bool bClover[4];
	for (int nCloverNumber = 0; nCloverNumber < 4; nCloverNumber++)
	{
		bClover[nCloverNumber] = CloverCSV->GetInt(nStage + nCloverNumber, 0);

		// ���łɃN���[�o�[���擾�ς݂Ȃ�폜����
		if (bClover[nCloverNumber] == true)
		{
			// �ԍ��̐������C�e���[�V����
			std::list<Object*>::iterator itr = Clovers.begin();
			for (int n = 0; n < nCloverNumber; n++) itr++;

			// �폜
			(*itr)->Delete();

			if ((*itr)->GetName() == OTIBA_NAME)
			{
#pragma region--- �����t
				Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
				// components
				auto trans = obj->AddComponent<CTransform>();					// �I�u�W�F�N�g�̍��W
				auto draw = obj->AddComponent<CBillboard>();					// �G�t�F�N�g�`��@�\
				auto collider = obj->AddComponent<CCollider>();					// �����蔻��
				auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// �����t�̋@�\
				obj->AddComponent<COutRange>();									// ��ʊO����
				// settings
				XMFLOAT3 pos = (*itr)->GetComponent<CTransform>()->Pos;
				trans->SetPosition(pos.x, pos.y, OTIBA_POS_Z);					// ���W
				draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// �T�C�Y
				draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// �摜�̕�����
				draw->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));	// �e�N�X�`��
				collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// �����蔻��̑傫��
				// add to list
				ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion
			}
			else if ((*itr)->GetName() == ICE_BLOCK_NAME)
			{
#pragma region ---�X
				ModelManager* pModelManager = ModelManager::GetInstance();
				//�@�I�u�W�F�N�g�̐���
				Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
				// Components
				auto trans = pIceBlock->AddComponent<CTransform>();			// �I�u�W�F�N�g�̍��W
				auto draw = pIceBlock->AddComponent<CDrawModel>();				// ���f���@�\
				auto collider = pIceBlock->AddComponent<CCollider>();		// �����蔻��
				auto ice = pIceBlock->AddComponent<CIceBlock>();			// �X�u���b�N�̋@�\
				auto gravity = pIceBlock->AddComponent<CGravity>();			// �d��
				auto timer = pIceBlock->AddComponent<CTimer>();				// ���Ԃ��w�肵�č폜�ł���悤�ɂ���
				auto out_range = pIceBlock->AddComponent<COutRange>();		// ��ʊO����
				// Settings
				XMFLOAT3 pos = (*itr)->GetComponent<CTransform>()->Pos;
				trans->SetPosition(pos.x, pos.y, ICE_BLOCK_POS_Z_OFFSET);						// ���W
				trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);		// �傫��
				collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);// �����蔻��̑傫��
				out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);						  // �ǂ̈ʒu���珈�����Ȃ����̐ݒ�
				// ���[�u	(�X�u���b�N�̃A�j���[�V�����֌W)
				// �A�j���[�V���������郂�f��������C���X�^���X�𕪂��Ȃ��Ƃ����Ȃ�
				// �g�p���Ă��Ȃ��C���X�^���X�����������ꍇ������Z�b�g����
				if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//�@���f��1
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//�@���f��2
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//�@���f��3
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//�@���f��4
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//�@���f��5
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//�@���f��6
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//�@���f��7
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//�@���f��8
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//�@���f��9
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
				}
				else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
				{
					draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//�@���f��10
					SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
				}
				else
				{
					// �S�ẴC���X�^���X�����ݎg�p���̏ꍇ
					MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
				}
				draw->AddAnimation(0, 0.0, 8.0, true);
				draw->SetAnimTime(0);
				//�@�I�u�W�F�N�g�̒ǉ�
				ObjectManager::GetInstance()->AddObject(pIceBlock);
				draw->Update();
#pragma endregion
			}
		}
	}
}

/**
 * @fn			StageManager::CreateBlock
 * @brief		�u���b�N����
 * @param[in]	PosX		x���W
 * @param[in]	PosY		y���W
 * @param[in]	state		�I�u�W�F�N�g�i���o�[
 * @param[in]	nBlockID	�u���b�N�ƕR�t���Ă���L�[
*/
Object* StageManager::CreateBlock(float PosX, float PosY, int state, int nBlockID)
{
	// N�͉����������Ȃ�
	if (state == N)	return nullptr;
	
	// ���f���ƃe�N�X�`���}�l�[�W���[
	ModelManager* pModelManager = ModelManager::GetInstance();
	TextureManager* pTexManager = TextureManager::GetInstance();

	// �u���b�N(��̕�
	if (state == B)
	{
#pragma region ---�u���b�N(��̕�
		Object* obj = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		// components
		auto trans		= obj->AddComponent<CTransform>();			// �I�u�W�F�N�g���W
		auto Draw		= obj->AddComponent<CDrawModel>();				// ���f���`��
		auto Collider	= obj->AddComponent<CCollider>();			// �����蔻��
		auto comID		= obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		obj->AddComponent<COutRange>();								// ��ʊO����
		// settings
		trans->SetPosition(PosX, PosY, BLOCK_OFFSET_Z);					// ���W
		trans->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);	// �X�P�[��
		Collider->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);		// �����蔻��̑傫��
		Collider->SetOffset(BLOCK_OFFSET_X, BLOCK_OFFSET_Y);			// ���S���W�ƃ��f�����W�̒��S�̂���
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);

		// �l�G�ɂ���ăe�N�X�`����ς���
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// �t
			Draw->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
			break;
		case SUMMER_1:	case SUMMER_2:	// ��
			Draw->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
			break;
		case FALL_1:	case FALL_2:	// �H
			Draw->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
			break;
		case WINTER_1:	case WINTER_2:	// �~
			Draw->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
			break;
		default:break;
		}
		// ���[���h�}�g���b�N�X���X�V���Ă���
		Draw->Update();
#pragma endregion

		return obj;
	}
	// �큕�ڂ�
	else if (state == KEY)
	{
#pragma region  ---��
		//�@�I�u�W�F�N�g�̐���
		Object* Seed = new Object(SEED_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto TransSeed = Seed->AddComponent<CTransform>();		//�@�g�����X�t�H�[��
		auto DrawSeed = Seed->AddComponent<CDrawModel>();			//�@3D�`��@�\
		auto CollisionSeed = Seed->AddComponent<CCollider>();   //�@�Փˋ@�\
		auto ComSeed = Seed->AddComponent<CSeed>();				//�@��
		auto comID = Seed->AddComponent<CTag>();
		comID->SetID(nBlockID);
		Seed->AddComponent<COutRange>();
		// settings
		DrawSeed->SetModel(ModelManager::GetInstance()->GetModel(SEED_MODEL_NUM));//�@���f��
		TransSeed->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);	//�@�X�P�[��
		TransSeed->SetPosition(PosX, PosY + SEED_POS_OFFSET_Y, BUD_POS_OFFSET_Z);//�@�|�W�V����
		CollisionSeed->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT); //�@�����蔻��̑傫��
		// add to list
		ObjectManager::GetInstance()->AddObject(Seed);
		// ���[���h�}�g���b�N�X���X�V���Ă���
		DrawSeed->Update();
#pragma endregion

#pragma region ---�ڂ�
		Object* FlowerBud = new Object(BUD_NAME, UPDATE_MODEL, DRAW_MODEL);
		// compponents
		auto transFlowerBud = FlowerBud->AddComponent<CTransform>();			//	�I�u�W�F�N�g���W
		auto drawFlowerBud = FlowerBud->AddComponent<CDrawModel>();				//	���f���`��
		auto collider = FlowerBud->AddComponent<CCollider>();					//	���f���`��
		auto bud = FlowerBud->AddComponent<CBud>();								//	�ڂ�
		FlowerBud->AddComponent<COutRange>();
		// settings
		transFlowerBud->SetPosition(PosX, PosY + BUD_POS_OFFSET_Y, BUD_POS_OFFSET_Z);//	���W
		transFlowerBud->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);		//	�X�P�[��
		collider->SetCollisionSize(BUD_COLLISION_X, BUD_COLLISION_Y);			// �����蔻��̑傫��
		//	���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(FlowerBud);

		// �l�G�ɂ���ăe�N�X�`����ς���
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:
		case SPRING_2:	// �t
			drawFlowerBud->SetModel(pModelManager->GetModel(SPRING_BUD_MODEL_NUM));
			break;
		case SUMMER_1:
		case SUMMER_2:	// ��
			drawFlowerBud->SetModel(pModelManager->GetModel(SUMMER_BUD_MODEL_NUM));
			break;
		case FALL_1:
		case FALL_2:	// �H
			drawFlowerBud->SetModel(pModelManager->GetModel(FALL_BUD_MODEL_NUM));
			break;
		case WINTER_1:
		case WINTER_2:	// �~
			drawFlowerBud->SetModel(pModelManager->GetModel(WINTER_BUD_MODEL_NUM));
			break;
		default:break;
		}
		// �A�j���[�V�����̒ǉ�
		drawFlowerBud->AddAnimation(0, BUD_ANIM_1_START, BUD_ANIM_1_END, false);	// �ڂ݊J��
		drawFlowerBud->AddAnimation(1, BUD_ANIM_2_START, BUD_ANIM_2_END, false);	// �ڂݕ���

		// ���[���h�}�g���b�N�X���X�V���Ă���
		drawFlowerBud->Update();
#pragma endregion

		return Seed;
	}
	// �A�ؔ�
	else if (state == GOAL)
	{
#pragma region ---�A�ؔ�(�S�[��
	Object* pPlantPot = new Object(POT_NAME, UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto transPot		= pPlantPot->AddComponent<CTransform>();	// �I�u�W�F�N�g�̍��W
	auto drawPot		= pPlantPot->AddComponent<CDrawModel>();		// ���f���`��
	auto colliderPot	= pPlantPot->AddComponent<CCollider>();		// �����蔻��
	auto comID = pPlantPot->AddComponent<CTag>();
	comID->SetID(nBlockID);
	pPlantPot->AddComponent<COutRange>();							// ��ʊO����
	// settings
	transPot->SetPosition(PosX, PosY + POT_POS_OFFSET_Y, POT_POS_Z);// ���W
	transPot->SetScale(POT_SCALE_X, POT_SCALE_Y, POT_SCALE_Z);		// �X�P�[��
	drawPot->SetModel(ModelManager::GetInstance()->GetModel(POT_MODEL_NUM));// ���f��
	colliderPot->SetCollisionSize(MAPCHIP_WIDTH * 3, 5.0f);// �����蔻��̑傫��
	colliderPot->SetOffset(0.0f, 25.0f);
	// add to list
	ObjectManager::GetInstance()->AddObject(pPlantPot);
	// ���[���h�}�g���b�N�X���X�V���Ă���
	drawPot->Update();
#pragma endregion
	return pPlantPot;
	}
	// �N���[�o�[
	else if (state == CLOVER)
	{
#pragma region ---�N���[�o�[
		return CloverComponent::CreateClover(XMFLOAT3(PosX, PosY, 0.0f), nBlockID);
#pragma endregion
	}
	// ���
	else if (state == ICICLE)
	{
#pragma region	---����̕���
		Object* pUpIcicle = new Object(ICICLE_UP_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto transIcicle = pUpIcicle->AddComponent<CTransform>();		// �I�u�W�F�N�g�̍��W
		auto drawIcicle = pUpIcicle->AddComponent<CDrawModel>();			// �`��@�\
		auto upcolliderIcicle = pUpIcicle->AddComponent<CCollider>();	// �Փˋ@�\
		pUpIcicle->AddComponent<CIcicle>();								// ���̋@�\
		auto outRange = pUpIcicle->AddComponent<COutRange>();
		auto comID = pUpIcicle->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		transIcicle->SetPosition(PosX, PosY);							// ���S���W
		transIcicle->SetScale(ICICLE_SCALE_X, ICICLE_SCALE_Y, ICICLE_SCALE_Z);	//�@�傫��
		drawIcicle->SetModel(ModelManager::GetInstance()->GetModel(ICICLE_MODEL_NUM));// �e�N�X�`��
		upcolliderIcicle->SetCollisionSize(ICICLE_UP_COLLISION_SIZE_X, ICICLE_UP_COLLISION_SIZE_Y);	// �����蔻��̑傫��
		upcolliderIcicle->SetOffset(ICICLE_UP_COLLISION_OFFSET_X, ICICLE_UP_COLLISION_OFFSET_Y);	// �����蔻��̒��S���W�̂���
		// add to list
		ObjectManager::GetInstance()->AddObject(pUpIcicle);
#pragma endregion
#pragma region	---�����蔻������I�u�W�F�N�g�̐���(���̉���)
		Object* pdownIcicle = new Object(ICICLE_DOWN_NAME, UPDATE_MODEL, DRAW_MODEL);
		// components
		auto transdownIcicle = pdownIcicle->AddComponent<CTransform>();		// �I�u�W�F�N�g�̍��W
		auto downcolliderIcicle = pdownIcicle->AddComponent<CCollider>();	// �Փˋ@�\
		auto FollowIce = pdownIcicle->AddComponent<CFollow>();				// �Ǐ]�@�\
		auto InformationIce = pdownIcicle->AddComponent<CInformation>();	// ���̃I�u�W�F�N�g�����擾����@�\
		auto CollisionIce = pdownIcicle->AddComponent<CPostCollision>();	// �Փ˓��e
		pdownIcicle->AddComponent<COutRange>();
		// settings
		transdownIcicle->SetPosition(PosX, PosY);							// ���S���W
		downcolliderIcicle->SetCollisionSize(20.0f, 60.0f);					// �����蔻��̑傫��
		downcolliderIcicle->SetOffset(0, -20);								// �����蔻��̒��S���W�̂���
		FollowIce->SetParent(pUpIcicle);									// �Ǐ]����I�u�W�F�N�g�̐ݒ�
		InformationIce->SetObjectInfo(pUpIcicle);							// ��̕����̏����擾���Ă���
		// add to list
		ObjectManager::GetInstance()->AddObject(pdownIcicle);
#pragma endregion

		return pUpIcicle;
	}
	// �N���̑�
	else if (state == WEB)
	{
#pragma region ---�N���̑�
	Object* pSpiderWeb = new Object("spider", UPDATE_DEBUG, DRAW_DEBUG);
	// components
	auto TransSpider = pSpiderWeb->AddComponent<CTransform>();			// �I�u�W�F�N�g�̍��W
	auto ColSpider = pSpiderWeb->AddComponent<CCollider>();				// �����蔻��
	auto DrawSpider = pSpiderWeb->AddComponent<CDrawEffect>();			// �N���̑��`��
	auto SpiderCom = pSpiderWeb->AddComponent<CSpiderWeb>();			// �N���̑��̋@�\
	pSpiderWeb->AddComponent<COutRange>();								// ��ʊO����
	auto comID = pSpiderWeb->AddComponent<CTag>();
	comID->SetID(nBlockID);
	// settings
	DrawSpider->SetTexture(pTexManager->GetTexture(SPIDER_TEX_NUM));	// �e�N�X�`��
	TransSpider->SetPosition(PosX, PosY - SPIDER_OFFESET_Y);			// ���W
	TransSpider->SetRotate(0.0f, SPIDER_ROT_Y, 0.0f);					// �X��
	DrawSpider->SetSize(SPIDER_WIDTH, SPIDER_HEIGHT);					// �T�C�Y
	DrawSpider->SetLoop(true);											// �A�j���[�V�������[�v
	SpiderCom->SetBindSize(SPIDER_BIND_WIDTH, SPIDER_BIND_HEIGHT);		// �S���͈͂̐ݒ�
	ColSpider->SetCollisionSize(SPIDER_WIDTH, SPIDER_HEIGHT);			// �����蔻��̑傫��
	// add to list
	ObjectManager::GetInstance()->AddObject(pSpiderWeb);
#pragma endregion
		return pSpiderWeb;
	}
	// �����t
	else if (state == LEAVES)
	{
#pragma region--- �����t
		Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
		// components
		auto trans		  = obj->AddComponent<CTransform>();			// �g�����X�t�H�[��
		auto draw		  = obj->AddComponent<CBillboard>();			// �G�t�F�N�g�`��@�\
		auto collider	  = obj->AddComponent<CCollider>();				// �����蔻��
		auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// �����t�̋@�\
		obj->AddComponent<COutRange>();									// ��ʊO����
		auto comID = obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		trans->SetPosition(PosX, PosY + OTIBA_OFFESET_Y, OTIBA_POS_Z);	// ���W
		draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// �T�C�Y
		draw->SetTexture(pTexManager->GetTexture(OTIBA_TEX_NUM));		// �e�N�X�`��
		draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// �摜�̕�����
		collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// �����蔻��̑傫��
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion
		return obj;
	}
	// �X�u���b�N
	else if (state == ICE_BLOCK)
	{
#pragma region ---�X
		Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
		// Components
		auto trans		= pIceBlock->AddComponent<CTransform>();		// �I�u�W�F�N�g�̍��W
		auto draw		= pIceBlock->AddComponent<CDrawModel>();			// ���f���@�\
		auto collider	= pIceBlock->AddComponent<CCollider>();			// �����蔻��
		auto gravity	= pIceBlock->AddComponent<CGravity>();			// �d��
		auto ice		= pIceBlock->AddComponent<CIceBlock>();			// �X�u���b�N�̋@�\
		auto Timer		= pIceBlock->AddComponent<CTimer>();			// ���Ԃ��w�肵�č폜�ł���悤�ɂ���
		auto out_range	= pIceBlock->AddComponent<COutRange>();			// ��ʊO����
		auto comID = pIceBlock->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// Settings
		trans->SetPosition(PosX, PosY + ICE_BLOCK_POS_Y_OFFSET, ICE_BLOCK_POS_Z_OFFSET);	// ���W
		trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);			// �X�P�[��
		collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);	// �����蔻��̑傫��
		out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);							// �ǂ̈ʒu���珈�����Ȃ����̐ݒ�
		// ���[�u	(�X�u���b�N�̃A�j���[�V�����֌W)
		// �A�j���[�V���������郂�f��������C���X�^���X�𕪂��Ȃ��Ƃ����Ȃ�
		// �g�p���Ă��Ȃ��C���X�^���X�����������ꍇ������Z�b�g����
		if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//�@���f��1
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//�@���f��2
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//�@���f��3
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//�@���f��4
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//�@���f��5
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//�@���f��6
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//�@���f��7
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//�@���f��8
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//�@���f��9
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//�@���f��10
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
		}
		else
		{
			// �S�ẴC���X�^���X�����ݎg�p���̏ꍇ
			MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
		}
		draw->AddAnimation(0, 0.0, 8.0, true);	// �A�j���[�V�����̐ݒ�
		draw->SetAnimTime(0);					// �A�j���[�V�������ԏ�����
		// Add to list
		ObjectManager::GetInstance()->AddObject(pIceBlock);
		draw->Update();
#pragma endregion
		return pIceBlock;
	}
	// �����t���N���[�o�[
	else if (state == CLOVER_AND_LEAVES)
	{
#pragma region--- �����t
		Object* obj = new Object(OTIBA_NAME, UPDATE_BG, DRAW_DEBUG);
		// components
		auto trans		  = obj->AddComponent<CTransform>();			// �I�u�W�F�N�g�̍��W
		auto draw		  = obj->AddComponent<CBillboard>();			// �G�t�F�N�g�`��@�\
		auto collider	  = obj->AddComponent<CCollider>();				// �����蔻��
		auto FallenLeaves = obj->AddComponent<CFallenLeaves>();			// �����t�̋@�\
		obj->AddComponent<COutRange>();									// ��ʊO����
		auto comID = obj->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// settings
		trans->SetPosition(PosX, PosY + OTIBA_OFFESET_Y, OTIBA_POS_Z);	// ���W
		draw->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);						// �T�C�Y
		draw->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);				// �摜�̕�����
		draw->SetTexture(pTexManager->GetTexture(OTIBA_TEX_NUM));		// �e�N�X�`��
		collider->SetCollisionSize(OTIBA_COLLISION_WIDTH, OTIBA_COLLISION_HEIGHT);	// �����蔻��̑傫��
		// add to list
		ObjectManager::GetInstance()->AddObject(obj);
#pragma endregion

#pragma region ---�����t����������ɃN���[�o�[���o��
		FallenLeaves->Clover();
#pragma endregion

		return obj;
	}
	// �X�u���b�N���N���[�o�[
	else if (state == CLOVER_AND_ICE_BLOCK)
	{
#pragma region ---�X
		//�@�I�u�W�F�N�g�̐���
		Object* pIceBlock = new Object(ICE_BLOCK_NAME, UPDATE_MODEL, DRAW_MODEL);
		// Components
		auto trans		= pIceBlock->AddComponent<CTransform>();		// �I�u�W�F�N�g�̍��W
		auto draw		= pIceBlock->AddComponent<CDrawModel>();			// ���f���@�\
		auto collider	= pIceBlock->AddComponent<CCollider>();			// �����蔻��
		auto ice		= pIceBlock->AddComponent<CIceBlock>();			// �X�u���b�N�̋@�\
		auto gravity	= pIceBlock->AddComponent<CGravity>();			// �d��
		auto timer		= pIceBlock->AddComponent<CTimer>();			// ���Ԃ��w�肵�č폜�ł���悤�ɂ���
		auto out_range	= pIceBlock->AddComponent<COutRange>();			// ��ʊO����
		auto comID = pIceBlock->AddComponent<CTag>();
		comID->SetID(nBlockID);
		// Settings
		trans->SetPosition(PosX, PosY + ICE_BLOCK_POS_Y_OFFSET, ICE_BLOCK_POS_Z_OFFSET);  // ���W
		trans->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);		  // �傫��
		collider->SetCollisionSize(ICE_BLOCK_COLLISION_WIDTH, ICE_BLOCK_COLLISION_HEIGHT);// �����蔻��̑傫��
		out_range->SetLimitRange(OUT_RANGE_X - 30.0f, OUT_RANGE_Y);						  // �ǂ̈ʒu���珈�����Ȃ����̐ݒ�
		// ���[�u	(�X�u���b�N�̃A�j���[�V�����֌W)
		// �A�j���[�V���������郂�f��������C���X�^���X�𕪂��Ȃ��Ƃ����Ȃ�
		// �g�p���Ă��Ȃ��C���X�^���X�����������ꍇ������Z�b�g����
		if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1));		//�@���f��1
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM1)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2));		//�@���f��2
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM2)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3));		//�@���f��3
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM3)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4));		//�@���f��4
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM4)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5));		//�@���f��5
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM5)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6));		//�@���f��6
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM6)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7));		//�@���f��7
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM7)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8));		//�@���f��8
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM8)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9));		//�@���f��9
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM9)] = true;
		}
		else if (SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] == false)
		{
			draw->SetModel(pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10));		//�@���f��10
			SceneGame::IceManager[pModelManager->GetModel(ICE_BLOCK_MODEL_NUM10)] = true;
		}
		else
		{
			// �S�ẴC���X�^���X�����ݎg�p���̏ꍇ
			MessageBox(GetMainWnd(), _T("ICE_BLOCK has exceeded capacity."), NULL, MB_OK);
		}
		draw->AddAnimation(0, 0.0, 8.0, true);
		draw->SetAnimTime(0);
		//�@�I�u�W�F�N�g�̒ǉ�
		ObjectManager::GetInstance()->AddObject(pIceBlock);
		draw->Update();
#pragma endregion

#pragma region ---�X�u���b�N����ꂽ��ɃN���[�o�[���o��
		timer->SetFunction(FUNC_CREATE_CLOVER);
#pragma endregion

		return pIceBlock;
	}
	// �u���b�N�̉��̕�
	else if (state == BLOCK_LOW)
	{
#pragma region ---�u���b�N(���̕�
		//	�I�u�W�F�N�g����
		Object* block = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		//	�R���|�[�l���g�̒ǉ�
		auto transBlock = block->AddComponent<CTransform>();		//	���W
		auto DrawBlock = block->AddComponent<CDrawModel>();			//	3d���f���`��
		auto ColliderBlock = block->AddComponent<CCollider>();		//	�����蔻��
		block->AddComponent<COutRange>();
		auto comID = block->AddComponent<CTag>();
		comID->SetID(nBlockID);
		//	�I�u�W�F�N�g�ݒ�
		transBlock->SetPosition(PosX, PosY, BLOCK_OFFSET_Z);				// ���W
		transBlock->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);	// �X�P�[��
		ColliderBlock->SetCollisionSize(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);		// �����蔻��̑傫��
		ColliderBlock->SetOffset(BLOCK_OFFSET_X, BLOCK_OFFSET_Y);			// ���S���W�ƃ��f�����W�̒��S�̂���
		//	���X�g�ɒǉ�
		ObjectManager::GetInstance()->AddObject(block);

		// �e�N�X�`���؂�ւ��i���f���؂�ւ�)
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1: case SPRING_2:	// �t
			DrawBlock->SetModel(pModelManager->GetModel(SPRING_BLOCK_LOW_MODEL_NUM));
			break;
		case SUMMER_1: case SUMMER_2:	// ��
			DrawBlock->SetModel(pModelManager->GetModel(SUMMER_BLOCK_LOW_MODEL_NUM));
			break;
		case FALL_1: case FALL_2:	// �H
			DrawBlock->SetModel(pModelManager->GetModel(FALL_BLOCK_LOW_MODEL_NUM));
			break;
		case WINTER_1: case WINTER_2:	// �~
			DrawBlock->SetModel(pModelManager->GetModel(WINTER_BLOCK_LOW_MODEL_NUM));
			break;
		default:break;
		}

		DrawBlock->Update();
#pragma endregion
		return block;
	}
	return nullptr;
 }
 
//-----------------------------------------------


/**
 * @fn			StageManager::UpdateShowObject
 * @brief		�z�u����I�u�W�F�N�g�̕\���̍X�V
  * @param[in]	fPosX	��������x���W
 * @param[in]	fPosY	��������y���W
*/
void StageManager::UpdateShowObject(float fPosX, float fPosY)
{
	if (!m_pShowObject)
	{
		// �I�u�W�F�N�g����
		m_pShowObject = new Object("debug", UPDATE_DEBUG, DRAW_DEBUG);
		// �R���|�[�l���g�̒ǉ�
		auto trans = m_pShowObject->AddComponent<CTransform>();	// �I�u�W�F�N�g�̍��W
		auto draw = m_pShowObject->AddComponent<CDrawModel>();		// ���f��
		auto draw2 = m_pShowObject->AddComponent<CDraw2D>();	// �e�N�X�`��
		draw2->SetSize(30.0f, 30.0f);
		draw2->SetTexture(TextureManager::GetInstance()->GetTexture( SPIDER_TEX_NUM));
		m_pShowObject->Start();
	}

	// ���W�ݒ�
	auto Transform = m_pShowObject->GetComponent<CTransform>();
	auto drawModel = m_pShowObject->GetComponent<CDrawModel>();
	auto draw2D = m_pShowObject->GetComponent<CDraw2D>();
	Transform->SetPosition(fPosX, fPosY);
	Transform->SetScale(1.0f, 1.0f, 1.0f);
	Transform->SetRotate(0.0f, 0.0f, 0.0f);
	draw2D->SetColor(1,1,1);
	draw2D->SetAlpha(1.0f);

#pragma region ----------���[���h���W���X�N���[�����W�ɕϊ�����
	// �r���{�[�h�p���[���h�ϊ�
	XMFLOAT4X4& mP = CCamera::Get()->GetProjMatrix();
	XMFLOAT4X4& mV = CCamera::Get()->GetViewMatrix();
	XMFLOAT4X4 mW(
		mV._11, mV._21, mV._31, 0.0f,
		mV._12, mV._22, mV._32, 0.0f,
		mV._13, mV._23, mV._33, 0.0f,
		fPosX, fPosY, 0.0f, 1.0f);
	// �r���[�|�[�g�ϊ�
	XMFLOAT4X4 mVP(
		SCREEN_WIDTH / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, SCREEN_HEIGHT / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	// �}�g���b�N�X������
	XMMATRIX matrix = XMLoadFloat4x4(&mW) * XMLoadFloat4x4(&mV) * XMLoadFloat4x4(&mP) * XMLoadFloat4x4(&mVP);

	// ���S���W�A�������W�A�E����W��2D���W�ɕϊ�
	static const XMFLOAT3 v[] =
	{
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
	};
	XMFLOAT4 vPos[3];
	for (int i = 0; i < 3; ++i)
	{
		XMStoreFloat4(&vPos[i], XMVector3Transform(XMLoadFloat3(&v[i]), matrix));
		// �������_��񓯎����_�ɕϊ�
		vPos[i].x /= vPos[i].w;
		vPos[i].y /= vPos[i].w;
		vPos[i].z /= vPos[i].w;
	}
	DirectX::XMFLOAT3 screenPos = { vPos[0].x, vPos[0].y, vPos->z };
#pragma endregion

	// �z�u������̂̃X�e�[�^�X�ʂɐݒ�
	switch (m_nMap_Sta)
	{
	case N: // �z�u�Ȃ�
	{
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// �X�N���[�����W�̐ݒ�
		Transform->SetPosition(screenPos.x, screenPos.y);
		// �e�N�X�`��
		draw2D->SetTexture(nullptr);
		// �F
		draw2D->SetColor(0.7f, 0.2f, 0.2f);
		draw2D->SetAlpha(0.7f);
		// �T�C�Y
		draw2D->SetSize(50.0f, 50.0f);
	}
		break;

	case B:// �u���b�N�i�\�ʁj
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// �X�P�[��
		Transform->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);
		// �l�G�ɂ���ăe�N�X�`����ς���
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:
		case SPRING_2:	// �t
			drawModel->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
			break;
		case SUMMER_1:
		case SUMMER_2:	// ��
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
			break;
		case FALL_1:
		case FALL_2:	// �H
			drawModel->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
			break;
		case WINTER_1:
		case WINTER_2:	// �~
			drawModel->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;

	case CLOVER:// �N���[�o�[
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// ��]
		Transform->SetRotate(CLOVER_ROTATE_X, CLOVER_ROTATE_Y, CLOVER_ROTATE_Z);
		// �X�P�[��
		Transform->SetScale(CLOVER_SCALE_X, CLOVER_SCALE_Y, CLOVER_SCALE_Z);
		// ���f��
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(CLOVER_MODEL_NUM));
	}
		break;

	case GOAL:// �A�ؔ�
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// �X�P�[��
		Transform->SetScale(POT_SCALE_X, POT_SCALE_Y, POT_SCALE_Z);
		// ���W�̃I�t�Z�b�g
		Transform->SetPosition(fPosX, fPosY + +POT_POS_OFFSET_Y, POT_POS_Z);
		// ���f��
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(POT_MODEL_NUM));
	}
		break;
	case KEY:// ��
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// �X�P�[��
		Transform->SetScale(SEED_SCALE_X, SEED_SCALE_Y, SEED_SCALE_Z);
		// ���W�̃I�t�Z�b�g
		Transform->SetPosition(fPosX, fPosY + BUD_POS_OFFSET_Y);
		// �l�G�ɂ���ăe�N�X�`����ς���
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1: case SPRING_2:	// �t
			drawModel->SetModel(pModelManager->GetModel(SPRING_BUD_MODEL_NUM));
			break;
		case SUMMER_1: case SUMMER_2:	// ��
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BUD_MODEL_NUM));
			break;
		case FALL_1: case FALL_2:		// �H
			drawModel->SetModel(pModelManager->GetModel(FALL_BUD_MODEL_NUM));
			break;
		case WINTER_1: case WINTER_2:	// �~
			drawModel->SetModel(pModelManager->GetModel(WINTER_BUD_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;
	case ICICLE:// ���
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// ���f��
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICICLE_MODEL_NUM));
		// �X�P�[��
		Transform->SetScale(ICICLE_SCALE_X, ICICLE_SCALE_Y, ICICLE_SCALE_Z);
	}
		break;
	case WEB: // �w偂̑�
	{
		// �`��t���O
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// �e�N�X�`��
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(SPIDER_TEX_NUM));
		// �T�C�Y
		draw2D->SetSize(160.0f, 160.0f);
		// uv
		draw2D->SetAnimSplit(1, 1);
		// �X�N���[�����W�̐ݒ�
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case LEAVES:// �����t
	{
		// �`��t���O
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// �e�N�X�`��
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));
		// �T�C�Y
		draw2D->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);
		// uv
		draw2D->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		// �X�N���[�����W�̐ݒ�
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case ICE_BLOCK:// �X�u���b�N
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;
		// �X�P�[��
		Transform->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);
		// ���f��
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICE_BLOCK_MODEL_NUM1));
	}
		break;
	case CLOVER_AND_LEAVES:// �����t�i�N���[�o�[����j
	{
		// �`��t���O
		drawModel->m_bDrawFlag = false;
		draw2D->m_bDrawFlag = true;

		// �e�N�X�`��
		draw2D->SetTexture(TextureManager::GetInstance()->GetTexture(OTIBA_TEX_NUM));
		// �T�C�Y
		draw2D->SetSize(OTIBA_WIDTH, OTIBA_HEIGHT);
		// uv
		draw2D->SetAnimSplit(OTIBA_SPLIT_X, OTIBA_SPLIT_Y);
		// �X�N���[�����W�̐ݒ�
		Transform->SetPosition(screenPos.x, screenPos.y);
	}
		break;
	case CLOVER_AND_ICE_BLOCK:// �X�u���b�N�i�N���[�o�[����j
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;
		// �X�P�[��
		Transform->SetScale(ICE_BLOCK_SCALE_X, ICE_BLOCK_SCALE_Y, ICE_BLOCK_SCALE_Z);
		// ���f��
		drawModel->SetModel(ModelManager::GetInstance()->GetModel(ICE_BLOCK_MODEL_NUM1));
	}
		break;

	case BLOCK_LOW:// �u���b�N�̉��̕�
	{
		// �`��t���O
		drawModel->m_bDrawFlag = true;
		draw2D->m_bDrawFlag = false;

		// �X�P�[��
		Transform->SetScale(BLOCK_SCALE_X, BLOCK_SCALE_Y, BLOCK_SCALE_Z);
		// �l�G�ɂ���ăe�N�X�`����ς���
		ModelManager* pModelManager = ModelManager::GetInstance();
		switch (SceneGame::GetInstance()->GetSeason())
		{
		case SPRING_1:	case SPRING_2:	// �t
			drawModel->SetModel(pModelManager->GetModel(SPRING_BLOCK_LOW_MODEL_NUM));
			break;
		case SUMMER_1:	case SUMMER_2:	// ��
			drawModel->SetModel(pModelManager->GetModel(SUMMER_BLOCK_LOW_MODEL_NUM));
			break;
		case FALL_1:	case FALL_2:	// �H
			drawModel->SetModel(pModelManager->GetModel(FALL_BLOCK_LOW_MODEL_NUM));
			break;
		case WINTER_1:	case WINTER_2:	// �~
			drawModel->SetModel(pModelManager->GetModel(WINTER_BLOCK_LOW_MODEL_NUM));
			break;
		default:break;
		}
	}
		break;

	case MAX_CHIP:
	default:
		break;
	}

	//// ���f���ݒ�
	//if (m_nMap_Sta == N)
	//{
	//	
	//}
	//
	//else if (m_nMap_Sta == B)
	//{
	//	
	//}
	//else if (m_nMap_Sta == BLOCK_LOW)
	//{
	//	
	//}
	//else if (m_nMap_Sta == CLOVER)
	//{
	//	
	//}
	//else if (m_nMap_Sta == KEY)
	//{
	//	
	//}
	//else if (m_nMap_Sta == GOAL)
	//{
	//	
	//}
	//else if (m_nMap_Sta == ICICLE)
	//{
	//	
	//}
	//else if (m_nMap_Sta == ICE_BLOCK)
	//{
	//	
	//}
	//else
	//{
	//	m_pShowObject->GetComponent<CTransform>()->SetScale(0.3f, 0.3f, 0.3f);
	//	// �l�G�ɂ���ăe�N�X�`����ς���
	//	ModelManager* pModelManager = ModelManager::GetInstance();
	//	switch (SceneGame::GetInstance()->GetSeason())
	//	{
	//	case SPRING_1:
	//	case SPRING_2:	// �t
	//		Draw->SetModel(pModelManager->GetModel(SPRING_BLOCK_MODEL_NUM));
	//		break;
	//	case SUMMER_1:
	//	case SUMMER_2:	// ��
	//		Draw->SetModel(pModelManager->GetModel(SUMMER_BLOCK_MODEL_NUM));
	//		break;
	//	case FALL_1:
	//	case FALL_2:	// �H
	//		Draw->SetModel(pModelManager->GetModel(FALL_BLOCK_MODEL_NUM));
	//		break;
	//	case WINTER_1:
	//	case WINTER_2:	// �~
	//		Draw->SetModel(pModelManager->GetModel(WINTER_BLOCK_MODEL_NUM));
	//		break;
	//	default:break;
	//	}
	//}
}
//-----------------------------------------------