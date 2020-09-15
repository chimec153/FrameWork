#include "UIInventory.h"
#include "Item.h"
#include "../Core/Camera.h"
#include "..//Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "UIPanel.h"
#include "..//Scene/Scene.h"
#include "Seed.h"
#include "Crop.h"
#include "../Core/Input.h"
#include "../Core.h"
#include "Text.h"
#include "UIButton.h"
#include "UITilePanel.h"
#include "../Collider/ColliderRect.h"
#include "UIContext.h"
#include "ObjManager.h"
#include "Player.h"
#include "../Core/Input.h"
#include "UIShop.h"
#include "UIBuildingShop.h"
#include "UIAnimalShop.h"
#include "Hair.h"
#include "../Core/PathManager.h"

UIInventory::UIInventory()	:
	m_iCount(0),
	m_iCursor(0),
	m_pCursorTex(nullptr),
	m_pBackPanel(nullptr),
	m_iCountX(0),
	m_iCountY(0),
	m_pInventoryPanel(nullptr),
	m_pBackTexture(nullptr),
	m_bExtended(false),
	m_pInfoName(nullptr),
	m_pInfoIconHealth(nullptr),
	m_pInfoIconEnergy(nullptr),
	m_pGoldText(nullptr),
	m_pShop(nullptr),
	m_pPrevColorBtn(nullptr),
	m_pNextColorBtn(nullptr),
	m_iColorIndex(0),
	m_pHairChangeBtn(nullptr),
	m_pPantChangeBtn(nullptr),
	m_iPantsColorIndex(0),
	m_pPrevPantsColorBtn(nullptr),
	m_pNextPantsColorBtn(nullptr),
	m_iCurrentHairColorIndex(0)
{
	m_vecItem.resize(36);

	memset(&m_vecItem[0], 0, 36 * sizeof(Item*));
}

UIInventory::~UIInventory()
{
	Safe_Release_VecList(m_vecItem);
	Safe_Release_VecList(m_vecPanel);
	Safe_Release_VecList(m_vecInvenPanel);
	Safe_Release_VecList(m_vecInfoDescription); 
	SAFE_RELEASE(m_pCursorTex);
	SAFE_RELEASE(m_pBackPanel);
	SAFE_RELEASE(m_pInventoryPanel);
	SAFE_RELEASE(m_pBackTexture);
	SAFE_RELEASE(m_pInfoName);
	SAFE_RELEASE(m_pInfoIconHealth);
	SAFE_RELEASE(m_pInfoIconEnergy);
	SAFE_RELEASE(m_pGoldText);
	SAFE_RELEASE(m_pShop);
	SAFE_RELEASE(m_pPrevColorBtn);
	SAFE_RELEASE(m_pNextColorBtn);
	SAFE_RELEASE(m_pHairChangeBtn);
	SAFE_RELEASE(m_pPrevPantsColorBtn);
	SAFE_RELEASE(m_pNextPantsColorBtn);
}

void UIInventory::AddObjectToLayer(Layer* pLayer)
{
	if(!pLayer->HasObject(this))
		pLayer->AddObject(this);

	size_t iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecItem[i])
		{
			if(!pLayer->HasObject(m_vecItem[i]))
				m_vecItem[i]->AddObjectToLayer(pLayer);
		}			
	}

	if (m_pShop)
		m_pShop->AddObjectToLayer(pLayer);

	Layer* pHUD = m_pScene->FindLayer("HUD");

	if (m_pPrevColorBtn)
		if (!pHUD->HasObject(m_pPrevColorBtn))
			pHUD->AddObject(m_pPrevColorBtn);

	if (m_pNextColorBtn)
	if (!pHUD->HasObject(m_pNextColorBtn))
		pHUD->AddObject(m_pNextColorBtn);

	if (m_pHairChangeBtn)
	if (!pHUD->HasObject(m_pHairChangeBtn))
		pHUD->AddObject(m_pHairChangeBtn);
}

void UIInventory::AddInfoPanelToLayer(Layer* pLayer)
{
	size_t iSize = m_vecPanel.size();

	if(!pLayer->HasObject(m_pBackPanel))
		pLayer->AddObject(m_pBackPanel);

	for (size_t i = 0; i < iSize; ++i)
	{
		if(!pLayer->HasObject(m_vecPanel[i]))
		pLayer->AddObject(m_vecPanel[i]);
	}

	if (m_pInfoName)
	{
		if (!pLayer->HasObject(m_pInfoName))
			pLayer->AddObject(m_pInfoName);
	}

	if (m_pInfoIconEnergy)
	{
		if (!pLayer->HasObject(m_pInfoIconEnergy))
			pLayer->AddObject(m_pInfoIconEnergy);
	}

	if (m_pInfoIconHealth)
	{
		if (!pLayer->HasObject(m_pInfoIconHealth))
			pLayer->AddObject(m_pInfoIconHealth);
	}

	if (m_pGoldText)
	{
		if (!pLayer->HasObject(m_pGoldText))
			pLayer->AddObject(m_pGoldText);
	}
}

