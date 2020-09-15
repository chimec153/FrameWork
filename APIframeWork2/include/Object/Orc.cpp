#include "Orc.h"
#include "../Animation/Animation.h"
#include "../Collider/ColliderRect.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Scene/MiniGameScene.h"
#include "Item.h"
#include "PrairieKing.h"
#include "../Core.h"
#include "../Core/Camera.h"

Orc::Orc()
{
	m_bTileEffect = true;
	m_eMonsterType = MMT_ORC;
}

Orc::Orc(const Orc& orc)	:
	MiniGameMonster(orc)
{
}

Orc::~Orc()
{
	if(m_pScene)
	((MiniGameScene*)m_pScene)->AddMonsterCount(-1);
}

bool Orc::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	SetSpeed(70.f);
	m_iHP = 1;

	Animation* pAni = CreateAnimation("OrcAni");

	pAni->LoadFromPath("orc.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int Orc::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);

	Obj* pKing = ((MiniGameScene*)m_pScene)->GetKing();

	bool bStealth = ((PrairieKing*)pKing)->IsState(KS_STEALTH);

	POSITION tAngle = pKing->GetPos() - m_tPos;

	if (bStealth)
	{
		if (GetSpeed() != 0.f)
		{
			Obj* pEmote = CreateCloneObj("Effect", "emote", m_pLayer);

			pEmote->SetAnimationCurrentClip("QuestionEmote");

			pEmote->SetPos(m_tPos.x, m_tPos.y - 32.f);

			SAFE_RELEASE(pEmote);
		}

		tAngle = 0.f;
		SetSpeed(0.f);
	}
	else
		SetSpeed(70.f);

	bool bZombie = ((PrairieKing*)pKing)->IsState(KS_ZOMBIE);

	if (bZombie)
		tAngle *= -1.f;

	SetAngle(tAngle);
	
		if (MoveAngle(fDeltaTime))
			m_tPrevMove = m_tMoveDir * m_fSpeed * fDeltaTime;

	SAFE_RELEASE(pKing);

	return 0;
}

int Orc::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);

	return 0;
}

void Orc::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void Orc::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);

#ifdef _DEBUG

	POSITION tCamPos = GET_SINGLE(Camera)->GetPos();

	RECT tRC = {(LONG)(m_tPos.x - 2 - tCamPos.x) , (LONG)(m_tPos.y - 2 - tCamPos.y), (LONG)(m_tPos.x + 2 - tCamPos.x),(LONG)(m_tPos.y + 2 - tCamPos.y) };

	FrameRect(hDC, &tRC, BLACKBRUSH);

	auto iter = m_ColliderList.begin();

	POSITION tHitPoint = (*iter)->GetHitPoint();

	tRC = { (LONG)(tHitPoint.x - 2 - tCamPos.x) , (LONG)(tHitPoint.y - 2 - tCamPos.y), (LONG)(tHitPoint.x + 2 -tCamPos.x),(LONG)(tHitPoint.y + 2- tCamPos.y) };

	FrameRect(hDC, &tRC, REDBRUSH);
#endif
}

Orc* Orc::Clone()
{
	return new Orc(*this);
}

void Orc::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orc::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orc::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void Orc::CreateItem()
{
}

void Orc::SetToPrevPos()
{
	SetPos(GetPos() - m_tPrevMove);

	m_tPrevMove = 0.f;
}
