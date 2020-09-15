#pragma once
#include "StaticObj.h"
class Stage :
	public StaticObj
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Stage();
	Stage(const Stage& stage);
	~Stage();

private:
	vector<class Tile*>		m_vecTile;
	int						m_iTileNumX;
	int						m_iTileNumY;
	int						m_iTileSizeX;
	int						m_iTileSizeY;
	char**					m_ppBlock;
	class Tile*				m_pSelectTile;
	POSITION				m_tStart;
	STAGE_TYPE				m_eStageType;
	SEASON					m_eSeason;

public:
	class Tile* GetSelectedTile()	const
	{
		return m_pSelectTile;
	}

	void SetStart(const POSITION& tPos);

	void SetStart(float x, float y)
	{
		m_tStart = POSITION(x, y);
	}

	void SetStageType(STAGE_TYPE eType)
	{
		m_eStageType = eType;
	}

	STAGE_TYPE GetStageType()	const
	{
		return m_eStageType;
	}

	void SetSeason(SEASON eSeason)
	{
		m_eSeason = eSeason;
	}

	SEASON GetSeason()	const
	{
		return m_eSeason;
	}

public:
	void CreateTile(int iNumX, int iNumY, int iSizeX, int iSizeY,
		const string& strKey = "", const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Stage* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ChangeTileTexture(const POSITION& tPos,
		class Texture* pTexture, int iLayer = 0);
	void ChangeTileTextureAll(class Texture* pCurrentTexture, class Texture* pTexture, int iLayer = 0);
	void ChangeTileOption(const POSITION& tPos,
		TILE_OPTION eOption);
	void ChangeTileImageOffset(const POSITION& tPos,
		const POSITION& tOffSet, int iLayer = 0);
	int GetTileIndex(const POSITION& tPos)	const;
	int GetTileIndex(float x, float y)	const;
	int GetTileSizeX()	const
	{
		return m_iTileSizeX;
	}
	int GetTileSizeY()	const
	{
		return m_iTileSizeY;
	}

	int GetTileNumX()	const
	{
		return m_iTileNumX;
	}

	int GetTileNumY()	const
	{
		return m_iTileNumY;
	}

	class Tile* GetTile(int iIndex)	const
	{
		size_t iSize = m_vecTile.size();

		if (iIndex == -1 || iSize <= iIndex)
			return nullptr;

		return m_vecTile[iIndex];
	}

	class Tile* GetTile(const POSITION& tPos)	const;
	class Tile* GetTile(float x, float y)	const
	{
		int iIndex = GetTileIndex(x, y);

		return GetTile(iIndex);
	}
	
public:
	void SetBlock(OBJ_BLOCK eBlock, class Obj* obj);
	void SetBlock(int iTileIndex, OBJ_BLOCK eBlock, class Obj* obj);
	void SetBlock(const POSITION& tPos, OBJ_BLOCK eBlock, class Obj* obj);
	void SetBlock(class Tile* pTile, OBJ_BLOCK eBlock, class Obj* obj);
	OBJ_BLOCK GetBlock(int iTileIndex)	const;
	OBJ_BLOCK GetBlock()	const;

private:
	void ClearTile();
};

