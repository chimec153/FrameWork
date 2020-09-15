#include "Shirt.h"
#include "Player.h"
#include "ObjManager.h"
#include "Item.h"
#include "UIInventory.h"
#include "../Animation/Animation.h"

Shirt::Shirt()	:
	m_pPlayer(nullptr)
{
}

Shirt::Shirt(const Shirt& shirt)	:
	MoveObj(shirt)
{
	m_pPlayer = shirt.m_pPlayer;
}

Shirt::~Shirt()
{
}

bool Shirt::Init()
{
	SetSize(16.f, 16.f);
	SetPivot(0.5f, 0.5f);

	Animation* pShirtAni = CreateAnimation("ShirtAni");

	pShirtAni->LoadFromPath("shirt.sac");

	SAFE_RELEASE(pShirtAni);

	return true;
}

int Shirt::Update(float fTime)
{
	MoveObj::Update(fTime);

	POSITION tAngle = GetAngle();

	PLAYER_ACTION eAction = m_pPlayer->GetPlayerAction();

	UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

	Item* pItem = nullptr;

	if (pInven)
		pItem = pInven->GetItem();

	SAFE_RELEASE(pInven);

	ITEM_TYPE eType = IT_NONE;

	if (pItem)
		eType = pItem->GetType();

	if (eAction == PA_WALK)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtWalkRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtWalkLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtWalkUp");
		}

		else
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtWalkDown");
		}
	}

	else if (eAction == PA_IDLE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			SetAnimationCurrentClip("ShirtIdleRight");

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			SetAnimationCurrentClip("ShirtIdleLeft");

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			SetAnimationCurrentClip("ShirtIdleUp");

		else
			SetAnimationCurrentClip("ShirtIdleDown");
	}

	else if (eAction == PA_ATTACK || eAction == PA_FARM)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtAttackRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtAttackLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtAttackUp");
		}

		else
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtAttackDown");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtWaterRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtWaterLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtWaterUp");
		}

		else
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtWaterDown");
		}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("ShirtEat");
	}

	else if (eAction == PA_RIDE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ShirtRideRight");
			SetAnimationCurrentClip("ShirtRideRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtRideLeft");
			SetAnimationCurrentClip("ShirtRideLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ShirtRideUp");
			SetAnimationCurrentClip("ShirtRideUp");
		}

		else
		{
			SetAnimationDefaultClip("ShirtRideDown");
			SetAnimationCurrentClip("ShirtRideDown");
		}
	}

	return 0;
}

void Shirt::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

Shirt* Shirt::Clone()
{
	return new Shirt(*this);
}
