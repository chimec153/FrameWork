#include "UITilePanel.h"
#include "..//Collider/ColliderRect.h"
#include "..//Core/Input.h"
#include "UIButton.h"
#include "../Scene/Layer.h"
#include "..//Scene/Scene.h"
#include "..//Scene/SceneManager.h"

UITilePanel::UITilePanel()	:
	m_bMousePress(false),
	m_tMousePos(-1.f,-1.f)
{
}

UITilePanel::UITilePanel(const UITilePanel& tilepanel)	:
	UI(tilepanel)
{
	*this = tilepanel;
}

UITilePanel::~UITilePanel()
{
}

bool UITilePanel::Init()
{
	ColliderRect* pColl = AddCollider<ColliderRect>("PanelBody");

	pColl->SetUI(true);

	pColl->AddCollisionFunction(CS_ENTER, this,
		&UITilePanel::MouseOn);
	pColl->AddCollisionFunction(CS_LEAVE, this,
		&UITilePanel::MouseOut);

	pColl->SetRect(0.f, 0.f, 400.f, 400.f);
	pColl->SetUI(true);

	SAFE_RELEASE(pColl);

	m_vecButton.reserve(64);

	return true;
}

void UITilePanel::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);

	if (m_bMousePress)
	{
		if (KEYDOWN("MouseLButton"))
		{
			m_tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();
			m_tMousePos -= m_tPos - m_tSize * m_tPivot;
		}

		else if (KEYPRESS("MouseLButton"))
		{
			m_tPos = GET_SINGLE(Input)->GetMouseWorldPos() - m_tMousePos;
		}
	}
	else
	{
		m_tMousePos.x = -1.f;
		m_tMousePos.y = -1.f;
	}
		
}

int UITilePanel::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);
	return 0;
}

int UITilePanel::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UITilePanel::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UITilePanel::Render(HDC hDC, float fDeltaTime)
{
	UI::Render(hDC, fDeltaTime);
}

UITilePanel* UITilePanel::Clone()
{
	return new UITilePanel(*this);
}

void UITilePanel::MouseOn(Collider* pSrc, Collider* pDest, float fTime)
{
	m_bMousePress = true;
}

void UITilePanel::MouseOut(Collider* pSrc, Collider* pDest, float fTime)
{
	m_bMousePress = false;
}
