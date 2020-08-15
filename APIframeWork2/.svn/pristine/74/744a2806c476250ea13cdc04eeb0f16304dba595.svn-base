#pragma once
#include "StaticObj.h"
class Portal :
	public StaticObj
{
	friend class Obj;
	friend class Scene;

protected:
	Portal();
	Portal(const Portal& portal);
	virtual ~Portal();

private:
	function<void(class Collider* pSrc, class Collider* pDest, float fTime)>	m_Callback;

public:
	void SetCallback(void(*pFunc)(class Collider*, class Collider*, float));
	template <typename T>
	void SetCallback(T* pObj, void(T::* pFunc)(class Collider*, class Collider*, float))
	{
		m_Callback = bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Portal* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class Collider* pSrc, class Collider* pDest, float fTime);
};

