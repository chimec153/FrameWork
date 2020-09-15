#include "Slime.h"
#include "../Collider/ColliderRect.h"
#include "Effect.h"
#include "..//Animation/Animation.h"
#include "UINum.h"
#include "..//Scene/Scene.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Core/Camera.h"
#include "SlimeHead.h"
#include "ObjManager.h"
#include "UIInventory.h"
#include "Tool.h"

Slime::Slime()	:
	m_pPaceTexture(nullptr),
	m_fDist(300.f),
	m_pHead(nullptr)
{
	m_bTileEffect = true;
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

Slime::Slime(const Slime& slime)	:
	FightObj(slime)
{
	m_pPaceTexture = slime.m_pPaceTexture;

	if (m_pPaceTexture)
		m_pPaceTexture->AddRef();

	m_pHead = nullptr;

	m_tPaceImageOffset = slime.m_tPaceImageOffset;
	m_fDist = slime.m_fDist;
}

Slime::~Slime()
{
	SAFE_RELEASE(m_pPaceTexture);
	SAFE_RELEASE(m_pHead);
}

void Slime::SetHead(SlimeHead* pHead)
{
	SAFE_RELEASE(m_pHead);

	m_pHead = pHead;

	if (m_pHead)
		m_pHead->AddRef();
}

bool Slime::Init()
{
	SetSize(32.f, 48.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(20.f);
	m_iHP = 20;

	Animation* pAni = CreateAnimation("slimeAni");

	pAni->AddClip("slimeIdle", AT_ATLAS, AO_LOOP,
		1.f, 4, 11, 0, 4, 4, 1, 1.f,0, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeIdle", 255, 255, 255);

	pAni->AddClip("slimeWalkLeft", AT_ATLAS, AO_ONCE_RETURN,
		1.f, 4, 11, 0, 0, 4, 1, 1.f,0, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeWalkLeft", 255, 255, 255);

	pAni->AddClip("slimeWalkRight", AT_ATLAS, AO_ONCE_RETURN,
		1.f, 4, 11, 0, 1, 4, 1, 1.f,0, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeWalkRight", 255, 255, 255);

	pAni->AddClip("slimeWalkUp", AT_ATLAS, AO_ONCE_RETURN,
		1.f, 4, 11, 0, 2, 4, 1, 1.f,0, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeWalkUp", 255, 255, 255);

	pAni->AddClip("slimeWalkDown", AT_ATLAS, AO_ONCE_RETURN,
		1.f, 4, 11, 0, 3, 4, 1, 1.f,0, "slime", TEXT("Characters\\Monsters\\Green Slime.bmp"));
	pAni->SetClipColorKey("slimeWalkDown", 255, 255, 255);

	SAFE_RELEASE(pAni);

	m_pPaceTexture = GET_SINGLE(ResourcesManager)->FindTexture("slime");
	m_pPaceTexture->SetColorKey(255, 255, 255);

	SetPaceImageOffset(POSITION(64.f,245.f));

	if (!FightObj::Init())
		return false;

	return true;
}

int Slime::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	Obj* pPlayer = GET_SINGLE(ObjManager)->GetPlayer();

	POSITION tPos = pPlayer->GetPos();

	SAFE_RELEASE(pPlayer);

	POSITION tDist = tPos - m_tPos;

	float fDist = tDist.Length();

	if (fDist <= m_fDist && !m_bHitted)
	{
		if (m_tMoveDir == 0.f)
		{
			Effect* pEmote = (Effect*)CreateCloneObj("HoeEffect", "emote", m_pLayer);

			pEmote->SetPos(m_tPos.x - m_tSize.x * m_tPivot.x, m_tPos.y - m_tSize.y * m_tPivot.y - pEmote->GetSize().y);

			pEmote->SetAnimationCurrentClip("SurpriseEmote");

			SAFE_RELEASE(pEmote);
		}

		tDist.Normalize();

		SetAngle(tDist);

		m_pHead->SetAngle(tDist);

		if (!m_pHead->GetScene())
			m_pLayer->AddObject(m_pHead);

		MoveAngle(fDeltaTime);

		if (tDist.x > 0.f)
		{
			m_pAnimation->SetCurrentClip("slimeWalkRight");
		}

		else if (tDist.x < 0.f)
		{
			m_pAnimation->SetCurrentClip("slimeWalkLeft");
		}

		else if (tDist.y > 0.f)
		{
			m_pAnimation->SetCurrentClip("slimeWalkDown");
		}

		else if (tDist.y < 0.f)
		{
			m_pAnimation->SetCurrentClip("slimeWalkUp");
		}
	}

	if (m_pHead)
		m_pHead->SetPos(GetPos());

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

	POSITION tLT = m_tPos - m_tSize * m_tPivot;
	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	tLT -= tCamPos;

	POSITION tAngle = GetAngle();

	if (tAngle.y > 0.f && fabsf(tAngle.x) < tAngle.y)
	{
		if (m_pPaceTexture)
			TransparentBlt(hDC, (int)tLT.x, (int)tLT.y, (int)m_tSize.x, (int)m_tSize.y, m_pPaceTexture->GetDC(),
			(int)m_tPaceImageOffset.x, (int)m_tPaceImageOffset.y, (int)m_tSize.x, (int)m_tSize.y, m_pPaceTexture->GetColorKey());
	}
}

Slime* Slime::Clone()
{
	return new Slime(*this);
}

void Slime::DieMotion()
{
	Die();

	m_pHead->Die();

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 2; ++i)
		{
			Effect* pEffect = (Effect*)CreateCloneObj("slimeEffect", "SlimeEffect", m_pLayer);

			POSITION tDir(i - 0.5f, j - 0.5f);
			pEffect->SetAngle(tDir);
			pEffect->SetPos(m_tPos);
			pEffect->SetImageOffset(i * 16.f, 16.f * j + 240.f);

			SAFE_RELEASE(pEffect);
		}
	}
}

void Slime::Collision(Collider* pSrc, Collider* pDest, float fTime)
{
}
