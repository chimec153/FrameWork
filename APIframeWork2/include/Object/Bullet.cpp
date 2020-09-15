#include "Bullet.h"
#include "../Resources/Texture.h"
#include "Player.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderSphere.h"
#include "Orc.h"
#include "../Animation/Animation.h"
#include "../Scene/Scene.h"
#include "Stage.h"
#include "Tile.h"

Bullet::Bullet()	:
	m_fDist(0.f),
	m_fLimitDist(500.f),
	m_iAttack(1)
{
}

Bullet::Bullet(const Bullet & bullet) :
	MoveObj(bullet)
{
	m_fLimitDist = bullet.m_fLimitDist;
	m_fDist = bullet.m_fDist;
	m_iAttack = bullet.m_iAttack;

	Collider* pCol = GetCollider("BulletBody");

	pCol->AddCollisionFunction(CS_ENTER, this, &Bullet::Hit);
	pCol->AddCollisionFunction(CS_STAY, this, &Bullet::Hit);

	SAFE_RELEASE(pCol);
}

Bullet::~Bullet()
{
}

bool Bullet::Init()
{
	Obj::SetCollide(true);

	SetSpeed(500.f);

	SetPivot(0.5f,0.5f);

	SetSize(10.f,  10.f);
	SetTexture("Mouse");
	SetImageOffset(1024.f, 3520.f);

	ColliderSphere* pSphere = AddCollider<ColliderSphere>("BulletBody");

	pSphere->SetSphere(POSITION(0.f, 0.f), 5.f);

	SAFE_RELEASE(pSphere);

	return true;
}

int Bullet::Update(float fDeltaTime)
{
	MoveObj::Update(fDeltaTime);

	MoveAngle(fDeltaTime);

	m_fDist += GetSpeed() * fDeltaTime;

	if (m_fDist >= m_fLimitDist)
	{
		Die();
	}

	Stage* pStage = m_pScene->GetStage();

	Tile* pTile = pStage->GetTile(m_tPos);

	if (pTile)
	{
		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption == TO_NOMOVE)
			Die();
	}
		
	return 0;
}

int Bullet::LateUpdate(float fDeltaTime)
{
	MoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void Bullet::Collision(float fDeltaTime)
{
	MoveObj::Collision(fDeltaTime);
}

void Bullet::Render(HDC hDC, float fDeltaTime)
{
	MoveObj::Render(hDC, fDeltaTime);
}

Bullet * Bullet::Clone()
{
	return new Bullet(*this);
}

void Bullet::Hit(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	if (pDest->GetTag() == "EnemyBody")
	{
		Orc* pOrc = (Orc*)pDest->GetObj();

		int iHP = pOrc->GetHP();

		if (iHP <= m_iAttack)
		{
			Die();

			pSrc->SetEnable(false);
		}
	}
}
