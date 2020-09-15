#include "Chicken.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"
#include "ObjManager.h"
#include "UIClockHand.h"
#include "Item.h"

Chicken::Chicken()	:
	m_eColor(CC_WHITE)
{
	m_bTileEffect = true;
	m_eAnimalType = ANIMAL_CHICKEN;
}

Chicken::Chicken(const Chicken& chicken)	:
	Animal(chicken)
{
	m_eColor = chicken.m_eColor;
}

Chicken::~Chicken()
{
}

bool Chicken::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		++m_iGrowDay;

		SetFeed(false);

		if (m_iGrowDay >= 3)
		{
			m_bBaby = false;
		}

		if (m_iGrowDay >= 4)
		{
			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			Item* pEgg = nullptr;

			if (m_eColor == CC_WHITE || m_eColor == CC_BLUE)
				pEgg = (Item*)CreateCloneObj("White Egg", "Egg", m_pLayer);

			else if (m_eColor == CC_BROWN)
				pEgg = (Item*)CreateCloneObj("Brown Egg", "Egg", m_pLayer);

			else if(m_eColor == CC_VOID)
				pEgg = (Item*)CreateCloneObj("Void Egg", "Egg", m_pLayer);

			if (pEgg)
			{
				pEgg->SetPos(m_tPos);

				Collider* pEggCol = pEgg->GetCollider("ItemBody");

				pEggCol->AddCollisionFunction(CS_ENTER, pEgg, &Item::CollEnter);
				pEggCol->AddCollisionFunction(CS_STAY, pEgg, &Item::ColStay);
				pEggCol->AddCollisionFunction(CS_LEAVE, pEgg, &Item::ColEnd);

				SAFE_RELEASE(pEggCol);

				pClockHand->AddCrop(pEgg);

				SAFE_RELEASE(pEgg);
			}
		}

		return true;
	}

	ActionChange(AA_IDLE);

	return false;
}

bool Chicken::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 1.0f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("ChickenAni");

	pAni->LoadFromPath("Chick.sac");
	pAni->LoadFromPath("Chick(Void).sac");
	pAni->LoadFromPath("Chick(Blue).sac");

	pAni->LoadFromPath("Chicken(White).sac");
	pAni->LoadFromPath("Chicken(Blue).sac");
	pAni->LoadFromPath("Chicken(Void).sac");
	pAni->LoadFromPath("Chicken(Brown).sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Chicken::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);

	if (m_iGrowDay>= 3 && IsFeed())
	{
		UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

		float fTime = pClockHand->GetTime();

		if (fTime >= 720.f)
		{
			ActionChange(AA_SIT);
			SetSpeed(0.f);
		}
	}

	return 0;
}

int Chicken::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Chicken::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Chicken::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Chicken* Chicken::Clone()
{
	return new Chicken(*this);
}

