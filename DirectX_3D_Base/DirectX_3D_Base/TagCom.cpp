#include "TagCom.h"
#include "ObjectManager.h"

std::map<int, Object> CTag::m_pStageObject;
CTag::CTag()
	: m_nID(0)
{
}

CTag::~CTag()
{
}

void CTag::Start()
{

}
void CTag::Update()
{
}
void CTag::Draw()
{
}

void CTag::OnCollisionEnter(Object* pObject)
{
}

Object* CTag::Find(int id)
{
	std::list<Object*>& objects = ObjectManager::GetInstance()->GetUpdateList();

	for (auto obj : objects)
	{
		auto com = obj->GetComponent<CTag>();
		if(!com) continue;

		if (com->m_nID == id)
		{
			return obj;
		}
	}

	return nullptr;
}
