#include "RockCrab.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/Scene.h"
#include "Effect.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"
#include "UINum.h"

RockCrab::RockCrab() :
	m_fDist(120.f),
	m_bRock(true),
	m_bAwake(false),
	m_iRock(3)
{
	m_bTileEffect = true;
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

RockCrab::RockCrab(const RockCrab& rockcrab) :
	FightObj(rockcrab)
{
	m_fDist = rockcrab.m_fDist;
	m_bRock = rockcrab.m_bRock;
	m_bAwake = rockcrab.m_bAwake;
	m_iRock = rockcrab.m_iRock;
}

RockCrab::~RockCrab()
{
}

bool RockCrab::Init()
{
	SetSize(32.f, 48.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(30.f);
	m_iHP = 30;

	Animation* pAni = CreateAnimation("RockCrabAni");

	pAni->AddClip("RockCrabOriginIdle", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 4, 1, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabOriginIdle", 255, 255, 255);

	pAni->AddClip("RockCrabIdleLeft", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 3, 1, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabIdleLeft", 255, 255, 255);
																		    
	pAni->AddClip("RockCrabIdleRight", AT_ATLAS, AO_LOOP,					    
		1.f, 4, 6, 0, 1, 1, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabIdleRight", 255, 255, 255);				    
																		    
	pAni->AddClip("RockCrabIdleUp", AT_ATLAS, AO_LOOP,						    
		1.f, 4, 6, 0, 2, 1, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabIdleUp", 255, 255, 255);					    
																		    
	pAni->AddClip("RockCrabIdleDown", AT_ATLAS, AO_LOOP,						    
		1.f, 4, 6, 0, 0, 1, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabIdleDown", 255, 255, 255);				    
																		    
	pAni->AddClip("RockCrabWalkLeft", AT_ATLAS, AO_ONCE_RETURN,				    
		1.f, 4, 6, 1, 3, 3, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabWalkLeft", 255, 255, 255);				    
																		    
	pAni->AddClip("RockCrabWalkRight", AT_ATLAS, AO_ONCE_RETURN,				    
		1.f, 4, 6, 1, 1, 3, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabWalkRight", 255, 255, 255);				    
																		    
	pAni->AddClip("RockCrabWalkUp", AT_ATLAS, AO_ONCE_RETURN,				    
		1.f, 4, 6, 1, 2, 3, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabWalkUp", 255, 255, 255);					    
																		    
	pAni->AddClip("RockCrabWalkDown", AT_ATLAS, AO_ONCE_RETURN,				    
		1.f, 4, 6, 1, 0, 3, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabWalkDown", 255, 255, 255);

	pAni->AddClip("RockCrabOriginWalk", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 1, 4, 3, 1, 1.f,0, "RockCrab", TEXT("Characters\\Monsters\\Rock Crab.bmp"));
	pAni->SetClipColorKey("RockCrabOriginWalk", 255, 255, 255);

	SAFE_RELEASE(pAni);

	if (!FightObj::Init())
		return false;

	return true;
}

int RockCrab::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

	POSITION tPos = pPlayer->GetPos();

	SAFE_RELEASE(pPlayer);

	POSITION tDist = tPos - m_tPos;

	float fDist = tDist.Length();

	if (fDist <= m_fDist)
	{
		tDist.Normalize();

		SetAngle(tDist);

		MoveAngle(fDeltaTime);

		float fAngle = tDist.AngleAxis();

		if (m_bRock)
		{
			if (tDist.x > 0.f && fAngle < 45.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabWalkRight");
			}

			else if (fAngle > 135.f && tDist.x < 0.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabWalkLeft");
			}

			else if (fAngle > 45.f && fAngle < 135.f && tDist.y >0.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabWalkDown");
			}

			else if (fAngle > 45.f && fAngle < 135.f && tDist.y < 0.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabWalkUp");
			}
		}
		else
			m_pAnimation->SetCurrentClip("RockCrabOriginWalk");

		m_bAwake = true;
	}
	else
	{
		POSITION tAngle = GetAngle();

		float fAngle = tAngle.AngleAxis();

		if (m_bRock)
		{
			if (tDist.x > 0.f && fAngle < 45.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabIdleRight");
			}

			else if (fAngle > 135.f && tDist.x < 0.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabIdleLeft");
			}

			else if (fAngle > 45.f && fAngle < 135.f && tDist.y < 0.f)
			{
				m_pAnimation->SetCurrentClip("RockCrabIdleUp");
			}

			else
			{
				m_pAnimation->SetCurrentClip("RockCrabIdleDown");
			}

		}

		else
			m_pAnimation->SetCurrentClip("RockCrabOriginIdle");

		m_bAwake = false;
	}

	return 0;
}

int RockCrab::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void RockCrab::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void RockCrab::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

RockCrab* RockCrab::Clone()
{
	return new RockCrab(*this);
}

void RockCrab::Collision(Collider* pSrc, Collider* pDest, float fTime)
{
}

void RockCrab::Hitted(int iAttack, const POSITION& tPos)
{
	if (!m_bRock)
	{
		if (AddHP(-iAttack))
			return;

		SetHitAngle(tPos);
		SetHitTime(0.5f);
	}

	else
	{
		iAttack = 0;

		if (m_bAwake)
			m_bRock = false;
	}		

	Layer* pLayer = m_pScene->FindLayer("UI");

	Obj* pNum = CreateCloneObj("Num", "Num", pLayer);

	((UINum*)pNum)->CreateNum(iAttack);

	pNum->SetPos(m_tPos.x, m_tPos.y - m_tSize.y * m_tPivot.y);

	SAFE_RELEASE(pNum);

	return;
}

void RockCrab::DieMotion()
{
	Die();

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 2; ++i)
		{
			Effect* pEffect = (Effect*)CreateCloneObj("RockCrabEffect", "RockCrabEffect", m_pLayer);

			POSITION tDir(i - 0.5f, j - 0.5f);
			pEffect->SetAngle(tDir);
			pEffect->SetPos(m_tPos);
			pEffect->SetImageOffset(i * 16.f, 16.f * j + 240.f);

			SAFE_RELEASE(pEffect);
		}
	}
}
