/**
 * @file    ModelManager.h
 * @brief	���f�����Ǘ��v���O����
 * @date	2021/11/29
 * @author	AT12A ���c ���l
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

//===== �C���N���[�h =====
#include <unordered_map>
#include "main.h"
#include "AssimpModel.h"

//===== �}�N����` =====
// �v���C���[
#define SPRING_PLAYER_NUM				(0)									
#define PATH_MODEL_SPRING_PLAYER		("data/model/Silfy_Anim_A.fbx")		
#define SUMMER_PLAYER_NUM				(1)									
#define PATH_MODEL_SUMMER_PLAYER		("data/model/Silfy_Anim_B.fbx")		
#define FALL_PLAYER_NUM					(2)									
#define PATH_MODEL_FALL_PLAYER			("data/model/Silfy_Anim_C.fbx")		
#define WINTER_PLAYER_NUM				(3)									
#define PATH_MODEL_WINTER_PLAYER		("data/model/Silfy_Anim_D.fbx")		
// �N���[�o�[
#define CLOVER_MODEL_NUM				(4)									
#define PATH_MODEL_CLOVER				("data/model/clover.fbx")	
// ��
#define SEED_MODEL_NUM					(5)									
#define PATH_MODEL_SEED					("data/model/Seed01.fbx")			
// �A�ؔ�
#define POT_MODEL_NUM					(6)									
#define PATH_MODEL_POT					("data/model/Ueki.fbx")				

//#define FOG_MODEL_NUM					(8)									
//#define PATH_MODEL_FOG					("data/model/Fog.fbx")				
//#define WOOD_MODEL_NUM					(9)									
//#define PATH_MODEL_WOOD					("data/model/Woods.fbx")			
// ���
#define ICICLE_MODEL_NUM				(12)								
#define PATH_MODEL_ICICLE				("data/model/Tsurara.fbx")			

// �X
#define ICE_BLOCK_MODEL_NUM1			(23)
#define ICE_BLOCK_MODEL_NUM2			(24)
#define ICE_BLOCK_MODEL_NUM3			(25)
#define ICE_BLOCK_MODEL_NUM4			(26)
#define ICE_BLOCK_MODEL_NUM5			(27)
#define ICE_BLOCK_MODEL_NUM6			(28)
#define ICE_BLOCK_MODEL_NUM7			(29)
#define ICE_BLOCK_MODEL_NUM8			(30)
#define ICE_BLOCK_MODEL_NUM9			(31)
#define ICE_BLOCK_MODEL_NUM10			(32)
#define PATH_MODEL_ICE_BLOCK			("data/model/Ice_anim.fbx")			

// �u���b�N(��̕�
#define SPRING_BLOCK_MODEL_NUM			(40)							
#define PATH_MODEL_SPRING_BLOCK			("data/model/JimenHi_A.fbx")	
#define SUMMER_BLOCK_MODEL_NUM			(41)							
#define PATH_MODEL_SUMMER_BLOCK			("data/model/JimenHi_B.fbx")	
#define FALL_BLOCK_MODEL_NUM			(42)							
#define PATH_MODEL_FALL_BLOCK			("data/model/JimenHi_C.fbx")	
#define WINTER_BLOCK_MODEL_NUM			(43)							
#define PATH_MODEL_WINTER_BLOCK			("data/model/JimenHi_D.fbx")

// �u���b�N(���̕�
#define SPRING_BLOCK_LOW_MODEL_NUM		(44)								
#define PATH_MODEL_SPRING_BLOCK_LOW		("data/model/JimenLow_A.fbx")		
#define SUMMER_BLOCK_LOW_MODEL_NUM		(45)								
#define PATH_MODEL_SUMMER_BLOCK_LOW		("data/model/JimenLow_B.fbx")		
#define FALL_BLOCK_LOW_MODEL_NUM		(46)								
#define PATH_MODEL_FALL_BLOCK_LOW		("data/model/JimenLow_C.fbx")		
#define WINTER_BLOCK_LOW_MODEL_NUM		(47)								
#define PATH_MODEL_WINTER_BLOCK_LOW		("data/model/JimenLow_D.fbx")		

// �ڂ�
#define SPRING_BUD_MODEL_NUM		(48)								
#define PATH_MODEL_SPRING_BUD		("data/model/Tsubomi_anim_A.fbx")	
#define SUMMER_BUD_MODEL_NUM		(49)								
#define PATH_MODEL_SUMMER_BUD		("data/model/Tsubomi_anim_B.fbx")	
#define FALL_BUD_MODEL_NUM			(50)								
#define PATH_MODEL_FALL_BUD			("data/model/Tsubomi_anim_C.fbx")	
#define WINTER_BUD_MODEL_NUM		(51)								
#define PATH_MODEL_WINTER_BUD		("data/model/Tsubomi_anim_D.fbx")	
// ��
#define SPRING_FLOWER_MODEL_NUM		(52)								
#define PATH_MODEL_SPRING_FLOWER	("data/model/GoalFlower_A_anim.fbx")
#define SUMMER_FLOWER_MODEL_NUM		(53)								
#define PATH_MODEL_SUMMER_FLOWER	("data/model/GoalFlower_B_anim.fbx")
#define FALL_FLOWER_MODEL_NUM		(54)								
#define PATH_MODEL_FALL_FLOWER		("data/model/GoalFlower_C_anim.fbx")
#define WINTER_FLOWER_MODEL_NUM		(55)								
#define PATH_MODEL_WINTER_FLOWER	("data/model/GoalFlower_D_anim.fbx")

// �v���C���[(�S�[���A�j���[�V����
#define SPRING_GOAL_MODEL_NUM		(56)								
#define PATH_MODEL_SPRING_GOAL		("data/model/Silfy_Anim_Goal_A.fbx")
#define SUMMER_GOAL_MODEL_NUM		(57)								
#define PATH_MODEL_SUMMER_GOAL		("data/model/Silfy_Anim_Goal_B.fbx")
#define FALL_GOAL_MODEL_NUM			(58)								
#define PATH_MODEL_FALL_GOAL		("data/model/Silfy_Anim_Goal_C.fbx")
#define WINTER_GOAL_MODEL_NUM		(59)								
#define PATH_MODEL_WINTER_GOAL		("data/model/Silfy_Anim_Goal_D.fbx")

// �l�t�̃N���[�o�[
#define END_CLOVER_MODEL_NUM		(60)							
#define PATH_MODEL_END_CLOVER		("data/model/end_clover.fbx")	



//===== �N���X��` =====

/**
 * @class	ModelManager
 * @brief	���f���}�l�[�W���[
*/
class ModelManager
{
private:
	//=== �����o�ϐ�
	static ModelManager*	m_pInstance;					// �C���X�^���X
	std::unordered_map<int, CAssimpModel*> 	m_ModelList; 	// ���f�����X�g

	//=== ���\�b�h
protected:
	ModelManager();											//	�R���X�g���N�^

public:
	~ModelManager();										//	�f�X�g���N�^
	static ModelManager* GetInstance();						//	�C���X�^���X��
	static void Uninit();									//	���f���}�l�[�W���[���
	bool AddModel(const char* pszFileName, int nModelID);	//	���f���ǉ�
	CAssimpModel* GetModel(int nModelID);					//	���f���擾
	bool EraseModel(int nModelID);							//	���f���폜

};
#endif // __MODEL_MANAGER_H__
