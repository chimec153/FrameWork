#include "Mouse.h"
#include "../Core.h"
#include "../Collider/ColliderPoint.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"

Mouse::Mouse()
{
}

Mouse::Mouse(const Mouse & mouse)	:
	UI(mouse)
{
}

Mouse::~Mouse()
{
}

bool Mouse::Init()
{
	POINT	pt;

	GetCursorPos(&pt);
	ScreenToClient(WINDOWHANDLE, &pt);

	ShowCursor(FALSE);

	m_tPos = pt;
	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
	m_tWorldPos = m_tPos + tCamPos;
	
	ColliderPoint* pColl = AddCollider<ColliderPoint>("Mouse");

	pColl->SetUI(true);

	SAFE_RELEASE(pColl);

	return true;
}

void Mouse::Input(float fDeltaTime)
{
	UI::Input(fDeltaTime);
}

int Mouse::Update(float fDeltaTime)
{
	UI::Update(fDeltaTime);

	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(WINDOWHANDLE, &pt);

	m_tMove.x = pt.x - m_tPos.x;
	m_tMove.y = pt.y - m_tPos.y;

	m_tPos = pt;

	POSITION	tCamPos = GET_SINGLE(Camera)->GetPos();
	
	m_tWorldPos = m_tPos + tCamPos;
	return 0;
}

int Mouse::LateUpdate(float fDeltaTime)
{
	UI::LateUpdate(fDeltaTime);
	return 0;
}

void Mouse::Collision(float fDeltaTime)
{
	UI::Collision(fDeltaTime);
}

void Mouse::Render(HDC hDC, float fDeltaTime)
{
	UI::Render(hDC, fDeltaTime);
#ifdef _DEBUG
	if (KEYPRESS("Debug"))
	{
		TCHAR strScene[32] = {};

		swprintf_s(strScene, TEXT("Scene: %lld"), (long long)m_pScene);

		TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y, strScene, lstrlen(strScene));

		TCHAR strPos[32] = {};

		swprintf_s(strPos, TEXT("Pos: %d, %d"), (int)m_tPos.x, (int)m_tPos.y);

		TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y-20, strPos, lstrlen(strPos));
	}
#endif
}

Mouse * Mouse::Clone()
{
	return new Mouse(*this);
}

void Mouse::Hit(Collider * pSrc, Collider * pDest, float fDeltaTime)
{
}
