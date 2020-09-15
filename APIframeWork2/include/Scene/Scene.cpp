#include "Scene.h"
#include "Layer.h"
#include "../Object/Obj.h"
#include "../Object/UIPanel.h"
#include "..//Object/UIBar.h"
#include "../Object/Player.h"
#include "..//Object/Effect.h"
#include "../Collider/ColliderRect.h"
#include "../Object/Minion.h"
#include "../Animation/Animation.h"
#include "../Object/Slime.h"
#include "../Object/Weapon.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderSphere.h"
#include "../Object/UINum.h"
#include "../Object/UIClockHand.h"
#include "../Core/Timer.h"
#include "../Object/UIInventory.h"
#include "../Object/FarmEffect.h"
#include "../Object/Tool.h"
#include "../Object/PadeEffect.h"
#include "../Core.h"
#include "../Object/Crop.h"
#include "../Object/Seed.h"
#include "../Object/Etc.h"
#include "..//Sound/SoundManager.h"
#include "../Core/PathManager.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/Tree.h"
#include "../Object/Building.h"
#include "../Object/Rock.h"
#include "../Resources/Texture.h"
#include "../Object/SlimeHead.h"
#include "../Object/Bug.h"
#include "../Object/Fly.h"
#include "../Object/RockCrab.h"
#include "../Object/Text.h"
#include "../Object/Stage.h"
#include "../Object/ObjManager.h"
#include "../Scene/SceneCollision.h"
#include "../Object/Grass.h"
#include "../Object/Egg.h"
#include "../Object/Furniture.h"

//unordered_map<string, Obj*> Scene::m_mapPrototype[SC_END];

Scene::Scene()	:
	m_bEditMode(false),
	m_pPadeEffect(nullptr),
	m_pStage(nullptr),
	m_bStart(false)
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("UTHUD", INT_MAX - 2);
	pLayer = CreateLayer("Default", 3);
	pLayer = CreateLayer("Back", 2);
	pLayer = CreateLayer("Stage",1);
	pLayer = CreateLayer("BackStage");
	m_eSceneType = SC_CURRENT;

	m_pCollision = new SceneCollision;

	m_pCollision->CreateCollisionSection(10, 10, POSITION(500.f, 500.f));
}

Scene::~Scene()
{
	ErasePrototype();
	Safe_Delete_VecList(m_LayerList);
	SAFE_RELEASE(m_pPadeEffect);
	Safe_Delete_VecList(m_vecstrProto);
	SAFE_RELEASE(m_pStage);
	SAFE_DELETE(m_pCollision);
}

void Scene::ErasePrototype(const string & strTag)
{
	unordered_map<string, Obj*>::iterator	iter = m_mapPrototype.find(strTag);

	if (!iter->second)
		return;

	SAFE_RELEASE(iter->second);

	m_mapPrototype.erase(iter);
}

void Scene::ErasePrototype()
{
	Safe_Release_Map(m_mapPrototype);
}

void Scene::ResetPadeInEffect()
{
	if (m_pPadeEffect)
		((PadeEffect*)m_pPadeEffect)->Reset();
}

Layer * Scene::CreateLayer(const string & strTag, int iZOrder)
{
	Layer* pLayer = new Layer;
	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() >= 2)
		m_LayerList.sort(Scene::LayerSort);

	return pLayer;
}

Layer * Scene::FindLayer(const string & strTag)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
			return *iter;
	}
	return NULL;
}

bool Scene::Init()
{
	GET_SINGLE(SoundManager)->LoadSound("RoadBGM", true, "RoadBgm.wav");
	GET_SINGLE(SoundManager)->LoadSound("wood", false, "wood.wav");
	GET_SINGLE(SoundManager)->LoadSound("rock", false, "poll.wav");

	CreateProtoNumber();

	Layer* pLayer = FindLayer("Default");
	Layer* pHUDLayer = FindLayer("HUD");
	Layer* pUILayer = FindLayer("UI");

	GET_SINGLE(ObjManager)->AddObject(pLayer);

	UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

	pClockHand->AddNightPanelToLayer(pUILayer);

	Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

	CreateUI(pPlayer);

	GET_SINGLE(ObjManager)->AddClockHand(pHUDLayer);

	GET_SINGLE(ObjManager)->AddInven(pHUDLayer);

	pPlayer->AddObjectToLayer(pHUDLayer);

	m_pPadeEffect = Obj::CreateObj<PadeEffect>("Pade", pUILayer);

	SAFE_RELEASE(pPlayer);

	return true;
}

