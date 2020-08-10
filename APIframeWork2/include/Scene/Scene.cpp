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

unordered_map<string, Obj*> Scene::m_mapPrototype[SC_END];

Scene::Scene()	:
	m_bEditMode(false),
	m_pPlayer(nullptr)
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("Default", 2);
	pLayer = CreateLayer("Back", 1);
	pLayer = CreateLayer("Stage");
	m_eSceneType = SC_CURRENT;
}

Scene::~Scene()
{
	ErasePrototype(m_eSceneType);
	Safe_Delete_VecList(m_LayerList);
	SAFE_RELEASE(m_pPlayer);
}

void Scene::ErasePrototype(const string & strTag,
	SCENE_CREATE sc)
{
	unordered_map<string, Obj*>::iterator	iter = m_mapPrototype[sc].find(strTag);

	if (!iter->second)
		return;
	SAFE_RELEASE(iter->second);
	m_mapPrototype[sc].erase(iter);
}

void Scene::ErasePrototype(SCENE_CREATE sc)
{
	Safe_Release_Map(m_mapPrototype[sc]);
}

void Scene::SetPlayer(Obj* pObj)
{
	SAFE_RELEASE(m_pPlayer);

	m_pPlayer = pObj;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

Obj* Scene::GetPlayer() const
{
	if (m_pPlayer)
		m_pPlayer->AddRef();

	return m_pPlayer;
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
	return true;
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

Obj * Scene::FindProtoType(const string & strKey,
	SCENE_CREATE sc)
{
	unordered_map<string, Obj*>::iterator	iter = m_mapPrototype[sc].find(strKey);

	if (iter == m_mapPrototype[sc].end())
		return NULL;

	return iter->second;
}

void Scene::ChangePrototype()
{
	ErasePrototype(SC_CURRENT);
	m_mapPrototype[SC_CURRENT] = m_mapPrototype[SC_NEXT];
	m_mapPrototype[SC_NEXT].clear();
}

void Scene::CreateUI(class Obj* pObj)
{
	Layer* pUILayer = FindLayer("UI");

	UIPanel* pClockPanel = Obj::CreateObj<UIPanel>("ClockPanel", pUILayer);

	pClockPanel->SetTexture("Mouse");
	pClockPanel->SetImageOffset(664.f, 864.f);
	pClockPanel->SetColorKey(255, 255, 255);
	pClockPanel->SetPos(1000, 40);
	pClockPanel->SetSize(144.f, 114.f);

	SAFE_RELEASE(pClockPanel);

	UIPanel* pBarPanel = Obj::CreateObj<UIPanel>("BarPanel", pUILayer);

	pBarPanel->SetTexture("Mouse");
	pBarPanel->SetImageOffset(512.f, 816.f);
	pBarPanel->SetColorKey(255, 255, 255);
	pBarPanel->SetPos(1000, 600);
	pBarPanel->SetSize(48.f, 112.f);

	SAFE_RELEASE(pBarPanel);

	UIBar* pEnergyBar = Obj::CreateObj<UIBar>("EnergyBar", pUILayer,
		POSITION(1006.f, 626.f), POSITION(12.f, 82.f));

	pEnergyBar->SetTexture("Mouse");
	pEnergyBar->SetImageOffset(160.f, 32.f);
	pEnergyBar->SetColorKey(255, 255, 255);
	pEnergyBar->SetMax(100.f);
	pEnergyBar->SetValue(100.f);

	UIBar* pHPBar = Obj::CreateObj<UIBar>("HPBar", pUILayer,
		POSITION(1030.f, 626.f), POSITION(12.f, 82.f));

	pHPBar->SetTexture("Mouse");
	pHPBar->SetImageOffset(172.f, 32.f);
	pHPBar->SetColorKey(255, 255, 255);
	pHPBar->SetMax(100.f);
	pHPBar->SetValue(100.f);

	((Player*)pObj)->SetHPBar(pHPBar);
	((Player*)pObj)->SetEnergyBar(pEnergyBar);

	SAFE_RELEASE(pHPBar);

	SAFE_RELEASE(pEnergyBar);

	UIInventory* pInven = Obj::CreateObj<UIInventory>("Inventory", pUILayer,
		POSITION(400.f, 700.f), POSITION(12.f, 82.f));

	((Player*)pObj)->SetInven(pInven);

	SAFE_RELEASE(pInven);

	CreateClockHand();
}

void Scene::CreateSlimeClone()
{
	Layer* pLayer = FindLayer("Default");

	Slime* pSlime = (Slime*)Obj::CreateCloneObj("Slime","slime", SC_NEXT, pLayer);

	pSlime->SetPos(200.f, 100.f);

	Collider* pCol = pSlime->GetCollider("SlimeBody");

	pCol->AddCollisionFunction(CS_ENTER, pSlime, &Slime::Collision);

	SAFE_RELEASE(pCol);
	SAFE_RELEASE(pSlime);
}

void Scene::CreateSlimeProto()
{
	Slime* pSlime = CreateProtoType<Slime>("Slime", SC_NEXT);

	SAFE_RELEASE(pSlime);

	Effect* pEffect = CreateProtoType<Effect>("slimeEffect", SC_NEXT);

	pEffect->SetPhysics(true);
	pEffect->SetSpeed(30.f);
	pEffect->SetSize(16.f, 16.f);
	pEffect->SetPivot(0.5f, 0.5f);
	pEffect->SetTexture("slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pEffect->SetTileEffect(false);

	SAFE_RELEASE(pEffect);
}

void Scene::CreateBatClone()
{
	Layer* pLayer = FindLayer("Default");

	Obj* pMinion = Obj::CreateCloneObj("Minion", "Minion", SC_NEXT, pLayer);

	pMinion->SetPos(300.f, 300.f);

	Collider* pCol = pMinion->GetCollider("MinionBody");

	pCol->AddCollisionFunction(CS_ENTER, (Minion*)pMinion,
		&Minion::CollisionBullet);
	pCol->AddCollisionFunction(CS_STAY, (Minion*)pMinion,
		&Minion::CollisionBullet);

	SAFE_RELEASE(pCol);
	SAFE_RELEASE(pMinion);
}

void Scene::CreateBatProto()
{
	Minion* pMinionProto = CreateProtoType<Minion>("Minion", SC_NEXT);

	SAFE_RELEASE(pMinionProto);

	Effect* pBatEffect = CreateProtoType<Effect>("batEffect", SC_NEXT);

	pBatEffect->SetPhysics(true);
	pBatEffect->SetSpeed(30.f);
	pBatEffect->SetSize(16.f, 16.f);
	pBatEffect->SetPivot(0.5f, 0.5f);
	pBatEffect->SetTexture("monster");
	pBatEffect->SetTileEffect(false);

	SAFE_RELEASE(pBatEffect);
}

void Scene::CreateProtoNumberSmall()
{//	736, 111, 10, 15
	UINum* pNum = CreateProtoType<UINum>("NumSm", SC_NEXT);

	pNum->SetTexture("Mouse", TEXT("UI\\Cursors.bmp"));
	pNum->SetColorKey(0, 0, 0);
	pNum->SetSize(16.f, 16.f);
	pNum->SetImageOffset(1025.f, 255.f);

	SAFE_RELEASE(pNum);
}

void Scene::CreateClockHand()
{
	Layer* pLayer = FindLayer("UI");

	UIClockHand* pClockHand = Obj::CreateObj<UIClockHand>("ClockHand", pLayer, POSITION(995.f, 30.f));

	GET_SINGLE(Timer)->SetClockHand(pClockHand);

	SAFE_RELEASE(pClockHand);
}

Obj* Scene::CreatePlayer()
{
	Layer* pLayer = FindLayer("Default");

	Player* pPlayer = Obj::CreateObj<Player>("Player", pLayer);

	SetPlayer(pPlayer);

	CWeapon* pWeapon = Obj::CreateObj<CWeapon>("weapon", pLayer);

	pPlayer->SetWeapon(pWeapon);

	SAFE_RELEASE(pWeapon);

	GET_SINGLE(Camera)->SetTarget(pPlayer);
	GET_SINGLE(Camera)->SetPivot(0.5f, 0.5f);

	return pPlayer;
}

bool Scene::LayerSort(Layer * pL1, Layer * pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}
