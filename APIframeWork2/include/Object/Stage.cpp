#include "Stage.h"
#include "../Resources/Texture.h"
#include "../Core.h"
#include "../Core/Camera.h"
#include "Tile.h"
#include "../Core/PathManager.h"
#include "../Core/Input.h"
#include "..//Scene/Scene.h"
#include "Player.h"

Stage::Stage()	:
	m_iTileNumX(0),
	m_iTileNumY(0),
	m_iTileSizeX(0),
	m_iTileSizeY(0),
	m_ppBlock(nullptr),	//	타일위에 있는 오브젝트 정보를 담고 있는 블록 이중포인터 배열이다.
	m_pSelectTile(nullptr)
{
}

Stage::Stage(const Stage & stage)	:
	StaticObj(stage)
{
	m_vecTile.clear();

	for (size_t i = 0; i < stage.m_vecTile.size(); ++i)
	{
		m_vecTile.push_back(stage.m_vecTile[i]->Clone());
	}
}

Stage::~Stage()
{
	ClearTile();
}

void Stage::CreateTile(int iNumX, int iNumY, 
	int iSizeX, int iSizeY, const string & strKey, 
	const wchar_t * pFileName, const string & strPathKey)
{
	ClearTile();

	m_iTileNumX = iNumX;
	m_iTileNumY = iNumY;
	m_iTileSizeX = iSizeX;
	m_iTileSizeY = iSizeY;

	m_ppBlock = new char* [iNumY];

	for (int i = 0; i < iNumY; ++i)
	{
		m_ppBlock[i] = new char[iNumX];
		memset(m_ppBlock[i], 0, iNumX);

		for (int j = 0; j < iNumX; ++j)
		{
			Tile* pTile = Obj::CreateObj<Tile>("Tile");

			pTile->SetSize((float)iSizeX, (float)iSizeY);
			pTile->SetPos((float)j*iSizeX, (float)i*iSizeY);

			pTile->SetTexture(strKey, pFileName, strPathKey);

			m_vecTile.push_back(pTile);
		}
	}
}

bool Stage::Init()
{
	SetPos(0.f, 0.f);
	SetSize(1920.f, 1080.f);
	SetPivot(0.f, 0.f);

	SetTexture("Stage", L"stage1.bmp");
	return true;
}

void Stage::Input(float fDeltaTime)
{
	StaticObj::Input(fDeltaTime);
}

int Stage::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);

	POSITION tMousePos = GET_SINGLE(Input)->GetMouseWorldPos();

	Obj* pPlayer = m_pScene->GetPlayer();

	if (pPlayer)
	{
		POSITION tPos = pPlayer->GetPos();
		POSITION tAngle = ((MoveObj*)pPlayer)->GetAngle();

		SAFE_RELEASE(pPlayer);

		POSITION tTilePos = {};
		Tile* pTile = nullptr;

		tTilePos.x = (float)((int)(tPos.x / m_iTileSizeX) * m_iTileSizeX);
		tTilePos.y = (float)((int)(tPos.y / m_iTileSizeY) * m_iTileSizeY);

		if (tMousePos.x > tTilePos.x - m_iTileSizeX &&
			tMousePos.x < tTilePos.x + 2 * m_iTileSizeX &&
			tMousePos.y > tTilePos.y - m_iTileSizeY &&
			tMousePos.y < tTilePos.y + 2 * m_iTileSizeY)
			pTile = GetTile(tMousePos);			

		else
			pTile = GetTile(tPos + tAngle * 32.f);

		if (pTile)
		{
			m_pSelectTile = pTile;

			pTile->SelectTile();
		}
	}

	return 0;
}

int Stage::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void Stage::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void Stage::Render(HDC hDC, float fDeltaTime)
{
	//StaticObj::Render(hDC, fDeltaTime);
	if (m_pTexture)
	{
		POSITION	tPos = m_tPos - m_tSize * m_tPivot;
		POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
		BitBlt(hDC, (int)tPos.x, (int)tPos.y, (int)GETRESOLUTION.iW,
			(int)GETRESOLUTION.iH, m_pTexture->GetDC(), (int)tCamPos.x, (int)tCamPos.y,
			SRCCOPY);
	}

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Render(hDC, fDeltaTime);
	}
	/*
	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
	for (int i = 1; i < m_iTileNumY; ++i)
	{
		MoveToEx(hDC, 0.f, i*m_iTileSizeY-tCamPos.y, NULL);
		LineTo(hDC, m_iTileNumX * m_iTileSizeX, i*m_iTileSizeY - tCamPos.y);
	}
	for (int i = 1; i < m_iTileNumX; ++i)
	{
		MoveToEx(hDC, i*m_iTileSizeX - tCamPos.x, 0, NULL);
		LineTo(hDC, i*m_iTileSizeX - tCamPos.x, m_iTileNumY * m_iTileSizeY);
	}*/
}

