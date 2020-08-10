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
	GET_SINGLE(Camera)->SetWorldResolution(3200, 3200);

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

	Player* pPlayer = (Player*)CreatePlayer();

	CreateProtoNumberSmall();

	CreateBatProto();

	CreateBatClone();

	CreateSlimeProto();

	CreateSlimeClone();

	CreateUI(pPlayer);

	SAFE_RELEASE(pPlayer);

	return true;
}