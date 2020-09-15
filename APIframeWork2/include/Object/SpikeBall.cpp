#include "SpikeBall.h"
#include "../Collider/ColliderRect.h"
#include "Bullet.h"
#include "../Animation/Animation.h"
#include "PrairieKing.h"
#include "../Scene/MiniGameScene.h"

SpikeBall::SpikeBall()	:
	m_bBall(false)
{
	m_bTileEffect = true;
	m_eMonsterType = MMT_SPIKEBALL;
}

SpikeBall::SpikeBall(const SpikeBall& ball)	:
	MiniGameMonster(ball)
{
	m_bBall = ball.m_bBall;
}

SpikeBall::~SpikeBall()
{
}

bool SpikeBall::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	SetSpeed(60.f);
	m_iHP = 2;

	Animation* pAni = CreateAnimation("OrcAni");

	pAni->LoadFromPath("spikeball.sac");

	SAFE_RELEASE(pAni);

	return true;
}

int SpikeBall::Update(float fDeltaTime)
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
	else if(!m_bBall)
		SetSpeed(60.f);

	bool bZombie = ((PrairieKing*)pKing)->IsState(KS_ZOMBIE);

	if (bZombie)
		tAngle *= -1.f;

	SetAngle(tAngle);

	MoveAngle(fDeltaTime);

	SAFE_RELEASE(pKing);

	return 0;
}

int SpikeBall::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void SpikeBall::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void SpikeBall::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

SpikeBall* SpikeBall::Clone()
{
	return new SpikeBall(*this);
}

void SpikeBall::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "BulletBody")
	{
		Bullet* pBullet = (Bullet*)pDest->GetObj();

		int iAttack = pBullet->GetAttack();

		m_iHP -= iAttack;

		if (!m_bBall)
		{
			m_bBall = true;

			m_pAnimation->SetCurrentClip("Ball");

			m_iHP += 5;

			SetSpeed(0.f);
		}

		if (m_iHP <= 0)
		{
			m_pAnimation->SetCurrentClip("Die");

			SetSpeed(0.f);

			pSrc->SetEnable(false);

			CreateItem();
		}
	}

	else if (strDest == "KingBody")
	{
		PrairieKing* pKing = (PrairieKing*)pDest->GetObj();

		bool bZombie = pKing->IsState(KS_ZOMBIE);

		if (bZombie)
		{
			m_pAnimation->SetCurrentClip("Die");

			SetSpeed(0.f);

			pSrc->SetEnable(false);

			CreateItem();
		}
	}

	else if (strDest == "EnemyBody")
	{
		POSITION tHitPoint = pSrc->GetHitPoint();

		RECTANGLE tRC = ((ColliderRect*)pSrc)->GetWorldInfo();

		if (tRC.l < tHitPoint.x &&
			tRC.r > tHitPoint.x &&
			tRC.t < tHitPoint.y &&
			tRC.b > tHitPoint.y)
		{

			POSITION tDestPos = pDest->GetObj()->GetPos();

			if (tHitPoint.x < tDestPos.x)
			{
				SetAngle(-1.f, 0.f);

				MoveAngle(fDeltaTime);
			}

			else
			{
				SetAngle(1.f, 0.f);

				MoveAngle(fDeltaTime);
			}
		}
	}
}

void SpikeBall::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void SpikeBall::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void SpikeBall::CreateItem()
{
}
