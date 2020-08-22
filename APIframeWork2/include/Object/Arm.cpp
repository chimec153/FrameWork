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
		if (eType == IT_SEED || eType == IT_CROP || eType == IT_ETC)
		{
			if (tAngle.x > 0.f)
			{
				SetAnimationDefaultClip("ArmLiftIdleRight");
				SetAnimationCurrentClip("ArmLiftWalkRight");
			}

			else if (tAngle.x < 0.f)
			{
				SetAnimationDefaultClip("ArmLiftIdleLeft");
				SetAnimationCurrentClip("ArmLiftWalkLeft");
			}

			else if (tAngle.y > 0.f)
			{
				SetAnimationDefaultClip("ArmLiftIdleDown");
				SetAnimationCurrentClip("ArmLiftWalkDown");
			}

			else if (tAngle.y < 0.f)
			{
				SetAnimationDefaultClip("ArmLiftIdleUp");
				SetAnimationCurrentClip("ArmLiftWalkUp");
			}
		}

		else
		{
			if (tAngle.x > 0.f)
			{
				SetAnimationDefaultClip("ArmIdleRight");
				SetAnimationCurrentClip("ArmWalkRight");
			}

			else if (tAngle.x < 0.f)
			{
				SetAnimationDefaultClip("ArmIdleLeft");
				SetAnimationCurrentClip("ArmWalkLeft");
			}

			else if (tAngle.y > 0.f)
			{
				SetAnimationDefaultClip("ArmIdleDown");
				SetAnimationCurrentClip("ArmWalkDown");
			}

			else if (tAngle.y < 0.f)
			{
				SetAnimationDefaultClip("ArmIdleUp");
				SetAnimationCurrentClip("ArmWalkUp");
			}
		}
	}

	else if (eAction == PA_IDLE)
	{
		if (eType == IT_SEED || eType == IT_CROP || eType == IT_ETC)
		{
			if (tAngle.x > 0.f)
				SetAnimationCurrentClip("ArmLiftIdleRight");

			else if (tAngle.x < 0.f)
				SetAnimationCurrentClip("ArmLiftIdleLeft");

			else if (tAngle.y > 0.f)
				SetAnimationCurrentClip("ArmLiftIdleDown");

			else if (tAngle.y < 0.f)
				SetAnimationCurrentClip("ArmLiftIdleUp");
		}

		else
		{
			if (tAngle.x > 0.f)
				SetAnimationCurrentClip("ArmIdleRight");

			else if (tAngle.x < 0.f)
				SetAnimationCurrentClip("ArmIdleLeft");

			else if (tAngle.y > 0.f)
				SetAnimationCurrentClip("ArmIdleDown");

			else if (tAngle.y < 0.f)
				SetAnimationCurrentClip("ArmIdleUp");
		}
	}

	else if (eAction == PA_ATTACK)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("ArmIdleRight");
			SetAnimationCurrentClip("ArmAttackRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("ArmIdleLeft");
			SetAnimationCurrentClip("ArmAttackLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("ArmIdleDown");
			SetAnimationCurrentClip("ArmAttackDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("ArmIdleUp");
			SetAnimationCurrentClip("ArmAttackUp");
		}
	}

	else if (eAction == PA_FARM)
	{
		if (tAngle.x > 0.f)
		{
			SetAnimationDefaultClip("ArmIdleRight");
			SetAnimationCurrentClip("ArmFarmRight");
		}

		else if (tAngle.x < 0.f)
		{
			SetAnimationDefaultClip("ArmIdleLeft");
			SetAnimationCurrentClip("ArmFarmLeft");
		}

		else if (tAngle.y > 0.f)
		{
			SetAnimationDefaultClip("ArmIdleDown");
			SetAnimationCurrentClip("ArmFarmDown");
		}

		else if (tAngle.y < 0.f)
		{
			SetAnimationDefaultClip("ArmIdleUp");
			SetAnimationCurrentClip("ArmFarmUp");
		}
	}

	else if (eAction == PA_WATER)
	{
	if (tAngle.x > 0.f)
	{
		SetAnimationDefaultClip("ArmIdleRight");
		SetAnimationCurrentClip("ArmWaterRight");
	}

	else if (tAngle.x < 0.f)
	{
		SetAnimationDefaultClip("ArmIdleLeft");
		SetAnimationCurrentClip("ArmWaterLeft");
	}

	else if (tAngle.y > 0.f)
	{
		SetAnimationDefaultClip("ArmIdleDown");
		SetAnimationCurrentClip("ArmWaterDown");
	}

	else if (tAngle.y < 0.f)
	{
		SetAnimationDefaultClip("ArmIdleUp");
		SetAnimationCurrentClip("ArmWaterUp");
	}
	}

	else if (eAction == PA_EAT)
	{
		SetAnimationCurrentClip("ArmEat");
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
