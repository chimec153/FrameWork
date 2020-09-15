#include "Rock.h"
#include "Etc.h"
#include "../Scene/Scene.h"
#include "../Collider/ColliderRect.h"
#include "Tile.h"
#include "../Scene/Scene.h"
#include "Stage.h"
#include "../Sound/SoundManager.h"

Rock::Rock()	:
	m_eRockType(ROCK_NONE),
	m_iPicked(0),
	m_fGemsDropRate(100.f)
{
	m_eBlock = OB_ROCK;
	m_bAlphaOn = true;
	m_cAlpha = 255;
}

Rock::Rock(const Rock& rock)	:
	StaticObj(rock)
{
	m_eRockType = rock.m_eRockType;
	m_iPicked = rock.m_iPicked;
	m_fGemsDropRate = rock.m_fGemsDropRate;
}

Rock::~Rock()
{
}

void Rock::Pick()
{
	++m_iPicked;

	if (m_iPicked >= 3)
		DestroyRock();

	GET_SINGLE(SoundManager)->Play("rock");
}

bool Rock::Init()
{
	return true;
}

void Rock::Input(float fDeltaTime)
{
}

int Rock::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);

	return 0;
}

int Rock::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void Rock::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void Rock::Render(HDC hDC, float fDeltaTime)
{
	StaticObj::Render(hDC, fDeltaTime);
}

Rock* Rock::Clone()
{
	return new Rock(*this);
}

void Rock::Save(FILE* pFile)
{
	StaticObj::Save(pFile);
}

void Rock::Load(FILE* pFile)
{
	StaticObj::Load(pFile);
}

void Rock::DestroyRock()
{
	float fRate = (rand() % 10001) / 100.f;

	if (fRate <= m_fGemsDropRate)
	{
		Item* pRock = nullptr;

		switch (m_eRockType)
		{
		case ROCK_NONE:
			pRock = (Item*)CreateCloneObj("Shingle", "Shingle",  m_pLayer);
			break;
		case ROCK_EMER:
			pRock = (Item*)CreateCloneObj("Emerald", "Emerald",  m_pLayer);
			break;
		case ROCK_AQUA:
			pRock = (Item*)CreateCloneObj("Aquamarine", "Aquamarine",  m_pLayer);
			break;
		case ROCK_RUBY:
			pRock = (Item*)CreateCloneObj("Ruby", "Ruby",  m_pLayer);
			break;
		case ROCK_AMET:
			pRock = (Item*)CreateCloneObj("Amethyst", "Amethyst",  m_pLayer);
			break;
		case ROCK_TOPA:
			pRock = (Item*)CreateCloneObj("Topaz", "Topaz",  m_pLayer);
			break;
		case ROCK_JADE:
			pRock = (Item*)CreateCloneObj("Jade", "Jade",  m_pLayer);
			break;
		case ROCK_DIAM:
			pRock = (Item*)CreateCloneObj("Diamond", "Diamond", m_pLayer);
			break;
		case ROCK_PRIS:
			pRock = (Item*)CreateCloneObj("PrismaticShard", "PrismaticShard", m_pLayer);
			break;
		}

		if (pRock)
		{
			pRock->SetPos(m_tPos);

			SAFE_RELEASE(pRock);
		}
	}

	Tile* pTile = m_pScene->GetStage()->GetTile(m_tPos);

	pTile->SetTileOption(TO_DIRT);
	pTile->DeleteObj();

	Obj* pEffect = CreateCloneObj("HoeEffect", "RockEffect", m_pLayer);

	pEffect->SetAnimationCurrentClip("RockEffect");

	pEffect->SetPos(GetPos() - GetSize() * GetPivot());

	SAFE_RELEASE(pEffect);
}
