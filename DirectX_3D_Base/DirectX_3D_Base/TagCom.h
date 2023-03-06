/**
 * @file	TagCom.h
 * @brief	�ȒP�Ȑ���
 * @author	�������l
 * @date	���t�i�J�n���H�j
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __TAG_COMPONENT_H__
#define __TAG_COMPONENT_H__

//===== �C���N���[�h =====
#include "Component.h"
#include <map>

//===== �O���錾 =====

//===== �}�N����` =====


//===== �񋓑̐錾 =====


//===== �N���X��` =====
class CTag : public Component
{
private:
	int m_nID;
public:
	CTag();
	~CTag() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;
	void SetID(int id)
	{
		m_nID = id;
	}

	static Object* Find(int id);
	static std::map<int, Object> m_pStageObject;
};

#endif // __TAG_COMPONENT_H__