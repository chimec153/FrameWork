#include "Pants.h"
#include "../Animation/Animation.h"
#include "Player.h"
#include "UIInventory.h"
#include "Item.h"
#include "ObjManager.h"

Pants::Pants()	:
	m_pPlayer(nullptr)
{
}

Pants::Pants(const Pants& pants)	:
	MoveObj(pants)
{
	m_pPlayer = pants.m_pPlayer;
}

Pants::~Pants()
{
}

bool Pants::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pPantsAni = CreateAnimation("PantsAni");

	pPantsAni->LoadFromPath("pants.sac");

	SAFE_RELEASE(pPantsAni);

	return true;
}

int Pants::Update(float fTime)
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
			SetAnimationDefaultClip("PantsIdleRight");
			SetAnimationCurrentClip("PantsWalkRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleLeft");
			SetAnimationCurrentClip("PantsWalkLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleUp");
			SetAnimationCurrentClip("PantsWalkUp");
		}

		else 
		{
			SetAnimationDefaultClip("PantsIdleDown");
			SetAnimationCurrentClip("PantsWalkDown");
		}
	}

	else if (eAction == PA_IDLE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			SetAnimationCurrentClip("PantsIdleRight");

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			SetAnimationCurrentClip("PantsIdleLeft");

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			SetAnimationCurrentClip("PantsIdleUp");

		else
			SetAnimationCurrentClip("PantsIdleDown");
	}

	else if (eAction == PA_ATTACK || eAction == PA_FARM)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleRight");
			SetAnimationCurrentClip("PantsAttackRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleLeft");
			SetAnimationCurrentClip("PantsAttackLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleUp");
			SetAnimationCurrentClip("PantsAttackUp");
		}

		else
		{
			SetAnimationDefaultClip("PantsIdleDown");
			SetAnimationCurrentClip("PantsAttackDown");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleRight");
			SetAnimationCurrentClip("PantsWaterRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleLeft");
			SetAnimationCurrentClip("PantsWaterLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsIdleUp");
			SetAnimationCurrentClip("PantsWaterUp");
		}

		else
		{
			SetAnimationDefaultClip("PantsIdleDown");
			SetAnimationCurrentClip("PantsWaterDown");
		}
	}
	
	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("PantsEat");
	}

	else if (eAction == PA_RIDE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("PantsRideRight");
			SetAnimationCurrentClip("PantsRideRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsRideLeft");
			SetAnimationCurrentClip("PantsRideLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("PantsRideUp");
			SetAnimationCurrentClip("PantsRideUp");
		}

		else
		{
			SetAnimationDefaultClip("PantsRideDown");
			SetAnimationCurrentClip("PantsRideDown");
		}
	}

	return 0;
}

void Pants::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

Pants* Pants::Clone()
{
	return new Pants(*this);
}
