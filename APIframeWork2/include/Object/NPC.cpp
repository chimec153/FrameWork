#include "NPC.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Input.h"
#include "../Scene/Scene.h"
#include "Player.h"
#include "UIInventory.h"
#include "ObjManager.h"
#include "UIShop.h"

NPC::NPC()	:
	m_fLimitDist(200.f),
	m_fDist(0.f),
	m_bWalkStart(false)
{
}

NPC::NPC(const NPC& npc)	:
	FightObj(npc)
{
	m_fLimitDist = npc.m_fLimitDist;
	m_fDist = npc.m_fDist;
	m_bWalkStart = npc.m_bWalkStart;
}

NPC::~NPC()
{
}

bool NPC::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);

	Animation* pAni = CreateAnimation("NPCAni");

	AddAnimationClip("HarveyIdleDown", AT_ATLAS, AO_LOOP, 1.f, 4, 13, 0, 0, 1, 1, 1.f,0, "HarveyAni", TEXT("Characters\\NPC\\Harvey.bmp"));
	SetAnimationClipColorKey("HarveyIdleDown", 255, 255, 255);

	AddAnimationClip("MarlonIdleDown", AT_ATLAS, AO_LOOP, 1.f, 4, 4, 0, 0, 1, 1, 1.f,0, "MarlonAni", TEXT("Characters\\NPC\\Marlon.bmp"));
	SetAnimationClipColorKey("MarlonIdleDown", 255, 255, 255);

	AddAnimationClip("RobinIdleDown", AT_ATLAS, AO_LOOP, 1.f, 4, 9, 0, 0, 1, 1, 1.f,0, "RobinAni", TEXT("Characters\\NPC\\Robin.bmp"));
	SetAnimationClipColorKey("RobinIdleDown", 255, 255, 255);

	pAni->LoadFromPath("Haley.sac");

	SAFE_RELEASE(pAni);

	ColliderRect* pRC = AddCollider<ColliderRect>("NPCBody");

	pRC->SetRect(-16.f, -32.f, 16.f, 32.f);

	pRC->AddCollisionFunction(CS_ENTER, this, &NPC::ColEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &NPC::CollStay);
	pRC->AddCollisionFunction(CS_LEAVE, this, &NPC::CollStay);

	SAFE_RELEASE(pRC);

	return true;
}

int NPC::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	MoveAngle(fDeltaTime);

	m_fDist += m_fSpeed * fDeltaTime;

	if (m_fDist >= m_fLimitDist)
	{
		m_fDist -= m_fLimitDist;

		m_tMoveDir *= -1;

		if (GetTag() == "Haley")
		{
			if(m_tMoveDir.x > 0.f)
				SetAnimationCurrentClip("HaleyWalkRight");

			else
				SetAnimationCurrentClip("HaleyWalkLeft");
		}
	}

	if (m_bWalkStart)
	{
		if (m_pAnimation->GetMotionEnd())
		{
			if (m_tMoveDir.x > 0.f)
				SetAnimationCurrentClip("HaleyWalkRight");

			else
				SetAnimationCurrentClip("HaleyWalkLeft");

			SetSpeed(50.f);

			m_bWalkStart = false;
		}

	}

	return 0;
}

int NPC::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);

	return 0;
}

void NPC::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void NPC::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

NPC* NPC::Clone()
{
	return new NPC(*this);
}

void NPC::ColEnter(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "Mouse")
	{
		if (KEYDOWN("MouseLButton"))
		{

		}
	}
}

void NPC::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "Mouse")
	{
		if (KEYDOWN("MouseLButton"))
		{
			Player* pPlayer = (Player*)GET_SINGLE(ObjManager)->GetPlayer();

			if (pPlayer)
			{
				UIInventory* pInven = GET_SINGLE(ObjManager)->GetInven();

				string strTag = GetTag();

				if (strTag == "Robin")
				{
					if (pInven)
						pInven->CreateBuildShop();
				}

				else if (strTag == "Haley")
				{
					if (pInven)
					{
						pInven->CreateAnimalShop();

						SetAnimationCurrentClip("HaleyIdleDown");

						SetSpeed(0.f);
					}
				}

				else
				{
					if (pInven)
						pInven->CreateShop(strTag);
				}

				SAFE_RELEASE(pPlayer);
				SAFE_RELEASE(pInven);
			}
		}
	}
}

void NPC::CollEnd(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "Mouse")
	{
	}
}
