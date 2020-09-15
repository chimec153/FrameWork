#include "UIContext.h"
#include "Text.h"
#include "../Collider/ColliderRect.h"
#include "UIInventory.h"
#include "../Resources/ResourcesManager.h"
#include "../Scene/Scene.h"
#include "UIPanel.h"
#include "../Core/Input.h"
#include "UIShop.h"

UIContext::UIContext() :
	m_pItem(nullptr),
	m_pName(nullptr),
	m_pPrice(nullptr),
	m_pIcon(nullptr),
	m_pInven(nullptr),
	m_iIndex(0)
{
}

UIContext::UIContext(const UIContext& context)	:
	UI(context)
{
	if (context.m_pItem)
		m_pItem = context.m_pItem->Clone();

	if (context.m_pName)
		m_pName = context.m_pName->Clone();

	if (context.m_pPrice)
		m_pPrice = context.m_pPrice->Clone();

	if (context.m_pIcon)
		m_pIcon = context.m_pIcon->Clone();

	m_pInven = context.m_pInven;
	m_iIndex = context.m_iIndex;
}

UIContext::~UIContext()
{
	SAFE_RELEASE(m_pItem);
	SAFE_RELEASE(m_pName);
	SAFE_RELEASE(m_pPrice);
	SAFE_RELEASE(m_pIcon);
}

void UIContext::SetIndex(int iIndex)
{
	Layer* pLayer = m_pScene->FindLayer("Default");

	m_iIndex = iIndex;

	PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(iIndex);

	m_pIcon = Obj::CreateObj<UIPanel>("MoneyIcon", pLayer);

	m_pIcon->SetTexture("Mouse");
	m_pIcon->SetImageOffset(386.f, 746.f);
	m_pIcon->SetEnable(false);
	m_pIcon->SetPos(11.f * 32.f, 0.f);
	m_pIcon->SetSize(18.f, 18.f);

	m_pItem = Obj::CreateObj<UIPanel>("ItemPanel", pLayer);

	if (iIndex < 36)
		m_pItem->SetTexture("items");

	else
		m_pItem->SetTexture("weapon", TEXT("Item\\weapons.bmp"));

	m_pItem->SetImageOffset(pInfo->tTileOffset.x * 32.f, pInfo->tTileOffset.y * 32.f);
	m_pItem->SetEnable(false);
	m_pItem->SetSize(32.f, 32.f);

	m_pName = Obj::CreateObj<Text>("ItemName", pLayer);

	char pName[256] = {};

	strcpy_s(pName, pInfo->strName.c_str());

	TCHAR strName[256] = {};

	MultiByteToWideChar(CP_ACP, NULL, pName, -1, strName, (int)strlen(pName));

	((Text*)m_pName)->SetText(strName);
	m_pName->SetEnable(false);
	m_pName->SetPos(32.f, 0.f);

	char pNumber[6] = {};

	TCHAR strNumber[256] = {};

	int iSize = sizeof(pNumber) / sizeof(char);

	int iDiv = 1;

	size_t iPriceSize = pInfo->vecPrice.size();

	for (int i = 0; i < iSize; ++i)
	{
		if (iPriceSize > 0)
		{
			if (pInfo->vecPrice[0] >= iDiv)
				pNumber[iSize - i - 1] = (pInfo->vecPrice[0] % (iDiv * 10)) / iDiv + '0';

			else
			{
				memmove(pNumber, pNumber + iSize - i, i);
				memset(pNumber + i, 0, iSize - i);

				break;
			}

			iDiv *= 10;
		}
	}

	MultiByteToWideChar(CP_ACP, NULL, pNumber, -1, strNumber, (int)strlen(pNumber));

	m_pPrice = Obj::CreateObj<Text>("Price", pLayer);
	((Text*)m_pPrice)->SetText(strNumber);
	m_pPrice->SetEnable(false);
	m_pPrice->SetPos(32.f * 8.f, 0.f);
}

bool UIContext::Init()
{
	POSITION tSize = GetSize();

	ColliderRect* pRC = AddCollider<ColliderRect>("ContextBody");

	pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);

	pRC->AddCollisionFunction(CS_ENTER, this, &UIContext::ColEnter);

	pRC->AddCollisionFunction(CS_STAY, this, &UIContext::ColStay);

	pRC->AddCollisionFunction(CS_LEAVE, this, &UIContext::ColEnd);

	pRC->SetUI(true);

	SAFE_RELEASE(pRC);

	return true;
}

void UIContext::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int UIContext::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	POSITION tPanelPos = GetPos();
	POSITION tPos = {};

	m_pItem->SetPos(tPos + tPanelPos);

	tPos += POSITION(37.f, 6.f);

	m_pName->SetPos(tPos + tPanelPos);
	m_pName->SetColor(102, 28, 28);

	tPos += POSITION(32.f * 12.f - 5.f, 0.f);

	m_pPrice->SetPos(tPos + tPanelPos);
	m_pPrice->SetColor(102, 28, 28);

	tPos += POSITION(32.f * 2.f, 1.f);

	m_pIcon->SetPos(tPos + tPanelPos);

	return 0;
}

int UIContext::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIContext::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void UIContext::Render(HDC hDC, float fDeltaTime)
{
	UI::Render(hDC, fDeltaTime);

	if(m_pItem)
		m_pItem->Render(hDC, fDeltaTime);

	if (m_pIcon)
		m_pIcon->Render(hDC, fDeltaTime);

	if (m_pName)
		m_pName->Render(hDC, fDeltaTime);

	if (m_pPrice)
		m_pPrice->Render(hDC, fDeltaTime);

#ifdef _DEBUG
	/*
	if (KEYPRESS("Debug"))
	{
		TCHAR strScene[32] = {};

		swprintf_s(strScene, TEXT("Scene: %lld"), (long long)m_pScene);

		TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y, strScene, lstrlen(strScene));
	}*/
#endif
}

UIContext* UIContext::Clone()
{
	return new UIContext(*this);
}

void UIContext::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		POSITION tPos = pDest->GetObj()->GetPos();

		PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(m_iIndex);

		if (m_pInven)
			m_pInven->InfoPanelOn(tPos, *pInfo);
	}
}

void UIContext::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		POSITION tPos = pDest->GetObj()->GetPos();

		if (m_pInven)
			m_pInven->InfoPanelUpdate(tPos);

		if (KEYDOWN("MouseLButton"))
		{
			m_pInven->GetShop()->BuyItem(m_iIndex);
		}
	}
}

void UIContext::ColEnd(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "Mouse")
	{
		POSITION tPos = pDest->GetObj()->GetPos();

		if (m_pInven)
			m_pInven->InfoPanelOff();
	}
}
