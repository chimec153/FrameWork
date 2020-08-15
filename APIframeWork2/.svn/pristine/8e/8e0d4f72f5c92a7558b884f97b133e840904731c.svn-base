#include "UIButton.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"
#include "../Sound/SoundManager.h"

UIButton::UIButton()	:
	m_bEnableCallback2(false),
	m_bEnableCallback(false),
	m_eState(BS_NONE),
	m_pParentUI(nullptr),
	m_tParentPrevPos(),
	m_iButtonIndex(0)
{
}

UIButton::UIButton(const UIButton & ui)	:
	UI(ui)
{
	m_bEnableCallback2 = false;
	m_bEnableCallback = false;
	m_eState = BS_NONE;
	m_pParentUI = ui.m_pParentUI;
	m_tParentPrevPos = ui.m_tParentPrevPos;
	m_iButtonIndex = 0;
}

UIButton::~UIButton()
{
}

void UIButton::SetParent(Obj* pObj)
{
	m_pParentUI = pObj;
	m_tParentPrevPos = pObj->GetPos();
}


bool UIButton::Init()
{
	ColliderRect* pColl = AddCollider<ColliderRect>("ButtonBody");

	pColl->SetUI(true);

	pColl->AddCollisionFunction(CS_ENTER, this,
		&UIButton::MouseOn);
	pColl->AddCollisionFunction(CS_LEAVE, this,
		&UIButton::MouseOut);

	pColl->SetUI(true);

	SAFE_RELEASE(pColl);

	return true;
}

void UIButton::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIButton::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	if (m_pParentUI)
	{
		POSITION tPos = m_pParentUI->GetPos();

		if (m_tParentPrevPos != tPos)
		{
			m_tPos += tPos - m_tParentPrevPos;
			m_tParentPrevPos = tPos;
		}
	}

	return 0;
}

int UIButton::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);

	if (m_eState != BS_NONE)
	{
		if (KEYPRESS("MouseLButton"))
		{
			m_eState = BS_CLICK;
		}
	}

	if (m_eState == BS_CLICK && KEYUP("MouseLButton"))
	{
		GET_SINGLE(SoundManager)->LoadSound("Effect1", false, "Effect1.wav");
		GET_SINGLE(SoundManager)->Play("Effect1");

		m_eState = BS_MOUSEON;

		string strTag = GetTag();

		if(strTag == "EndButton")
			SetImageOffset(444.f, 490.f);

		else if(strTag == "StartButton")
			SetImageOffset(0.f, 490.f);

		else if (strTag == "EditButton")
			SetImageOffset(148.f, 490.f);

		else if (strTag == "EditAniButton")
			SetImageOffset(296.f, 490.f);

		if (m_bEnableCallback)
			m_BtnCallback(fDeltaTime);

		if (m_bEnableCallback2)
			m_BtnCallback2(m_iButtonIndex,fDeltaTime);
	}

	return 0;
}

void UIButton::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIButton::Render(HDC hDC, float fDeltaTime)
{
	UI::Render(hDC, fDeltaTime);
}

UIButton * UIButton::Clone()
{
	return new UIButton(*this);
}

void UIButton::MouseOn(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		m_eState = BS_MOUSEON;

		string strTag = GetTag();
		if (strTag == "EndButton")
			SetImageOffset(444.f, 490.f);

		else if (strTag == "StartButton")
			SetImageOffset(0.f, 490.f);

		else if (strTag == "EditButton")
			SetImageOffset(148.f, 490.f);

		else if (strTag == "EditAniButton")
			SetImageOffset(296.f, 490.f);
	}
}

void UIButton::MouseOut(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		m_eState = BS_NONE;

		string strTag = GetTag();
		if (strTag == "EndButton")
			SetImageOffset(444.f, 374.f);

		else if (strTag == "StartButton")
			SetImageOffset(0.f, 374.f);

		else if (strTag == "EditButton")
			SetImageOffset(148.f, 374.f);

		else if (strTag == "EditAniButton")
			SetImageOffset(296.f, 374.f);
	}
}
