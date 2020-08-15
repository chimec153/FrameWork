#include "Bullet.h"
#include "../Resources/Texture.h"
#include "Player.h"
#include "../Core/Camera.h"
#include "../Collider/ColliderSphere.h"

Bullet::Bullet()	:
	m_fDist(0.f),
	m_fLimitDist(500.f)
{
}

Bullet::Bullet(const Bullet & bullet) :
	MoveObj(bullet)
{
	m_fLimitDist = bullet.m_fLimitDist;
	m_fDist = bullet.m_fDist;
}

Bullet::~Bullet()
{
}

bool Bullet::Init()
{
	Obj::SetCollide(true);
	SetSpeed(500.f);
	SetPivot(0.5f,0.5f);
	SetTexture("PlayerBullet", L"playerbullet.bmp");

	SetColorKey(0, 91, 127);
	ColliderSphere* pSphere = AddCollider<ColliderSphere>("BulletBody");
	pSphere->SetSphere(POSITION(0.f, 0.f), 8.5f);

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
	//Ellipse(hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);
}

Bullet * Bullet::Clone()
{
	return new Bullet(*this);
}

void Bullet::Hit(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
	if(GetTag() == "PlayerBullet" && pDest->GetTag() == "MinionBody")
		Die();

	else if(GetTag() == "MinionBullet" && pDest->GetTag() == "PlayerBody")
		Die();
}
