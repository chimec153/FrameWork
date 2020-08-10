#include "Slime.h"
#include "../Collider/ColliderRect.h"
#include "Effect.h"
#include "..//Animation/Animation.h"
#include "UINum.h"
#include "..//Scene/Scene.h"

Slime::Slime()
{
	m_bTileEffect = true;
}

Slime::Slime(const Slime& slime)	:
	FightObj(slime)
{
}

Slime::~Slime()
{
}

bool Slime::Init()
{
	SetSize(32.f, 64.f);
	SetPivot(0.5f, 0.5f);
	m_iHP = 20;

	Animation* pAni = CreateAnimation("slimeAni");
	pAni->AddClip("slimeIdle", AT_ATLAS, AO_LOOP,
		1.f, 4, 8, 0, 0, 4, 1, 1.f, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeIdle", 255, 255, 255);

	SAFE_RELEASE(pAni);

	ColliderRect* pRC = AddCollider<ColliderRect>("SlimeBody");

	pRC->SetRect(-16.f, -32.f, 16.f, 32.f);

	SAFE_RELEASE(pRC);

	return true;
}

int Slime::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	if (m_iHP <= 0.f)
	{
		Die();

		for (int j = 0; j < 2; ++j)
		{
			for (int i = 0; i < 2; ++i)
			{
				Effect* pEffect = (Effect*)CreateCloneObj("slimeEffect", "SlimeEffect", SC_CURRENT, m_pLayer);

				POSITION tDir(i - 0.5f, j - 0.5f);
				pEffect->SetAngle(tDir);
				pEffect->SetPos(m_tPos);
				pEffect->SetImageOffset(i * 16.f, 16.f * j + 236.f);

				SAFE_RELEASE(pEffect);
			}
		}
	}

	return 0;
}

int Slime::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Slime::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Slime::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Slime* Slime::Clone()
{
	return new Slime(*this);
}

void Slime::Collision(Collider* pSrc, Collider* pDest, float fTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "attack")
	{
		m_iHP -= 10;

		Layer* pLayer = m_pScene->FindLayer("UI");

		Obj* pNum = CreateCloneObj("NumSm", "Num", SC_CURRENT, pLayer);
		pNum->SetPos(m_tPos.x, m_tPos.y - m_tSize.y * m_tPivot.y);

		((UINum*)pNum)->CreateNum(10);

		SAFE_RELEASE(pNum);
	}
}
