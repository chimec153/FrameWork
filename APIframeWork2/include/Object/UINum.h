#pragma once
#include "UI.h"
class UINum :
	public UI
{
	friend class Scene;

protected:
	UINum();
	UINum(const UINum& num);
	virtual ~UINum();

private:
	int							m_iNum;		//	UINum은 숫자를 가진다. 0부터 9까지.
	vector<class UINum*>		m_vecNum;
	float						m_fDist;
	float						m_fLimitDist;
	float						m_fSpeed;

public:
	void CreateNum(int iNum);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual UINum* Clone();

private:
	void SetNum(int iNum);
};

