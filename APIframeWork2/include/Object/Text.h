#pragma once
#include "UI.h"
class Text :
	public UI
{
	friend class Obj;
	friend class Scene;

private:
	Text();
	Text(const Text& text);
	virtual ~Text();

private:
	TCHAR*			m_pText;
	int				m_iLength;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Text* Clone();
};

