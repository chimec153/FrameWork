#include "SceneCave.h"
#include "../Core/Camera.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Collider/ColliderSphere.h"
#include "../Object/Minion.h"
#include "../Object/UIBar.h"
#include "../Object/UIPanel.h"
#include "SceneManager.h"
#include "../Core/PathManager.h"
#include "..//Object/Effect.h"
#include "..//Collider/ColliderRect.h"
#include "..//Object/Slime.h"
#include "../Animation/Animation.h"
#include "InGameScene.h"

SceneCave::SceneCave()	:
	m_pStage(nullptr)
{
}

SceneCave::~SceneCave()
{
	SAFE_RELEASE(m_pStage);
}

bool SceneCave::Init()
{
	Layer* pStageLayer = FindLayer("Stage");

	m_pStage = Obj::CreateObj<Stage>("Stage", pStageLayer);

	m_pStage->CreateTile(100, 100, 16, 16, "Tiles", L"TileSheets\\Flooring.bmp");

	char	strFileName[MAX_PATH] = {};

	const char* strRootPath = GET_SINGLE(PathManager)->FindPathMultiByte(DATA_PATH);

	if (strRootPath)
		strcat_s(strFileName, strRootPath);

	strcat_s(strFileName, "cave.tmp");

	FILE* pLoadFile = nullptr;

	fopen_s(&pLoadFile, strFileName, "rb");

	if (pLoadFile)
	{
		if (m_pStage)
			m_pStage->Load(pLoadFile);

		Layer* pLayer = FindLayer("Default");

		if (pLayer)
			pLayer->Load(pLoadFile);

		fclose(pLoadFile);
	}

	if (!Scene::Init(POSITION(32.f * 8.5f, 32.f * 7.f)))	//	8,5 out
		return false;

	CreateBatProto();

	CreateBatClone();

	CreateSlimeProto();

	CreateSlimeClone();

	ColliderRect* pPortal = m_pStage->AddCollider<ColliderRect>("OutPortal");

	pPortal->SetRect(32.f * 8.f, 32.f * 2.f, 32.f * 9.f, 32.f * 3.f);
	pPortal->AddCollisionFunction(CS_ENTER, this, &SceneCave::OutPortalCol);

	SAFE_RELEASE(pPortal);

	return true;
}

void SceneCave::OutPortalCol(Collider* pSrc, Collider* pDest, float fTime)
{
	GET_SINGLE(SceneManager)->CreateScene<InGameScene>(SC_NEXT);
}