void UIInventory::SetObjectLayer(Layer* pLayer)
{
	Scene* pScene = pLayer->GetScene();

	SetLayer(pLayer);
	SetScene(pScene);

	size_t iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecItem[i])
		{
			m_vecItem[i]->SetLayer(pLayer);
			m_vecItem[i]->SetScene(pScene);
		}
	}

	if (m_pShop)
		m_pShop->SetObjectLayer(pLayer);

	if (m_pPrevColorBtn)
	{
		m_pPrevColorBtn->SetLayer(pLayer);
		m_pPrevColorBtn->SetScene(pScene);
	}

	if (m_pNextColorBtn)
	{
		m_pNextColorBtn->SetLayer(pLayer);
		m_pNextColorBtn->SetScene(pScene);
	}

	if (m_pHairChangeBtn)
	{
		m_pHairChangeBtn->SetLayer(pLayer);
		m_pHairChangeBtn->SetScene(pScene);
	}
}

void UIInventory::SetInfoPanelLayer(Layer* pLayer)
{
	Scene* pScene = pLayer->GetScene();

	size_t iSize = m_vecPanel.size();

	m_pBackPanel->SetLayer(pLayer);
	m_pBackPanel->SetScene(pScene);

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecPanel[i]->SetLayer(pLayer);
		m_vecPanel[i]->SetScene(pScene);
	}

	if (m_pInfoName)
	{
		m_pInfoName->SetLayer(pLayer);
		m_pInfoName->SetScene(pScene);
	}

	if (m_pInfoIconEnergy)
	{
		m_pInfoIconEnergy->SetLayer(pLayer);
		m_pInfoIconEnergy->SetScene(pScene);
	}

	if (m_pInfoIconHealth)
	{
		m_pInfoIconHealth->SetLayer(pLayer);
		m_pInfoIconHealth->SetScene(pScene);
	}

	if (m_pGoldText)
	{
		m_pGoldText->SetLayer(pLayer);
		m_pGoldText->SetScene(pScene);
	}
}

void UIInventory::AddItemToLayer(Layer* pLayer)
{
	for (int i = 0; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i])
		{
			if (!pLayer->HasObject(m_vecItem[i]))
				m_vecItem[i]->AddObjectToLayer(pLayer);

			m_vecItem[i]->SetEnable(true);
		}
	}
}

void UIInventory::ReadColorFromFile(char* pFileName)
{
	char strFullPath[MAX_PATH] = {};

	const char* pDataPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	if (pDataPath)
		strcat(strFullPath, pDataPath);

	strcat(strFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFullPath, "rt");

	if (pFile)
	{
		for (int i = 0; i < HAIR_COLOR_MAX * 9; ++i)
		{
			char cLine[256] = {};

			fgets(cLine, 256, pFile);

			char* pContext = nullptr;

			char* pResult =	strtok_s(cLine, ", ", &pContext);

			int iRed = atoi(pResult);

			int iGreen = atoi(strtok_s(nullptr, ", ", &pContext));

			int iBlue = atoi(pContext);

			m_vecColor.push_back(RGB(iRed, iGreen, iBlue));
		}

		fclose(pFile);
	}
}

void UIInventory::SetGoldText(int iGold)
{
	TCHAR strNumber[9] = {};

	int iSize = sizeof(strNumber) / sizeof(TCHAR);

	int iDiv = 1;

	for (int i = 0; i < iSize; ++i)
	{
		if (iGold >= iDiv)
			strNumber[iSize - i - 1] = (iGold % (iDiv * 10)) / iDiv + '0';

		else
		{
			memmove(strNumber, strNumber + iSize - i, sizeof(TCHAR)* i);
			memset(strNumber + i, 0, sizeof(TCHAR)*(iSize - i));

			break;
		}

		iDiv *= 10;
	}

	if (!m_pGoldText)
		CreateGoldText();
	
	m_pGoldText->SetText(strNumber);

	if (m_pShop)
		m_pShop->SetGoldText(strNumber);
}

bool UIInventory::Init()
{
	SetSize(422.f,48.f);
	SetTexture("InventoryShortcut", TEXT("UI\\InvenShortCut.bmp"));
	SetColorKey(255, 255, 255);

	m_pCursorTex = GET_SINGLE(ResourcesManager)->LoadTexture("Menu", TEXT("Maps\\MenuTiles.bmp"));
	m_pCursorTex->SetColorKey(255, 255, 255);

	m_pBackTexture = GET_SINGLE(ResourcesManager)->LoadTexture("pade", TEXT("UI\\Pade.bmp"));

	ReadColorFromFile((char*)"color.txt");

	return true;
}

void UIInventory::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);

	if(!m_pInventoryPanel)
		CreateInventory();

	if (KEYDOWN("Inventory"))
	{
		size_t iSize = m_vecInvenPanel.size();

		if (!m_bExtended)
		{
			m_bExtended = true;

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->SetEnable(false);

			m_pInventoryPanel->SetEnable(true);

			m_pPrevColorBtn->SetEnable(true);

			m_pNextColorBtn->SetEnable(true);

			m_pHairChangeBtn->SetEnable(true);
		}

		else
		{
			m_bExtended = false;

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->SetEnable(false);

			m_pInventoryPanel->SetEnable(false);

			m_pPrevColorBtn->SetEnable(false);

			m_pNextColorBtn->SetEnable(false);

			m_pHairChangeBtn->SetEnable(false);
		}
	}
}

