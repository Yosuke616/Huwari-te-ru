//===== �C���N���[�h�K�[�h =====
#ifndef __OBJECT_INFO_H__
#define __OBJECT_INFO_H__

// �G�߃e�N�X�`��
//#define	SCENE_CHANGE

#define WIMD_GAUGE_MAX		(250)		// �g�p�ł��镗�̗�
#define WIND_CREATE_DIFF	(40)		// ���𐶐����邽�߂ɕK�v�ȃ}�E�X�̈ړ���

#define WIND_POWER_X		(0.05f)
#define WIND_POWER_Y		(0.05f)

#define WINDENDING_POWER_X		(0.1f)
#define WINDENDING_POWER_Y		(0.1f)

//===== �񋓑� ======

//	�I�u�W�F�N�g�̍X�V�̏���
enum UPDATE_ORDER
{
	UPDATE_BG,			//	�w�i
	UPDATE_PLAYER,		//	�v���C���[
	UPDATE_FIELD,		//	�n�`
	UPDATE_EFFECT,		//	�G�t�F�N�g
	UPDATE_MODEL,		//	���f��
	UPDATE_DEBUG,		//	debug
	UPDATE_UI,			//	UI
	UPDATE_OPTION,		//	�I�v�V�����I�u�W�F�N�g
	UPDATE_GOLETTER,

	MAX_UPDATE_ORDER
};

//	�I�u�W�F�N�g�̕`��̏���
enum DRAW_ORDER
{
	DRAW_BG,		//	�w�i
	DRAW_FIELD,		//	�n�`
	DRAW_MODEL,		//	�v���C���[
	DRAW_PLAYER,	//	�v���C���[
	DRAW_EFFECT,	//	�G�t�F�N�g

	DRAW_DEBUG,		//	debug
	DRAW_UI_FRAME,	//	UI�g
	DRAW_GOLETTER,
	DRAW_UI,		//	UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//	�I�v�V�����I�u�W�F�N�g
	DRAW_OPTION,	//	�I�v�V�����I�u�W�F�N�g

	MAX_DRAW_ORDER
};

//@@@@@ �V���t�B------------------------------------------------
#define PLAYER_NAME				("shilfy")
#define PLAYER_POS_X			(-200.0f)				//posX
#define PLAYER_POS_Y			(0.0f)					//posY
#define PLAYER_POS_Z			(0.0f)					//PosZ
#define PLAYER_OFFSET_X			(0.0f)					//Center offset
#define PLAYER_OFFSET_Y			(15.0f)					//Center offset
#define PLAYER_SIZE_X			(20.0f)
#define PLAYER_SIZE_Y			(30.0f)

	/***** �A�j���[�V���� ******/

// �v���C���[��FBX�̐�
enum PLAYER_FBX
{
	FBX_ONE = 0,	// ���(�ҋ@�A���ɐ�����Ă���A�����Ă���A�Q�b�g������
	FBX_TWO,		// ���(�S�[��������
};

// 1�ڂ�fbx
enum ANIM_ID
{
	PLAYER_IDLE = 0,
	PLAYER_CARRIED,
	PLAYER_FALL,
	PLAYER_GET,
	PLAYER_BIND,
	PLAYER_GAMEOVER,

	MAX_ANIMATION
};
// 2�ڂ�fbx
#define PLAYER_GOAL	(0)

// �A�j���[�V��������
#define PLAYER_ANIM_IDLE_S		(0)		// �ҋ@Start
#define PLAYER_ANIM_IDLE_E		(7.95)	// �ҋ@End

#define PLAYER_ANIM_CARRIED_S	(8.34)	// ���ɉ^�΂�Ă���Start
#define PLAYER_ANIM_CARRIED_E	(9.03)	// ���ɉ^�΂�Ă���End

#define PLAYER_ANIM_FALL_S		(9.3)	// �����Ă���Start
#define PLAYER_ANIM_FALL_E		(11.3)	// �����Ă���End

