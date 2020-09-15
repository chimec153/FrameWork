#include "Minion.h"
#include "../Resources/Texture.h"
#include "../Core.h"
#include "../Collider/ColliderRect.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "..//Animation/Animation.h"
#include "Effect.h"
#include "UINum.h"
#include "../Core/Camera.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"

Minion::Minion() :
	m_bWake(false)
{
}

Minion::Minion(const Minion & minion)	:
	FightObj(minion)
{
	m_bWake = false;
}

Minion::~Minion()
{
}

bool Minion::Init()
{
	SetPos(800.f, 100.f);
	SetSize(32.f, 64.f);
	SetSpeed(50.f);
	SetPivot(0.5f, 0.5f);
	m_iHP = 50;
	m_tMoveDir = POSITION(0.f, 1.f);

	Animation* pAni = CreateAnimation("monsterAnimation");

	AddAnimationClip("monsterIdle", AT_ATLAS, AO_LOOP, 1.f, 4, 4,
		0, 1, 1, 1, 3.f,0, "monster", TEXT("Characters\\Monsters\\Bat.bmp"));
	SetAnimationClipColorKey("monsterIdle", 255, 255, 255);

	AddAnimationClip("monsterMove", AT_ATLAS, AO_ONCE_RETURN, 1.f, 4, 4,
		0, 0, 4, 1, 3.f,0, "monster", TEXT("Characters\\Monsters\\Bat.bmp"));
	SetAnimationClipColorKey("monsterMove", 255, 255, 255);

	SAFE_RELEASE(pAni);

	if (!FightObj::Init())
		return false;

	return true;
}


int Minion::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	if (m_tPos.y + m_tSize.y >= GETRESOLUTION.iH)
	{
		m_tPos.y = GETRESOLUTION.iH - m_tSize.y;
		m_tMoveDir *= -1.f;
	}
	else if (m_tPos.y <= 0.f)
	{
		m_tPos.y = 0.f;
		m_tMoveDir *= -1.f;
	}

	Obj* pObj = GET_SINGLE(ObjManager)->GetPlayer();

	if (pObj)
	{
		POSITION tPos = pObj->GetPos();

		SAFE_RELEASE(pObj);

		tPos -= m_tPos;

		float fDist = tPos.Length();

		if (fDist < 200.f && !m_bHitted)
		{
			m_bWake = true;
			m_pAnimation->ChangeClip("monsterMove");

			SetAngle(tPos);
			MoveAngle(fDeltaTime);

			POSITION tPos = m_tMoveDir;

			float fAngle = tPos.AngleAxis();

			fAngle += 90.f;

			tPos.x = cosf(DegToRad(fAngle));
			tPos.y = sinf(DegToRad(fAngle));

			if (m_tMoveDir.y < 0)
				tPos.x *= -1.f;

			tPos.Normalize();

			tPos *= 16.f;	// 회전 반경

			m_tTurnPivot = m_tPos + tPos;
		}

		else if (m_bWake)
		{
			m_pAnimation->ChangeClip("monsterMove");

			POSITION tPos = m_tTurnPivot - m_tPos;

			float fAngle = tPos.AngleAxis();

			fAngle += 90.f;

			tPos.x = -cosf(DegToRad(fAngle));

			if (tPos.y < 0)
				tPos.x *= -1.f;

			tPos.y = -sinf(DegToRad(fAngle));

			SetAngle(tPos);
			MoveAngle(fDeltaTime);
		}
	}

	return 0;
}

int Minion::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Minion::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Minion::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);

#ifdef _DEBUG
	/*
	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	MoveToEx(hDC, (int)(m_tPos.x - tCamPos.x), (int)(m_tPos.y - tCamPos.y), NULL);
	LineTo(hDC, (int)(m_tPos.x + m_tMoveDir.x * 32.f - tCamPos.x), (int)(m_tPos.y + m_tMoveDir.y * 32.f - tCamPos.y));

	POSITION tPos = m_tMoveDir;

	float fAngle = tPos.AngleAxis();

	fAngle += 90.f;

	tPos.x = cosf(DegToRad(fAngle));
	tPos.y = sinf(DegToRad(fAngle));

	if (m_tMoveDir.y < 0)
		tPos.x *= -1.f;

	tPos.Normalize();

	MoveToEx(hDC, (int)(m_tPos.x - tCamPos.x), (int)(m_tPos.y - tCamPos.y), NULL);
	LineTo(hDC, (int)(m_tPos.x + tPos.x * 32.f - tCamPos.x), (int)(m_tPos.y + tPos.y * 32.f - tCamPos.y));*/
#endif
}

Minion * Minion::Clone()
{
	return new Minion(*this);
}

void Minion::CollisionBullet(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
}

void Minion::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Minion::DieMotion()
{
	m_bLife = false;

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 2; ++i)
		{
			Effect* pEffect = (Effect*)CreateCloneObj("batEffect", "BatEffect", m_pLayer);

			POSITION tDir(i - 0.5f, j - 0.5f);

			pEffect->SetAngle(tDir);
			pEffect->SetPos(m_tPos);
			pEffect->SetImageOffset(i * 16.f, 16.f * j + 192.f);

			SAFE_RELEASE(pEffect);
		}
	}
}