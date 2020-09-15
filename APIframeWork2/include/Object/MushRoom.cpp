#include "MushRoom.h"

MushRoom::MushRoom()
{
	m_bTileEffect = true;
	m_eMonsterType = MMT_MUSHROOM;
}

MushRoom::MushRoom(const MushRoom& mushroom)	:
	MiniGameMonster(mushroom)
{
}

MushRoom::~MushRoom()
{
}

bool MushRoom::Init()
{
	if (!MiniGameMonster::Init())
		return false;

	return true;
}

int MushRoom::Update(float fDeltaTime)
{
	MiniGameMonster::Update(fDeltaTime);
	return 0;
}

int MushRoom::LateUpdate(float fDeltaTime)
{
	MiniGameMonster::LateUpdate(fDeltaTime);
	return 0;
}

void MushRoom::Collision(float fDeltaTime)
{
	MiniGameMonster::Collision(fDeltaTime);
}

void MushRoom::Render(HDC hDC, float fDeltaTime)
{
	MiniGameMonster::Render(hDC, fDeltaTime);
}

MushRoom* MushRoom::Clone()
{
	return new MushRoom(*this);
}

void MushRoom::CollisionBullet(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void MushRoom::CollStay(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void MushRoom::CollLeave(Collider* pSrc, Collider* pDest, float fDeltaTime)
{
}

void MushRoom::CreateItem()
{
}
