#pragma once
#include "UI.h"
class UIClockHand :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIClockHand();
	UIClockHand(const UIClockHand& clockhand);
	~UIClockHand();

private:
	float m_fTime;
	POSITION		m_tOriginPos;

public:
	void AddTime(float fTime)
	{
		m_fTime += fTime * 120.f;	//	24 * 60 = 1440

		if (m_fTime >= 1440.f)
			m_fTime = 0.f;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIClockHand* Clone();
};

