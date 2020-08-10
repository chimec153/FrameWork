#pragma once
#include "UI.h"
class UIBar :
	public UI
{
private:
	friend class Obj;
	friend class Scene;

private:
	UIBar();
	UIBar(const UIBar& bar);
	~UIBar();

private:
	float		m_fMax;
	float		m_fMin;
	float		m_fValue;
	POSITION	m_tOriginSize;
	POSITION	m_tOriginPos;

public:
	void SetValue(float fValue)
	{
		m_fValue = fValue;

		if (m_fValue > m_fMax)
			m_fValue = m_fMax;

		if (m_fValue < m_fMin)
			m_fValue = m_fMin;

		m_tSize.y = m_tOriginSize.y * (m_fValue - m_fMin) / (m_fMax - m_fMin);

		POSITION tPos = m_tOriginPos;
		
		tPos.y += m_tOriginSize.y - m_tSize.y;

		SetPos(tPos);
	}

	void SetMax(float fMax)
	{
		m_fMax = fMax;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UIBar* Clone();
};