int UIInventory::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	POSITION tPos = {};

	bool bEnable = m_pInventoryPanel->GetEnable();

	bool bShop = false;

	if (m_pShop)
	{
		bShop = m_pShop->IsShopPanelOn();

		m_pShop->Update(fDeltaTime);
	}

	for (int i = 0; i < INVENTORY_SHORT; ++i)
	{
		if (m_vecItem[i])
		{
			if (!m_vecItem[i]->IsDrag())
			{
				POSITION tSize = m_vecItem[i]->GetSize();

				if (!bEnable && !bShop)
				{
					tPos = m_tPos + tCamPos + POSITION(8.f, 8.f);

					tPos += m_vecItem[i]->GetPivot() * tSize;

					tPos.x += (tSize.x + 2) * i;
				}

				else
				{
					tPos = m_pInventoryPanel->GetPos() + tCamPos;

					if (bShop)
					{
						tPos = m_pShop->GetShopPanelPos() + tCamPos;

						tPos.x -= 16.f;
						tPos.y -= 16.f;
					}

					tPos += m_vecItem[i]->GetPivot() * tSize;

					tPos.x += tSize.x * i + 16.f;

					tPos.y += 16.f;
				}

				m_vecItem[i]->SetPos(tPos);
			}

			m_vecItem[i]->EnableItem();
		}
	}

	for (int i = INVENTORY_SHORT; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i])
		{
			if (!m_vecItem[i]->IsDrag())
			{
				POSITION tSize = m_vecItem[i]->GetSize();

				if (!bShop)
					tPos = m_pInventoryPanel->GetPos() + tCamPos;

				else
					tPos = m_pShop->GetShopPanelPos() + tCamPos;

				tPos += m_vecItem[i]->GetPivot() * tSize;

				tPos.x += tSize.x * (i % INVENTORY_SHORT);

				tPos.y += (i / INVENTORY_SHORT) * tSize.y;

				if (!bShop)
				{
					tPos.x += 16.f;

					tPos.y += 21.f;
				}

				m_vecItem[i]->SetPos(tPos);
			}

			if(bEnable || bShop)
				m_vecItem[i]->EnableItem();

			else
				m_vecItem[i]->DisableItem();
		}			
	}

	POSITION tMousePos = GET_SINGLE(Input)->GetMouseClientPos();

	InfoPanelUpdate(tMousePos);

	return 0;
}

int UIInventory::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void UIInventory::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);

	if (m_pShop)
	{
		m_pShop->Collision(fDeltaTime);
	}
}

void UIInventory::Render(HDC hDC, float fDeltaTime)
{
	bool bEnable = m_pInventoryPanel->GetEnable();

	bool bShop = false;

	if(m_pShop)
		bShop = m_pShop->IsShopPanelOn();

	RESOLUTION tRS = GET_SINGLE(Core)->GetResolution();

	if (bEnable || bShop)
	{
		BLENDFUNCTION tBF = {};

		tBF.BlendFlags = 0;
		tBF.BlendOp = 0;
		tBF.AlphaFormat = AC_SRC_OVER;
		tBF.SourceConstantAlpha = 125;

		GdiAlphaBlend(hDC, 0, 0, tRS.iW, tRS.iH, m_pBackTexture->GetDC(), 0, 0, tRS.iW, tRS.iH, tBF);

		if (bShop)
			m_pShop->Render(hDC, fDeltaTime);

		else
		{
			m_pInventoryPanel->Render(hDC, fDeltaTime);

			size_t iSize = m_vecInvenPanel.size();

			for (size_t i = 0; i < iSize; ++i)
				m_vecInvenPanel[i]->Render(hDC, fDeltaTime);
		}
	}

	UI::Render(hDC, fDeltaTime);

	for (int i = 0; i < INVENTORY_SHORT; ++i)
	{
		if (m_vecItem[i])
			m_vecItem[i]->Render(hDC, fDeltaTime);
	}

	if (bShop)
	{
		for (int i = 0; i < INVENTORY_MAX; ++i)
		{
			if (m_vecItem[i])
			{
				m_vecItem[i]->SetAlpha(120);
				m_vecItem[i]->EnableAlpha(true);
			}				
		}
	}

	else
	{
		for (int i = 0; i < INVENTORY_MAX; ++i)
		{
			if (m_vecItem[i])
				m_vecItem[i]->SetAlpha(255);
		}
	}

	if (bEnable || bShop)
	{
		for (int i = INVENTORY_SHORT; i < INVENTORY_MAX; ++i)
		{
			if (m_vecItem[i])
				m_vecItem[i]->Render(hDC, fDeltaTime);
		}
	}

	if (m_pCursorTex)
		TransparentBlt(hDC, (int)m_tPos.x + 8 + m_iCursor * 34, (int)m_tPos.y + 8, 32, 32, m_pCursorTex->GetDC(),
			0, 448, 32, 32, m_pCursorTex->GetColorKey());
	
}

UIInventory* UIInventory::Clone()
{
	return new UIInventory(*this);
}

