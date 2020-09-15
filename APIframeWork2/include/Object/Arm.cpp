#include "Arm.h"
#include "Player.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "Item.h"
#include "../Animation/Animation.h"

Arm::Arm()	:
	m_pPlayer(nullptr)
{
}

Arm::Arm(const Arm& arm)	:
	MoveObj(arm)
{
	m_pPlayer = arm.m_pPlayer;
}

Arm::~Arm()
{
}

bool Arm::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pArmAni = CreateAnimation("Arm");

	pArmAni->LoadFromPath("arm.sac");

	SAFE_RELEASE(pArmAni);

	return true;
}

int Arm::Update(float fTime)
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
		if (eType == IT_SEED || eType == IT_CROP || eType == IT_ETC)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				SetAnimationDefaultClip("ArmLiftIdleRight");
				SetAnimationCurrentClip("ArmLiftWalkRight");
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				SetAnimationDefaultClip("ArmLiftIdleLeft");
				SetAnimationCurrentClip("ArmLiftWalkLeft");
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				SetAnimationDefaultClip("ArmLiftIdleUp");
				SetAnimationCurrentClip("ArmLiftWalkUp");
			}

			else
			{
				SetAnimationDefaultClip("ArmLiftIdleDown");
				SetAnimationCurrentClip("ArmLiftWalkDown");
			}
		}

		else
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				SetAnimationDefaultClip("ArmIdleRight");
				SetAnimationCurrentClip("ArmWalkRight");
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				SetAnimationDefaultClip("ArmIdleLeft");
				SetAnimationCurrentClip("ArmWalkLeft");
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				SetAnimationDefaultClip("ArmIdleUp");
				SetAnimationCurrentClip("ArmWalkUp");
			}

			else 
			{
				SetAnimationDefaultClip("ArmIdleDown");
				SetAnimationCurrentClip("ArmWalkDown");
			}
		}
	}

	else if (eAction == PA_IDLE)
	{
		if (eType == IT_SEED || eType == IT_CROP || eType == IT_ETC)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
				SetAnimationCurrentClip("ArmLiftIdleRight");

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("ArmLiftIdleLeft");

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("ArmLiftIdleUp");

			else
				SetAnimationCurrentClip("ArmLiftIdleDown");
		}

		else
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
				SetAnimationCurrentClip("ArmIdleRight");

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("ArmIdleLeft");

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
				SetAnimationCurrentClip("ArmIdleUp");

			else
				SetAnimationCurrentClip("ArmIdleDown");
		}
	}

	else if (eAction == PA_ATTACK)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleRight");
			SetAnimationCurrentClip("ArmAttackRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleLeft");
			SetAnimationCurrentClip("ArmAttackLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleUp");
			SetAnimationCurrentClip("ArmAttackUp");
		}

		else
		{
			SetAnimationDefaultClip("ArmIdleDown");
			SetAnimationCurrentClip("ArmAttackDown");
		}
	}

	else if (eAction == PA_FARM)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleRight");
			SetAnimationCurrentClip("ArmFarmRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleLeft");
			SetAnimationCurrentClip("ArmFarmLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleUp");
			SetAnimationCurrentClip("ArmFarmUp");
		}

		else
		{
			SetAnimationDefaultClip("ArmIdleDown");
			SetAnimationCurrentClip("ArmFarmDown");
		}
	}

	else if (eAction == PA_WATER)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleRight");
			SetAnimationCurrentClip("ArmWaterRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleLeft");
			SetAnimationCurrentClip("ArmWaterLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmIdleUp");
			SetAnimationCurrentClip("ArmWaterUp");
		}

		else
		{
			SetAnimationDefaultClip("ArmIdleDown");
			SetAnimationCurrentClip("ArmWaterDown");
		}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("ArmEat");
	}

	else if (eAction == PA_RIDE)
	{
		if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
		{
			SetAnimationDefaultClip("ArmRideRight");
			SetAnimationCurrentClip("ArmRideRight");
		}

		else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmRideLeft");
			SetAnimationCurrentClip("ArmRideLeft");
		}

		else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
		{
			SetAnimationDefaultClip("ArmRideUp");
			SetAnimationCurrentClip("ArmRideUp");
		}

		else
		{
			SetAnimationDefaultClip("ArmRideDown");
			SetAnimationCurrentClip("ArmRideDown");
		}
	}

	return 0;
}

void Arm::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

Arm* Arm::Clone()
{
	return new Arm(*this);
}
