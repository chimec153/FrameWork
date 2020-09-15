#include "Bug.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/Scene.h"
#include "Effect.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"

Bug::Bug()	:
	m_fDist(250.f)
{
	m_bTileEffect = false;
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

Bug::Bug(const Bug& bug)	:
	FightObj(bug)
{
	m_fDist = bug.m_fDist;
}

Bug::~Bug()
{
}

bool Bug::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(30.f);
	m_iHP = 30;

	Animation* pAni = CreateAnimation("BugAni");

	pAni->AddClip("BugIdleLeft", AT_ATLAS, AO_LOOP,
		0.5f, 4, 5, 0, 3, 2, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugIdleLeft", 255, 255, 255);

	pAni->AddClip("BugIdleRight", AT_ATLAS, AO_LOOP,
		1.f, 4, 5, 0, 1, 2, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugIdleRight", 255, 255, 255);

	pAni->AddClip("BugIdleUp", AT_ATLAS, AO_LOOP,
		1.f, 4, 5, 0, 2, 2, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugIdleUp", 255, 255, 255);

	pAni->AddClip("BugIdleDown", AT_ATLAS, AO_LOOP,
		1.f, 4, 5, 0, 0, 2, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugIdleDown", 255, 255, 255);

	pAni->AddClip("BugWalkLeft", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 5, 0, 3, 4, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugWalkLeft", 255, 255, 255);

	pAni->AddClip("BugWalkRight", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 5, 0, 1, 4, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugWalkRight", 255, 255, 255);

	pAni->AddClip("BugWalkUp", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 5, 0, 2, 4, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugWalkUp", 255, 255, 255);

	pAni->AddClip("BugWalkDown", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 5, 0, 0, 4, 1, 1.f,0, "bug", TEXT("Characters\\Monsters\\bug.bmp"));
	pAni->SetClipColorKey("BugWalkDown", 255, 255, 255);

	SAFE_RELEASE(pAni);

	if (!FightObj::Init())
		return false;

	return true;
}

int Bug::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

	POSITION tPos = pPlayer->GetPos();

	SAFE_RELEASE(pPlayer);

	POSITION tDist = tPos - m_tPos;

	float fDist = tDist.Length();

	if (fDist <= m_fDist && !m_bHitted)
	{
		tDist.Normalize();

		SetAngle(tDist);

		MoveAngle(fDeltaTime);

		float fAngle = tDist.AngleAxis();

		if (tDist.x > 0.f && fAngle < 45.f)
		{
			m_pAnimation->SetCurrentClip("BugWalkRight");
		}

		else if (fAngle > 135.f && tDist.x <0.f)
		{
			m_pAnimation->SetCurrentClip("BugWalkLeft");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y >0.f)
		{
			m_pAnimation->SetCurrentClip("BugWalkDown");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y <0.f)
		{
			m_pAnimation->SetCurrentClip("BugWalkUp");
		}
	}
	else
	{
		POSITION tAngle = GetAngle();

		float fAngle = tAngle.AngleAxis();

		if (tDist.x > 0.f && fAngle < 45.f)
		{
			m_pAnimation->SetCurrentClip("BugIdleRight");
		}

		else if (fAngle > 135.f && tDist.x < 0.f)
		{
			m_pAnimation->SetCurrentClip("BugIdleLeft");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y >0.f)
		{
			m_pAnimation->SetCurrentClip("BugIdleDown");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y < 0.f)
		{
			m_pAnimation->SetCurrentClip("BugIdleUp");
		}
	}

	return 0;
}

int Bug::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Bug::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Bug::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
/*
#ifdef _DEBUG	
	float fAngle = GetAngle().AngleAxis();

	TCHAR strAngle[32] = {};
	swprintf_s(strAngle, TEXT("Angle: %.2f"), fAngle);
	TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y, strAngle, lstrlen(strAngle));
#endif*/
}

Bug* Bug::Clone()
{
	return new Bug(*this);
}

void Bug::Collision(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Bug::DieMotion()
{
	Die();

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 2; ++i)
		{
			Effect* pEffect = (Effect*)CreateCloneObj("BugEffect", "BugEffect", m_pLayer);

			POSITION tDir(i - 0.5f, j - 0.5f);
			pEffect->SetAngle(tDir);
			pEffect->SetPos(m_tPos);
			pEffect->SetImageOffset(i * 16.f, 16.f * j + 128.f);

			SAFE_RELEASE(pEffect);
		}
	}
}
