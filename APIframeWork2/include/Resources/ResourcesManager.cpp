#include "ResourcesManager.h"
#include "Texture.h"
#include "../Core/PathManager.h"

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
	Safe_Delete_VecList(m_vecItemInfo);
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

	LoadItemInfo(TEXT("SeedData.txt"));

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

bool ResourcesManager::LoadItemInfo(const TCHAR* pFileName, const string& strPathKey)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pDataPath = GET_SINGLE(PathManager)->FindPath(strPathKey);

	if (pDataPath)
		lstrcat(strFullPath, pDataPath);

	lstrcat(strFullPath, pFileName);

	char pMultibytePath[MAX_PATH] = {};

#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, NULL, strFullPath, -1, pMultibytePath, lstrlen(strFullPath), NULL, NULL);
#else
	strcpy_s(pMultibytePath, strFullPath);
#endif

	FILE* pFile = nullptr;

	fopen_s(&pFile, pMultibytePath, "rt");

	if (pFile)
	{
		char cLine[256] = {};

		fgets(cLine, 256, pFile);

		int iCount = atoi(cLine);

		for (int i = 0; i < iCount; ++i)
		{
			fgets(cLine, 256, pFile);

			PITEMINFO pInfo = new ITEMINFO;

			fgets(cLine, 256, pFile);

			size_t iSize = strlen(cLine);

			for (size_t i = 0; i < iSize; ++i)
			{
				if (cLine[i] == '\t')
				{
					cLine[i] = 0;
					break;
				}
			}

			pInfo->strName = cLine;

			fgets(cLine, 256, pFile);

			int iPriceCount = atoi(cLine);

			for (int i = 0; i < iPriceCount; ++i)
			{
				fgets(cLine, 256, pFile);

				pInfo->vecPrice.push_back(atoi(cLine));
			}

			m_vecItemInfo.push_back(pInfo);
		}

		fclose(pFile);
	}

	return true;
}

PITEMINFO ResourcesManager::FindItemInfo(int iIndex)
{
	size_t iSize = m_vecItemInfo.size();

	if (iSize <= iIndex)
		return	nullptr;

	return m_vecItemInfo[iIndex];
}
