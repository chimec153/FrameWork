#include "Collider.h"
#include "../Object/Obj.h"
#include "../Math.h"
Collider::Collider()	:
	m_eCollType(COLLIDER_TYPE(0)),
	m_pObj(nullptr),
	m_tHitPoint(),
	m_bColl(false),
	m_bUI(false)
{
}

Collider::Collider(const Collider & coll)	:
	Ref(coll)
{
	*this = coll;
	m_iRef = 1;
	m_eCollType = coll.m_eCollType;
	m_pObj = coll.m_pObj;
	
	//for (int i = 0; i < CS_END; ++i)
	//{
	//	auto iter = coll.m_FuncList[i].begin();
	//	auto iterEnd = coll.m_FuncList[i].end();

	//	for (;iter != iterEnd;++iter)
	//	{
	//		function<void(Collider*, Collider*, float)>		func;

	//		func = bind(*iter, placeholders::_1, placeholders::_2,
	//			placeholders::_3);

	//		m_FuncList[i].push_back(func);
	//	}
	//}
}

Collider::~Collider()
{
	list<Collider*>::iterator iter;
	list<Collider*>::iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollisionList(this);
	}
}

bool Collider::HasSameSection(Collider* pDest)
{
	const list<int> pSectionList = *(pDest->GetSectionList());

	list<int>::const_iterator iter = pSectionList.begin();
	list<int>::const_iterator iterEnd = pSectionList.end();

	list<int>::iterator iter1 = m_SectionList.begin();
	list<int>::iterator iter1End = m_SectionList.end();

	for (; iter != iterEnd; ++iter)
	{
		for (iter1 = m_SectionList.begin(); iter1 != iter1End; ++iter1)
		{
			if ((*iter) == (*iter1))
				return true;
		}
	}

	return false;
}
bool Collider::HasCollisionSection(int iIndex)
{
	auto iter = m_SectionList.begin();
	auto iterEnd = m_SectionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == iIndex)
		{
			if (m_SectionList.size() > 1)
			{
				int a = 1;
			}

			return true;
		}

	}

	return false;
}

void Collider::Input(float fDeltaTime)
{
}

int Collider::Update(float fDeltaTime)
{
	return 0;
}

int Collider::LateUpdate(float fDeltaTime)
{
	return 0;
}

bool Collider::Collision(Collider* pDest)
{
	return false;
}

void Collider::Render(HDC hDC, float fDeltaTime)
{
}

void Collider::Save(FILE * pFile)
{
	size_t iLength = m_strTag.length();
	fwrite(&iLength, sizeof(iLength), 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);
	fwrite(&m_eCollType, 4, 1, pFile);
	/*
	for (int i = 0; i < CS_END; ++i)
	{
		size_t iSize = m_FuncList->size();
		fwrite(&iSize, sizeof(size_t), 1, pFile);

		auto iter = m_FuncList[i].begin();
		auto iterEnd = m_FuncList[i].end();

		for (; iter != iterEnd; ++iter)
		{
			function<void(Collider*, Collider*, float)>		func;

			func = bind(*iter, placeholders::_1, placeholders::_2,
				placeholders::_3);

			m_FuncList[i].push_back(func);
		}
	}*/
}

void Collider::Load(FILE * pFile)
{
	size_t iLength = 0;
	fread(&iLength, sizeof(size_t), 1, pFile);

	char strTag[256] = {};
	fread(strTag, 1, iLength, pFile);

	m_strTag = strTag;

	fread(&m_eCollType, 4, 1, pFile);
}

bool Collider::CollisionRectToRect(const RECTANGLE & src, 
	const RECTANGLE & dest)
{
	m_bColl = false;

	if (src.l > dest.r)
		return false;
	else if (src.r < dest.l)
		return false;
	else if (src.t > dest.b)
		return false;
	else if (src.b < dest.t)
		return false;

	//	충돌체들이 겹쳐서 생기는 사각형의 중심 좌표를 계산한다.
	m_tHitPoint.x = ((src.l < dest.l ? dest.l : src.l) + (src.r < dest.r ? src.r : dest.r)) / 2.f;
	m_tHitPoint.y = ((src.t < dest.t ? dest.t : src.t) + (src.b < dest.b ? src.b : dest.b)) / 2.f;

	m_bColl = true;

	return true;
}

