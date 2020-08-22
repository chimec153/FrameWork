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

	POSITION tAngle = m_pPlayer->GetAngle();

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
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtWalkRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtWalkLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtWalkDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtWalkUp");
		}
	}

	else if (eAction == PA_IDLE)
	{
		if (tAngle.x > 0.f)
			SetAnimationCurrentClip("ShirtIdleRight");

		else if (tAngle.x < 0.f)
			SetAnimationCurrentClip("ShirtIdleLeft");

		else if (tAngle.y > 0.f)
			SetAnimationCurrentClip("ShirtIdleDown");

		else if (tAngle.y < 0.f)
			SetAnimationCurrentClip("ShirtIdleUp");
	}

	else if (eAction == PA_ATTACK || eAction == PA_FARM)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtAttackRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtAttackLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtAttackDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtAttackUp");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleRight");
			SetAnimationCurrentClip("ShirtWaterRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleLeft");
			SetAnimationCurrentClip("ShirtWaterLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleDown");
			SetAnimationCurrentClip("ShirtWaterDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("ShirtIdleUp");
			SetAnimationCurrentClip("ShirtWaterUp");
		}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("ShirtEat");
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
