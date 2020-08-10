#include "StaticObj.h"

StaticObj::StaticObj()
{
}

StaticObj::StaticObj(const StaticObj & obj)	:
	Obj(obj)
{

}

StaticObj::~StaticObj()
{
}

void StaticObj::Input(float fDeltaTime)
{
	Obj::Input(fDeltaTime);
}

int StaticObj::Update(float fDeltaTime)
{
	Obj::Update(fDeltaTime);
	return 0;
}

int StaticObj::LateUpdate(float fDeltaTime)
{
	Obj::LateUpdate(fDeltaTime);
	return 0;
}

void StaticObj::Collision(float fDeltaTime)
{
	Obj::Collision(fDeltaTime);
}

void StaticObj::Render(HDC hDC, float fDeltaTime)
{
	Obj::Render(hDC, fDeltaTime);
}

void StaticObj::Save(FILE * pFile)
{
	Obj::Save(pFile);
}
void StaticObj::Load(FILE * pFile)
{
	Obj::Load(pFile);
}
