#include "ColliderPixel.h"
#include "../Object/Obj.h"
#include "ColliderSphere.h"
#include "ColliderRect.h"
#include "ColliderPoint.h"
#include "../Core/PathManager.h"

ColliderPixel::ColliderPixel()
{
	m_eCollType = CT_PIXEL;
	m_iWidth = 0;
	m_iHeight = 0;
}

ColliderPixel::ColliderPixel(const ColliderPixel & coll)	:
	Collider(coll)
{
	m_iWidth = coll.m_iWidth;
	m_iHeight = coll.m_iHeight;
}

ColliderPixel::~ColliderPixel()
{
}

bool ColliderPixel::SetPixelInfo(char * pFileName, 
	const string & strPathKey)
{
	m_strFileName = pFileName;
	m_strPathKey = strPathKey;

	const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(strPathKey);
	string strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	FILE*	pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
		return false;

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_iWidth = ih.biWidth;
	m_iHeight = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_iWidth * m_iHeight);

	fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);

	int iSize = sizeof(PIXEL);

	PPIXEL	pPixelArr = new PIXEL[m_iWidth];

	for (int i = 0; i < m_iHeight / 2; ++i)
	{
		memcpy(pPixelArr, &m_vecPixel[i*m_iWidth], sizeof(PIXEL)*m_iWidth);
		memcpy(&m_vecPixel[i*m_iWidth], &m_vecPixel[(m_iHeight - i - 1)*m_iWidth],
			sizeof(PIXEL)*m_iWidth);
		memcpy(&m_vecPixel[(m_iHeight - i - 1)*m_iWidth], pPixelArr,
			sizeof(PIXEL)*m_iWidth);
	}

	delete[]	pPixelArr;

	fclose(pFile);

	return true;

	/*fopen_s(&pFile, "test.bmp", "wb");

	fwrite(&fh, sizeof(fh), 1, pFile);
	fwrite(&ih, sizeof(ih), 1, pFile);
	fwrite(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);

	fclose(pFile);*/

}

bool ColliderPixel::Init()
{
	return true;
}

void ColliderPixel::Input(float fDeltaTime)
{
}

int ColliderPixel::Update(float fDeltaTime)
{
	return 0;
}

int ColliderPixel::LateUpdate(float fDeltaTime)
{
	return 0;
}

bool ColliderPixel::Collision(Collider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPixel(((ColliderRect*)pDest)->GetWorldInfo(),
			m_vecPixel, m_iWidth, m_iHeight);
	case CT_POINT:
		return CollisionPixelToPoint(m_vecPixel, m_iWidth, m_iHeight,
			((ColliderPoint*)pDest)->GetPoint());
	}

	return false;
}

void ColliderPixel::Render(HDC hDC, float fDeltaTime)
{
}

ColliderPixel * ColliderPixel::Clone()
{
	return new ColliderPixel(*this);
}

void ColliderPixel::Save(FILE * pFile)
{
	Collider::Save(pFile);

	size_t iLength = m_strFileName.length();

	fwrite(&iLength, sizeof(size_t), 1, pFile);

	fwrite(m_strFileName.c_str(), 1, iLength, pFile); 
	
	iLength = m_strPathKey.length();

	fwrite(&iLength, sizeof(size_t), 1, pFile);

	fwrite(m_strPathKey.c_str(), 1, iLength, pFile);
}

void ColliderPixel::Load(FILE * pFile)
{
	Collider::Load(pFile);
}
