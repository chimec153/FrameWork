#include "Hair.h"
#include "Player.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Item.h"
#include "../Animation/Animation.h"

Hair::Hair()	:
	m_pPlayer(nullptr)
{
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
				SetAnimationDefaultClip("HairIdleRight");
				SetAnimationCurrentClip("HairWalkRight");
			}

			else if (tAngle.x < 0.f)
			{
				SetAnimationDefaultClip("HairIdleLeft");
				SetAnimationCurrentClip("HairWalkLeft");
			}

			else if (tAngle.y > 0.f)
			{
				SetAnimationDefaultClip("HairIdleDown");
				SetAnimationCurrentClip("HairWalkDown");
			}

			else if (tAngle.y < 0.f)
			{
				SetAnimationDefaultClip("HairIdleUp");
				SetAnimationCurrentClip("HairWalkUp");
			}
	}

	else if (eAction == PA_IDLE)
	{
			if (tAngle.x > 0.f)
				SetAnimationCurrentClip("HairIdleRight");

			else if (tAngle.x < 0.f)
				SetAnimationCurrentClip("HairIdleLeft");

			else if (tAngle.y > 0.f)
				SetAnimationCurrentClip("HairIdleDown");

			else if (tAngle.y < 0.f)
				SetAnimationCurrentClip("HairIdleUp");
	}

	else if (eAction == PA_ATTACK || eAction == PA_FARM)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("HairIdleRight");
			SetAnimationCurrentClip("HairAttackRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("HairIdleLeft");
			SetAnimationCurrentClip("HairAttackLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("HairIdleDown");
			SetAnimationCurrentClip("HairAttackDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("HairIdleUp");
			SetAnimationCurrentClip("HairAttackUp");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("HairIdleRight");
			SetAnimationCurrentClip("HairWaterRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("HairIdleLeft");
			SetAnimationCurrentClip("HairWaterLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("HairIdleDown");
			SetAnimationCurrentClip("HairWaterDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("HairIdleUp");
			SetAnimationCurrentClip("HairWaterUp");
		}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("HairEat");
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