bool Collider::CollisionRectToSphere(const RECTANGLE & src, 
	const SPHERE & dest)
{
	if ((src.l <= dest.tCenter.x && dest.tCenter.x <= src.r) ||
		(src.t <= dest.tCenter.y && dest.tCenter.y <= src.b))
	{
		RECTANGLE	tRC = src;
		tRC.l -= dest.fRadius;
		tRC.t -= dest.fRadius;
		tRC.r += dest.fRadius;
		tRC.b += dest.fRadius;

		if (dest.tCenter.x < tRC.l)
			return false;
		else if (dest.tCenter.x > tRC.r)
			return false;
		else if (dest.tCenter.y < tRC.t)
			return false;
		else if (dest.tCenter.y > tRC.b)
			return false;

		return true;
	}
	POSITION	tPos[4];
	tPos[0] = POSITION(src.l, src.t);
	tPos[1] = POSITION(src.r, src.t);
	tPos[2] = POSITION(src.l, src.b);
	tPos[3] = POSITION(src.r, src.b);

	for (int i = 0; i < 4; ++i)
	{
		float	fDist = Math::Distance(tPos[i], dest.tCenter);

		if (fDist <= dest.fRadius)
			return true;
	}

	return false;
}

bool Collider::CollisionSphereToSphere(const SPHERE & src, const SPHERE & dest)
{
	float	fDist = Math::Distance(src.tCenter,
		dest.tCenter);


	m_bColl = fDist <= src.fRadius + dest.fRadius;

	return m_bColl;
}

bool Collider::CollisionRectToPixel(const RECTANGLE & src, 
	const vector<PIXEL>& vecPixel, int iWidth, int iHeight)
{
	m_tHitPoint.x = -1;
	m_tHitPoint.y = -1;

	float iStartX, iEndX;
	float iStartY, iEndY;

	iStartX = src.l < 0 ? 0 : src.l;
	iEndX = src.r >= iWidth ? iWidth - 1 : src.r;

	iStartY = src.t < 0 ? 0 : src.t;
	iEndY = src.b >= iHeight ? iHeight - 1 : src.b;

	for (int i = (int)iStartY; i <= (int)iEndY; ++i)
	{
		for (int j = (int)iStartX; j <= (int)iStartX; ++j)
		{
			int idx = i * iWidth + j;
			const PIXEL&	pixel = vecPixel[idx];
			if (pixel.r == 0 && pixel.g == 96 && pixel.b == 0)
			{
				m_tHitPoint.x = (float)j;
				m_tHitPoint.y = (float)i;
				return true;
			}
		}
	}

	return false;
}

bool Collider::CollisionSphereToPoint(const SPHERE & src, const POSITION & dest)
{
	return Math::Distance(dest,src.tCenter)<=src.fRadius;
}

bool Collider::CollisionPixelToPoint(const vector<PIXEL>& vecPixel, 
	int iWidth, int iHeight, const POSITION & dest)
{
	if (dest.y < 0 || dest.x < 0 || dest.x >= iWidth || dest.y >= iHeight)
		return false;

	int idx = (int)dest.y * iWidth + (int)dest.x;
	const PIXEL&	pixel = vecPixel[idx];
	if (pixel.r == 0 && pixel.g == 96 && pixel.b == 0)
	{
		m_tHitPoint = dest;
		return true;
	}
	return false;
}

bool Collider::CollisionRectToPoint(const RECTANGLE & src, const POSITION & dest)
{
	if (dest.x < src.l)
		return false;
	else if (dest.x > src.r)
		return false;
	else if (dest.y < src.t)
		return false;
	else if (dest.y > src.b)
		return false;
	return true;
}