void UIInventory::CreateInfoPanel(int iCountX, int iCountY)
{
	Layer* pLayer = m_pScene->FindLayer("UI");

	m_pBackPanel = CreateObj<UIPanel>("BackPanel", pLayer);

	m_pBackPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

	m_pBackPanel->SetSize(32.f * (iCountX-1), 32.f * (iCountY-1));

	m_pBackPanel->SetEnable(false);

	m_iCountX = iCountX;
	m_iCountY = iCountY;

	POSITION tPos = m_pBackPanel->GetSize();

	for (int i = 0; i < m_iCountY; ++i)
	{
		for (int j = 0; j < m_iCountX; ++j)
		{
			UIPanel* pPanel = CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(32.f * j + tPos.x - 16.f, 32.f * i + tPos.y - 16.f);

			if (i == 0 && j != 0 && j != m_iCountX - 1)
				pPanel->SetImageOffset(64.f, 0.f);

			else if (i == m_iCountY - 1 && j != 0 && j != m_iCountX - 1)
				pPanel->SetImageOffset(64.f, 96.f);

			else if (j == 0 && i != 0 && i != m_iCountY - 1)
			{
				if (i != 1)
					pPanel->SetImageOffset(0.f, 64.f);

				else
					pPanel->SetImageOffset(0.f, 32.f);
			}

			else if (j == m_iCountX - 1 && i != 0 && i != m_iCountY - 1)
			{
				if (i != 1)
					pPanel->SetImageOffset(96.f, 64.f);

				else
					pPanel->SetImageOffset(96.f, 32.f);
			}

			else if (i == m_iCountY - 1 && j == 0 )
				pPanel->SetImageOffset(0.f, 96.f);

			else if (i == 0 && j == m_iCountX - 1)
				pPanel->SetImageOffset(96.f, 0.f);

			else if (i == m_iCountY - 1 && j == m_iCountX - 1)
				pPanel->SetImageOffset(96.f, 96.f);

			else if(i == 1 && j != 0 && j!=m_iCountX-1)
				pPanel->SetImageOffset(64.f, 32.f);

			else if(i!=0 || j!=0)
				pPanel->SetImageOffset(96.f, 192.f);

			pPanel->SetEnable(false);

			m_vecPanel.push_back(pPanel);
		}
	}

	m_pInfoName = CreateObj<Text>("ItemName", pLayer);

	m_pInfoName->SetEnable(false);

	m_pInfoIconEnergy = CreateObj<UIPanel>("EnergyIcon", pLayer);

	m_pInfoIconEnergy->SetSize(20.f, 20.f);
	m_pInfoIconEnergy->SetTexture("Mouse");
	m_pInfoIconEnergy->SetImageOffset(0.f, 856.f);
	m_pInfoIconEnergy->SetEnable(false);

	m_pInfoIconHealth = CreateObj<UIPanel>("HealthIcon", pLayer);

	m_pInfoIconHealth->SetSize(20.f, 20.f);
	m_pInfoIconHealth->SetTexture("Mouse");
	m_pInfoIconHealth->SetImageOffset(0.f, 876.f);
	m_pInfoIconHealth->SetEnable(false);
}

void UIInventory::InfoPanelOn(const POSITION& tPos, const ITEMINFO& tInfo)
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
			m_vecPanel[i * m_iCountX + j]->SetEnable(true);
	}

	if (&tInfo)
	{
		if(tInfo.eItemType != 0)
		{
			char strName[256] = {};

			strcpy_s(strName, tInfo.strName.c_str());

			TCHAR wstrName[256] = {};

			MultiByteToWideChar(CP_ACP, NULL, strName, -1, wstrName, (int)strlen(strName));

			m_pInfoName->SetText(wstrName);

			size_t iSize = tInfo.vecComment.size();

			Layer* pUILayer = m_pScene->FindLayer("UI");

			for (size_t i = 0; i < iSize; ++i)
			{
				Text* pText = CreateObj<Text>("Description", pUILayer);

				pText->SetText(tInfo.vecComment[i]);

				m_vecInfoDescription.push_back(pText);
			}

			if (tInfo.iHPRecovery != 0)
			{
				Text* pHealthText = CreateObj<Text>("HealthText", pUILayer);

				int iHealth = tInfo.iHPRecovery;

				TCHAR strHealth[256] = {};

				TCHAR strFullText[256] = {};

				swprintf_s(strHealth, TEXT("+ %d"), iHealth);

				lstrcat(strFullText, strHealth);

				lstrcat(strFullText, TEXT(" Health"));

				pHealthText->SetText(strFullText);

				m_pInfoIconHealth->SetEnable(true);

				m_vecInfoDescription.push_back(pHealthText);
			}

			if (tInfo.iEnergyRecovery != 0)
			{
				Text* pEnergyText = CreateObj<Text>("EnergyText", pUILayer);

				int iEnergy = tInfo.iEnergyRecovery;

				TCHAR strEnergy[256] = {};

				TCHAR strFullText[256] = {};

				swprintf_s(strEnergy, TEXT("+ %d"), iEnergy);

				lstrcat(strFullText, strEnergy);

				lstrcat(strFullText, TEXT(" Health"));

				pEnergyText->SetText(strFullText);

				m_pInfoIconEnergy->SetEnable(true);

				m_vecInfoDescription.push_back(pEnergyText);
			}
		}

		else
		{
			Layer* pUILayer = m_pScene->FindLayer("UI");

			size_t iSize = tInfo.vecComment.size();

			for (size_t i = 0; i < iSize; ++i)
			{
				Text* pText = CreateObj<Text>("AnimalDescript", pUILayer);

				pText->SetText(tInfo.vecComment[i]);

				m_vecInfoDescription.push_back(pText);
			}

			char strName[256] = {};

			strcpy_s(strName, tInfo.strName.c_str());

			TCHAR pName[256] = {};

#ifdef _UNICODE
			int iLength = MultiByteToWideChar(CP_ACP, NULL, strName, -1, pName, 0);
			MultiByteToWideChar(CP_ACP, NULL, strName, -1, pName, iLength);
#else
			strcpy_s(pName, strName);
#endif

			pName[iLength - 1] = 0;

			m_pInfoName->SetText(pName);
		}
	}
	else
		m_pInfoName->SetText(TEXT(""));

	m_pInfoName->SetEnable(true);

	m_pBackPanel->SetEnable(true);
}

