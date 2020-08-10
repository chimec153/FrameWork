#pragma once
#include "MoveObj.h"
class Crop :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

protected:
	Crop();
	Crop(const Crop& crop);
	virtual ~Crop();

protected:
	CROP_TYPE		m_eType;
	int				m_iDay;

public:
	void SetType(CROP_TYPE eType)
	{
		m_eType = eType;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Crop* Clone();
};

