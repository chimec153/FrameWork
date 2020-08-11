#include "Tool.h"

Tool::Tool()
{
	m_eType = IT_TOOL;
}

Tool::Tool(const Tool& tool)	:
	Item(tool)
{
}

Tool::~Tool()
{
}

bool Tool::Init()
{
	if (!Item::Init())
		return false;

	SetSize(32.f, 32.f);

	return true;
}

int Tool::Update(float fDeltaTime)
{
	Item::Update(fDeltaTime);
	return 0;
}

int Tool::LateUpdate(float fDeltaTime)
{
	Item::LateUpdate(fDeltaTime);
	return 0;
}

void Tool::Collision(float fDeltaTime)
{
	Item::Collision(fDeltaTime);
}

void Tool::Render(HDC hDC, float fDeltaTime)
{
	Item::Render(hDC, fDeltaTime);
}

Tool* Tool::Clone()
{
	return new Tool(*this);
}
