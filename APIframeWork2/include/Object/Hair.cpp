#include "Hair.h"
#include "Player.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Item.h"
#include "../Animation/Animation.h"

Hair::Hair()	:
	m_pPlayer(nullptr)
{
	//m_cAlpha = 255;
	//m_bAlphaOn = true;
}

Hair::Hair(const Hair& hair)	:
	MoveObj(hair)
{
	m_pPlayer = hair.m_pPlayer;
}

Hair::~Hair()
{
}

bool Hair::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pHairAni = CreateAnimation("HairAni");

	pHairAni->LoadFromPath("hair.sac");

	SAFE_RELEASE(pHairAni);

	return true;
}

int Hair::Update(float fTime)
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
			SetAnimationDefaultClip("HairIdleRight");
			SetAnimationCurrentClip("HairWalkRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleLeft");
			SetAnimationCurrentClip("HairWalkLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleUp");
			SetAnimationCurrentClip("HairWalkUp");
		}

		else
		{
			SetAnimationDefaultClip("HairIdleDown");
			SetAnimationCurrentClip("HairWalkDown");
		}
	}

	else if (eAction == PA_IDLE)
	{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
				SetAnimationCurrentClip("HairIdleRight");

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("HairIdleLeft");

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("HairIdleUp");

			else
				SetAnimationCurrentClip("HairIdleDown");
	}

	else if (eAction == PA_ATTACK || eAction == PA_FARM)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleRight");
			SetAnimationCurrentClip("HairAttackRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleLeft");
			SetAnimationCurrentClip("HairAttackLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleUp");
			SetAnimationCurrentClip("HairAttackUp");
		}

		else
		{
			SetAnimationDefaultClip("HairIdleDown");
			SetAnimationCurrentClip("HairAttackDown");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleRight");
			SetAnimationCurrentClip("HairWaterRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleLeft");
			SetAnimationCurrentClip("HairWaterLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairIdleUp");
			SetAnimationCurrentClip("HairWaterUp");
		}

		else
		{
			SetAnimationDefaultClip("HairIdleDown");
			SetAnimationCurrentClip("HairWaterDown");
		}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("HairEat");
	}

	else if (eAction == PA_RIDE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("HairRideRight");
			SetAnimationCurrentClip("HairRideRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairRideLeft");
			SetAnimationCurrentClip("HairRideLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("HairRideUp");
			SetAnimationCurrentClip("HairRideUp");
		}

		else
		{
			SetAnimationDefaultClip("HairRideDown");
			SetAnimationCurrentClip("HairRideDown");
		}
	}

	return 0;
}

void Hair::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

Hair* Hair::Clone()
{
	return new Hair(*this);
}
