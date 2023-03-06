#include "DeleteTimerCom.h"
#include "CloverComponent.h"
#include "TransformComponent.h"

CTimer::CTimer()
	: m_nTime(-1), m_nFuncState(FUNC_DELETE)
{

}
CTimer::~CTimer()
{

}

void CTimer::Update()
{
	if (m_nTime > 0)
	{
		m_nTime--;

		if (m_nTime == 0)
		{
			switch (m_nFuncState)
			{
			case FUNC_DELETE:			Parent->Delete();
				break;
			case FUNC_CREATE_CLOVER:	Parent->Delete();
				break;
			default:	break;
			}
		}
	}
}
void CTimer::SetTime(int time)
{
	m_nTime = time;
}

void CTimer::SetFunction(E_FUNCTION func)
{
	m_nFuncState = func;
}

int CTimer::GetFunktion()
{
	return m_nFuncState;
}