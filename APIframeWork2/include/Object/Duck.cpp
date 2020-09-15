#include "Duck.h"
#include "../Animation/Animation.h"
#include "Egg.h"
#include "ObjManager.h"
#include "UIClockHand.h"

Duck::Duck()
{
	m_eAnimalType = ANIMAL_DUCK;
	m_bTileEffect = true;
}

Duck::Duck(const Duck& duck)	:
	Animal(duck)
{
}

Duck::~Duck()
{
}

bool Duck::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("DuckAni");

	pAni->LoadFromPath("duck.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Duck::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);
	return 0;
}

int Duck::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Duck::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Duck::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Duck* Duck::Clone()
{
	return new Duck(*this);
}

void Duck::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x > 0.f || m_tMoveDir.y < 0.f)
		{
			SetAnimationCurrentClip("DuckIdleRight");
			SetAnimationDefaultClip("DuckIdleRight");
		}

		else
		{
			SetAnimationCurrentClip("DuckIdleLeft");
			SetAnimationDefaultClip("DuckIdleLeft");
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x > 0.f || m_tMoveDir.y < 0.f)
		{
			SetAnimationCurrentClip("DuckWalkRight");
			SetAnimationDefaultClip("DuckIdleRight");
		}

		else
		{
			SetAnimationCurrentClip("DuckWalkLeft");
			SetAnimationDefaultClip("DuckIdleLeft");
		}
	}

	else if (m_eAction == AA_EAT)
	{
		SetAnimationCurrentClip("DuckEat");
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_tMoveDir.x > 0.f || m_tMoveDir.y < 0.f)
		{
			SetAnimationCurrentClip("DuckSleepRight");
			SetAnimationDefaultClip("DuckIdleRight");
		}

		else
		{
			SetAnimationCurrentClip("DuckSleepLeft");
			SetAnimationDefaultClip("DuckIdleLeft");
		}
	}
}

bool Duck::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		SetFeed(false);

		++m_iFeedCount;

		if (m_iFeedCount >= 1)
		{
			Egg* pDuckEgg = (Egg*)CreateCloneObj("Duck Egg", "Duck Egg", m_pLayer);

			pDuckEgg->SetPos(m_tPos);

			UIClockHand* pClockHand = (UIClockHand*)GET_SINGLE(ObjManager)->GetClockHand();

			pClockHand->AddCrop(pDuckEgg);

			SAFE_RELEASE(pDuckEgg);

			--m_iFeedCount;
		}
	}

	if (m_iDay % 4 == 0)
	{
		Obj* pFeather = CreateCloneObj("Duck's feather", "Feather", m_pLayer);

		pFeather->SetPos(m_tPos);

		SAFE_RELEASE(pFeather);
	}

	ActionChange(AA_IDLE);

	return false;
}