void UIInventory::InfoPanelUpdate(const POSITION& tPos)
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
			m_vecPanel[i * m_iCountX + j]->SetPos(tPos.x + 32.f * j, tPos.y + 32.f * i);
	}

	POSITION tOffset = POSITION(16.f, 16.f);

	m_pBackPanel->SetPos(tPos + tOffset);

	tOffset += 4.f;

	if(m_pInfoName)
		m_pInfoName->SetPos(tPos + tOffset);

	tOffset.y += 20.f;
	
	size_t iSize = m_vecInfoDescription.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		tOffset.y += 20.f;

		m_vecInfoDescription[i]->SetPos(tPos + tOffset);
	}

	if (m_pInfoIconEnergy->GetEnable())
	{
		tOffset.y -= 20.f;

		m_pInfoIconEnergy->SetPos(tPos + tOffset);

		tOffset.x += 20.f;

		m_vecInfoDescription[iSize - 2]->SetPos(tPos + tOffset);
	}

	if (m_pInfoIconHealth->GetEnable())
	{
		tOffset.x -= 20.f;

		tOffset.y += 20.f;

		m_pInfoIconHealth->SetPos(tPos + tOffset);

		tOffset.x += 20.f;

		m_vecInfoDescription[iSize - 1]->SetPos(tPos + tOffset);
	}
}

void UIInventory::InfoPanelOff()
{
	for (int i = 0; i < m_iCountX; ++i)
	{
		for (int j = 0; j < m_iCountY; ++j)
			m_vecPanel[i * m_iCountX + j]->SetEnable(false);
	}

	size_t iSize = m_vecInfoDescription.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecInfoDescription[i]->Die();

		SAFE_RELEASE(m_vecInfoDescription[i]);
	}

	m_vecInfoDescription.clear();

	m_pInfoName->SetEnable(false);

	m_pBackPanel->SetEnable(false);

	m_pInfoIconEnergy->SetEnable(false);

	m_pInfoIconHealth->SetEnable(false);
}

