#include "Text.h"
#include "../Core/FontManager.h"

Text::Text()	:
	m_iLength(MAX_PATH),
	m_hFont(0),
	m_tColor(0),
	m_bBackColor(false),
	m_bUI(true)
{
	m_pText = new TCHAR[MAX_PATH];
}

Text::Text(const Text& text)	:
	UI(text)
{
	m_pText = new TCHAR[MAX_PATH];
	lstrcpy(m_pText, text.m_pText);

	m_iLength = text.m_iLength;

	m_hFont = text.m_hFont;
	m_tColor = text.m_tColor;
	m_bBackColor = text.m_bBackColor;
	m_bUI = text.m_bUI;
}

Text::~Text()
{
	delete[] m_pText;
}

void Text::SetText(const TCHAR* pText)
{
	if (m_iLength < lstrlen(pText))
	{
		delete[] m_pText;

		m_pText = new TCHAR[lstrlen(pText)];
	}

	lstrcpy(m_pText, pText);
}

void Text::SetFont(const string& strName)
{
	m_hFont = GET_SINGLE(FontManager)->FindFont(strName);
}

bool Text::Init()
{
//	m_vecSize.push_back(POSITION(21.f, 24.f));
//	m_vecImageOffset.push_back(POSITION(401.f, 30.f));
//
//	m_vecSize.push_back(POSITION(18.f, 24.f));
//	m_vecImageOffset.push_back(POSITION(293.f, 63.f));
//
//	m_vecSize.push_back(POSITION(20.f, 24.f));
//	m_vecImageOffset.push_back(POSITION(367.f, 37.f));
//
//	m_vecSize.push_back(POSITION(19.f, 24.f));	//	D
//	m_vecImageOffset.push_back(POSITION(367.f, 37.f));

	SetFont("NormalFont");

	return true;
}

void Text::Input(float fDeltaTime)
{
}

int Text::Update(float fDeltaTime)
{
	return 0;
}

int Text::LateUpdate(float fDeltaTime)
{
	return 0;
}

void Text::Collision(float fDeltaTime)
{
}

void Text::Render(HDC hDC, float fDeltaTime)
{
	HFONT hPrevFont = (HFONT)SelectObject(hDC, m_hFont);

	COLORREF tColor = SetTextColor(hDC, m_tColor);

	int iMode = 0;

	if(!m_bBackColor)
		SetBkMode(hDC, TRANSPARENT);

	else
		iMode = SetBkMode(hDC, OPAQUE);

	TextOut(hDC, (int)m_tPos.x, (int)m_tPos.y, m_pText, lstrlen(m_pText));

	SetTextColor(hDC, tColor);
	SelectObject(hDC, hPrevFont);
	SetBkMode(hDC, iMode);
}

Text* Text::Clone()
{
	return new Text(*this);
}
