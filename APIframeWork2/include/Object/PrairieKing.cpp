#include "PrairieKing.h"
#include "../Core/Input.h"
#include "Bullet.h"
#include "../Animation/Animation.h"
#include "PrairieKingLeg.h"
#include "../Collider/ColliderRect.h"
#include "Item.h"
#include "../Scene/MiniGameScene.h"
#include "Stage.h"
#include "Tile.h"
#include "FarmEffect.h"

PrairieKing::PrairieKing()	:
	m_pLeg(nullptr),
	m_eAction(PA_IDLE),
	m_fLimitTime(0.3f),
	m_fTime(0.f),
	m_eItemType(ITEM_NONE),
	m_iLife(3),
	m_iMoney(0),
	m_fZombieTime(8.5f),
	m_fStealthTime(5.f),
	m_iState(0),
	m_fReviveTime(5.f),
	m_fSmokeTime(0.f)
{
	m_bTileEffect = true;
}

PrairieKing::PrairieKing(const PrairieKing& king)	:
	MoveObj(king)
{
	m_pLeg = king.m_pLeg;
	m_eAction = king.m_eAction;
	m_fLimitTime = king.m_fLimitTime;
	m_fTime = king.m_fTime;
	m_eItemType = king.m_eItemType;
	m_iLife = king.m_iLife;
	m_iMoney = king.m_iMoney;
	m_fZombieTime = king.m_fZombieTime;
	m_fStealthTime = king.m_fStealthTime;
	m_iState = king.m_iState;
	m_fReviveTime = king.m_fReviveTime;
	m_fSmokeTime = king.m_fSmokeTime;
}

PrairieKing::~PrairieKing()
{
	SAFE_RELEASE(m_pLeg);
}

void PrairieKing::SetLeg(PrairieKingLeg* pLeg)
{
	SAFE_RELEASE(m_pLeg);

	m_pLeg = pLeg;

	if (m_pLeg) 
	{
		m_pLeg->SetKing(this);

		m_pLeg->AddRef();
	}
}

bool PrairieKing::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(200.f);

	Animation* pAni = CreateAnimation("KingAni");

	pAni->LoadFromPath("king.sac");

	m_pAnimation->SetCurrentClip("Idle");

	SAFE_RELEASE(pAni);

	ColliderRect* pRC = AddCollider<ColliderRect>("KingBody");

	pRC->SetRect(-16.f, -16.f, 16.f, 16.f);

	pRC->AddCollisionFunction(CS_ENTER, this, &PrairieKing::ColEnter);
	pRC->AddCollisionFunction(CS_STAY, this, &PrairieKing::ColEnter);

	SAFE_RELEASE(pRC);

	return true;
}

