#include "UINum.h"
#include "..//Core/Camera.h"

UINum::UINum()	:
	m_iNum(0),
	m_fDist(0.f),
	m_fLimitDist(30.f),
	m_fSpeed(30.f)
{
}

UINum::UINum(const UINum& num)	:
	UI(num)
{
	m_iNum = num.m_iNum;
	m_fLimitDist = num.m_fLimitDist;
	m_fSpeed = num.m_fSpeed;
	m_fDist = 0.f;
}

UINum::~UINum()
{
	Safe_Release_VecList(m_vecNum);
}

void UINum::SetNum(int iNum)
{
	m_iNum = iNum % 10;

	if (m_iNum > 5)
	{
		m_tImageOffset.x -= m_tSize.x * 6;
		m_tImageOffset.y += m_tSize.y;
	}

	POSITION tPos = m_tImageOffset;

	tPos.x += m_tSize.x* m_iNum;

	SetImageOffset(tPos);
}

void UINum::CreateNum(int iNum)
{
	int i = 1;

	SetNum(iNum);

	while (iNum >= 10)
	{
		UINum* pObj = (UINum*)CreateCloneObj("NumSm", "num", SC_CURRENT, m_pLayer);

		POSITION tPos = m_tPos;

		tPos.x -= m_tSize.x * i;

		pObj->SetPos(tPos);

		pObj->SetNum(iNum / 10);

		m_vecNum.push_back(pObj);

		iNum /= 10;

		++i;
	}
}

bool UINum::Init()
{
	if (!UI::Init())
		return false;

	return true;
}

void UINum::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UINum::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	m_fDist += m_fSpeed * fDeltaTime;
	m_tPos.y -= m_fSpeed * fDeltaTime;

	if (m_fDist >= m_fLimitDist)
	{
		Die();
	}

	return 0;
}

int UINum::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UINum::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UINum::Render(HDC hDC, float fDeltaTime)
{
	Obj::Render(hDC, fDeltaTime);
}

UINum* UINum::Clone()
{
	return new UINum(*this);
}
