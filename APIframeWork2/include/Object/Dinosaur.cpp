#include "Dinosaur.h"
#include "../Animation/Animation.h"

Dinosaur::Dinosaur()
{
}

Dinosaur::Dinosaur(const Dinosaur& dinosaur)	:
	FightObj(dinosaur)
{
}

Dinosaur::~Dinosaur()
{
}

bool Dinosaur::Init()
{
	SetSize(32.f, 32.f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(50.f);

	Animation* pAni = CreateAnimation("DinosaurAni");

	pAni->AddClip("DinosaurIdleDown", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 0, 1, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurIdleRight", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 1, 1, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurIdleUp", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 2, 1, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurIdleLeft", AT_ATLAS, AO_LOOP, 0.4f, 4, 7, 0, 3, 1, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));

	pAni->AddClip("DinosaurWalkDown", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 0, 3, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurWalkRight", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 1, 3, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurWalkUp", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 2, 3, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));
	pAni->AddClip("DinosaurWalkLeft", AT_ATLAS, AO_ONCE_RETURN, 0.4f, 4, 7, 1, 3, 3, 1,
		1.f,0, "Dinosaur", TEXT("Animals\\Dinosaur.bmp"));

	SAFE_RELEASE(pAni);

	return true;
}

int Dinosaur::Update(float fDeltaTime)
{
	FightObj::Update(fDeltaTime);
	return 0;
}

int Dinosaur::LateUpdate(float fDeltaTime)
{
	FightObj::LateUpdate(fDeltaTime);
	return 0;
}

void Dinosaur::Collision(float fDeltaTime)
{
	FightObj::Collision(fDeltaTime);
}

void Dinosaur::Render(HDC hDC, float fDeltaTime)
{
	FightObj::Render(hDC, fDeltaTime);
}

Dinosaur* Dinosaur::Clone()
{
	return new Dinosaur(*this);
}
