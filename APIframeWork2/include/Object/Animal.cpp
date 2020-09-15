#include "Animal.h"
#include "../Core/Input.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "Item.h"
#include "UIClockHand.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "FarmEffect.h"

Animal::Animal()	:
	m_iDay(0),
	m_iFeedCount(0),
	m_bBaby(true),
	m_eAction(AA_IDLE),
	m_bFeed(false),
	m_eAnimalType(ANIMAL_CHICKEN),
	m_iGrowDay(0),
	m_fTime(0.f),
	m_fLimitTime(2.f),
	m_fEatLimitTime(5.f),
	m_iFriendShip(0)
{
}

Animal::Animal(const Animal& animal)	:
	FightObj(animal)
{
	m_iDay = animal.m_iDay;
	m_iFeedCount = animal.m_iFeedCount;
	m_bBaby = animal.m_bBaby;
	m_eAction = animal.m_eAction;
	m_bFeed = animal.m_bFeed;
	m_eAnimalType = animal.m_eAnimalType;
	m_iGrowDay = animal.m_iGrowDay;
	m_fTime = animal.m_fTime;
	m_fLimitTime = animal.m_fLimitTime;
	m_fEatLimitTime = animal.m_fEatLimitTime;
	m_iFriendShip = animal.m_iFriendShip;

	Collider* pCol = GetCollider("AnimalBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &Animal::ColEnter);
	pCol->AddCollisionFunction(CS_STAY, this, &Animal::ColEnter);

	SAFE_RELEASE(pCol);
}

Animal::~Animal()
{
}

bool Animal::AddDay(int iDay)
{
	m_iDay += iDay;

	return false;
}

bool Animal::Init()
{
	ColliderRect* pRC = AddCollider<ColliderRect>("AnimalBody");

	POSITION tSize = GetSize();

	POSITION tPivot = GetPivot();

	pRC->SetRect(tSize.x * -tPivot.x, tSize.y * -tPivot.y, tSize.x * (1-tPivot.x), tSize.y * (1-tPivot.y));

	SAFE_RELEASE(pRC);

	m_iAttack = 0;

	return true;
}

int Animal::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	MoveAngle(fDeltaTime);

	m_fTime += fDeltaTime;

	if (m_fTime >= m_fLimitTime)
	{
		m_fTime -= m_fLimitTime;

		if (m_eAction != AA_SIT && m_eAction != AA_EAT)
		{
			int iRand = rand() % 2;

			if (iRand == AA_WALK)
			{
				int iAxis = rand() % 2;
				int iDir = (rand() % 2) * 2 - 1;

				SetAngle((float)(iAxis * iDir), (float)((iAxis + 1) % 2 * iDir));

				ActionChange(AA_WALK);

				SetSpeed(40.f);
			}

			else
			{
				ActionChange(AA_IDLE);

				SetSpeed(0.f);
			}
		}

		else if (m_eAction == AA_EAT)
		{
			bool bEnd = m_pAnimation->GetMotionEnd();

			if (bEnd)
				ActionChange(AA_IDLE);
		}
	}

	UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

	float fTime = pClockHand->GetTime();

	if (fTime >= 1200.f && m_eAction != AA_EAT)
	{
		ActionChange(AA_SIT);
		SetSpeed(0.f);
	}

	if (m_eAction == AA_EAT)
	{
		if (m_fTime >= m_fEatLimitTime)
		{
			m_fTime -= m_fEatLimitTime;

			ActionChange(AA_IDLE);
		}
	}

	return 0;
}

int Animal::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Animal::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Animal::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Animal* Animal::Clone()
{
	return new Animal(*this);
}

void Animal::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		if (KEYDOWN("MouseLButton") || KEYDOWN("MouseRButton"))
		{
			UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

			Item* pItem = pInven->GetItem();

			if (pItem)
			{
				int iIndex = pItem->GetIndex();

				if (iIndex == 178)
				{
					if (!IsFeed())
					{
						SetFeed(true);

						pInven->DeleteItem();

						m_eAction = AA_EAT;

						ActionChange(m_eAction);

						SetSpeed(0.f);

						m_fTime = 0.f;

						Obj* pEffect = Obj::CreateCloneObj("HoeEffect", "Emote", m_pLayer);

						pEffect->SetAnimationCurrentClip("LoveEmote");

						POSITION tSize = pEffect->GetSize();

						pEffect->SetPos(m_tPos.x - tSize.x * m_tPivot.x, m_tPos.y - m_tSize.y * m_tPivot.y - tSize.y);

						SAFE_RELEASE(pEffect);

						++m_iFriendShip;
					}
				}
			}

			SAFE_RELEASE(pInven);
		}
	}
}

void Animal::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Animal::ColLeave(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Animal::ActionChange(ANIMAL_ACTION eAction)
{
}