void Scene::Start()
{
	m_bStart = true;
}

void Scene::Input(float fDeltaTime)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Input(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			++iter;
	}
}

int Scene::Update(float fDeltaTime)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			++iter;
	}
	return 0;
}

int Scene::LateUpdate(float fDeltaTime)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			++iter;
	}
	return 0;
}

void Scene::Collision(float fDeltaTime)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			++iter;
	}

	m_pCollision->Collision(fDeltaTime);
}

void Scene::Render(HDC hDC, float fDeltaTime)
{
	list<Layer*>::iterator iter;
	list<Layer*>::iterator iterEnd = m_LayerList.end();
	for (iter = m_LayerList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Render(hDC, fDeltaTime);
		if (!(*iter)->GetLife())
		{
			SAFE_DELETE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
		}
		else
			++iter;
	}
}

Obj * Scene::FindProtoType(const string & strKey)
{
	unordered_map<string, Obj*>::iterator	iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}

void Scene::ChangePrototype()
{
	ErasePrototype();
}

void Scene::CreateUI(class Obj* pObj)
{
	Layer* pUTHUDLayer = FindLayer("UTHUD");

	UIPanel* pClockPanel = Obj::CreateObj<UIPanel>("ClockPanel", pUTHUDLayer);

	pClockPanel->SetTexture("Mouse");
	pClockPanel->SetImageOffset(664.f, 864.f);
	pClockPanel->SetColorKey(255, 255, 255);
	pClockPanel->SetPos(1000, 40);
	pClockPanel->SetSize(144.f, 114.f);
	pClockPanel->EnableAlpha(true);
	pClockPanel->SetAlpha(255);

	SAFE_RELEASE(pClockPanel);

	UIPanel* pBarPanel = Obj::CreateObj<UIPanel>("BarPanel", pUTHUDLayer);

	pBarPanel->SetTexture("Mouse");
	pBarPanel->SetImageOffset(512.f, 816.f);
	pBarPanel->SetColorKey(255, 255, 255);
	pBarPanel->SetPos(1000, 600);
	pBarPanel->SetSize(48.f, 112.f);

	SAFE_RELEASE(pBarPanel);
}

void Scene::CreateProtoNumber()
{
	UINum* pNum = CreateProtoType<UINum>("Num");

	pNum->SetTexture("Mouse", TEXT("UI\\Cursors.bmp"));
	pNum->SetColorKey(0, 0, 0);
	pNum->SetSize(16.f, 16.f);
	pNum->SetOriginOffset(1024.f, 255.f);
	pNum->SetAlpha(255);
	pNum->EnableAlpha(true);

	SAFE_RELEASE(pNum);

	UINum* pNumSm = CreateProtoType<UINum>("NumSm");

	pNumSm->SetTexture("Mouse", TEXT("UI\\Cursors.bmp"));
	pNumSm->SetColorKey(0, 0, 0);
	pNumSm->SetSize(10.f, 14.f);
	pNumSm->SetOriginOffset(736.f, 112.f);
	pNumSm->SetAlpha(255);
	pNumSm->EnableAlpha(true);
	pNumSm->SetOneLine(true);

	SAFE_RELEASE(pNumSm);
}

