#include "ResourcesManager.h"
#include "Texture.h"
DEFINITION_SINGLE(ResourcesManager)
ResourcesManager::ResourcesManager()	:
	m_hInst(nullptr),
	m_hDC(nullptr),
	m_pBackBuffer(nullptr)
{
}

ResourcesManager::~ResourcesManager()
{
	SAFE_RELEASE(m_pBackBuffer);
	Safe_Release_Map(m_mapTexture);
}

Texture * ResourcesManager::GetBackBuffer() const
{
	m_pBackBuffer->AddRef();
	return m_pBackBuffer;
}

bool ResourcesManager::Init(HINSTANCE hInst,HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;
	m_pBackBuffer = LoadTexture("BackBuffer", L"backbuffer.bmp");
	return true;
}

Texture * ResourcesManager::LoadTexture(
	const string & strKey, const wchar_t * pFileName, 
	const string & strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName,
		strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTexture(const string& strKey, const vector<const TCHAR*>& vecFileName, const string& strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;
		
	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, vecFileName, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return	nullptr;
	}

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTexture(const string& strKey, const TCHAR* pFileName, int iCount, const string& strPathKey)
{
	Texture*	pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTexture(m_hInst, m_hDC, strKey, pFileName, iCount, strPathKey))
	{
		SAFE_RELEASE(pTexture);
		return	nullptr;
	}

	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture* ResourcesManager::LoadTextureFromFullPath(const string& strKey, const wchar_t* pFileName)
{
	Texture* pTexture = FindTexture(strKey);

	if (pTexture)
		return pTexture;

	pTexture = new Texture;

	if (!pTexture->LoadTextureFromFullPath(m_hInst, m_hDC, strKey, pFileName))
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}
	pTexture->AddRef();
	m_mapTexture.insert(make_pair(strKey, pTexture));

	return pTexture;
}

Texture * ResourcesManager::LoadTexture(FILE * pFile)
{
	int iLength = 0;

	char	strKey[MAX_PATH] = {};
	char	strPathKey[MAX_PATH] = {};
	wchar_t strFileName[MAX_PATH] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strFileName, 2, iLength, pFile);
	strFileName[iLength] = 0;

	iLength = 0;
	fread(&iLength, 4, 1, pFile);
	fread(strPathKey, 1, iLength, pFile);
	strPathKey[iLength] = 0;

	bool	bColorKey = false;
	COLORREF	dwColorKey = 0;
	fread(&bColorKey, 1, 1, pFile);
	fread(&dwColorKey, sizeof(COLORREF), 1, pFile);

	Texture* pTex = GET_SINGLE(ResourcesManager)->LoadTexture(strKey, strFileName, strPathKey);

	if (pTex && bColorKey)
		pTex->SetColorKey(dwColorKey);

	return pTex;
}

Texture * ResourcesManager::FindTexture(const string & strKey)
{
	unordered_map<string, class Texture*>::iterator	iter = m_mapTexture.find(strKey);
	if (iter == m_mapTexture.end())
		return NULL;
	iter->second->AddRef();

	return iter->second;
}
