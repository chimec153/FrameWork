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
	static unordered_map<string, class Obj*> m_mapPrototype[SC_END];

public:
	static void ErasePrototype(const string& strTag,
		SCENE_CREATE sc);
	static void ErasePrototype(SCENE_CREATE sc);

protected:
	list<class Layer*>	m_LayerList;
	SCENE_CREATE		m_eSceneType;
	bool				m_bEditMode;
	class Obj*			m_pPlayer;

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

	void SetPlayer(class Obj* pObj);
	class Obj* GetPlayer()	const;

public:
	class Layer * CreateLayer(const string& strTag,
		int iZOrder = 0);
	class Layer* FindLayer(const string& strTag);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

public:
	static bool LayerSort(class Layer* pL1, class Layer* pL2);

public:
	template <typename T>
	static T* CreateProtoType(const string& strTag,
		SCENE_CREATE sc)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}
		pObj->AddRef();
		m_mapPrototype[sc].insert(make_pair(strTag, pObj));
		return pObj;
	}
	static Obj* FindProtoType(const string& strKey,
		SCENE_CREATE sc);
	static void ChangePrototype();

public:
	void CreateUI(class Obj* pObj);
	void CreateSlimeClone();
	void CreateSlimeProto();
	void CreateBatClone();
	void CreateBatProto();
	void CreateProtoNumberSmall();
	void CreateClockHand();
	class Obj* CreatePlayer();
};