void UIInventory::CreateInventory()
{
	int iTileNumX = 14;
	int iTileNumY = 10;

	Layer* pLayer = m_pScene->FindLayer("Default");

	m_pInventoryPanel = CreateObj<UIPanel>("InventoryPanel", pLayer);

	m_pInventoryPanel->SetTexture("backPanel", TEXT("UI\\FarmLeft.bmp"));

	m_pInventoryPanel->SetSize((iTileNumX-1) * 32.f, (iTileNumY - 1) * 32.f);

	RESOLUTION tRS = GET_SINGLE(Camera)->GetClientRS();

	POSITION tSize = m_pInventoryPanel->GetSize();

	m_pInventoryPanel->SetPos(tRS.iW / 2.f - tSize.x / 2.f, tRS.iH / 2.f - tSize.y / 2.f);

	m_pInventoryPanel->SetEnable(false);

	m_vecInvenPanel.reserve(132);

	POSITION tPos = m_pInventoryPanel->GetPos();

	for (int i = 0; i < iTileNumY; ++i)
	{
		for (int j = 0; j < iTileNumX; ++j)
		{
			if (i == 0 || i == iTileNumY-1 || j == 0 || j == iTileNumX-1)
			{
				UIPanel* pPanel = CreateObj<UIPanel>("Panel", pLayer);

				pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

				pPanel->SetColorKey(255, 255, 255);

				pPanel->SetSize(32.f, 32.f);

				pPanel->SetPos(32.f * j + tPos.x - 16.f, 32.f * i + tPos.y - 16.f);

				if (i == 0 && j != 0 && j != iTileNumX - 1)
					pPanel->SetImageOffset(64.f, 0.f);

				else if (i == iTileNumY - 1 && j != 0 && j != iTileNumX - 1)
					pPanel->SetImageOffset(64.f, 96.f);

				else if (j == 0 && i != 0 && i != iTileNumY - 1)
					pPanel->SetImageOffset(0.f, 64.f);

				else if (j == iTileNumX - 1 && i != 0 && i != iTileNumY - 1)
					pPanel->SetImageOffset(96.f, 64.f);

				else if (i == iTileNumY - 1 && j == 0)
					pPanel->SetImageOffset(0.f, 96.f);

				else if (i == 0 && j == iTileNumX - 1)
					pPanel->SetImageOffset(96.f, 0.f);

				else if (i == iTileNumY - 1 && j == iTileNumX - 1)
					pPanel->SetImageOffset(96.f, 96.f);

				pPanel->SetEnable(false);

				m_vecInvenPanel.push_back(pPanel);
			}
		}		
	}

	for (int i = 0; i < iTileNumX; ++i)
	{
		UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

		pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

		pPanel->SetColorKey(255, 255, 255);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(32.f * i + tPos.x - 16.f, tRS.iH / 2.f - 32.f);

		if (i != 0 && i != iTileNumX-1)
			pPanel->SetImageOffset(64.f, 32.f);

		else if (i == 0)
			pPanel->SetImageOffset(0.f, 32.f);

		else
			pPanel->SetImageOffset(96.f, 32.f);

		pPanel->SetEnable(false);

		m_vecInvenPanel.push_back(pPanel);
	}

	for (int i = 0; i < 12; ++i)
	{
		UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

		pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

		pPanel->SetColorKey(255, 255, 255);

		pPanel->SetSize(32.f, 32.f);

		pPanel->SetPos(32.f * i + tPos.x + 16.f, tPos.y + 16.f);

		pPanel->SetImageOffset(64.f, 64.f);

		pPanel->SetEnable(false);

		m_vecInvenPanel.push_back(pPanel);
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 12; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(32.f * i + tPos.x + 16.f, 32.f * (j + 1) + tPos.y + 16.f + 5.f);

			pPanel->SetImageOffset(64.f, 64.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);
		}
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(14.f + 100.f * j + tPos.x, 32.f * (i + 4) + tPos.y + 16.f);

			if (i == 0 && j == 0)
				pPanel->SetImageOffset(32.f, 320.f);

			else if (i == 1 && j == 0)
				pPanel->SetImageOffset(96.f, 320.f);

			else if (i == 2 && j == 0)
				pPanel->SetImageOffset(0.f, 320.f);

			else if (i == 0 && j == 1)
				pPanel->SetImageOffset(64.f, 320.f);

			else if (i == 1 && j == 1)
				pPanel->SetImageOffset(32.f, 288.f);

			else if (i == 2 && j == 1)
				pPanel->SetImageOffset(0.f, 288.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);
		}
	}

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 3; ++i)
		{
			UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

			pPanel->SetTexture("menuTile", TEXT("Maps\\MenuTiles.bmp"));

			pPanel->SetColorKey(255, 255, 255);

			pPanel->SetSize(32.f, 32.f);

			pPanel->SetPos(48.f + 32*j + tPos.x, 32.f * (i + 4) + tPos.y + 16.f);

			if ((i + j) % 2 == 0)
				pPanel->SetImageOffset(0.f, 512.f);

			else
				pPanel->SetImageOffset(64.f, 512.f);

			pPanel->SetEnable(false);

			m_vecInvenPanel.push_back(pPanel);
		}
	}

	UIPanel* pPanel = Obj::CreateObj<UIPanel>("Panel", pLayer);

	pPanel->SetTexture("playerIdleDown", TEXT("aniii.bmp"));

	pPanel->SetColorKey(0, 0, 0);

	pPanel->SetSize(32.f, 64.f);

	pPanel->SetPos(64.f +tPos.x, 32.f * 5.f + tPos.y);

	pPanel->SetEnable(false);

	m_vecInvenPanel.push_back(pPanel);

	Layer* pUILayer = m_pScene->FindLayer("UI");

	m_pHairChangeBtn = Obj::CreateObj<UIButton>("HairButton", pUILayer);	//	머리 색 변경 버튼을 생성한다.

	m_pHairChangeBtn->SetSize(18.f, 18.f);
	m_pHairChangeBtn->SetImageOffset(864.f, 878.f);
	m_pHairChangeBtn->SetTexture("Mouse");
	m_pHairChangeBtn->SetCallback(CS_STAY, this, &UIInventory::ChangeHairColor);
	m_pHairChangeBtn->SetAlpha(255);
	m_pHairChangeBtn->EnableAlpha(true);
	m_pHairChangeBtn->SetPos(68.f + tPos.x, 32.f * 7.f + tPos.y + 18.f);
	m_pHairChangeBtn->SetEnable(true);

	ColliderRect* pChangeButtonRC = (ColliderRect*)m_pHairChangeBtn->GetCollider("ButtonBody");

	pChangeButtonRC->SetRect(0.f, 0.f, 18.f, 18.f);

	SAFE_RELEASE(pChangeButtonRC);

	m_pPrevColorBtn = Obj::CreateObj<UIButton>("HairColorPrevButton", pUILayer);	//	머리 이전 색 버튼을 생성한다.

	m_pPrevColorBtn->SetSize(24.f, 22.f);
	m_pPrevColorBtn->SetImageOffset(704.f, 990.f);
	m_pPrevColorBtn->SetTexture("Mouse");
	m_pPrevColorBtn->SetCallback(CS_STAY, this, &UIInventory::GetColorPrev);
	m_pPrevColorBtn->SetAlpha(255);
	m_pPrevColorBtn->EnableAlpha(true);
	m_pPrevColorBtn->SetPos(32.f + tPos.x, 32.f * 7.f + tPos.y + 18.f);
	m_pPrevColorBtn->SetEnable(true);

	ColliderRect* pUpButtonRC = (ColliderRect*)m_pPrevColorBtn->GetCollider("ButtonBody");

	pUpButtonRC->SetRect(0.f, 0.f, 24.f, 22.f);

	SAFE_RELEASE(pUpButtonRC);

	m_pNextColorBtn = Obj::CreateObj<UIButton>("HairColorNextButton", pUILayer);	//	머리 다음 색 버튼을 생성한다.

	m_pNextColorBtn->SetSize(24.f, 22.f);
	m_pNextColorBtn->SetImageOffset(730.f, 990.f);
	m_pNextColorBtn->SetTexture("Mouse");
	m_pNextColorBtn->SetCallback(CS_STAY, this, &UIInventory::GetColorNext);
	m_pNextColorBtn->SetAlpha(255);
	m_pNextColorBtn->EnableAlpha(true);
	m_pNextColorBtn->SetPos(96.f + tPos.x, 32.f * 7.f + tPos.y + 18.f);
	m_pNextColorBtn->SetEnable(true);

	ColliderRect* pDownButtonRC = (ColliderRect*)m_pNextColorBtn->GetCollider("ButtonBody");

	pDownButtonRC->SetRect(0.f, 0.f, 24.f, 22.f);

	SAFE_RELEASE(pDownButtonRC);

	m_pPrevColorBtn->SetEnable(false);

	m_pNextColorBtn->SetEnable(false);

	m_pHairChangeBtn->SetEnable(false);
}

