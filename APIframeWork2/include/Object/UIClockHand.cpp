#include "UIClockHand.h"
#include "../Animation/Animation.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "Crop.h"
#include "../Object/FarmEffect.h"
#include "../Scene/Scene.h"
#include "Stage.h"
#include "Text.h"
#include "ObjManager.h"
#include "Player.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneHome.h"
#include "UIInventory.h"
#include "UIPanel.h"
#include "UIShop.h"
#include "Animal.h"
#include "Egg.h"

UIClockHand::UIClockHand()	:
	m_fTime(720.f),
	m_pNightPanel(nullptr),
	m_bRain(false),
	m_iDay(1),
	m_iWeek(0),
	m_iMonth(3),
	m_fRainTime(0.f),
	m_fRainTimeMax(0.5f),
	m_pWeekText(nullptr),
	m_pTimeText(nullptr),
	m_pWeatherPanel(nullptr),
	m_pSeasonPanel(nullptr),
	m_bBackToHome(true)
{
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

UIClockHand::UIClockHand(const UIClockHand& clockhand)	:
	UI(clockhand)
{
	m_fTime = clockhand.m_fTime;
	m_pNightPanel = clockhand.m_pNightPanel;
	m_bRain = clockhand.m_bRain;
	m_iDay = clockhand.m_iDay;
	m_iWeek = clockhand.m_iWeek;
	m_fRainTimeMax = clockhand.m_fRainTimeMax;
	m_fRainTime = 0.f;

	if (clockhand.m_pWeekText)
		m_pWeekText = clockhand.m_pWeekText->Clone();

	if (clockhand.m_pTimeText)
		m_pTimeText = clockhand.m_pTimeText->Clone();

	if (clockhand.m_pWeatherPanel)
		m_pWeatherPanel = clockhand.m_pWeatherPanel->Clone();

	if (clockhand.m_pSeasonPanel)
		m_pSeasonPanel = clockhand.m_pSeasonPanel->Clone();

	m_iMonth = clockhand.m_iMonth;
}

UIClockHand::~UIClockHand()
{
	SAFE_RELEASE(m_pNightPanel);
	SAFE_RELEASE_VECLIST(m_CropList);
	SAFE_RELEASE_VECLIST(m_AnimalList);
	SAFE_DELETE_VECLIST(m_vecWeekText);
	SAFE_RELEASE(m_pWeekText);
	SAFE_RELEASE(m_pTimeText);
	SAFE_RELEASE(m_pWeatherPanel);
	SAFE_RELEASE(m_pSeasonPanel);
}

void UIClockHand::AddTime(float fTime)
{
	m_fTime += fTime * 2.f;

	if (m_fTime >= 1440.f)
	{
		m_fTime -= 1440.f;
	}

	else if (m_fTime >= 120.f && m_fTime < 360.f)
	{
		AddDay();

		m_fTime = 360.f;

		Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

		pPlayer->SetHP(50);
		pPlayer->SetEnergy(50);

		if (m_bBackToHome)
		{
			GET_SINGLE(SceneManager)->CreateScene<SceneHome>("Home", SC_NEXT);

			pPlayer->SetPos(288.f, 288.f);

			SAFE_RELEASE(pPlayer);
		}
	}

	TCHAR strHour[32] = {};
	TCHAR strMinute[32] = {};

	swprintf_s(strHour, TEXT("%d"), (int)m_fTime / 60);

	swprintf_s(strMinute, TEXT("%d"), (int)m_fTime % 60);

	TCHAR strText[MAX_PATH] = {};

	lstrcat(strText, strHour);

	lstrcat(strText, TEXT(":"));

	lstrcat(strText, strMinute);

	if(m_fTime < 720.f)
		lstrcat(strText, TEXT(" Am"));

	else
		lstrcat(strText, TEXT(" Pm"));

	m_pTimeText->SetText(strText);
}

void UIClockHand::AddDay()
{
	m_bRain = false;

	++m_iDay;

	m_iWeek = ++m_iWeek % 7;

	TCHAR strNum[32] = {};

	swprintf_s(strNum, TEXT("%d"), m_iDay % 31+1);

	TCHAR strText[MAX_PATH] = {};

	lstrcat(strText, m_vecWeekText[m_iWeek]);

	lstrcat(strText, strNum);

	m_pWeekText->SetText(strText);

	m_fTime = 0.f;

	auto iter = m_CropList.begin();
	auto iterEnd = m_CropList.end();

	for (; iter != iterEnd;)
	{
		ITEM_TYPE eType = ((Item*)(*iter))->GetType();

		if (eType == IT_CROP)
		{
			bool bFrozen = ((Crop*)* iter)->IsFrozen();

			if (!bFrozen)
			{
				if (!((Crop*)(*iter))->IsStart())
					((Crop*)(*iter))->TimeStart();

				else
					((Crop*)(*iter))->AddDay(1);

				CROP_TYPE eCropType = ((Crop*)* iter)->GetCropType();

				if (eCropType != CROP_NONE && m_iDay % 365 > 270)
				{
					POSITION tOffset = ((Crop*)* iter)->GetImageOffset();

					if (tOffset.x < 256.f)
						tOffset.x += 256.f;

					((Crop*)* iter)->SetImageOffset(tOffset.x, 12.f * 64.f);
					((Crop*)* iter)->SetFrozen(true);
				}
			}

			++iter;
		}

		else if (eType == IT_EGG)
		{
			if (((Egg*)(*iter))->AddDay())
			{
				SAFE_RELEASE((*iter));
				iter = m_CropList.erase(iter);
				iterEnd = m_CropList.end();
			}
			else
				++iter;
		}
	}

	auto iterAnimal = m_AnimalList.begin();
	auto iterAnimalEnd = m_AnimalList.end();

	for (; iterAnimal != iterAnimalEnd; ++iterAnimal)
	{
		((Animal*)(*iterAnimal))->AddDay(1);
	}

	float fRain = 0;

	if (m_iDay % 365 > 270)
	{
		fRain = rand() % 10001 / 100.f + 28.f;

		m_pSeasonPanel->SetImageOffset(812.f, 930.f);
	}		

	else if (m_iDay % 365 > 180)
	{
		fRain = rand() % 10001 / 100.f + 15.f;

		m_pSeasonPanel->SetImageOffset(812.f, 914.f);
	}		

	else if (m_iDay % 365 > 90)
	{
		fRain = rand() % 10001 / 100.f;

		m_pSeasonPanel->SetImageOffset(812.f, 898.f);
	}		

	else if (m_iDay % 365 <= 90)
	{
		fRain = rand() % 10001 / 100.f + 15.f;

		m_pSeasonPanel->SetImageOffset(812.f, 882.f);
	}		

	if (fRain < 30.f)
	{
		m_bRain = true;

		m_fRainTimeMax = 0.02f;

		m_pWeatherPanel->SetImageOffset(730.f, 842.f);
	}
	else
		m_pWeatherPanel->SetImageOffset(682.f, 842.f);

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	UIShop* pShop = pInven->GetShop(); 
	
	if(pShop)
		pShop->DisableShopPanel(0, 0.f);

	SAFE_RELEASE(pInven);
}

void UIClockHand::SetNightPanel(Obj* pObj)
{
	SAFE_RELEASE(m_pNightPanel);

	m_pNightPanel = pObj;

	if (m_pNightPanel)
		m_pNightPanel->AddRef();
}

void UIClockHand::SetWeatherPanel(UIPanel* pPanel)
{
	SAFE_RELEASE(m_pWeatherPanel);

	m_pWeatherPanel = pPanel;

	if (m_pWeatherPanel)
		m_pWeatherPanel->AddRef();
}

void UIClockHand::SetSeasonPanel(UIPanel* pPanel)
{
	SAFE_RELEASE(m_pSeasonPanel);

	m_pSeasonPanel = pPanel;

	if (m_pSeasonPanel)
		m_pSeasonPanel->AddRef();
}

void UIClockHand::SetWeekText(Text* pText)
{
	SAFE_RELEASE(m_pWeekText);

	m_pWeekText = pText;

	if (m_pWeekText)
	{
		m_pWeekText->AddRef();

		TCHAR strNum[32] = {};

		swprintf_s(strNum, TEXT("%d"), m_iDay);

		TCHAR strText[MAX_PATH] = {};

		lstrcat(strText, m_vecWeekText[m_iWeek]);

		lstrcat(strText, strNum);

		m_pWeekText->SetText(strText);
	}
}

void UIClockHand::SetTimeText(Text* pText)
{
	SAFE_RELEASE(m_pTimeText);

	m_pTimeText = pText;

	if (m_pTimeText)
		m_pTimeText->AddRef();
}

bool UIClockHand::Init()
{
	SetSize(100.f, 100.f);
	m_tOriginPos = m_tPos;

	SetTexture("ClockHand", TEXT("UI\\Arrow"), 13);

	if(m_pTexture)
		m_pTexture->SetColorKeyAll(RGB(255, 255, 255));

	TCHAR* pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Sun, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Mon, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Tue, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Wed, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Thu, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Fri, "));

	m_vecWeekText.push_back(pWeek);

	pWeek = new TCHAR[MAX_PATH];

	lstrcpy(pWeek, TEXT("Sat, "));

	m_vecWeekText.push_back(pWeek);

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

	float fHour = m_fTime / 60.f;

	int iAlpha = 0;

	if (!m_bRain)
	{
		if (fHour <= 4.5f || fHour >= 19.5f)
			iAlpha = 120;

		else if (fHour < 7.5f && fHour > 4.5f)
			iAlpha = (int)(-40.f * fHour + 300.f);

		else if (fHour > 16.5f && fHour < 19.5f)
			iAlpha = (int)(-660.f + 40.f * fHour);
	}

	else
	{
		STAGE_TYPE eType = m_pScene->GetStage()->GetStageType();

		if(eType == STAGE_OUTDOOR)
			iAlpha = 120;
	}

	m_pNightPanel->SetAlpha(iAlpha);

	if (m_bRain)
	{
		m_fRainTime += fDeltaTime;

		while (m_fRainTime >= m_fRainTimeMax)		
		{
			m_fRainTime -= m_fRainTimeMax;

			int iTileNumX = m_pScene->GetStage()->GetTileNumX();
			int iTileNumY = m_pScene->GetStage()->GetTileNumY();

			int iIndexX = rand() % (iTileNumX-1);
			int iIndexY = rand() % (iTileNumY-1);

			STAGE_TYPE eType = m_pScene->GetStage()->GetStageType();

			if (eType == STAGE_OUTDOOR)
			{
				Layer* pLayer = m_pScene->FindLayer("Default");

				FarmEffect* pEffect = (FarmEffect*)CreateCloneObj("HoeEffect", "RainEffect", pLayer);

				if (pEffect)
				{
					POSITION tPos = POSITION(32.f, 32.f);

					pEffect->SetPos(iIndexX * tPos.x, iIndexY * tPos.y);

					pEffect->SetAnimationCurrentClip("Rain");

					SAFE_RELEASE(pEffect);
				}
			}
		}
	}

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
	if (12 < (int)m_fTime / 120.f)
		return;

	if (m_pTexture)
		m_pTexture->RenderByAlpha(m_cAlpha, hDC, m_tPos, POSITION::Zero, m_tSize, 12 - (int)(m_fTime/120.f));
}

UIClockHand* UIClockHand::Clone()
{
	return new UIClockHand(*this);
}

void UIClockHand::AddObjectToLayer(Layer* pLayer)
{
	pLayer->AddObject(this);

	if (m_pWeekText)
		pLayer->AddObject(m_pWeekText);

	if (m_pTimeText)
		pLayer->AddObject(m_pTimeText);

	if (m_pWeatherPanel)
		pLayer->AddObject(m_pWeatherPanel);

	if (m_pSeasonPanel)
		pLayer->AddObject(m_pSeasonPanel);
}

void UIClockHand::AddNightPanelToLayer(Layer* pLayer)
{
	if (m_pNightPanel)
		pLayer->AddObject(m_pNightPanel);
}