void Scene::CreateClockHand()
{
	RESOLUTION tRS = GET_SINGLE(Core)->GetResolution();

	Layer* pLayer = FindLayer("UI");

	UIClockHand* pClockHand = Obj::CreateObj<UIClockHand>("ClockHand", pLayer, POSITION(995.f, 30.f));

	GET_SINGLE(ObjManager)->SetClockHand(pClockHand);

	UIPanel* pNightPanel = Obj::CreateObj<UIPanel>("Night", pLayer,
		POSITION::Zero, POSITION((float)tRS.iW, (float)tRS.iH));

	pNightPanel->SetTexture("Night", TEXT("UI\\night.bmp"));
	pNightPanel->SetAlpha(120);
	pNightPanel->EnableAlpha(true);

	pClockHand->SetNightPanel(pNightPanel);

	Layer* pHUDLayer = FindLayer("HUD");

	Text* pText = Obj::CreateObj<Text>("WeekDayText", pHUDLayer);

	pText->SetFont("NormalFont");
	pText->SetColor(102, 28, 28);
	pText->SetPos(1056.f, 50.f);

	pClockHand->SetWeekText(pText);

	Text* pTimeText = Obj::CreateObj<Text>("TimeText", pHUDLayer);

	pTimeText->SetFont("NormalFont");
	pTimeText->SetColor(102, 28, 28);
	pTimeText->SetPos(1056.f, 96.f);

	pClockHand->SetTimeText(pTimeText);

	UIPanel* pWeatherPanel = Obj::CreateObj<UIPanel>("WeatherPanel", pHUDLayer);

	pWeatherPanel->SetTexture("Mouse");
	pWeatherPanel->SetImageOffset(682.f, 842.f);
	pWeatherPanel->SetColorKey(255, 255, 255);
	pWeatherPanel->SetPos(1060.f, 72.f);
	pWeatherPanel->SetSize(24.f, 16.f);

	pClockHand->SetWeatherPanel(pWeatherPanel);

	SAFE_RELEASE(pWeatherPanel);

	UIPanel* pSeasonPanel = Obj::CreateObj<UIPanel>("SeasonPanel", pHUDLayer);

	pSeasonPanel->SetTexture("Mouse");
	pSeasonPanel->SetImageOffset(812.f, 882.f);
	pSeasonPanel->SetColorKey(255, 255, 255);
	pSeasonPanel->SetPos(1108.f, 72.f);
	pSeasonPanel->SetSize(24.f, 16.f);

	pClockHand->SetSeasonPanel(pSeasonPanel);

	SAFE_RELEASE(pSeasonPanel);

	SAFE_RELEASE(pText);

	SAFE_RELEASE(pTimeText);

	SAFE_RELEASE(pNightPanel);

	SAFE_RELEASE(pClockHand);
}

void Scene::CreateFarmEffect()
{
	Layer* pLayer = FindLayer("Default");

	FarmEffect* pEffect = CreateProtoType<FarmEffect>("HoeEffect");

	SAFE_RELEASE(pEffect);
}