void PrairieKing::Input(float fTime)
{
	POSITION tAngle = {};

	if(m_eAction != PA_SMOKE)
		m_eAction = PA_IDLE;

	if (KEYPRESS("MoveFront"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveUp");

		else
			m_pAnimation->SetCurrentClip("ShotUp");

		SetAngle(0.f, -1.f);
		MoveAngle(fTime);
		m_eAction = PA_WALK;
	}

	if (KEYPRESS("MoveBack"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveDown");

		else
			m_pAnimation->SetCurrentClip("ShotDown");
		SetAngle(0.f, 1.f);
		MoveAngle(fTime);
		m_eAction = PA_WALK;
	}

	if (KEYPRESS("MoveLeft"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveLeft");

		else
			m_pAnimation->SetCurrentClip("ShotLeft");

		SetAngle(-1.f, 0.f);
		MoveAngle(fTime);
		m_eAction = PA_WALK;
	}

	if (KEYPRESS("MoveRight"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveRight");

		else
			m_pAnimation->SetCurrentClip("ShotRight");

		SetAngle(1.f, 0.f);
		MoveAngle(fTime);
		m_eAction = PA_WALK;
	}
	
	if (KEYDOWN("FocusLeft"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveLeft");

		else
			m_pAnimation->SetCurrentClip("ShotLeft");

		tAngle.x -= 1.f;
	}

	if (KEYDOWN("FocusRight"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveRight");

		else
			m_pAnimation->SetCurrentClip("ShotRight");

		tAngle.x += 1.f;
	}

	if (KEYDOWN("FocusUp"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveUp");

		else
			m_pAnimation->SetCurrentClip("ShotUp");

		tAngle.y -= 1.f;
	}

	if (KEYDOWN("FocusDown"))
	{
		if (m_iState & KS_ZOMBIE)
			m_pAnimation->SetCurrentClip("ZombieWalk");

		else if (m_iState & KS_REVIVE)
			m_pAnimation->SetCurrentClip("ReviveDown");

		else
			m_pAnimation->SetCurrentClip("ShotDown");

		tAngle.y += 1.f;
	}
	

	if (KEYPRESS("FocusLeft"))
	{
		if (m_eItemType == ITEM_MACHINE)
		{
			if (m_iState & KS_ZOMBIE)
				m_pAnimation->SetCurrentClip("ZombieWalk");

			else if (m_iState & KS_REVIVE)
				m_pAnimation->SetCurrentClip("ReviveLeft");

			else
				m_pAnimation->SetCurrentClip("ShotLeft");

			tAngle.x -= 1.f;
		}
	}
	
	if (KEYPRESS("FocusRight"))
	{
		if (m_eItemType == ITEM_MACHINE)
		{
			if (m_iState & KS_ZOMBIE)
				m_pAnimation->SetCurrentClip("ZombieWalk");

			else if (m_iState & KS_REVIVE)
				m_pAnimation->SetCurrentClip("ReviveRight");

			else
				m_pAnimation->SetCurrentClip("ShotRight");

			tAngle.x += 1.f;
		}
	}

	if (KEYPRESS("FocusUp"))
	{
		if (m_eItemType == ITEM_MACHINE)
		{
			if (m_iState & KS_ZOMBIE)
				m_pAnimation->SetCurrentClip("ZombieWalk");

			else if (m_iState & KS_REVIVE)
				m_pAnimation->SetCurrentClip("ReviveUp");

			else
				m_pAnimation->SetCurrentClip("ReviveUp");

			tAngle.y -= 1.f;
		}
	}

	if (KEYPRESS("FocusDown"))
	{
		if (m_eItemType == ITEM_MACHINE)
		{
			if (m_iState & KS_ZOMBIE)
				m_pAnimation->SetCurrentClip("ZombieWalk");

			else if (m_iState & KS_REVIVE)
				m_pAnimation->SetCurrentClip("ReviveDown");

			else
				m_pAnimation->SetCurrentClip("ShotDown");

			tAngle.y += 1.f;
		}
	}

	if (KEYDOWN("Fire"))
	{
		ItemUse();
	}

	if (tAngle != 0.f && m_fTime <= 0.f)
	{
		Fire(tAngle, fTime);

		m_fTime = m_fLimitTime;

		if (m_eItemType == ITEM_MACHINE)
			m_fTime = m_fLimitTime / 4.f;

		else if (m_eItemType == ITEM_SHOTGUN)
			m_fTime = m_fLimitTime * 2.f;
	}
}

int PrairieKing::Update(float fTime)
{
	MoveObj::Update(fTime);

	if (m_fTime > 0.f)
		m_fTime -= fTime;

	if (m_eItemType == ITEM_COFFEE)
		m_fSpeed = 250.f;

	else if (m_eItemType == ITEM_NUKE)
	{
		Layer* pLayer = m_pScene->FindLayer("Default");

		pLayer->Die();

		pLayer = m_pScene->CreateLayer("Default", 2);

		pLayer->AddObject(this);

		pLayer->AddObject(m_pLeg);

		m_fSmokeTime = 0.5f;

		m_eItemType = ITEM_NONE;
	}

	else if (m_eItemType == ITEM_TOMB)
	{
		m_iState |= KS_ZOMBIE;

		Layer* pLayer = m_pScene->FindLayer("Default");

		Obj* pEffect = CreateCloneObj("Effect", "Effect", pLayer);

		pEffect->SetAnimationCurrentClip("Thunder");

		pEffect->SetPos(m_tPos.x - 16.f, m_tPos.y - 48.f);

		m_pAnimation->SetCurrentClip("ZombieWalk");

		SAFE_RELEASE(pEffect)

		m_eItemType = ITEM_NONE;
	}

	else if (m_eItemType == ITEM_SMOKE)
	{
		m_eAction = PA_SMOKE;

		m_pAnimation->SetCurrentClip("Smoke");

		m_iState |= KS_STEALTH;

		m_eItemType = ITEM_NONE;
	}

	if (m_iState & KS_ZOMBIE)
	{
		m_fZombieTime -= fTime;

		SetSpeed(350.f);

		if (m_fZombieTime < 0.f)
		{
			m_iState ^= KS_ZOMBIE;

			m_fZombieTime = 5.f;

			SetSpeed(250.f);
		}			
	}

	else if (m_iState & KS_STEALTH)
	{
		m_fStealthTime -= fTime;

		if (m_fStealthTime < 0.f)
		{
			m_iState ^= KS_STEALTH;

			m_fStealthTime = 5.f;
		}
	}

	else if (m_iState & KS_REVIVE)
	{
		m_fReviveTime -= fTime;

		if (m_fReviveTime < 0.f)
		{
			m_iState ^= KS_REVIVE;
		}
	}

	if (m_eAction == PA_SMOKE)
	{
		if (m_pAnimation->GetMotionEnd())
		{
			m_eAction = PA_IDLE;

			TILE_OPTION eOption = TO_NONE;

			POSITION tPos = {};

			do
			{
				int iRandX = rand() % 15 +1;
				int iRandY = rand() % 15+1;

				Stage* pStage = m_pScene->GetStage();

				int iSizeX = pStage->GetTileSizeX();
				int iSizeY = pStage->GetTileSizeY();

				tPos = POSITION((float)(iRandX * iSizeX), (float)(iRandY * iSizeY));

				Tile* pTile = pStage->GetTile(tPos);

				TILE_OPTION eOption = pTile->GetTileOption();
			}
			while (eOption == TO_NOMOVE);

			SetPos(tPos);
		}
	}

	if (m_fSmokeTime > 0.f)
	{
		m_fSmokeTime -= m_fTime;

		Layer* pLayer = m_pScene->FindLayer("Default");

		int iRandX = rand() % 16;
		int iRandY = rand() % 16;

		FarmEffect* pEffect = (FarmEffect*)Obj::CreateCloneObj("Effect", "effect", pLayer);

		pEffect->SetAnimationCurrentClip("Smoke");

		pEffect->SetPos(iRandX * 32.f, iRandY * 32.f);

		SAFE_RELEASE(pEffect);
	}

	return 0;
}

void PrairieKing::PostUpdate(float fTime)
{
	MoveObj::LateUpdate(fTime);
}

void PrairieKing::Collision(float fTime)
{
	MoveObj::Collision(fTime);
}

void PrairieKing::Render(HDC hDC, float fTime)
{
	MoveObj::Render(hDC, fTime);
}

PrairieKing* PrairieKing::Clone()
{
	return new PrairieKing(*this);
}

void PrairieKing::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	string strDest = pDest->GetTag();

	if (strDest == "EnemyBody")
	{
		if (!m_iState || (m_iState & KS_STEALTH))
		{
			--m_iLife;

			SetPos(272.f, 272.f);

			m_fReviveTime = 5.f;

			m_iState |= KS_REVIVE;

			m_pAnimation->SetCurrentClip("ReviveDown");
		}
	}

	else if (strDest == "ItemBody")
	{
		Obj* pItem = pDest->GetObj();

		MINI_ITEM_TYPE eItemType = (MINI_ITEM_TYPE)((Item*)pItem)->GetType();

		if (eItemType == ITEM_MONEY)
			++m_iMoney;

		else if (eItemType == ITEM_5MONEY)
			m_iMoney += 5;

		else if (eItemType == ITEM_LIFE)
			++m_iLife;

		else
		{
			ItemUse();

			((MiniGameScene*)m_pScene)->SetItemType(eItemType);
		}

		pItem->Die();

		pDest->Die();
	}
}

void PrairieKing::Fire(const POSITION& tAngle, float fTime)
{
	if (m_eItemType == ITEM_SHOTGUN)
	{
		for (int i = 0; i < 3; ++i)
		{
			Bullet* pBullet = (Bullet*)CreateCloneObj("Bullet", "PlayerBullet", m_pLayer);

			float fAngle = tAngle.AngleAxis();

			if (tAngle.y > 0.f)
				fAngle *= -1.f;

			fAngle += 15.f * (i-1);

			pBullet->SetAngle(cosf(DegToRad(fAngle)), -sinf(DegToRad(fAngle)));

			pBullet->SetPos(m_tPos + tAngle * pBullet->GetSize() * pBullet->GetPivot());

			SAFE_RELEASE(pBullet);
		}
	}

	else if (m_eItemType == ITEM_WHEEL || m_eItemType == ITEM_BADGE)
	{
		for (int i = 0; i < 8; ++i)
		{
			Bullet* pBullet = (Bullet*)CreateCloneObj("Bullet", "PlayerBullet", m_pLayer);

			float fAngle = tAngle.AngleAxis();

			fAngle += 45.f * i;

			pBullet->SetAngle(cosf(DegToRad(fAngle)), sinf(DegToRad(fAngle)));

			pBullet->SetPos(m_tPos + tAngle * pBullet->GetSize() * pBullet->GetPivot());

			SAFE_RELEASE(pBullet);
		}
	}

	else
	{
		Bullet* pBullet = (Bullet*)CreateCloneObj("Bullet", "PlayerBullet", m_pLayer);

		pBullet->SetAngle(tAngle);

		pBullet->SetPos(m_tPos + tAngle * pBullet->GetSize() * pBullet->GetPivot());

		SAFE_RELEASE(pBullet);
	}
}

void PrairieKing::ItemUse()
{
	MINI_ITEM_TYPE eItemType = ((MiniGameScene*)m_pScene)->GetItemType();

	if (eItemType != ITEM_NONE)
		m_eItemType = eItemType;

	((MiniGameScene*)m_pScene)->SetItemType(ITEM_NONE);
}
