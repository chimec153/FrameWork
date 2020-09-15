#include "FightObj.h"
#include "../Scene/Scene.h"
#include "UINum.h"
#include "UIBar.h"
#include "../Collider/ColliderRect.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "Tool.h"

FightObj::FightObj()	:
	m_iHP(0),
	m_iEnergy(0),
	m_iAttack(5),
	m_fHitSpeed(180.f),
	m_fHitTime(0.f),
	m_bHitted(false)
{
}

FightObj::FightObj(const FightObj& fighter)	:
	MoveObj(fighter)
{
	m_iHP = fighter.m_iHP;
	m_iEnergy = fighter.m_iEnergy;
	m_iAttack = fighter.m_iAttack;
	m_fHitTime = 0.f;
	m_fHitSpeed = fighter.m_fHitSpeed;
	m_bHitted = false;

	Collider* pCol = GetCollider("FightObjBody");

	if (pCol) {
		pCol->AddCollisionFunction(CS_ENTER, this, &FightObj::ColEnter);
		pCol->AddCollisionFunction(CS_STAY, this, &FightObj::ColEnter);

		SAFE_RELEASE(pCol);
	}
}

FightObj::~FightObj()
{
}

bool FightObj::Init()
{
	ColliderRect* pRC = AddCollider<ColliderRect>("FightObjBody");

	POSITION tSize = GetSize();

	POSITION tPivot = GetPivot();

	pRC->SetRect(tSize.x * -tPivot.x, tSize.y * -tPivot.y, tSize.x * (1-tPivot.x), tSize.y * (1-tPivot.y));

	SAFE_RELEASE(pRC);

	return true;
}

void FightObj::Input(float fDeltaTime)
{
	MoveObj::Input(fDeltaTime);
}

int FightObj::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	if (m_fHitTime > 0.f)
	{
		m_fHitTime -= fDeltaTime;

		SetAngle(m_tHitAngle);

		MoveAngle(m_fHitSpeed, fDeltaTime);
	}

	else if(m_bHitted)
		m_bHitted = false;

	return 0;
}

int FightObj::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void FightObj::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void FightObj::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

void FightObj::Hitted(int iAttack, const POSITION& tPos)
{
	if (iAttack == 0)
		return;

	AddHP(-iAttack);

	SetHitAngle(tPos);
	SetHitTime(0.5f);

	Layer* pLayer = m_pScene->FindLayer("UI");

	Obj* pNum = CreateCloneObj("Num", "Num", pLayer);

	pNum->SetPos(m_tPos.x, m_tPos.y - m_tSize.y * m_tPivot.y);

	((UINum*)pNum)->CreateNum(iAttack);

	SAFE_RELEASE(pNum);
}

void FightObj::DieMotion()
{
}

void FightObj::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "attack" && !m_bHitted)
	{
		bool bEnable = pDest->GetEnable();

		if (bEnable)
		{
			Obj* pObj = pDest->GetObj();

			POSITION tPos = pObj->GetPos();

			UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

			Item* pItem = pInven->GetItem();

			SAFE_RELEASE(pInven);

			int iAttack = ((Tool*)pItem)->GetAttack();

			((FightObj*)pSrc->GetObj())->Hitted(iAttack, tPos);

			if (m_iHP <= 0)
				DieMotion();
		}
	}
}

void FightObj::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
}
