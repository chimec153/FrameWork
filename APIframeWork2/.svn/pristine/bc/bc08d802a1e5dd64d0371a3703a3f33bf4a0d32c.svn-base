#pragma once
#include "../Ref.h"
#include "../Object/Obj.h"

class Collider :
	public Ref
{
protected:
	friend class Obj;

protected:
	Collider();
	Collider(const Collider& coll);
	virtual ~Collider()=0;

protected:
	COLLIDER_TYPE	m_eCollType;
	class Obj*		m_pObj;
	list<Collider*>	m_CollisionList;
	list<function<void(Collider*, Collider*, float)>> m_FuncList[CS_END];
	POSITION		m_tHitPoint;
	bool			m_bColl;
	bool			m_bUI;

public:
	POSITION GetHitPoint()	const
	{
		return m_tHitPoint;
	}

	void SetHitPoint(const POSITION& tPos)
	{
		m_tHitPoint = tPos;
	}

	void SetUI(bool bUI)
	{
		m_bUI = bUI;
	}

	bool IsUI()	const
	{
		return m_bUI;
	}

public:
	void AddCollisionFunction(COLLISION_STATE eState,
		void(*pFunc)(Collider*, Collider*, float))
	{
		function<void(Collider*, Collider*, float)>		func;

		func = bind(pFunc, placeholders::_1, placeholders::_2,
			placeholders::_3);

		m_FuncList[eState].push_back(func);
	}

	template <typename T>
	void AddCollisionFunction(COLLISION_STATE eState, T* pObj,
		void(T::*pFunc)(Collider*, Collider*, float))
	{
		function<void(Collider*, Collider*, float)>		func;

		func = bind(pFunc, pObj, placeholders::_1, placeholders::_2,
			placeholders::_3);

		m_FuncList[eState].push_back(func);
	}

	void CallFunction(COLLISION_STATE eState,
		Collider* pDest, float fDeltaTime)
	{
		list<function<void(Collider*, Collider*, float)>>::iterator		iter;
		list<function<void(Collider*, Collider*, float)>>::iterator		iterEnd = m_FuncList[eState].end();

		for (iter = m_FuncList[eState].begin(); iter != iterEnd; ++iter)
		{
			(*iter)(this, pDest, fDeltaTime);
		}
	}

public:
	void AddCollider(Collider* pCollider)
	{
		m_CollisionList.push_back(pCollider);
	}

	bool CheckCollisionList(Collider* pCollider)
	{
		list<Collider*>::iterator	iter;
		list<Collider*>::iterator	iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == pCollider)
				return true;
		}
		return false;
	}
	void EraseCollisionList(Collider* pCollider)
	{
		list<Collider*>::iterator iter;
		list<Collider*>::iterator iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
		{
			if (*iter == pCollider)
			{
				m_CollisionList.erase(iter);
				break;
			}
		}
}
public:
	COLLIDER_TYPE GetColliderType() const
	{
		return m_eCollType;
	}

	class Obj* GetObj()		const
	{
		return m_pObj;
	}

public:
	void SetObj(class Obj* pObj)
	{
		m_pObj = pObj;
	}

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(Collider* pDest);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual Collider* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

protected:
	bool CollisionRectToRect(const RECTANGLE& src,
		const RECTANGLE& dest);
	bool CollisionRectToSphere(const RECTANGLE& src,
		const SPHERE& dest);
	bool CollisionSphereToSphere(const SPHERE& src,
		const SPHERE& dest);
	bool CollisionRectToPixel(const RECTANGLE& src,
		const vector<PIXEL>& vecPixel, int iWidth, int iHeight);
	bool CollisionRectToPoint(const RECTANGLE& src,
		const POSITION& dest);
	bool CollisionSphereToPoint(const SPHERE& src,
		const POSITION& dest);
	bool CollisionPixelToPoint(const vector<PIXEL>& vecPixel,
		int iWidth, int iHeight, const POSITION& dest);
};

