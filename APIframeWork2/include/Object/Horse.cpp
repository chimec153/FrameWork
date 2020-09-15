#include "Horse.h"
#include "../Animation/Animation.h"
#include "../Core/Input.h"
#include "Horse.h"
#include "../Collider/ColliderRect.h"
#include "Player.h"

Horse::Horse()	:
	m_pPlayer(nullptr),
	m_eOption(HORSE_IDLE)
{
	m_eAnimalType = ANIMAL_HORSE;
	m_cAlpha = 255;
	m_bAlphaOn = true;
}

Horse::Horse(const Horse& horse)	:
	Animal(horse)
{
	m_pPlayer = horse.m_pPlayer;
	m_eOption = horse.m_eOption;
}

Horse::~Horse()
{
	SAFE_RELEASE(m_pPlayer);
}

void Horse::SetPlayer(Player* pPlayer)
{
	SAFE_RELEASE(m_pPlayer);

	m_pPlayer = pPlayer;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

bool Horse::Init()
{
	SetSize(64.f, 64.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(300.f);
	m_bTileEffect = true;

	Animation* pAni = CreateAnimation("HorseAni");

	pAni->LoadFromPath("horse.sac");

	m_pAnimation->SetCurrentClip("HorseIdleDown");

	SAFE_RELEASE(pAni);

	ColliderRect* pRC = AddCollider<ColliderRect>("HorseBody");

	pRC->SetRect(-32.f, -32.f, 32.f, 32.f);

	pRC->AddCollisionFunction(CS_ENTER, this, &Horse::ColEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &Horse::ColStay);

	SAFE_RELEASE(pRC)

	return true;
}

void Horse::Input(float fDeltaTime)
{
	SetAngle(0.f, 0.f);

	if (m_pPlayer)
	{
		if (KEYPRESS("MoveFront"))
		{
			SetAngle(0.f, -1.f);
			MoveAngle(fDeltaTime);

			m_pAnimation->SetCurrentClip("HorseWalkUp");
			m_pAnimation->SetDefaultClip("HorseIdleUp");

			m_eOption = HORSE_WALK;
		}

		else if (KEYPRESS("MoveBack"))
		{
			SetAngle(0.f, 1.f);
			MoveAngle(fDeltaTime);

			m_pAnimation->SetCurrentClip("HorseWalkDown");
			m_pAnimation->SetDefaultClip("HorseIdleDown");

			m_eOption = HORSE_WALK;
		}

		else if (KEYPRESS("MoveLeft"))
		{
			SetAngle(-1.f, 0.f);
			MoveAngle(fDeltaTime);

			m_pAnimation->SetCurrentClip("HorseWalkLeft");
			m_pAnimation->SetDefaultClip("HorseIdleLeft");

			m_eOption = HORSE_WALK;
		}

		else if (KEYPRESS("MoveRight"))
		{
			SetAngle(1.f, 0.f);
			MoveAngle(fDeltaTime);

			m_pAnimation->SetCurrentClip("HorseWalkRight");
			m_pAnimation->SetDefaultClip("HorseIdleRight");

			m_eOption = HORSE_WALK;
		}
	}
}

int Horse::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);

	if (m_pPlayer)
	{
		POSITION tPos = m_tPos;

		tPos.y += 0.3f * m_tSize.y;

		m_pPlayer->SetPos(tPos);
	}

	return 0;
}

int Horse::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);

	if (m_eOption == HORSE_WALK && m_tMoveDir == 0.f)
	{
		m_eOption = HORSE_IDLE;

		m_pAnimation->ReturnClip();
	}

	return 0;
}

void Horse::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Horse::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Horse* Horse::Clone()
{
	return new Horse(*this);
}

void Horse::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
}

void Horse::ColStay(Collider* pSrc, Collider* pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		if (KEYDOWN("Key_C"))
		{
			if (!m_pPlayer)
			{
				Obj* pObj = pDest->GetObj();

				SetPlayer((Player*)pObj);

				((Player*)pObj)->SetPlayerAction(PA_RIDE);
			}

			else
			{
				m_pPlayer->SetPos(m_tPos + m_tSize * m_tPivot);

				m_pPlayer->SetPlayerAction(PA_IDLE);

				SAFE_RELEASE(m_pPlayer);
			}
		}
	}
}
