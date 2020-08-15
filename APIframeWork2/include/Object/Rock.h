#pragma once
#include "StaticObj.h"
class Rock :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

protected:
	Rock();
	Rock(const Rock& rock);
	virtual ~Rock();

private:
	ROCK_TYPE		m_eRockType;
	int				m_iPicked;
	float			m_fGemsDropRate;

public:
	void Pick();

	void SetRockType(ROCK_TYPE eType)
	{
		m_eRockType = eType;
	}

	void SetDropRate(float fRate)
	{
		m_fGemsDropRate = fRate;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Rock* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void DestroyRock();
};

