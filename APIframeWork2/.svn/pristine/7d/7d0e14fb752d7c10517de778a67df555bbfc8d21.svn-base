#include "Portal.h"

Portal::Portal()
{
}

Portal::Portal(const Portal& portal)	:
	StaticObj(portal)
{
}

Portal::~Portal()
{
}

void Portal::SetCallback(void(*pFunc)(Collider*, Collider*, float))
{
	m_Callback = bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

bool Portal::Init()
{
	return true;
}

void Portal::Input(float fDeltaTime)
{

}

int Portal::Update(float fDeltaTime)
{
	StaticObj::Update(fDeltaTime);
	return 0;
}

int Portal::LateUpdate(float fDeltaTime)
{
	StaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void Portal::Collision(float fDeltaTime)
{
	StaticObj::Collision(fDeltaTime);
}

void Portal::Render(HDC hDC, float fDeltaTime)
{
	StaticObj::Render(hDC, fDeltaTime);
}

Portal* Portal::Clone()
{
	return new Portal(*this);
}

void Portal::Save(FILE* pFile)
{
	StaticObj::Save(pFile);
}

void Portal::Load(FILE* pFile)
{
	StaticObj::Load(pFile);
}

void Portal::ColEnter(Collider* pSrc, Collider* pDest, float fTime)
{
	m_Callback(pSrc, pDest, fTime);
}
