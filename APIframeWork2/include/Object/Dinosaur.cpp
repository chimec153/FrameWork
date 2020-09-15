#include "Dinosaur.h"
#include "../Animation/Animation.h"

Dinosaur::Dinosaur()
{
	m_eAnimalType = ANIMAL_DINOSAUR;
	m_bTileEffect = true;
}

Dinosaur::Dinosaur(const Dinosaur& dinosaur)	:
	Animal(dinosaur)
{
}

Dinosaur::~Dinosaur()
{
}

bool Dinosaur::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("DinosaurAni");

	pAni->LoadFromPath("dinosaur.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Dinosaur::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);
	return 0;
}

int Dinosaur::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Dinosaur::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Dinosaur::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Dinosaur* Dinosaur::Clone()
{
	return new Dinosaur(*this);
}

void Dinosaur::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			SetAnimationCurrentClip("DinosaurIdleLeft");
			SetAnimationDefaultClip("DinosaurIdleLeft");
		}

		else if (m_tMoveDir.x > 0.f)
		{
			SetAnimationCurrentClip("DinosaurIdleRight");
			SetAnimationDefaultClip("DinosaurIdleRight");
		}

		else if (m_tMoveDir.y < 0.f)
		{
			SetAnimationCurrentClip("DinosaurIdleUp");
			SetAnimationDefaultClip("DinosaurIdleUp");
		}

		else
		{
			SetAnimationCurrentClip("DinosaurIdleDown");
			SetAnimationDefaultClip("DinosaurIdleDown");
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			SetAnimationCurrentClip("DinosaurWalkLeft");
			SetAnimationDefaultClip("DinosaurIdleLeft");
		}

		else if (m_tMoveDir.x > 0.f)
		{
			SetAnimationCurrentClip("DinosaurWalkRight");
			SetAnimationDefaultClip("DinosaurIdleRight");
		}

		else if (m_tMoveDir.y < 0.f)
		{
			SetAnimationCurrentClip("DinosaurWalkUp");
			SetAnimationDefaultClip("DinosaurIdleUp");
		}

		else
		{
			SetAnimationCurrentClip("DinosaurWalkDown");
			SetAnimationDefaultClip("DinosaurIdleDown");
		}
	}

	else if (m_eAction == AA_EAT)
	{
		SetAnimationCurrentClip("DinosaurEat");
	}

	else if (m_eAction == AA_SIT)
	{
		SetAnimationCurrentClip("DinosaurSleep");
	}
}

bool Dinosaur::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;
	}

	ActionChange(AA_IDLE);

	return false;
}