#define PLAYER_ANIM_GET_S		(11.67)	// ��A�N���[�o�[�擾��Start
#define PLAYER_ANIM_GET_E		(12)	// ��A�N���[�o�[�擾��End

#define PLAYER_ANIM_BIND_S		(12.1)	// �N���̑��Ɉ���������Start
#define PLAYER_ANIM_BIND_E		(12.31)	// �N���̑��Ɉ���������End

#define PLAYER_ANIM_GAMEOVER_S	(12.336)// �Q�[���I�[�o�[Start
#define PLAYER_ANIM_GAMEOVER_E	(16.2)	// �Q�[���I�[�o�[End


#define PLAYER_ANIM_GOAL_S		(0)	// ��A�N���[�o�[�擾��Start
#define PLAYER_ANIM_GOAL_E		(4)	// ��A�N���[�o�[�擾��End

//-----------------------------------------------------------

//@@@@@ �T�E���h�֘A
#define BGM_NAME				("config_bgm")
#define SE_NAME					("config_se")
#define BACK_NAME				("back")
#define BGM_POS_Y			(0.0f)
#define SE_POS_Y			(-150.0f)
#define STICK_WIDTH				(400.0f)
#define STICK_HEIGHT			(180.0f)
#define CONFIG_STICK_WIDTH		(15.0f)
#define CONFIG_STICK_HEIGHT		(50.0f)
#define GAUGE_LEFT				(0.0f - (STICK_WIDTH / 2))



//@@@@@ ��
#define WIND_NAME				("Wind")
#define WIND_WIDTH				(42.0f)
#define WIND_HEIGHT				(34.0f)
#define WIND_SPLIT_X			(4)
#define WIND_SPLIT_Y			(4)
#define SWAP_FRAME				(2)

//@@@@@ ���Q�[�W
#define WIND_GAUGE_WIDTH		(500.0f)
#define WIND_GAUGE_HEIGHT		(500.0f)
#define WIND_GAUGE_POS_X		(-(SCREEN_WIDTH - WIND_GAUGE_WIDTH) / 2)
#define WIND_GAUGE_POS_Y		((SCREEN_HEIGHT - WIND_GAUGE_HEIGHT) / 2 + 150.0f)

//@@@@@ �u���b�N
#define BLOCK_NAME				("Block")
#define MAPCHIP_WIDTH			(30.0f)			// �}�b�v�`�b�v�̉���
#define MAPCHIP_HEIGHT			(25.0f)			// �}�b�v�`�b�v�̏c��
#define BLOCK_OFFSET_X			(0.0f)			//Center offset
#define BLOCK_OFFSET_Y			(-8.0f)			//Center offset
#define BLOCK_OFFSET_Z			(10.0f)			//Center offset
#define BLOCK_SCALE_X			(2.35f)			//scale
#define BLOCK_SCALE_Y			(1.94f)			//scale
#define BLOCK_SCALE_Z			(6.5f)			//scale

//@@@@@ ��
#define SEED_NAME				("Seed")
#define SEED_SIZE_X				(50.0f)
#define SEED_SIZE_Y				(50.0f)
#define SEED_POS_OFFSET_X		(0.0f)
#define SEED_POS_OFFSET_Y		(10.0f)	
#define SEED_SCALE_X			(5.0f)
#define SEED_SCALE_Y			(5.0f)
#define SEED_SCALE_Z			(5.0f)

//@@@@@ �ڂ�
#define BUD_NAME				("FlowerBud")
#define BUD_POS_OFFSET_X		(0.0f)
#define BUD_POS_OFFSET_Y		(-20.0f)
#define BUD_POS_OFFSET_Z		(10.0f)
#define BUD_COLLISION_X			(120.0f)
#define BUD_COLLISION_Y			(120.0f)
#define BUD_ANIM_1_START		(0.0)
#define BUD_ANIM_1_END			(1.0)
#define BUD_ANIM_2_START		(1.0)
#define BUD_ANIM_2_END			(1.6)

