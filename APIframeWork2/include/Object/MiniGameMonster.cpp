#include "MiniGameMonster.h"
#include "../Collider/ColliderRect.h"
#include "Bullet.h"
#include "../Animation/Animation.h"
#include "PrairieKing.h"
#include "../Scene/Scene.h"

MiniGameMonster::MiniGameMonster()	:
	m_iHP(1),
	m_eMonsterType(MMT_NONE)
{
}

MiniGameMonster::MiniGameMonster(const MiniGameMonster& mon)	:
	MoveObj(mon)
{
	m_iHP = mon.m_iHP;
	m_eMonsterType = mon.m_eMonsterType;

	Collider* pCol = GetCollider("EnemyBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &MiniGameMonster::CollisionBullet);
	pCol->AddCollisionFunction(CS_STAY, this, &MiniGameMonster::CollisionBullet);
	pCol->AddCollisionFunction(CS_LEAVE, this, &MiniGameMonster::CollLeave);

	SAFE_RELEASE(pCol);
}

MiniGameMonster::~MiniGameMonster()
{
}

bool MiniGameMonster::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);

	ColliderRect* pRC = AddCollider<ColliderRect>("EnemyBody");

	pRC->SetRect(-16.f, -16.f, 16.f, 16.f);

	SAFE_RELEASE(pRC);

	return true;
}

int MiniGameMonster::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);
	return 0;
}

int MiniGameMonster::LateUpdate(float fDeltaTime)	
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void MiniGameMonster::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void MiniGameMonster::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

MiniGameMonster* MiniGameMonster::Clone()
{
	return new MiniGameMonster(*this);
}

void MiniGameMonster::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "BulletBody")
	{
		Bullet* pBullet = (Bullet*)pDest->GetObj();

		int iAttack = pBullet->GetAttack();

		m_iHP -= iAttack;

		if (m_iHP <= 0)
		{
			m_pAnimation->SetCurrentClip("Die");

			SetSpeed(0.f);

			pSrc->SetEnable(false);

			CreateItem();
		}
	}

	else if (strDest == "KingBody")
	{
		PrairieKing* pKing = (PrairieKing*)pDest->GetObj();

		bool bZombie = pKing->IsState(KS_ZOMBIE);

		if (bZombie)
		{
			m_pAnimation->SetCurrentClip("Die");

			SetSpeed(0.f);

			pSrc->SetEnable(false);

			CreateItem();
		}
	}

	else if (strDest == "EnemyBody" && m_bTileEffect)
	{
		POSITION tHitPoint = pSrc->GetHitPoint();

		RECTANGLE tRC = ((ColliderRect*)pSrc)->GetWorldInfo();

		if (tRC.l < tHitPoint.x &&
			tRC.r > tHitPoint.x &&
			tRC.t < tHitPoint.y &&
			tRC.b > tHitPoint.y)
		{

			POSITION tDestPos = pDest->GetObj()->GetPos();

			if (tHitPoint.x < tDestPos.x)
			{
				SetAngle(-1.f, 0.f);

				MoveAngle(fDeltaTime);
			}

			else
			{
				SetAngle(1.f, 0.f);

				MoveAngle(fDeltaTime);
			}
		}
	}
}

void MiniGameMonster::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void MiniGameMonster::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void MiniGameMonster::CreateItem()
{
	int iRand = rand() % 100;

	if (iRand < 10)
	{
		Layer* pLayer = m_pScene->FindLayer("Default");

		Obj* pMoney = CreateCloneObj("Money", "Money", pLayer);

		pMoney->SetPos(m_tPos);

		SAFE_RELEASE(pMoney);
	}

	else if (iRand < 15)
	{
		int iItem = rand() % 7 + 1;

		if (iItem > 5)
			++iItem;

		if (iItem == ITEM_WHEEL)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("Wheel", "Wheel", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_MACHINE)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("MachineGun", "MachineGun", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_TOMB)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("TombStone", "TombStone", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_COFFEE)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("Coffee", "Coffee", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_SHOTGUN)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("ShotGun", "ShotGun", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_SMOKE)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("SmokeBomb", "SmokeBomb", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == ITEM_NUKE)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("Nuke", "Nuke", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}
	}

	else if (iRand < 16)
	{
		int iItem = rand() % 3;

		if (iItem == 0)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("Badge", "Badge", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == 1)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("Life", "Life", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}

		else if (iItem == 2)
		{
			Layer* pLayer = m_pScene->FindLayer("Default");

			Obj* pMoney = CreateCloneObj("5Money", "5Money", pLayer);

			pMoney->SetPos(m_tPos);

			SAFE_RELEASE(pMoney);
		}
	}
}
