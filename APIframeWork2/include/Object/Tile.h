#pragma once
#include "StaticObj.h"
class Tile :
	public StaticObj
{
private:
	friend class Obj;
	friend class Stage;

private:
	Tile();
	Tile(const Tile& tile);
	~Tile();

private:
	TILE_OPTION		m_eOption;
	class Texture*	m_pOptionTex;
	class Texture*	m_pUpperTex;
	class Obj*		m_pObj;
	POSITION		m_tUpperImageOffset;
	class Texture*	m_pSelectTex;
	bool			m_bSelect;

public:
	void SetUpperTexture(class Texture* pTexture);
	void SetUpperTexture(const string& strKey,
		const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetUpperColorKey(unsigned char r, unsigned char g, unsigned char b);

public:
	void SetTileOption(TILE_OPTION eOption);

	TILE_OPTION GetTileOption()		const
	{
		return m_eOption;
	}

	class Obj* GetObj()	const
	{
		return m_pObj;
	}

	void SetObj(class Obj* obj)
	{
		m_pObj = obj;
	}

	void DeleteObj()
	{
		if (m_pObj)
		{
			m_pObj->SetEnable(false);
			SAFE_RELEASE(m_pObj);
			m_pObj = nullptr;
		}
	}

	void SetUpperImageOffset(const _SIZE& tOffset)
	{
		m_tUpperImageOffset = tOffset;
	}

	void SetUpperImageOffset(float x, float y)
	{
		m_tUpperImageOffset.x = x;
		m_tUpperImageOffset.y = y;
	}

	void SelectTile();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Tile* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