Stage * Stage::Clone()
{
	return new Stage(*this);
}

void Stage::Save(FILE * pFile)
{
	StaticObj::Save(pFile);
	fwrite(&m_iTileNumX,	4, 1, pFile);
	fwrite(&m_iTileNumY,	4, 1, pFile);
	fwrite(&m_iTileSizeX,	4, 1, pFile);
	fwrite(&m_iTileSizeY,	4, 1, pFile);

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i]->Save(pFile);
	}

}

void Stage::Load(FILE * pFile)
{
	ClearTile();

	StaticObj::Load(pFile);
	fread(&m_iTileNumX, 4, 1, pFile);
	fread(&m_iTileNumY, 4, 1, pFile);
	fread(&m_iTileSizeX, 4, 1, pFile);
	fread(&m_iTileSizeY, 4, 1, pFile);

	m_ppBlock = new char* [m_iTileNumY];

	for (int i = 0; i < m_iTileNumY; ++i)
	{
		m_ppBlock[i] = new char[m_iTileNumX];
		memset(m_ppBlock[i], 0, m_iTileNumX);
	}

	for (int i = 0; i < m_iTileNumX*m_iTileNumY; ++i)
	{
		Tile* pTile = Obj::CreateObj<Tile>("Tile");

		pTile->Load(pFile);

		m_vecTile.push_back(pTile);
	}

	GET_SINGLE(Camera)->SetWorldResolution(m_iTileNumX * m_iTileSizeX, m_iTileNumY * m_iTileSizeY);
}

void Stage::ChangeTileImageOffset(const POSITION& tPos,
	const POSITION& tOffSet, int iLayer)
{
	int iIndex = GetTileIndex(tPos);

	if (iIndex == -1)
		return;

	if(iLayer == 0)
		m_vecTile[iIndex]->SetImageOffset(tOffSet);

	else if(iLayer == 1)
		m_vecTile[iIndex]->SetUpperImageOffset(tOffSet);
}

void Stage::ChangeTileTexture(const POSITION& tPos, Texture* pTexture, int iLayer)
{
	int iIndex = GetTileIndex(tPos);

	if (iIndex == -1)
		return;

	if(iLayer == 0)
		m_vecTile[iIndex]->SetTexture(pTexture);

	else if(iLayer == 1)
		m_vecTile[iIndex]->SetUpperTexture(pTexture);
}

void Stage::ChangeTileOption(const POSITION & tPos,
	TILE_OPTION eOption)
{
	int iIndex = GetTileIndex(tPos);

	if (iIndex == -1)
		return;
	m_vecTile[iIndex]->SetTileOption(eOption);
}

int Stage::GetTileIndex(const POSITION & tPos)	const
{
	return GetTileIndex(tPos.x, tPos.y);
}

int Stage::GetTileIndex(float x, float y)	const
{
	int idxX = (int)x / m_iTileSizeX;
	int idxY = (int)y / m_iTileSizeY;

	if (idxX < 0 || idxX >= m_iTileNumX)
		return -1;

	else if (idxY < 0 || idxY >= m_iTileNumY)
		return -1;

	return idxY * m_iTileNumX +idxX;
}

Tile* Stage::GetTile(const POSITION& tPos) const
{
	int iIndex = GetTileIndex(tPos);

	if (iIndex < 0)
		return nullptr;

	return m_vecTile[iIndex];
}

void Stage::SetBlock(int iTileIndex, OBJ_BLOCK eBlock, class Obj* obj)
{
	//m_vecTile[iTileIndex]->DeleteObj();
	m_vecTile[iTileIndex]->SetObj(obj);

	obj->SetPos(iTileIndex % m_iTileNumX * m_iTileSizeX + m_iTileSizeX / 2.f, 
		iTileIndex / m_iTileNumX * m_iTileSizeY + m_iTileSizeY / 2.f);

	m_ppBlock[iTileIndex / m_iTileNumX][iTileIndex % m_iTileNumX] = eBlock;
}

OBJ_BLOCK Stage::GetBlock(int iTileIndex)	const
{
	return OBJ_BLOCK(m_ppBlock[iTileIndex / m_iTileNumX][iTileIndex % m_iTileNumX]);
}

void Stage::ClearTile()
{
	for (int i = 0; i < m_iTileNumY; ++i)
		SAFE_DELETE_ARRAY(m_ppBlock[i]);
	SAFE_DELETE_ARRAY(m_ppBlock);

	for (int i = 0; i < m_vecTile.size(); ++i)
	{
		Obj::EraseObj(m_vecTile[i]);
	}

	Safe_Release_VecList(m_vecTile);
}
