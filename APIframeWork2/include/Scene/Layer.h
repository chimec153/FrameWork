#pragma once
#include "../game.h"
class Layer
{
private:
	friend class Scene;

private:
	Layer();
public:
	~Layer();
private:
	class Scene*	m_pScene;
	string	m_strTag;
	int		m_iZOrder;
	list<class Obj*>	m_ObjList;
	bool		m_bEnable;
	bool		m_bLife;

public:
	void SetEnable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void Die()
	{
		m_bLife = false;
	}

	bool GetEnable()	const
	{
		return m_bEnable;
	}
	bool GetLife()	const
	{
		return m_bLife;
	}

public:
	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

	void SetZOrder(int iZOder)
	{
		m_iZOrder = iZOder;
	}

	void SetScene(class Scene* pScene)
	{
		m_pScene = pScene;
	}
	int GetZOrder()	const
	{
		return m_iZOrder;
	}

	string GetTag()	const
	{
		return m_strTag;
	}

	Scene* GetScene() const
	{
		return m_pScene;
	}
public:
	void AddObject(class Obj* pObj);

public:
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
	void Sort();

private:
	static bool SortYOrder(class Obj* pElem1, class Obj* pElem2);

};

