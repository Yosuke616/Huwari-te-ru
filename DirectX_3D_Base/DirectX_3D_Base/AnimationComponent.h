/**
 * @file	AnimationComponent.h
 * @brief	fbx�؂�ւ��@�\
 * @author	���c���l
 * @date	2022/12/31
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

//===== �C���N���[�h =====
#include "main.h"
#include "Component.h"

#include <unordered_map>

//===== �}�N����` =====

//===== �O���錾 =====
class CDrawModel;

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CAnimation : public Component
{
private:
	int m_nAnim;										//	�A�j���[�V�����ԍ�(0 �����ȃ��[�V���� 1 �N���A)
	std::unordered_map<int, CDrawModel*> m_AnimationList;	//	�A�j���[�V�������X�g

public:
	CAnimation();							//	�R���X�g���N�^
	~CAnimation() override;					//	�f�X�g���N�^
	void Start() override;					//	������
	void Update() override;					//	�X�V
	void Draw() override;					//	�`��
	void ChangeAnimation(int anim);			//	�A�j���[�V�����ԍ��ύX
	void AddAnimation(CDrawModel* fbx_anim, int number);
	//getter
	CDrawModel* GetMoel(int fbx_number);
};
#endif // __GRAVITY_H__
