#include "Fly.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "../Scene/Scene.h"
#include "Effect.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"

Fly::Fly()	:
	m_fDist(250.f)
{
	m_bTileEffect = false;
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

Fly::Fly(const Fly& fly)	:
	FightObj(fly)
{
	m_fDist = fly.m_fDist;
}

Fly::~Fly()
{
}

bool Fly::Init()
{
	SetSize(32.f, 48.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(30.f);
	m_iHP = 30;

	Animation* pAni = CreateAnimation("FlyAni");

	pAni->AddClip("FlyIdleLeft", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 3, 2, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyIdleLeft", 255, 255, 255);

	pAni->AddClip("FlyIdleRight", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 1, 2, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyIdleRight", 255, 255, 255);

	pAni->AddClip("FlyIdleUp", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 2, 2, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyIdleUp", 255, 255, 255);

	pAni->AddClip("FlyIdleDown", AT_ATLAS, AO_LOOP,
		1.f, 4, 6, 0, 0, 2, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyIdleDown", 255, 255, 255);

	pAni->AddClip("FlyWalkLeft", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 6, 0, 3, 4, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyWalkLeft", 255, 255, 255);

	pAni->AddClip("FlyWalkRight", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 6, 0, 1, 4, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyWalkRight", 255, 255, 255);

	pAni->AddClip("FlyWalkUp", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 6, 0, 2, 4, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyWalkUp", 255, 255, 255);

	pAni->AddClip("FlyWalkDown", AT_ATLAS, AO_ONCE_RETURN,
		0.5f, 4, 6, 0, 0, 4, 1, 1.f,0, "Fly", TEXT("Characters\\Monsters\\Fly.bmp"));
	pAni->SetClipColorKey("FlyWalkDown", 255, 255, 255);

	SAFE_RELEASE(pAni);

	if (!FightObj::Init())
		return false;

	return true;
}

int Fly::Update(float fDeltaTime)
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
			m_pAnimation->SetCurrentClip("FlyWalkRight");
		}

		else if (fAngle > 135.f && tDist.x < 0.f)
		{
			m_pAnimation->SetCurrentClip("FlyWalkLeft");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y >0.f)
		{
			m_pAnimation->SetCurrentClip("FlyWalkDown");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y < 0.f)
		{
			m_pAnimation->SetCurrentClip("FlyWalkUp");
		}
	}
	else
	{
		POSITION tAngle = GetAngle();

		float fAngle = tAngle.AngleAxis();

		if (tDist.x > 0.f && fAngle < 45.f)
		{
			m_pAnimation->SetCurrentClip("FlyIdleRight");
		}

		else if (fAngle > 135.f && tDist.x < 0.f)
		{
			m_pAnimation->SetCurrentClip("FlyIdleLeft");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y >0.f)
		{
			m_pAnimation->SetCurrentClip("FlyIdleDown");
		}

		else if (fAngle > 45.f && fAngle < 135.f && tDist.y < 0.f)
		{
			m_pAnimation->SetCurrentClip("FlyIdleUp");
		}
	}
	return 0;
}

int Fly::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Fly::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Fly::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Fly* Fly::Clone()
{
	return new Fly(*this);
}

void Fly::Collision(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Fly::DieMotion()
{
	Die();

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 2; ++i)
		{
			Effect* pEffect = (Effect*)CreateCloneObj("FlyEffect", "FlyEffect", m_pLayer);

			POSITION tDir(i - 0.5f, j - 0.5f);
			pEffect->SetAngle(tDir);
			pEffect->SetPos(m_tPos);
			pEffect->SetImageOffset(i * 16.f, 16.f * j + 240.f);

			SAFE_RELEASE(pEffect);
		}
	}
}