void UIInventory::SwapItem(Item* pItem, const POSITION& tPos)
{
	if (!m_pInventoryPanel->GetEnable())
	{
		POSITION tSwapPos = tPos - POSITION(8.f, 8.f) - m_tPos;

		int iIndexX = (int)(tSwapPos.x / 33.f);
		int iIndexY = (int)(tSwapPos.y / 33.f);
		
		if (iIndexY == 0 && iIndexX < 12 && iIndexX >= 0)
		{
			size_t iSize = m_vecItem.size();

			int iIndex = 0;

			for (size_t i = 0; i < iSize; ++i)
			{
				if (m_vecItem[i] == pItem)
				{
					iIndex = (int)i;
					break;
				}
			}

			if (m_vecItem[iIndexX])
			{
				Item* pTempItem = m_vecItem[iIndexX];
				m_vecItem[iIndexX] = m_vecItem[iIndex];
				m_vecItem[iIndex] = pTempItem;
			}
			else
			{
				m_vecItem[iIndexX] = pItem;
				m_vecItem[iIndex] = nullptr;
			}
		}
	}

	else
	{
		POSITION tSwapPos = tPos - POSITION(16.f, 16.f) - m_pInventoryPanel->GetPos();

		int iIndexX = (int)(tSwapPos.x / 32.f);
		int iIndexY = (int)(tSwapPos.y / 32.f);

		if (iIndexY >= 0 && iIndexY <= 2 && iIndexX < 12 && iIndexX >= 0)
		{
			size_t iSize = m_vecItem.size();

			int iIndex = 0;

			for (size_t i = 0; i < iSize; ++i)
			{
				if (m_vecItem[i] == pItem)
				{
					iIndex = (int)i;
					break;
				}
			}

			if (m_vecItem[iIndexX + iIndexY * 12])
			{
				Item* pTempItem = m_vecItem[iIndexX + iIndexY * 12];
				m_vecItem[iIndexX + iIndexY * 12] = m_vecItem[iIndex];
				m_vecItem[iIndex] = pTempItem;
			}
			else
			{
				m_vecItem[iIndexX + iIndexY * 12] = pItem;
				m_vecItem[iIndex] = nullptr;
			}
		}
	}
}