//@@@@@ �A�ؔ�(�S�[��
#define POT_NAME				("PlantPot")
#define POT_POS_Z				(22.0f)			// ���ɒu���Ȃ��ƕ�
#define POT_POS_OFFSET_Y		(-20.0f)		// �X�e�[�W�z��̍��W�Ƃ̂���
#define POT_SCALE_X				(15.0f)	
#define POT_SCALE_Y				(10.0f)
#define POT_SCALE_Z				(10.0f)

//@@@@@ ��
#define FLOAER_ROTATE_X			(-10.0f)
//#define FLOAER_ROTATE_Y		(6.0f)
#define FLOAER_POS_X_OFFSET		(0)
#define FLOAER_POS_Y_OFFSET		(0)
#define FLOAER_POS_Z_OFFSET		(8.0f)
#define FLOAER_SCALE_X			(6.0f)
#define FLOAER_SCALE_Y			(6.0f)
#define FLOAER_SCALE_Z			(6.0f)
#define FLOAER_ANIM_S			(0.0)
#define FLOAER_ANIM_E			(4.8)

//@@@@@ �w�i
#define BG_NAME					("BG")
#define BG_POS_Y				(100.0f)
#define BG_LENGTH				(SCREEN_HEIGHT)

//@@@@@	�J�[�\��
#define CURSOR_NAME				("menuobject_cursor")
#define GAME_CURSOR_NAME		("gameobject_cursor")
#define CURSOR_WIDTH			(30.0f)
#define CURSOR_HEIGHT			(30.0f)
#define CURSOR_WIDTH_COL		(30.0f)
#define CURSOR_HEIGHT_COL		(30.0f)

//@@@@@ �l�p���X
#define BLOCK_MOVE
#define ICE_BLOCK_NAME					("ice_block")
#define ICE_BLOCK_POS_Y_OFFSET			(-6.0f)
#define ICE_BLOCK_POS_Z_OFFSET			(-6.0f)
#define ICE_BLOCK_COLLISION_WIDTH		(30.0f)			// �}�b�v�`�b�v�̉���
#define ICE_BLOCK_COLLISION_HEIGHT		(28.0f)			// �}�b�v�`�b�v�̏c��
#define ICE_BLOCK_SCALE_X				(6.9f)
#define ICE_BLOCK_SCALE_Y				(6.0f)
#define ICE_BLOCK_SCALE_Z				(2.0f)
#define MAX_ICE_VELOCITY				(1.0f)	// �X�u���b�N�̑��x




#define CLOVER_NUMBER_NAME	("clover_num")



//2021/12/21 Shimizu Yosuke----------------------------
//@@@@@ �N���[�o�[
#define CLOVER_NAME				("Clover")
#define CLOVER_POS_Z			(5.0f)
#define CLOVER_OFFSET_Y			(-8.0f)		// 2022/01/22 �o�ꎞ�A�n�ʂɐڂ��Ă���悤�ɂ���
#define CLOVER_ROTATE_X			(-70.0f)
#define CLOVER_ROTATE_Y			(0.0f)
#define CLOVER_ROTATE_Z			(-45.0f)
#define CLOVER_SCALE_X			(3.0f)
#define CLOVER_SCALE_Y			(3.0f)
#define CLOVER_SCALE_Z			(3.0f)
#define MAX_CLOVER_VELOCITY		(1.5f)

//@@@@@ �N���[�o�[UI
#define CLOVER_UI_NAME		("Clover_UI")
#define CLOVER_UI_POS_X		(-550.0f)
#define CLOVER_UI_POS_Y		(-50.0f)
#define CLOVER_UI_SIZE_X	(45.0f)
#define CLOVER_UI_SIZE_Y	(45.0f)
#define CLOVER_POS_CHENGE	(40.0f)
//2022/1/30/Clover_UI ---------------------------
#define CLOVER_UI_GAP_X		(1.5f)
#define CLOVER_UI_GAP_Y		(3.0f)
//-----------------------------------------------

