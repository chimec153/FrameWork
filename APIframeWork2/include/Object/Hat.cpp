#include "Hat.h"
#include "../Animation/Animation.h"
#include "Player.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "Item.h"

Hat::Hat()	:
	m_pPlayer(nullptr),
	m_eHatType(HT_NONE)
{
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

Hat::Hat(const Hat& hat)	:
	MoveObj(hat)
{
	m_pPlayer = hat.m_pPlayer;
	m_eHatType = hat.m_eHatType;
}

Hat::~Hat()
{
	auto iter = m_vecHatName.begin();
	auto iterEnd = m_vecHatName.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE_ARRAY(*iter);
	}

	m_vecHatName.clear();
}

void Hat::SetHatType(HAT_TYPE eType)
{
	int iPrevX = m_eHatType % 12;
	int iPrevY = m_eHatType / 12;

	int iNewX = eType % 12;
	int iNewY = eType / 12;

	m_pAnimation->AddFramePos(iNewX - iPrevX, (iNewY - iPrevY) * 4);

	m_eHatType = eType;
}

bool Hat::Init()
{
	SetSize(40.f, 40.f);
	SetPivot(0.5f, 0.5f); 

	m_eHatType = HT_COWBOY;
	
	char strHat[][30] =
	{
		"Cowboy"
	};

	for (int i = 0; i < sizeof(strHat) / sizeof(strHat[0]); ++i)
	{
		char* pHat = new char[30];

		strcpy(pHat, strHat[i]);

		m_vecHatName.push_back(pHat);
	}		

	Animation* pHatAni = CreateAnimation("HatAni");

	pHatAni->LoadFromPath("hat.sac");

	SAFE_RELEASE(pHatAni);

	return true;
}

int Hat::Update(float fTime)
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

	if (m_eHatType != HT_NONE)
	{
		char strAni[64] = {};

		strcat(strAni, m_vecHatName[0]);

		char strWalk[64] = {};

		strcat(strWalk, m_vecHatName[0]);

		if (eAction == PA_WALK)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				strcat(strAni, "IdleRight");
				strcat(strWalk, "WalkRight");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleLeft");
				strcat(strWalk, "WalkLeft");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleUp");
				strcat(strWalk, "WalkUp");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else
			{
				strcat(strAni, "IdleDown");
				strcat(strWalk, "WalkDown");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}
		}

		else if (eAction == PA_IDLE)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				strcat(strAni, "IdleRight");

				SetAnimationCurrentClip(strAni);
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleLeft");

				SetAnimationCurrentClip(strAni);
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleUp");

				SetAnimationCurrentClip(strAni);
			}

			else
			{
				strcat(strAni, "IdleDown");

				SetAnimationCurrentClip(strAni);
			}
		}

		else if (eAction == PA_ATTACK || eAction == PA_FARM)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				strcat(strAni, "IdleRight");
				strcat(strWalk, "AttackRight");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleLeft");
				strcat(strWalk, "AttackLeft");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleUp");
				strcat(strWalk, "AttackUp");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else
			{
				strcat(strAni, "IdleDown");
				strcat(strWalk, "AttackDown");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}
		}

		else if (eAction == PA_WATER)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				strcat(strAni, "IdleRight");
				strcat(strWalk, "WaterRight");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleLeft");
				strcat(strWalk, "WaterLeft");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "IdleUp");
				strcat(strWalk, "WaterUp");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}

			else
			{
				strcat(strAni, "IdleDown");
				strcat(strWalk, "WaterDown");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strWalk);
			}
		}

		else if (eAction == PA_EAT)
		{
			strcat(strAni, "Eat");

			SetAnimationCurrentClip(strAni);
		}

		else if (eAction == PA_RIDE)
		{
			if (tAngle.x > tAngle.y && -tAngle.x < tAngle.y)
			{
				strcat(strAni, "RideRight");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strAni);
			}

			else if (tAngle.x < tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "RideLeft");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strAni);
			}

			else if (tAngle.x > tAngle.y && -tAngle.x > tAngle.y)
			{
				strcat(strAni, "RideUp");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strAni);
			}

			else
			{
				strcat(strAni, "RideDown");

				SetAnimationDefaultClip(strAni);
				SetAnimationCurrentClip(strAni);
			}
		}
	}

	return 0;
}

void Hat::Render(HDC hDC, float fTime)
{
	if(m_eHatType != HT_NONE)
		MoveObj::Render(hDC, fTime);
}

Hat* Hat::Clone()
{
	return new Hat(*this);
}