void Scene::CreateProtoTypes()
{
	Minion* pMinionProto = CreateProtoType<Minion>("Minion");

	SAFE_RELEASE(pMinionProto);

	Effect* pBatEffect = CreateProtoType<Effect>("batEffect");

	pBatEffect->SetPhysics(true);
	pBatEffect->SetSpeed(30.f);
	pBatEffect->SetSize(16.f, 16.f);
	pBatEffect->SetPivot(0.5f, 0.5f);
	pBatEffect->SetTexture("monster");
	pBatEffect->SetTileEffect(false);

	SAFE_RELEASE(pBatEffect);

	SlimeHead* pHead = CreateProtoType<SlimeHead>("SlimeHead");

	SAFE_RELEASE(pHead);

	Slime* pSlime = CreateProtoType<Slime>("Slime");

	SAFE_RELEASE(pSlime);

	Effect* pEffect = CreateProtoType<Effect>("slimeEffect");

	pEffect->SetPhysics(true);
	pEffect->SetSpeed(30.f);
	pEffect->SetSize(16.f, 16.f);
	pEffect->SetPivot(0.5f, 0.5f);
	pEffect->SetTexture("slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pEffect->SetTileEffect(false);

	SAFE_RELEASE(pEffect);

	Bug* pBug = CreateProtoType<Bug>("Bug");

	SAFE_RELEASE(pBug);

	Effect* pBugEffect = CreateProtoType<Effect>("BugEffect");

	pBugEffect->SetPhysics(true);
	pBugEffect->SetSpeed(30.f);
	pBugEffect->SetSize(16.f, 16.f);
	pBugEffect->SetPivot(0.5f, 0.5f);
	pBugEffect->SetTexture("Bug", TEXT("Characters\\Monsters\\Bug.bmp"));
	pBugEffect->SetColorKey(255, 255, 255);
	pBugEffect->SetTileEffect(false);

	SAFE_RELEASE(pBugEffect);

	Fly* pFly = CreateProtoType<Fly>("Fly");

	SAFE_RELEASE(pFly);

	Effect* pFlyEffect = CreateProtoType<Effect>("FlyEffect");

	pFlyEffect->SetPhysics(true);
	pFlyEffect->SetSpeed(30.f);
	pFlyEffect->SetSize(16.f, 16.f);
	pFlyEffect->SetPivot(0.5f, 0.5f);
	pFlyEffect->SetTexture("Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pFlyEffect->SetColorKey(255, 255, 255);
	pFlyEffect->SetTileEffect(false);

	SAFE_RELEASE(pFlyEffect);

	RockCrab* pRockCrab = CreateProtoType<RockCrab>("RockCrab");

	SAFE_RELEASE(pRockCrab);

	Effect* pRockCrabEffect = CreateProtoType<Effect>("RockCrabEffect");

	pRockCrabEffect->SetPhysics(true);
	pRockCrabEffect->SetSpeed(30.f);
	pRockCrabEffect->SetSize(16.f, 16.f);
	pRockCrabEffect->SetPivot(0.5f, 0.5f);
	pRockCrabEffect->SetTexture("RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pRockCrabEffect->SetColorKey(255, 255, 255);
	pRockCrabEffect->SetTileEffect(false);

	SAFE_RELEASE(pRockCrabEffect);

	int iSize = GET_SINGLE(ResourcesManager)->GetItemCount();
	
	for (int i = 0; i < iSize; ++i)
	{
		PITEMINFO pInfo = GET_SINGLE(ResourcesManager)->FindItemInfo(i);

		ITEM_TYPE eItemType = pInfo->eItemType;

		if (eItemType == IT_SEED)
		{
			Seed* pSeed = CreateProtoType<Seed>(pInfo->strName.c_str());

			pSeed->SetImageOffset(pSeed->GetSize() * pInfo->tTileOffset);

			pSeed->SetCropType(pInfo->eCropType);

			pSeed->SetItemType(eItemType);

			pSeed->SetIndex((int)(24 * pInfo->tTileOffset.y + pInfo->tTileOffset.x));

			pSeed->SetSellPrice(pInfo->iSeedSellPrice);

			pSeed->SetFileIndex(i);

			SAFE_RELEASE(pSeed);

			Crop* pCrop = CreateProtoType<Crop>(pInfo->strCropName.c_str());

			pCrop->SetItemType(IT_CROP);

			pCrop->SetImageOffset(pCrop->GetSize() * pInfo->tCropOffset);

			pCrop->SetCropType(pInfo->eCropType);

			pCrop->SetMaxStage(pInfo->iMaxStage);

			pCrop->SetHP(pInfo->iHPRecovery);

			pCrop->SetEnergy(pInfo->iEnergyRecovery);

			pCrop->SetRegrowthDay(pInfo->iRegrowthDay);

			for (int i = 0; i < pInfo->iMaxStage - 1; ++i)
				pCrop->AddMaxDay(pInfo->vecMaxDay[i]);

			pCrop->SetIndex((int)(24 * pInfo->tHarvestOffset.y + pInfo->tHarvestOffset.x));

			pCrop->SetSellPrice(pInfo->iSellPrice);

			pCrop->SetFileIndex(i);

			SAFE_RELEASE(pCrop);
		}

		else if (eItemType == IT_TOOL)
		{
			Tool* pTool = CreateProtoType<Tool>(pInfo->strName.c_str());

			pTool->SetToolType(pInfo->eToolType);

			if (pInfo->eToolType == TOOL_SWORD || pInfo->eToolType == TOOL_SCYTHE)
			{
				pTool->SetAttack(pInfo->iAttack);

				pTool->SetTexture("weapon", TEXT("Item\\weapons.bmp"));
				pTool->SetColorKey(255, 255, 255);
			}

			else
			{
				pTool->SetTexture("tools", TEXT("Item\\tools.bmp"));
				pTool->SetColorKey(255, 255, 255);
			}

			pTool->SetImageOffset(pTool->GetSize()* pInfo->tTileOffset);

			pTool->SetIndex((int)(8 * pInfo->tTileOffset.y + pInfo->tTileOffset.x));

			pTool->SetItemType(eItemType);

			pTool->SetSellPrice(pInfo->iSellPrice);

			pTool->SetFileIndex(i);

			SAFE_RELEASE(pTool);
		}

		else if (eItemType == IT_ETC)
		{
			Etc* pEtc = CreateProtoType<Etc>(pInfo->strName.c_str());

			pEtc->SetImageOffset(pEtc->GetSize() * pInfo->tTileOffset);

			pEtc->SetIndex((int)(24 * pInfo->tTileOffset.y + pInfo->tTileOffset.x));

			pEtc->SetFileIndex(i);

			pEtc->SetSellPrice(pInfo->iSellPrice);

			SAFE_RELEASE(pEtc);
		}

		else if (eItemType == IT_EGG)
		{
			Egg* pEgg = CreateProtoType<Egg>(pInfo->strName.c_str());

			pEgg->SetImageOffset(pEgg->GetSize() * pInfo->tTileOffset);

			pEgg->SetIndex((int)(24 * pInfo->tTileOffset.y + pInfo->tTileOffset.x));

			pEgg->SetFileIndex(i);

			pEgg->SetEggType(pInfo->eEggType);

			pEgg->SetSellPrice(pInfo->iSellPrice);

			SAFE_RELEASE(pEgg);
		}
	}
}

Obj* Scene::CreatePlayer()
{
	Layer* pLayer = FindLayer("Default");

	Player* pPlayer = Obj::CreateObj<Player>("Player", pLayer);

	GET_SINGLE(ObjManager)->SetPlayer(pPlayer);

	CWeapon* pWeapon = Obj::CreateObj<CWeapon>("weapon", pLayer);

	pPlayer->SetWeapon(pWeapon);

	pWeapon->SetPlayer(pPlayer);

	GET_SINGLE(ObjManager)->SetWeapon(pWeapon);

	SAFE_RELEASE(pWeapon);

	GET_SINGLE(Camera)->SetTarget(pPlayer);
	GET_SINGLE(Camera)->SetPivot(0.5f, 0.5f);

	return pPlayer;
}

	//	텍스트 파일로 읽어온 오브젝트 및 텍스쳐 정보로부터 오브젝트와 오브젝트의 텍스쳐를 등록하는 함수이다.
void Scene::LoadFile()
{
	FILE* pFile = nullptr;

	const wchar_t* pFileName = TEXT("objdata.txt");

	const wchar_t* pPath = GET_SINGLE(PathManager)->FindPath(DATA_PATH);
	wchar_t pPathDest[MAX_PATH] = {};

	wcscat(pPathDest, pPath);

	wcscat(pPathDest, pFileName);

	_bstr_t b(pPathDest);
	const char* strPath = b;

	fopen_s(&pFile, strPath, "rt");

	if (!pFile)
		return;

	int iCount = 0;

	char cLine[256] = {};

	fgets(cLine, 256, pFile);

	iCount = atoi(cLine);
	m_vecstrProto.reserve(iCount);

	for (int i = 0; i < iCount; ++i)
	{
		fgets(cLine, 256, pFile);

		fgets(cLine, 256, pFile);

		int iObj = atoi(cLine);

		if (iObj == LT_LABEL)
		{
			fgets(cLine, 256, pFile);

			char* strProto = new char[MAX_PATH];
			strcpy_s(strProto, MAX_PATH, cLine);

			m_vecstrProto.push_back(strProto);
		}

		else if (iObj == LT_OBJECT)
		{
			fgets(cLine, 256, pFile);

			OBJ_BLOCK eBlock = (OBJ_BLOCK)atoi(cLine);

			Obj* pProto = nullptr;

			fgets(cLine, 256, pFile);
			cLine[strlen(cLine) - 1] = 0;

			char* strProto = new char[MAX_PATH];
			strcpy_s(strProto, MAX_PATH, cLine);
			m_vecstrProto.push_back(strProto);

			switch (eBlock)
			{
			case OB_TREE:
				pProto = Scene::CreateProtoType<CTree>(cLine);
				break;
			case OB_BUILDING:
				pProto = Scene::CreateProtoType<CBuilding>(cLine);
				break;
			case OB_CROP:
				pProto = Scene::CreateProtoType<Crop>(cLine);
				break;
			case OB_ROCK:
				pProto = Scene::CreateProtoType<Rock>(cLine);
				break;
			case OB_GRASS:
				pProto = Scene::CreateProtoType<Grass>(cLine);
				break;
			case OB_FURNITURE:
				pProto = Scene::CreateProtoType<Furniture>(cLine);
				break;
			}

			fgets(cLine, 256, pFile);
			char* pContext = nullptr;
			char* pResult = strtok_s(cLine, ", ", &pContext);

			pContext[strlen(pContext) - 1] = 0;
			std::string str(pContext);
			std::wstring ws;
			ws.assign(str.begin(), str.end());
			const wchar_t* usern = ws.c_str();

			Texture* pTex = GET_SINGLE(ResourcesManager)->LoadTexture(
				pResult, usern);

			pProto->SetTexture(pTex);

			if (eBlock == OB_TREE)
				((CTree*)pProto)->SetTreeTexture(pTex);

			fgets(cLine, 256, pFile);
			pResult = strtok_s(cLine, ", ", &pContext);

			int iRed = atoi(pResult);
			pResult = strtok_s(nullptr, ", ", &pContext);

			int iGreen = atoi(pResult);
			int iBlue = atoi(pContext);

			pProto->SetColorKey(iRed, iGreen, iBlue);
			pTex->SetColorKey(iRed, iGreen, iBlue);

			fgets(cLine, 256, pFile);
			pResult = strtok_s(cLine, ", ", &pContext);

			float iSize_x = (float)atof(pResult);
			float iSize_y = (float)atof(pContext);
			pProto->SetSize(iSize_x, iSize_y);

			fgets(cLine, 256, pFile);
			pResult = strtok_s(cLine, ", ", &pContext);
			pProto->SetPivot((float)atof(pResult), (float)atof(pContext));

			fgets(cLine, 256, pFile);
			pResult = strtok_s(cLine, ", ", &pContext);
			pProto->SetImageOffset((float)atof(pResult), (float)atof(pContext));

			if (eBlock == OB_TREE)
			{
				fgets(cLine, 256, pFile);
				pResult = strtok_s(cLine, ", ", &pContext);
				((CTree*)pProto)->SetTreeSize((float)atof(pResult), (float)atof(pContext));

				fgets(cLine, 256, pFile);
				pResult = strtok_s(cLine, ", ", &pContext);
				((CTree*)pProto)->SetTreePivot((float)atof(pResult), (float)atof(pContext));

				fgets(cLine, 256, pFile);
				pResult = strtok_s(cLine, ", ", &pContext);
				((CTree*)pProto)->SetTreeOffset((float)atof(pResult), (float)atof(pContext));
			}

			else if (eBlock == OB_ROCK)
			{
				fgets(cLine, 256, pFile);

				ROCK_TYPE eType = (ROCK_TYPE)atoi(cLine);

				((Rock*)pProto)->SetRockType(eType);

				fgets(cLine, 256, pFile);

				float fRate = (float)atof(cLine);

				((Rock*)pProto)->SetDropRate(fRate);
			}

			fgets(cLine, 256, pFile);
			int iCollSize = atoi(cLine);

			for (int j = 0; j < iCollSize; ++j)
			{
				fgets(cLine, 256, pFile);
				cLine[strlen(cLine) - 1] = 0;

				ColliderRect* pRC = pProto->AddCollider<ColliderRect>(cLine);

				if (eBlock == OB_FURNITURE)
				{
					pRC->SetRect((1-pProto->m_tPivot.x) * iSize_x, (1-pProto->m_tPivot.y) * iSize_y, 
						pProto->m_tPivot.x * iSize_x, pProto->m_tPivot.y * iSize_y);
				}

				else
				{
					fgets(cLine, 256, pFile);
					pResult = strtok_s(cLine, ", ", &pContext);
					float fLeft = (float)atof(pResult);

					pResult = strtok_s(nullptr, ", ", &pContext);
					float fTop = (float)atof(pResult);

					pResult = strtok_s(nullptr, ", ", &pContext);
					float fRight = (float)atof(pResult);

					pResult = strtok_s(nullptr, ", ", &pContext);
					float fBottom = (float)atof(pResult);

					pRC->SetRect(fLeft, fTop, fRight, fBottom);
				}

				SAFE_RELEASE(pRC);
			}

			SAFE_RELEASE(pTex);

			SAFE_RELEASE(pProto);
		}
	}

	fclose(pFile);
}

bool Scene::LayerSort(Layer * pL1, Layer * pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}
