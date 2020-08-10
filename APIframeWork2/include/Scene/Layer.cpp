#include "Layer.h"
#include "../Object//Obj.h"
#include "../Collider/CollisionManager.h"
#include "..///Object/Tree.h"

Layer::Layer()	:
	m_iZOrder(0),
	m_strTag(""),
	m_pScene(NULL),
	m_bLife(true),
	m_bEnable(true)
{
}

Layer::~Layer()
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		Obj::EraseObj(*iter);
		SAFE_RELEASE((*iter));
	}
	m_ObjList.clear();
	
}

void Layer::AddObject(Obj * pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();

	m_ObjList.push_back(pObj);
}

void Layer::Input(float fDeltaTime)
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Input(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			Obj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();

		}
		else
			++iter;
	}
}

int Layer::Update(float fDeltaTime)
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			Obj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			++iter;
	}
	return 0;
}

int Layer::LateUpdate(float fDeltaTime)
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->LateUpdate(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			Obj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			++iter;
	}
	return 0;
}

void Layer::Collision(float fDeltaTime)
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Collision(fDeltaTime);
		if (!(*iter)->GetLife())
		{
			Obj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
		{
			GET_SINGLE(CollisionManager)->AddObject(*iter);

			++iter;
		}
	}
}

void Layer::Render(HDC hDC,float fDeltaTime)
{
	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();
	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if(GetTag() == "Default")
			m_ObjList.sort(Layer::SortYOrder);

		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Render(hDC,fDeltaTime);
		if (!(*iter)->GetLife())
		{
			Obj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			++iter;

	}
}

void Layer::Save(FILE* pFile)
{
	size_t iSize = m_ObjList.size();

	fwrite(&iSize, sizeof(iSize), 1, pFile);

	list<Obj*>::iterator iter;
	list<Obj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;++iter)
	{
		(*iter)->Save(pFile);
	}
}

void Layer::Load(FILE* pFile)
{
	size_t iSize = 0;

	fread(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i=0;i<iSize;++i)
	{
		Obj* pObj = Obj::CreateObj<CTree>("", this);

		pObj->Load(pFile);

		SAFE_RELEASE(pObj);
	}
}

void Layer::Sort()
{
	m_ObjList.sort(Layer::SortYOrder);
}

bool Layer::SortYOrder(Obj* pElem1, Obj* pElem2)
{
	return pElem1->GetPos().y < pElem2->GetPos().y;
}
