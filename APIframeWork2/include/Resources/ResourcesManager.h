#pragma once
#include "../game.h"
class ResourcesManager
{
private:
	unordered_map<string, class Texture*> m_mapTexture;
	HINSTANCE m_hInst;
	HDC	m_hDC;
	class Texture* m_pBackBuffer;

public:
	class Texture* GetBackBuffer() const;
	bool Init(HINSTANCE hInst,HDC hDC);
	class Texture* LoadTexture(const string& strKey,
		const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(const string& strKey,
		const vector<const TCHAR*>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTexture(const string& strKey,
		const TCHAR* pFileName,
		int iCount,	const string& strPathKey = TEXTURE_PATH);
	class Texture* LoadTextureFromFullPath(const string& strKey,
		const wchar_t* pFileName);
	class Texture* LoadTexture(FILE* pFile);
	class Texture* FindTexture(const string& strKey);
	DECLARE_SINGLE(ResourcesManager)
};