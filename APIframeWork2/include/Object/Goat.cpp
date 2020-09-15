#include "Goat.h"
#include "../Animation/Animation.h"

Goat::Goat()	:
	m_iLevel(0)
{
	m_eAnimalType = ANIMAL_GOAT;
	m_bTileEffect = true;
}

Goat::Goat(const Goat& goat)	:
	Animal(goat)
{
	m_iLevel = goat.m_iLevel;
}

Goat::~Goat()
{
}

bool Goat::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("GoatAni");

	pAni->LoadFromPath("goat.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Goat::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);
	return 0;
}

int Goat::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Goat::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Goat::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Goat* Goat::Clone()
{
	return new Goat(*this);
}

void Goat::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyIdleLeft");
					SetAnimationDefaultClip("GoatBabyIdleLeft");
			}

			else
			{
					SetAnimationCurrentClip("GoatIdleLeft");
					SetAnimationDefaultClip("GoatIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("GoatBabyIdleRight");
				SetAnimationDefaultClip("GoatBabyIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("GoatIdleRight");
				SetAnimationDefaultClip("GoatIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyIdleUp");
					SetAnimationDefaultClip("GoatBabyIdleUp");
			}

			else
			{
					SetAnimationCurrentClip("GoatIdleUp");
					SetAnimationDefaultClip("GoatIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("GoatBabyIdleDown");
				SetAnimationDefaultClip("GoatBabyIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("GoatIdleDown");
				SetAnimationDefaultClip("GoatIdleDown");
			}
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyWalkLeft");
					SetAnimationDefaultClip("GoatBabyIdleLeft");
			}

			else
			{
					SetAnimationCurrentClip("GoatWalkLeft");
					SetAnimationDefaultClip("GoatIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyWalkRight");
					SetAnimationDefaultClip("GoatBabyIdleRight");
			}

			else
			{
					SetAnimationCurrentClip("GoatWalkRight");
					SetAnimationDefaultClip("GoatIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyWalkUp");
					SetAnimationDefaultClip("GoatBabyIdleUp");
			}

			else
			{
					SetAnimationCurrentClip("GoatWalkUp");
					SetAnimationDefaultClip("GoatIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
					SetAnimationCurrentClip("GoatBabyWalkDown");
					SetAnimationDefaultClip("GoatBabyIdleDown");
			}

			else
			{
					SetAnimationCurrentClip("GoatWalkDown");
					SetAnimationDefaultClip("GoatIdleDown");
			}
		}
	}

	else if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
				SetAnimationCurrentClip("GoatBabyEat");
		}

		else
		{
				SetAnimationCurrentClip("GoatEat");
		}
	}

	else if (m_eAction == AA_SIT)
	{
	if (m_tMoveDir.x < 0.f)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("GoatBabySleepLeft");
			SetAnimationDefaultClip("GoatBabyIdleLeft");
		}

		else
		{
			SetAnimationCurrentClip("GoatSleepLeft");
			SetAnimationDefaultClip("GoatIdleLeft");
		}
	}

	else if (m_tMoveDir.x > 0.f)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("GoatBabySleepRight");
			SetAnimationDefaultClip("GoatBabyIdleRight");
		}

		else
		{
			SetAnimationCurrentClip("GoatSleepRight");
			SetAnimationDefaultClip("GoatIdleRight");
		}
	}

	else if (m_tMoveDir.y < 0.f)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("GoatBabySleepUp");
			SetAnimationDefaultClip("GoatBabyIdleUp");
		}

		else
		{
			SetAnimationCurrentClip("GoatSleepUp");
			SetAnimationDefaultClip("GoatIdleUp");
		}
	}

	else
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("GoatBabySleepDown");
			SetAnimationDefaultClip("GoatBabyIdleDown");
		}

		else
		{
			SetAnimationCurrentClip("GoatSleepDown");
			SetAnimationDefaultClip("GoatIdleDown");
		}
	}
	}
}

bool Goat::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;

		if (m_bBaby)
		{
			if (m_iFeedCount >= 5)
				m_bBaby = false;
		}
	}

	ActionChange(AA_IDLE);

	return false;
}
