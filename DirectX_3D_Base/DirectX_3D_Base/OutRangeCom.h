/**
 * @file	OutRangCom.h
 * @brief	�ȒP�Ȑ���
 * @author	�������l
 * @date	���t�i�J�n���H�j
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __OUT_RANG_H__
#define __OUT_RANG_H__

//===== �C���N���[�h =====
#include "Component.h"

//===== �O���錾 =====
class CTransform;

//===== �}�N����` =====
#define OUT_RANGE_X	(300.0f)
#define OUT_RANGE_Y	(140.0f)

//===== �񋓑̐錾 =====


//===== �N���X��` =====
class COutRange : public Component
{
private:
	CTransform* m_pTransform;

	XMFLOAT2 m_LimitRange;

private:
	void NoFunction();
public:
	COutRange();
	~COutRange() override;
	void Start() override;
	void Update() override;
	void Draw() override;
	void OnCollisionEnter(Object* pObject) override;

	void SetLimitRange(float x, float y);
};
#endif // __OUT_RANG_H__