void UIInventory::AddItem(Item* pItem)
{
	if (pItem)
	{
		ITEM_TYPE eItemType = pItem->GetType();

		if (eItemType != IT_TOOL)
		{
			for (int i = 0; i < 36; ++i)
			{
				if (m_vecItem[i])
				{
					ITEM_TYPE eItemType = m_vecItem[i]->GetType();

					if (eItemType == pItem->GetType())
					{
						if (eItemType != IT_TOOL)
						{
							int iIndex = m_vecItem[i]->GetIndex();

							if (iIndex == pItem->GetIndex())
							{
								m_vecItem[i]->AddItemCount(1);

								pItem->Die();

								return;
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < 36; ++i)
		{
			if (!m_vecItem[i])
			{
				m_vecItem[i] = pItem;

				ITEM_TYPE eItemType = m_vecItem[i]->GetType();

				if (eItemType == IT_SEED || eItemType == IT_CROP || eItemType == IT_ETC)
				{
					int iItemCount = m_vecItem[i]->GetItemCount();

					m_vecItem[i]->CreateItemNumber();

				}

				pItem->AddItemCount(1);

				pItem->AddRef();

				pItem->SetInventory(this);

				++m_iCount;

				break;
			}
		}
	}
}

void UIInventory::DeleteItem()
{
	if (m_vecItem[m_iCursor])
	{
		int iCount = m_vecItem[m_iCursor]->GetItemCount();

		if (iCount > 1)
			m_vecItem[m_iCursor]->AddItemCount(-1);

		else
		{
			m_vecItem[m_iCursor]->DeleteItemNumber();

			m_vecItem[m_iCursor]->Die();

			SAFE_RELEASE(m_vecItem[m_iCursor]);

			--m_iCount;
		}
	}
}

void UIInventory::DeleteItem(Item* pItem)
{
	for (int i = 0; i < INVENTORY_MAX; ++i)
	{
		if (m_vecItem[i] == pItem)
		{
			int iCount = m_vecItem[i]->GetItemCount();

			if (iCount == 1)
			{
				m_vecItem[i] = nullptr;

				pItem->DeleteItemNumber();

				pItem->Die();

				--m_iCount;
			}				

			else
				m_vecItem[i]->AddItemCount(-1);
		}
	}
}

void UIInventory::CreateGoldText()
{
	if (m_pScene)
	{
		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		m_pGoldText = Obj::CreateObj<Text>("GoldText", pHUDLayer);

		m_pGoldText->SetPos(1052.f, 130.f);

		m_pGoldText->SetColor(102, 27, 0);
	}
}

void UIInventory::CreateShop(const string& strName)
{
	if (!m_pShop)
	{
		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		m_pShop = Obj::CreateObj<UIShop>("Shop", pHUDLayer);

		m_pShop->SetEnable(false);

		m_pShop->SetInven(this);
	}

	m_pShop->CreateShopPanel(strName);

	m_pShop->SetGoldText(m_pGoldText->GetText());

	return;
}

void UIInventory::CreateBuildShop()
{
		Layer* pHUDLayer = m_pScene->FindLayer("HUD");

		UIBuildingShop* pBuildingShop = Obj::CreateObj<UIBuildingShop>("Shop", pHUDLayer);

		pBuildingShop->SetInven(this);

		pBuildingShop->CreatePanel();

		SAFE_RELEASE(pBuildingShop);
}

void UIInventory::CreateAnimalShop()
{
	Layer* pHUDLayer = m_pScene->FindLayer("HUD");

	UIAnimalShop* pAnimalShop = Obj::CreateObj<UIAnimalShop>("Shop", pHUDLayer);

	pAnimalShop->SetInven(this);

	pAnimalShop->CreatePanel();

	SAFE_RELEASE(pAnimalShop);
}

void UIInventory::ChangeHairColor(int iIndex, float fTime)
{
	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	Hair* pHair = (Hair*)pPlayer->GetHair();

	Texture* pTexture = GET_SINGLE(ResourcesManager)->FindTexture("81210");

	Texture* pLeftTexture = GET_SINGLE(ResourcesManager)->FindTexture("81210Left");

	FrameInfo tInfo = pHair->GetFrame("HairIdleDown");

	FrameInfo tLeftInfo = pHair->GetFrame("HairIdleLeft");

	HDC hDC = pTexture->GetDC();

	HDC hLeftDC = pLeftTexture->GetDC();

	size_t iColorSize = m_vecColor.size() / HAIR_COLOR_MAX;

	for (int k = 0; k < iColorSize; ++k)
	{
		COLORREF tPrevColor = m_vecColor[k + m_iCurrentHairColorIndex * iColorSize];

		COLORREF tColor = m_vecColor[k + m_iColorIndex * iColorSize];

		for (int j = 0; j < 192; ++j)
		{
			for (int i = 0; i < 32; ++i)
			{
				COLORREF tCurrentColor = GetPixel(hDC, (int)(i + tInfo.tStart.x), (int)(j + tInfo.tStart.y));

				if (tCurrentColor == tPrevColor)
				{
					SetPixel(hDC, (int)(i + tInfo.tStart.x), (int)(j + tInfo.tStart.y), tColor);
				}
			}
		}

		for (int j = 0; j < 64; ++j)
		{
			for (int i = 0; i < 32; ++i)
			{
				COLORREF tCurrentColor = GetPixel(hLeftDC, (int)(i + tLeftInfo.tStart.x), (int)(j + tLeftInfo.tStart.y));

				if (tCurrentColor == tPrevColor)
				{
					SetPixel(hLeftDC, (int)(i + tLeftInfo.tStart.x), (int)(j + tLeftInfo.tStart.y), tColor);
				}
			}
		}
	}

	m_iCurrentHairColorIndex = m_iColorIndex;

	SAFE_RELEASE(pPlayer);

	SAFE_RELEASE(pTexture);

	SAFE_RELEASE(pLeftTexture);
}

void UIInventory::GetColorPrev(int iIndex, float fTime)
{
	--m_iColorIndex;

	if (m_iColorIndex < 0)
	{
		++m_iColorIndex;

		return;
	}

	size_t iSize = m_vecInvenPanel.size();

	Texture* pTexture = m_vecInvenPanel[iSize - 1]->GetTexture();

	size_t iColorSize = m_vecColor.size() / HAIR_COLOR_MAX;

	HDC hDC = pTexture->GetDC();

	for (int k = 0; k < iColorSize; ++k)
	{
		COLORREF tPrevColor = m_vecColor[k + (m_iColorIndex + 1) * iColorSize];

		COLORREF tColor = m_vecColor[k + m_iColorIndex * iColorSize];

		for (int j = 0; j < 64; ++j)
		{
			for (int i = 0; i < 32; ++i)
			{
				COLORREF tCurrentColor = GetPixel(hDC, i,j);

				if (tCurrentColor == tPrevColor)
				{
					SetPixel(hDC, i, j, tColor);
				}
			}
		}
	}

	SAFE_RELEASE(pTexture);
}

void UIInventory::GetColorNext(int iIndex, float fTime)
{
	++m_iColorIndex;

	if (m_iColorIndex == HAIR_COLOR_MAX)
	{
		--m_iColorIndex;

		return;
	}		

	size_t iSize = m_vecInvenPanel.size();

	size_t iColorSize = m_vecColor.size() / HAIR_COLOR_MAX;

	Texture* pTexture = m_vecInvenPanel[iSize - 1]->GetTexture();

	HDC hDC = pTexture->GetDC();

	for (int k = 0; k < iColorSize; ++k)
	{
		COLORREF tPrevColor = m_vecColor[k + (m_iColorIndex-1) * iColorSize];

		COLORREF tColor = m_vecColor[k + m_iColorIndex * iColorSize];

		for (int j = 0; j < 64; ++j)
		{
			for (int i = 0; i < 32; ++i)
			{
				COLORREF tCurrentColor = GetPixel(hDC, i, j);

				if (tCurrentColor == tPrevColor)
				{
					SetPixel(hDC, i, j, tColor);
				}
			}
		}
	}

	SAFE_RELEASE(pTexture);
}
