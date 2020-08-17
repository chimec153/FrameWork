#include "ObjManager.h"
#include "Player.h"
#include "UIInventory.h"
#include "../Core/Timer.h"
#include "UIClockHand.h"

DEFINITION_SINGLE(ObjManager)

ObjManager::ObjManager()	:
	m_pPlayer(nullptr),
	m_pInventory(nullptr),
	m_pWeapon(nullptr),
	m_pClockHand(nullptr)
{

}

ObjManager::~ObjManager()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pInventory);
	SAFE_RELEASE(m_pWeapon);
	SAFE_RELEASE(m_pClockHand);
}

void ObjManager::SetPlayer(Obj* pObj)
{
	SAFE_RELEASE(m_pPlayer);

	m_pPlayer = (Player*)pObj;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

Obj* ObjManager::GetPlayer() const
{
	if (m_pPlayer)
		m_pPlayer->AddRef();

	return m_pPlayer;
}

UIInventory* ObjManager::GetInven()	const
{
	if (m_pInventory)
		m_pInventory->AddRef();

	return m_pInventory;
}

void ObjManager::SetInven(UIInventory* pInven)
{
	SAFE_RELEASE(m_pInventory);

	m_pInventory = pInven;

	if (m_pInventory)
		m_pInventory->AddRef();
}

void ObjManager::SetWeapon(Obj* pObj)
{
	SAFE_RELEASE(m_pWeapon);

	m_pWeapon = pObj;

	if (m_pWeapon)
		m_pWeapon->AddRef();
}

Obj* ObjManager::GetWeapon() const
{
	if (m_pWeapon)
		m_pWeapon->AddRef();

	return m_pWeapon;
}

void ObjManager::SetClockHand(Obj* pObj)
{
	SAFE_RELEASE(m_pClockHand);

	m_pClockHand = pObj;

	if (m_pClockHand)
		m_pClockHand->AddRef();
}

bool ObjManager::Init()
{


	return true;
}

int ObjManager::Update(float fTime)
{
	if (m_pClockHand)
		((UIClockHand*)m_pClockHand)->AddTime(fTime * GET_SINGLE(Timer)->GetTimeScale());

	return 0;
}

void ObjManager::AddObject(Layer* pLayer)
{
	if (m_pPlayer)
	{
		pLayer->AddObject(m_pPlayer);
	}
		

	if (m_pWeapon)
		pLayer->AddObject(m_pWeapon);
}

void ObjManager::AddInven(Layer* pLayer)
{
	if (m_pInventory)
	{
		m_pInventory->AddObjectToLayer(pLayer);
	}		
}

void ObjManager::AddClockHand(Layer* pLayer)
{
	if (m_pClockHand)
		((UIClockHand*)m_pClockHand)->AddObjectToLayer(pLayer);
}
