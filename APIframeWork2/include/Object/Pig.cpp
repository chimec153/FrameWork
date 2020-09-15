#include "Pig.h"
#include "../Animation/Animation.h"

Pig::Pig()	:
	m_iLevel(0)
{
	m_eAnimalType = ANIMAL_PIG;
	m_bTileEffect = true;
}

Pig::Pig(const Pig& pig)	:
	Animal(pig)
{
	m_iLevel = pig.m_iLevel;
}

Pig::~Pig()
{
}

bool Pig::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("PigAni");

	pAni->LoadFromPath("pig.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Pig::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);
	return 0;
}

int Pig::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Pig::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Pig::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Pig* Pig::Clone()
{
	return new Pig(*this);
}

void Pig::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletIdleLeft");
				SetAnimationDefaultClip("PigletIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("PigIdleLeft");
				SetAnimationDefaultClip("PigIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletIdleRight");
				SetAnimationDefaultClip("PigletIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("PigIdleRight");
				SetAnimationDefaultClip("PigIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletIdleUp");
				SetAnimationDefaultClip("PigletIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("PigIdleUp");
				SetAnimationDefaultClip("PigIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletIdleDown");
				SetAnimationDefaultClip("PigletIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("PigIdleDown");
				SetAnimationDefaultClip("PigIdleDown");
			}
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletWalkLeft");
				SetAnimationDefaultClip("PigletIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("PigWalkLeft");
				SetAnimationDefaultClip("PigIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletWalkRight");
				SetAnimationDefaultClip("PigletIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("PigWalkRight");
				SetAnimationDefaultClip("PigIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletWalkUp");
				SetAnimationDefaultClip("PigletIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("PigWalkUp");
				SetAnimationDefaultClip("PigIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletWalkDown");
				SetAnimationDefaultClip("PigletIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("PigWalkDown");
				SetAnimationDefaultClip("PigIdleDown");
			}
		}
	}

	else if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("PigletEat");
		}

		else
		{
			SetAnimationCurrentClip("PigEat");
		}
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletSleepLeft");
				SetAnimationDefaultClip("PigletIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("PigSleepLeft");
				SetAnimationDefaultClip("PigIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletSleepRight");
				SetAnimationDefaultClip("PigletIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("PigSleepRight");
				SetAnimationDefaultClip("PigIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletSleepUp");
				SetAnimationDefaultClip("PigletIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("PigSleepUp");
				SetAnimationDefaultClip("PigIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("PigletSleepDown");
				SetAnimationDefaultClip("PigletIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("PigSleepDown");
				SetAnimationDefaultClip("PigIdleDown");
			}
		}
	}
}

bool Pig::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;

		if (m_iFeedCount >= 5)
		{
			if (m_bBaby)
				m_bBaby = false;
		}
	}


	ActionChange(AA_IDLE);

	return false;
}
