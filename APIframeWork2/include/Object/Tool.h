#pragma once
#include "Item.h"
class Tool :
	public Item
{
private:
	friend class Obj;
	friend class Scene;

private:
	Tool();
	Tool(const Tool& tool);
	~Tool();

private:
	TOOL_TYPE		m_eToolType;

public:
	void SetToolType(TOOL_TYPE eType)
	{
		m_eToolType = eType;
	}

	TOOL_TYPE GetToolType()	const
	{
		return m_eToolType;
	}

public:
	virtual bool Init();
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Tool* Clone();
};

