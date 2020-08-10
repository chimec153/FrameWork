#include "UIClockHand.h"
#include "../Animation/Animation.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"

UIClockHand::UIClockHand()	:
	m_fTime(0.f)
{
}

UIClockHand::UIClockHand(const UIClockHand& clockhand)	:
	UI(clockhand)
{
}

UIClockHand::~UIClockHand()
{
}

bool UIClockHand::Init()
{
	SetSize(100.f, 100.f);
	m_tOriginPos = m_tPos;

	SetTexture("ClockHand", TEXT("UI\\Arrow"), 13);

	if(m_pTexture)
		m_pTexture->SetColorKeyAll(RGB(255, 255, 255));

	return true;
}

void UIClockHand::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIClockHand::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	POSITION tPos = {};

	tPos.x = -cosf(-PI / 2.f + (int)(m_fTime / 120.f) * PI / 12.f);
	tPos.y = -sinf(-PI / 2.f + (int)(m_fTime / 120.f) * PI / 12.f);

	tPos.Normalize();

	m_tPos = m_tOriginPos + tPos * 20.f;
	return 0;
}

int UIClockHand::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIClockHand::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIClockHand::Render(HDC hDC, float fDeltaTime)
{
	if (m_pTexture)
		m_pTexture->Render(hDC, m_tPos, POSITION::Zero, m_tSize, 12 - (int)(m_fTime/120.f));
}

UIClockHand* UIClockHand::Clone()
{
	return new UIClockHand(*this);
}
