#pragma once

#include "../game.h"
class Scene
{
protected:
	friend class SceneManager;

protected:
	Scene();
	virtual ~Scene()=0;

private:
	unordered_map<string, class Obj*>	m_mapPrototype;
	class SceneCollision*				m_pCollision;

public:
	class SceneCollision* GetSceneCollision()	const
	{
		return m_pCollision;
	}

public:
	void ErasePrototype(const string& strTag);
	void ErasePrototype();

protected:
	list<class Layer*>		m_LayerList;
	SCENE_CREATE			m_eSceneType;
	bool					m_bEditMode;
	class Obj*				m_pPadeEffect;
	class Stage*			m_pStage;
	vector<char*>			m_vecstrProto;
	bool					m_bStart;

public:
	class Stage* GetStage()	const
	{
		return m_pStage;
	}

public:
	void SetSceneType(SCENE_CREATE eType)
	{
		m_eSceneType = eType;
	}
	SCENE_CREATE GetSceneType()
	{
		return m_eSceneType;
	}

	bool IsEditMode()	const
	{
		return m_bEditMode;
	}

	void SetEditMode(bool bEdit)
	{
		m_bEditMode = bEdit;
	}

	void ResetPadeInEffect();

	bool IsStarted()	const
	{
		return m_bStart;
	}

public:
	class Layer * CreateLayer(const string& strTag,
		int iZOrder = 0);
	class Layer* FindLayer(const string& strTag);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

public:
	static bool LayerSort(class Layer* pL1, class Layer* pL2);

public:
	template <typename T>
	T* CreateProtoType(const string& strTag)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
		pObj->AddRef();
		m_mapPrototype.insert(make_pair(strTag, pObj));
		return pObj;
	}
	Obj* FindProtoType(const string& strKey);
	void ChangePrototype();

public:
	void CreateUI(class Obj* pObj);
	void CreateProtoNumber();
	void CreateClockHand();
	void CreateFarmEffect();
	void CreateProtoTypes();
	class Obj* CreatePlayer();

protected:
	void LoadFile();
};