//�N���[�o�[UI����]��������
#define CLOVER_ROTATE_BASE  	(0.0f)
#define CLOVER_ROTATE		(90.0f)

//@@@@@ ���UI
#define	SEED_UI_NAME		("ssed_ui")
#define	SEED_UI_POS_X		(-540.0f)
#define	SEED_UI_POS_Y		(100.0f)
#define	SEED_UI_SIZE_X		(125.0f)
#define	SEED_UI_SIZE_Y		(125.0f)

//@@@@@ ��
#define SNOW_NAME			("snow")

//@@@@@ �|�[�Y���j���[
#define STOP_GAME_NAME				("stop_game")
#define STOP_GAME_SIZE_X			(100.0f)
#define STOP_GAME_SIZE_Y			(100.0f)
#define STOP_GAME_POS_X				(-565.0f)
#define STOP_GAME_POS_Y				(-300.0f)
#define STOP_GAME_COLLSION_WIDTH	(100.0f)
#define STOP_GAME_COLLSION_HEIGHT	(100.0f)

//@@@@@ �`���[�g���A��
#define TUTORIAL_NAME				("tutorial")
#define TUTORIAL_COLL_SIZE_HEIGHT	(50.0f)
#define TUTORIAL_COLL_SIZE_WIDTH	(50.0f)
//#define TUTORIAL_SIZE_Y				(50.0f)
//#define TUTORIAL_SIZE_X				(50.0f)

//�`���[�g���A��
//�T�C�Y����������������
//2022/1/26/Touch_Tutorial Shimizu Yosuke -------------
#define TUTORIAL_SIZE_X				(SCREEN_WIDTH - 160.0f)
#define TUTORIAL_SIZE_Y				(SCREEN_HEIGHT - 160.0f)
//-----------------------------------------------------

//��L�̃`���[�g���A���̉��ɒǉ�
//2022/1/26/WindCar Shimizu Yosuke --------------------
//����(�`���[�g���A��)
#define WINDCAR_NAME				("windcar")
#define WINDCAR_POS_X				(-450.0f)
#define WINDCAR_POS_Y				(-285.0f)
#define WINDCAR_SIZE_X				(100.0f)
#define WINDCAR_SIZE_Y				(100.0f)
#define WINDCAR_COLL_WIDTH			(50.0f)
#define WINDCAR_COLL_HEIGHT			(50.0f)

//���Ԃ̖_
#define WINDCAR_STICK_NAME			("stick")
#define WINDCAR_STICK_POS_X			(-450.0f)
#define WINDCAR_STICK_POS_Y			(-300.0f)
#define WINDCAR_STICK_SIZE_X		(100.0f)
#define WINDCAR_STICK_SIZE_Y		(100.0f)

//-----------------------------------------------------


//2022/1/30/Touch_Tutorial Shimizu Yosuke -------------
#define CLICK_TEXT_NAME				("clicl_text")
#define CLICK_TEXT_SIZE_X			(500.0f)
#define CLICK_TEXT_SIZE_Y			(250.0f)
#define CLICK_TEXT_POS_X			(0.0f)
#define CLICK_TEXT_POS_Y			(-300.0f)

//2022/1/31/Load_Tutorial Shimizu Yosuke --------------------------------------
#define LOAD_TUTORIAL_NAME			("tutorial_name")
//-----------------------------------------------------------------------------
//-----------------------------------------------------


//-----------------------------------------------------


