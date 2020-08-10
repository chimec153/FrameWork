#include "MoveObj.h"
#include "..//Object/Stage.h"
#include "../Scene/InGameScene.h"
#include "..//Scene/SceneManager.h"
#include "../Object/Tile.h"

MoveObj::MoveObj() :
	m_fSpeed(100.f),
	m_bMove(false),
	m_bFalling(true),
	m_fForceOrigin(0.f),
	m_fForce(0.f),
	m_bTileEffect(false)
{
}

MoveObj::MoveObj(const MoveObj & obj)	:
	Obj(obj)
{
	m_tMoveDir = obj.m_tMoveDir;
	m_fSpeed = obj.m_fSpeed;
	m_fForceOrigin = 0.f;
	m_fForce = 0.f;
	m_bMove = false;
	m_bFalling = false;
	m_bTileEffect = obj.m_bTileEffect;
}

MoveObj::~MoveObj()
{
}

void MoveObj::Move(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;
	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.x -= x;
			m_tPos.y -= y;
		}
	}
}

void MoveObj::Move(float x, float y, float fDeltaTime)
{
	m_tPos.x += x* fDeltaTime;
	m_tPos.y += y* fDeltaTime;
	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.x -= x * fDeltaTime;
			m_tPos.y -= y * fDeltaTime;
		}
	}
}

void MoveObj::Move(const POSITION & tMove)
{
	m_tPos += tMove;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos -= tMove;
		}
	}
}

void MoveObj::Move(const POSITION & tMove, float fDeltaTime)
{
	m_tPos += tMove*fDeltaTime;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos -= tMove * fDeltaTime;
		}
	}
}

void MoveObj::MoveX(float x)
{
	m_tPos.x += x;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.x -= x;
		}
	}
}

void MoveObj::MoveXFromSpeed(float fDeltaTime, float fScale)
{
	m_tPos.x += m_fSpeed * fDeltaTime* fScale;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.x -= m_fSpeed * fDeltaTime * fScale;
		}
	}
}

void MoveObj::MoveX(float x, float fDeltaTime)
{
	m_tPos.x += x* fDeltaTime;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.x -= x * fDeltaTime;
		}
	}
}

void MoveObj::MoveY(float y)
{
	m_tPos.y += y;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.y -= y;
		}
	}
}

void MoveObj::MoveYFromSpeed(float fDeltaTime, float fScale)
{
	m_tPos.y += m_fSpeed * fDeltaTime* fScale;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.y -= m_fSpeed * fDeltaTime * fScale;
		}
	}
}

void MoveObj::MoveY(float y, float fDeltaTime)
{
	m_tPos.y += y * fDeltaTime;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos.y -= y * fDeltaTime;
		}
	}
}

void MoveObj::MoveAngle()
{
	m_tPos += m_tMoveDir * m_fSpeed;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos -= m_tMoveDir * m_fSpeed;
		}
	}
}

void MoveObj::MoveAngle(float fDeltaTime)
{
	m_tPos += m_tMoveDir * m_fSpeed*fDeltaTime;

	m_bMove = true;

	if (m_bTileEffect)
	{
		Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

		int iIndex = pStage->GetTileIndex(m_tPos);

		if (iIndex < 0)
			return;

		Tile* pTile = pStage->GetTile(iIndex);

		if (!pTile)
			return;

		TILE_OPTION eOption = pTile->GetTileOption();

		if (eOption >= TO_NOMOVE)
		{
			m_tPos -= m_tMoveDir * m_fSpeed * fDeltaTime;
		}
	}
}

void MoveObj::Jump()
{
	if (!m_bFalling)
	{
		m_bFalling = true;
		m_fForce = m_fForceOrigin;
	}
}

void MoveObj::JumpEnd()
{
	m_bFalling = false;
	m_fForce = 0.f;
}

bool MoveObj::Init()
{
	return true;
}

void MoveObj::Input(float fDeltaTime)
{
	Obj::Input(fDeltaTime);
}

int MoveObj::Update(float fDeltaTime)
{
	if (m_blsPhysics)
	{
		m_fGravityTime += fDeltaTime;

		m_fForce -= (GRAVITY * m_fGravityTime * m_fGravityTime);
		m_tPos.y -= m_fForce * fDeltaTime;

		if (m_bTileEffect)
		{
			Stage* pStage = ((InGameScene*)GET_SINGLE(SceneManager)->GetScene())->GetStage();

			int iIndex = pStage->GetTileIndex(m_tPos);

			if (iIndex >= 0)
			{
				Tile* pTile = pStage->GetTile(iIndex);

				if (pTile)
				{
					TILE_OPTION eOption = pTile->GetTileOption();

					if (eOption >= TO_NOMOVE)
					{
						m_tPos.y += m_fForce * fDeltaTime;
					}
				}
			}
		}
	}
	Obj::Update(fDeltaTime);
	return 0;
}

int MoveObj::LateUpdate(float fDeltaTime)
{
	Obj::LateUpdate(fDeltaTime);
	return 0;
}

void MoveObj::Collision(float fDeltaTime)
{
	Obj::Collision(fDeltaTime);
}

void MoveObj::Render(HDC hDC, float fDeltaTime)
{
	Obj::Render(hDC, fDeltaTime);

	m_bMove = false;
}
