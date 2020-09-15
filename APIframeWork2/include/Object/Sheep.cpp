#include "Sheep.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Item.h"
#include "Tool.h"

Sheep::Sheep()	:
	m_iLevel(0)
{
	m_eAnimalType = ANIMAL_SHEEP;
	m_bTileEffect = true;
}

Sheep::Sheep(const Sheep& sheep)	:
	Animal(sheep)
{
	m_iLevel = sheep.m_iLevel;

	Collider* pCol = GetCollider("AnimalBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &Sheep::ColEnter);
	pCol->AddCollisionFunction(CS_STAY, this, &Sheep::ColEnter);

	SAFE_RELEASE(pCol);
}

Sheep::~Sheep()
{
}

bool Sheep::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 1.f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("SheepAni");

	pAni->LoadFromPath("sheep.sac");

	SAFE_RELEASE(pAni);

	if (!Animal::Init())
		return false;

	return true;
}

int Sheep::Update(float fDeltaTime)
{
	Animal::Update(fDeltaTime);
	return 0;
}

int Sheep::LateUpdate(float fDeltaTime)
{
	Animal::LateUpdate(fDeltaTime);
	return 0;
}

void Sheep::Collision(float fDeltaTime)
{
	Animal::Collision(fDeltaTime);
}

void Sheep::Render(HDC hDC, float fDeltaTime)
{
	Animal::Render(hDC, fDeltaTime);
}

Sheep* Sheep::Clone()
{
	return new Sheep(*this);
}

void Sheep::ActionChange(ANIMAL_ACTION eAction)
{
	m_eAction = eAction;

	if (m_eAction == AA_IDLE)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedIdleLeft");
				SetAnimationDefaultClip("SheepShearedIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("SheepIdleLeft");
				SetAnimationDefaultClip("SheepIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedIdleRight");
				SetAnimationDefaultClip("SheepShearedIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("SheepIdleRight");
				SetAnimationDefaultClip("SheepIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedIdleUp");
				SetAnimationDefaultClip("SheepShearedIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("SheepIdleUp");
				SetAnimationDefaultClip("SheepIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedIdleDown");
				SetAnimationDefaultClip("SheepShearedIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("SheepIdleDown");
				SetAnimationDefaultClip("SheepIdleDown");
			}
		}
	}

	else if (m_eAction == AA_WALK)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedWalkLeft");
				SetAnimationDefaultClip("SheepShearedIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("SheepWalkLeft");
				SetAnimationDefaultClip("SheepIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedWalkRight");
				SetAnimationDefaultClip("SheepShearedIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("SheepWalkRight");
				SetAnimationDefaultClip("SheepIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedWalkUp");
				SetAnimationDefaultClip("SheepShearedIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("SheepWalkUp");
				SetAnimationDefaultClip("SheepIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedWalkDown");
				SetAnimationDefaultClip("SheepShearedIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("SheepWalkDown");
				SetAnimationDefaultClip("SheepIdleDown");
			}
		}
	}

	else if (m_eAction == AA_EAT)
	{
		if (m_bBaby)
		{
			SetAnimationCurrentClip("SheepShearedEat");
		}

		else
		{
			SetAnimationCurrentClip("SheepEat");
		}
	}

	else if (m_eAction == AA_SIT)
	{
		if (m_tMoveDir.x < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedSleepLeft");
				SetAnimationDefaultClip("SheepShearedIdleLeft");
			}

			else
			{
				SetAnimationCurrentClip("SheepSleepLeft");
				SetAnimationDefaultClip("SheepIdleLeft");
			}
		}

		else if (m_tMoveDir.x > 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedSleepRight");
				SetAnimationDefaultClip("SheepShearedIdleRight");
			}

			else
			{
				SetAnimationCurrentClip("SheepSleepRight");
				SetAnimationDefaultClip("SheepIdleRight");
			}
		}

		else if (m_tMoveDir.y < 0.f)
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedSleepUp");
				SetAnimationDefaultClip("SheepShearedIdleUp");
			}

			else
			{
				SetAnimationCurrentClip("SheepSleepUp");
				SetAnimationDefaultClip("SheepIdleUp");
			}
		}

		else
		{
			if (m_bBaby)
			{
				SetAnimationCurrentClip("SheepShearedSleepDown");
				SetAnimationDefaultClip("SheepShearedIdleDown");
			}

			else
			{
				SetAnimationCurrentClip("SheepSleepDown");
				SetAnimationDefaultClip("SheepIdleDown");
			}
		}
	}
}

bool Sheep::AddDay(int iDay)
{
	Animal::AddDay(iDay);

	if (IsFeed())
	{
		++m_iFeedCount;

		SetFeed(false);
	}

	if (m_bBaby)
	{
		if (m_iFeedCount > 0)
		{
			if (m_iDay > 2)
			{
				m_bBaby = false;

				m_iDay = 0;

				m_iFeedCount -= 1;
			}
		}
	}

	ActionChange(AA_IDLE);

	return false;
}

void Sheep::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		if (KEYDOWN("MouseLButton"))
		{
			if (!m_bBaby)
			{
				UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

				Item* pItem = pInven->GetItem();

				ITEM_TYPE eItemType = IT_NONE;

				if(pItem)
					eItemType = pItem->GetType();

				if (eItemType == IT_TOOL)
				{
					TOOL_TYPE eToolType = ((Tool*)pItem)->GetToolType();

					if (eToolType == TOOL_SCISSOR)
					{
						m_bBaby = true;

						m_iDay = 0;

						ActionChange(AA_IDLE);

						Item* pWool = (Item*)CreateCloneObj("Fleece", "Fleece", m_pLayer);

						pWool->SetPos(m_tPos);

						SAFE_RELEASE(pWool);
					}
				}

				SAFE_RELEASE(pInven);
			}
		}
	}
}

void Sheep::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
}
