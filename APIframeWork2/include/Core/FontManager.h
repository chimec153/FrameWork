#pragma once

#include "../Core/PathManager.h"

class FontManager
{
private:
	unordered_map<string, HFONT>		m_mapFont;

public:
	bool Init();
	void AddFont(const string& strName, int iHeight, int iWeight, bool bItalic, bool bUnderLine, bool bStrikeOut, const TCHAR* pFontName);
	HFONT FindFont(const string& strName);

	DECLARE_SINGLE(FontManager)
};