// naito-----------------------------------------------
//@@@@@	���
#define ICICLE_UP_NAME					("Icicle")
#define ICICLE_DOWN_NAME				("Down")	// ���̓����蔻��
#define ICICLE_SCALE_X					(7.0f)		//scale
#define ICICLE_SCALE_Y					(7.0f)		//scale
#define ICICLE_SCALE_Z					(7.0f)		//scale
#define ICICLE_SIZE_X					(40.0f)		// ���S�̂̑傫��
#define ICICLE_SIZE_Y					(100.0f)	// ���S�̂̑傫��
#define ICICLE_UP_COLLISION_OFFSET_X	(0.0f)		// ��̂����蔻��̑傫��
#define ICICLE_UP_COLLISION_OFFSET_Y	(30.0f)		// ��̂����蔻��̑傫��
#define ICICLE_UP_COLLISION_SIZE_X		(40.0f)		// ���̂����蔻��̑傫��	
#define ICICLE_UP_COLLISION_SIZE_Y		(40.0f)		// ���̂����蔻��̑傫��

//@@@@@ �Ђ�Ђ�
#define	LEAF_NAME			("leaf")

//@@@@@ �Q�[���I�[�o�[����
#define GOLETTER_NAME		("goletter")		
#define GOLETTER_POS_X		(0.0f)			//�����̈ʒu
#define GOLETTER_POS_Y		(400.0f)		//�����̈ʒu
#define GOLETTER_SIZE_X		(600.0f)		//�����̃T�C�Y
#define GOLETTER_SIZE_Y		(300.0f)		//�����̃T�C�Y

//@@@@@ �G���f�B���O�̃N���[�o�[
#define ENDING_CLOVER_NAME	("ending_clover")
//----------------------------------------------------


// sawada---------------------------------------------
//@@@@@	���j���[�ꗗ
#define TITLE_NAME				("title")
#define SELECT_NAME				("select")
#define CONTINUE_NAME			("continue")
#define RESTART_NAME			("restart")	
#define NEW_GAME_NAME			("new_game")
#define OPTION_NAME				("option")
#define EXIT_GAME_NAME			("exit_game")
//#define MENU_NAME				("Menu")
#define MENU_COLLISION_WIDTH	(450.0f)
#define MENU_COLLISION_HEIGHT	(100.0f)
#define MENU_WIDTH				(450.0f)
#define MENU_HEIGHT				(250.0f)
#define MENU_POS_X				(  0.0f)
#define MENU_POS_Y				(-10.0f)
#define TITLEMENU_POS_X			(300.0f)
#define TITLEMENU_POS_Y			(-50.0f)
// --------------------------------------------------


// simisho-------------------------------------------
//@@@@@ �G�߃��S
#define SEASON_LOGO_X		(0.0f)
#define SEASON_LOGO_Y		(-50.0f)
#define SEASON_LOGO_WIDTH	(512.0f)
#define SEASON_LOGO_HEIGHT	(288.0f)
// --------------------------------------------------


// tomoya--------------------------------------------
//@@@@@ �Z���N�g���j���[
#define RIGHT_ARROW_NAME	("RightArrow")
#define LEFT_ARROW_NAME		("LeftArrow")
#define STAGE1_NAME			("StageMap1")
#define STAGE2_NAME			("StageMap2")

//@@@@@ �N���̑�
#define SPIDER_WIDTH		(80.0f)
#define SPIDER_HEIGHT		(80.0f)
#define SPIDER_OFFESET_Y	(10.0f)
#define SPIDER_BIND_WIDTH	(35.0f)
#define SPIDER_BIND_HEIGHT	(35.0f)
#define SPIDER_ROT_Y		(30.0f)
#define SPIDER_ROT_Y		(30.0f)
// --------------------------------------------------

// ryo-ga--------------------------------------------
#define OTIBA_NAME				("fallen_leaves")
#define OTIBA_POS_Z				(-7.0f)
#define OTIBA_WIDTH				(40.0f)
#define OTIBA_HEIGHT			(30.0f)
#define OTIBA_OFFESET_Y			(-8.0f)
#define OTIBA_COLLISION_WIDTH	(20.0f)
#define OTIBA_COLLISION_HEIGHT	(20.0f)
#define OTIBA_SPLIT_X			(4)
#define OTIBA_SPLIT_Y			(4)
#define OTIBA_ANIM_SPEED		(2)
// --------------------------------------------------
#endif // !__OBJECT_INFO_H__
