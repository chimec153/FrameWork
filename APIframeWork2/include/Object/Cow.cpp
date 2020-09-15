#include "Cow.h"
#include "../Animation/Animation.h"
#include "UIClockHand.h"
#include "ObjManager.h"
#include "../Core/Input.h"
#include "UIInventory.h"

Cow::Cow()	:
	m_eColor(COW_WHITE),
	m_bMilk(false)
{
	m_eAnimalType = ANIMAL_MILKCOW;
	m_bTileEffect = true;
}

Cow::Cow(const Cow& cow)	:
	Animal(cow)
{
	m_eColor = cow.m_eColor;
	m_bMilk = cow.m_bMilk;

	Collider* pCol = GetCollider("AnimalBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &Cow::ColEnter);
	pCol->AddCollisionFunction(CS_STAY, this, &Cow::ColEnter);

	SAFE_RELEASE(pCol);
}

Cow::~Cow()
{
}

bool Cow::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("CawAni");

	pAni->LoadFromPath("Cow.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Cow::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);

	return 0;
}

int Cow::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Cow::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Cow::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Cow* Cow::Clone()
{
	return new Cow(*this);
}

void Cow::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfIdleLeft");
					SetAnimationDefaultClip("WhiteCalfIdleLeft");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfIdleLeft");
					SetAnimationDefaultClip("BrownCalfIdleLeft");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteIdleLeft");
					SetAnimationDefaultClip("WhiteIdleLeft");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleLeft");
					SetAnimationDefaultClip("BrownIdleLeft");
				}
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfIdleRight");
					SetAnimationDefaultClip("WhiteCalfIdleRight");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfIdleRight");
					SetAnimationDefaultClip("BrownCalfIdleRight");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteIdleRight");
					SetAnimationDefaultClip("WhiteIdleRight");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleRight");
					SetAnimationDefaultClip("BrownIdleRight");
				}
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfIdleUp");
					SetAnimationDefaultClip("WhiteCalfIdleUp");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfIdleUp");
					SetAnimationDefaultClip("BrownCalfIdleUp");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteIdleUp");
					SetAnimationDefaultClip("WhiteIdleUp");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleUp");
					SetAnimationDefaultClip("BrownIdleUp");
				}
			}
		}

		else
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfIdleDown");
					SetAnimationDefaultClip("WhiteCalfIdleDown");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfIdleDown");
					SetAnimationDefaultClip("BrownCalfIdleDown");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteIdleDown");
					SetAnimationDefaultClip("WhiteIdleDown");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleDown");
					SetAnimationDefaultClip("BrownIdleDown");
				}
			}
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfWalkLeft");
					SetAnimationDefaultClip("WhiteCalfIdleLeft");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfWalkLeft");
					SetAnimationDefaultClip("BrownCalfIdleLeft");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteWalkLeft");
					SetAnimationDefaultClip("WhiteIdleLeft");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkLeft");
					SetAnimationDefaultClip("BrownIdleLeft");
				}
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfWalkRight");
					SetAnimationDefaultClip("WhiteCalfIdleRight");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfWalkRight");
					SetAnimationDefaultClip("BrownCalfIdleRight");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteWalkRight");
					SetAnimationDefaultClip("WhiteIdleRight");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkRight");
					SetAnimationDefaultClip("BrownIdleRight");
				}
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfWalkUp");
					SetAnimationDefaultClip("WhiteCalfIdleUp");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfWalkUp");
					SetAnimationDefaultClip("BrownCalfIdleUp");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteWalkUp");
					SetAnimationDefaultClip("WhiteIdleUp");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkUp");
					SetAnimationDefaultClip("BrownIdleUp");
				}
			}
		}

		else
		{
			if (m_bBaby)
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteCalfWalkDown");
					SetAnimationDefaultClip("WhiteCalfIdleDown");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownCalfWalkDown");
					SetAnimationDefaultClip("BrownCalfIdleDown");
				}
			}

			else
			{
				if (m_eColor == COW_WHITE)
				{
					SetAnimationCurrentClip("WhiteWalkDown");
					SetAnimationDefaultClip("WhiteIdleDown");
				}

				else if (m_eColor == COW_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkDown");
					SetAnimationDefaultClip("BrownIdleDown");
				}
			}
		}
	}

	else if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
			if (m_eColor == COW_WHITE)
			{
				SetAnimationCurrentClip("WhiteCalfEat");
			}

			else if (m_eColor == COW_BROWN)
			{
				SetAnimationCurrentClip("BrownCalfEat");
			}
		}

		else
		{
			if (m_eColor == COW_WHITE)
			{
				SetAnimationCurrentClip("WhiteEat");
			}

			else if (m_eColor == COW_BROWN)
			{
				SetAnimationCurrentClip("BrownEat");
			}
		}
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_bBaby)
		{
			if (m_eColor == COW_WHITE)
			{
				SetAnimationCurrentClip("WhiteCalfSleep");
			}

			else if (m_eColor == COW_BROWN)
			{
				SetAnimationCurrentClip("BrownCalfSleep");
			}
		}

		else
		{
			if (m_eColor == COW_WHITE)
			{
				SetAnimationCurrentClip("WhiteSleep");
			}

			else if (m_eColor == COW_BROWN)
			{
				SetAnimationCurrentClip("BrownSleep");
			}
		}
	}
}

bool Cow::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (m_bBaby)
	{
		if (m_iFeedCount >= 6)
		{
			m_bBaby = false;
		}
	}

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;

		if (!m_bBaby)
		{
			if (!m_bMilk)
				m_bMilk = true;
		}
	}

	ActionChange(AA_IDLE);

	return false;
}

void Cow::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		if (KEYDOWN("MouseLButton") || KEYPRESS("MouseLButton"))
		{
			if (m_bMilk)
			{
				UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

				Obj* pMilk = CreateCloneObj("Milk", "Milk",m_pLayer);

				pInven->AddItem((Item*)pMilk);

				SAFE_RELEASE(pMilk);

				SAFE_RELEASE(pInven);

				m_bMilk = false;

				Obj* pEffect = Obj::CreateCloneObj("HoeEffect", "Emote", m_pLayer);

				pEffect->SetAnimationCurrentClip("LoveEmote");

				POSITION tSize = pEffect->GetSize();

				pEffect->SetPos(m_tPos.x - tSize.x * m_tPivot.x, m_tPos.y - m_tSize.y * m_tPivot.y - tSize.y);

				SAFE_RELEASE(pEffect);

				++m_iFriendShip;
			}
		}
	}
}
