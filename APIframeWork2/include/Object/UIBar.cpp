#include "UIBar.h"

UIBar::UIBar()	:
	m_fMax(1.f),
	m_fMin(0.f),
	m_fValue(1.f),
	m_bHorizon(false)
{
}

UIBar::UIBar(const UIBar& bar)
{
	*this = bar;
}

UIBar::~UIBar()
{
}

bool UIBar::Init()
{
	m_tOriginSize = m_tSize;
	m_tOriginPos = m_tPos;
	return true;
}

void UIBar::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIBar::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);
	return 0;
}

int UIBar::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIBar::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIBar::Render(HDC hDC, float fDeltaTime)
{
	UI::Render(hDC, fDeltaTime);
}

UIBar* UIBar::Clone()
{
	return new UIBar(*this);
}
