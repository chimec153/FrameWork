#include "Timer.h"
#include "../Object/UIClockHand.h"

DEFINITION_SINGLE(Timer)
Timer::Timer()	:
	m_fTimeScale(1.f),
	m_pClockHand(nullptr)
{

}
Timer::~Timer()
{
	SAFE_RELEASE(m_pClockHand);
}

void Timer::SetClockHand(Obj* pObj)
{
	SAFE_RELEASE(m_pClockHand);

	m_pClockHand = pObj;

	if (m_pClockHand)
		m_pClockHand->AddRef();
}
bool Timer::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;

	return true;
}

void Timer::Update()
{
	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) /
		(float)m_tSecond.QuadPart;

	m_tTime = tTime;

	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_fFPSTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFrame = 0;
//#ifdef _DEBUG
		char	strFPS[64] = {};
		sprintf_s(strFPS, "FPS : %.f\n", m_fFPS);
		//_cprintf(strFPS);
		SetWindowTextA(m_hWnd, strFPS);
		OutputDebugStringA(strFPS);
//#endif
	}

	if (m_pClockHand)
		((UIClockHand*)m_pClockHand)->AddTime(m_fDeltaTime);
}
