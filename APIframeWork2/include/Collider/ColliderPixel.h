#pragma once
#include "Collider.h"


class ColliderPixel :
	public Collider
{
protected:
	friend class Obj;

protected:
	ColliderPixel();
	ColliderPixel(const ColliderPixel & coll);
	~ColliderPixel();

private:
	vector<PIXEL>	m_vecPixel;
	string	m_strFileName;
	string	m_strPathKey;
	int		m_iWidth;
	int		m_iHeight;

public:
	const vector<PIXEL>& GetPixel() const
	{
		return m_vecPixel;
	}

	int  GetWidth() const
	{
		return m_iWidth;
	}

	int  GetHeight() const
	{
		return m_iHeight;
	}

public:
	bool SetPixelInfo(char* pFileName, const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(Collider* pDest);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual ColliderPixel* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};
