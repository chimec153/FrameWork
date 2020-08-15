#pragma once
#include "MoveObj.h"
class PadeEffect :
	public MoveObj
{
private:
	friend class Obj;
	friend class Scene;

private:
	PadeEffect();
	PadeEffect(const PadeEffect& padeeffect);
	~PadeEffect();

private:
	bool		m_bPadeDir;
	float		m_fPadeRate;

public:
	void PadeOut()
	{
		m_bPadeDir = true;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual PadeEffect* Clone();
};