void Chicken::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Chicken::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Chicken::ColLeave(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Chicken::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickWalkUp");
					SetAnimationDefaultClip("ChickIdleUp");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickWalkUp");
					SetAnimationDefaultClip("BlueChickIdleUp");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickWalkUp");
					SetAnimationDefaultClip("DarkChickIdleUp");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenWalkUp");
					SetAnimationDefaultClip("ChickenIdleUp");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueWalkUp");
					SetAnimationDefaultClip("BlueIdleUp");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkUp");
					SetAnimationDefaultClip("BrownIdleUp");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkWalkUp");
					SetAnimationDefaultClip("ChickenDarkIdleUp");
				}
			}
		}

		else if (m_tMoveDir.y > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickWalkDown");
					SetAnimationDefaultClip("ChickIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickWalkDown");
					SetAnimationDefaultClip("BlueChickIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickWalkDown");
					SetAnimationDefaultClip("DarkChickIdleDown");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenWalkDown");
					SetAnimationDefaultClip("ChickenIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueWalkDown");
					SetAnimationDefaultClip("BlueIdleDown");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkDown");
					SetAnimationDefaultClip("BrownIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkWalkDown");
					SetAnimationDefaultClip("ChickenDarkIdleDown");
				}
			}
		}

		else if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickWalkLeft");
					SetAnimationDefaultClip("ChickIdleLeft");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickWalkLeft");
					SetAnimationDefaultClip("BlueChickIdleLeft");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickWalkLeft");
					SetAnimationDefaultClip("DarkChickIdleLeft");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenWalkLeft");
					SetAnimationDefaultClip("ChickenIdleLeft");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueWalkLeft");
					SetAnimationDefaultClip("BlueIdleLeft");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkLeft");
					SetAnimationDefaultClip("BrownIdleLeft");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkWalkLeft");
					SetAnimationDefaultClip("ChickenDarkIdleLeft");
				}
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickWalkRight");
					SetAnimationDefaultClip("ChickIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickWalkRight");
					SetAnimationDefaultClip("BlueChickIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickWalkRight");
					SetAnimationDefaultClip("DarkChickIdleRight");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenWalkRight");
					SetAnimationDefaultClip("ChickIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueWalkRight");
					SetAnimationDefaultClip("BlueIdleRight");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownWalkRight");
					SetAnimationDefaultClip("BrownIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkWalkRight");
					SetAnimationDefaultClip("ChickenDarkIdleRight");
				}
			}
		}
	}

	else if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickIdleUp");
					SetAnimationDefaultClip("ChickIdleUp");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickIdleUp");
					SetAnimationDefaultClip("BlueChickIdleUp");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickIdleUp");
					SetAnimationDefaultClip("DarkChickIdleUp");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenIdleUp");
					SetAnimationDefaultClip("ChickenIdleUp");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueIdleUp");
					SetAnimationDefaultClip("BlueIdleUp");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleUp");
					SetAnimationDefaultClip("BrownIdleUp");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkIdleUp");
					SetAnimationDefaultClip("ChickenDarkIdleUp");
				}
			}
		}

		else if (m_tMoveDir.y > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickIdleDown");
					SetAnimationDefaultClip("ChickIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickIdleDown");
					SetAnimationDefaultClip("BlueChickIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickIdleDown");
					SetAnimationDefaultClip("DarkChickIdleDown");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenIdleDown");
					SetAnimationDefaultClip("ChickenIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueIdleDown");
					SetAnimationDefaultClip("BlueIdleDown");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleDown");
					SetAnimationDefaultClip("BrownIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkIdleDown");
					SetAnimationDefaultClip("ChickenDarkIdleDown");
				}
			}
		}

		else if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickIdleLeft");
					SetAnimationDefaultClip("ChickIdleLeft");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickIdleLeft");
					SetAnimationDefaultClip("BlueChickIdleLeft");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickIdleLeft");
					SetAnimationDefaultClip("DarkChickIdleLeft");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenIdleLeft");
					SetAnimationDefaultClip("ChickenIdleLeft");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueIdleLeft");
					SetAnimationDefaultClip("BlueIdleLeft");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleLeft");
					SetAnimationDefaultClip("BrownIdleLeft");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkIdleLeft");
					SetAnimationDefaultClip("ChickenDarkIdleLeft");
				}
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickIdleRight");
					SetAnimationDefaultClip("ChickIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickIdleRight");
					SetAnimationDefaultClip("BlueChickIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickIdleRight");
					SetAnimationDefaultClip("DarkChickIdleRight");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenIdleRight");
					SetAnimationDefaultClip("ChickenIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueIdleRight");
					SetAnimationDefaultClip("BlueIdleRight");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownIdleRight");
					SetAnimationDefaultClip("BrownIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkIdleRight");
					SetAnimationDefaultClip("ChickenDarkIdleRight");
				}
			}
		}
	}

	if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
			if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
			{
				SetAnimationCurrentClip("ChickEat");
			}

			else if (m_eColor == CC_BLUE)
			{
				SetAnimationCurrentClip("BlueChickEat");
			}

			else
			{
				SetAnimationCurrentClip("DarkChickEat");
			}
		}

		else
		{
			if (m_eColor == CC_WHITE)
				SetAnimationCurrentClip("ChickenEat");

			else if (m_eColor == CC_BLUE)
				SetAnimationCurrentClip("BlueEat");

			else if (m_eColor == CC_BROWN)
				SetAnimationCurrentClip("BrownEat");

			else
				SetAnimationCurrentClip("ChickenDarkEat");
		}

		m_fLimitTime = 5.f;

		m_fTime = 0.f;
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickFly");
					SetAnimationDefaultClip("ChickIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickFly");
					SetAnimationDefaultClip("BlueChickIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickFly");
					SetAnimationDefaultClip("DarkChickIdleRight");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenSitUp");
					SetAnimationDefaultClip("ChickenIdleUp");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueSitUp");
					SetAnimationDefaultClip("BlueIdleUp");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownSitUp");
					SetAnimationDefaultClip("BrownIdleUp");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkSitUp");
					SetAnimationDefaultClip("ChickenDarkIdleUp");
				}
			}
		}

		else if (m_tMoveDir.y > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickSitDown");
					SetAnimationDefaultClip("ChickIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickSitDown");
					SetAnimationDefaultClip("BlueChickIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickSitDown");
					SetAnimationDefaultClip("DarkChickIdleDown");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenSitDown");
					SetAnimationDefaultClip("ChickenIdleDown");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueSitDown");
					SetAnimationDefaultClip("BlueIdleDown");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownSitDown");
					SetAnimationDefaultClip("BrownIdleDown");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkSitDown");
					SetAnimationDefaultClip("ChickenDarkIdleDown");
				}
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("ChickSitRight");
					SetAnimationDefaultClip("ChickIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueChickSitRight");
					SetAnimationDefaultClip("BlueChickIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("DarkChickSitRight");
					SetAnimationDefaultClip("DarkChickIdleRight");
				}
			}

			else
			{
				if (m_eColor == CC_WHITE)
				{
					SetAnimationCurrentClip("ChickenSitRight");
					SetAnimationDefaultClip("ChickenIdleRight");
				}

				else if (m_eColor == CC_BLUE)
				{
					SetAnimationCurrentClip("BlueSitRight");
					SetAnimationDefaultClip("BlueIdleRight");
				}

				else if (m_eColor == CC_BROWN)
				{
					SetAnimationCurrentClip("BrownSitRight");
					SetAnimationDefaultClip("BrownIdleRight");
				}

				else
				{
					SetAnimationCurrentClip("ChickenDarkSitRight");
					SetAnimationDefaultClip("ChickenDarkIdleRight");
				}
			}
		}

		else if (m_tMoveDir.x < 0.f)
		{
		if (m_bBaby)
		{
			if (m_eColor == CC_WHITE || m_eColor == CC_BROWN)
			{
				SetAnimationCurrentClip("ChickSitLeft");
				SetAnimationDefaultClip("ChickIdleLeft");
			}

			else if (m_eColor == CC_BLUE)
			{
				SetAnimationCurrentClip("BlueChickSitLeft");
				SetAnimationDefaultClip("BlueChickIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("DarkChickSitLeft");
				SetAnimationDefaultClip("DarkChickIdleLeft");
			}
		}

		else
		{
			if (m_eColor == CC_WHITE)
			{
				SetAnimationCurrentClip("ChickenSitLeft");
				SetAnimationDefaultClip("ChickenIdleLeft");
			}

			else if (m_eColor == CC_BLUE)
			{
				SetAnimationCurrentClip("BlueSitLeft");
				SetAnimationDefaultClip("BlueIdleLeft");
			}

			else if (m_eColor == CC_BROWN)
			{
				SetAnimationCurrentClip("BrownSitLeft");
				SetAnimationDefaultClip("BrownIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("ChickenDarkSitLeft");
				SetAnimationDefaultClip("ChickenDarkIdleLeft");
			}
		}
		}

		m_fLimitTime = 5.f;

		m_fTime = 0.f;
	}
}