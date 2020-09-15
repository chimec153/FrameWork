#include "Rabbit.h"
#include "../Animation/Animation.h"
#include "../Core/Input.h"

Rabbit::Rabbit()	:
	m_iLevel(0)
{
	m_eAnimalType = ANIMAL_RABBIT;
	m_bTileEffect = true;
}

Rabbit::Rabbit(const Rabbit& rabbit)	:
	Animal(rabbit)
{
	m_iLevel = rabbit.m_iLevel;
}

Rabbit::~Rabbit()
{
}

bool Rabbit::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("RabbitAni");

	pAni->LoadFromPath("rabbit.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Rabbit::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);

	return 0;
}

int Rabbit::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Rabbit::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Rabbit::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Rabbit* Rabbit::Clone()
{
	return new Rabbit(*this);
}

void Rabbit::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyIdleLeft");
				SetAnimationDefaultClip("RabbitBabyIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("RabbitIdleLeft");
				SetAnimationDefaultClip("RabbitIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyIdleRight");
				SetAnimationDefaultClip("RabbitBabyIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("RabbitIdleRight");
				SetAnimationDefaultClip("RabbitIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyIdleUp");
				SetAnimationDefaultClip("RabbitBabyIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("RabbitIdleUp");
				SetAnimationDefaultClip("RabbitIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyIdleDown");
				SetAnimationDefaultClip("RabbitBabyIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("RabbitIdleDown");
				SetAnimationDefaultClip("RabbitIdleDown");
			}
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyWalkLeft");
				SetAnimationDefaultClip("RabbitBabyIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("RabbitWalkLeft");
				SetAnimationDefaultClip("RabbitIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyWalkRight");
				SetAnimationDefaultClip("RabbitBabyIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("RabbitWalkRight");
				SetAnimationDefaultClip("RabbitIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyWalkUp");
				SetAnimationDefaultClip("RabbitBabyIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("RabbitWalkUp");
				SetAnimationDefaultClip("RabbitIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabyWalkDown");
				SetAnimationDefaultClip("RabbitBabyIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("RabbitWalkDown");
				SetAnimationDefaultClip("RabbitIdleDown");
			}
		}
	}

	else if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("RabbitBabyEat");
		}

		else
		{
			SetAnimationCurrentClip("RabbitEat");
		}
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabySleepLeft");
				SetAnimationDefaultClip("RabbitBabyIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("RabbitSleepLeft");
				SetAnimationDefaultClip("RabbitIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabySleepRight");
				SetAnimationDefaultClip("RabbitBabyIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("RabbitSleepRight");
				SetAnimationDefaultClip("RabbitIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabySleepUp");
				SetAnimationDefaultClip("RabbitBabyIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("RabbitSleepUp");
				SetAnimationDefaultClip("RabbitIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("RabbitBabySleepDown");
				SetAnimationDefaultClip("RabbitBabyIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("RabbitSleepDown");
				SetAnimationDefaultClip("RabbitIdleDown");
			}
		}
	}
}

bool Rabbit::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;

		if (m_bBaby) 
		{
			if (m_iFeedCount >= 3)
			{
				m_bBaby = false;
			}
		}
	}

	if (!m_bBaby && m_iDay % 4 == 0)
	{
		if (m_iFriendShip < 7)
		{
			Obj* pWool = CreateCloneObj("Fleece", "Wool", m_pLayer);

			pWool->SetPos(m_tPos);

			SAFE_RELEASE(pWool);
		}

		else
		{
			Obj* pFeet = CreateCloneObj("Rabbit's Feet", "rabbit feet", m_pLayer);

			pFeet->SetPos(m_tPos);

			SAFE_RELEASE(pFeet);
		}
	}

	ActionChange(AA_IDLE);

	return false;
}

void Rabbit::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
}
