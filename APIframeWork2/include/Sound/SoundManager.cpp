#include "SoundManager.h"
#include "../Core/PathManager.h"
DEFINITION_SINGLE(SoundManager)

SoundManager::SoundManager()	:
	m_pSystem(NULL)
{
}

SoundManager::~SoundManager()
{
	unordered_map<string, PSOUNDINFO>::iterator	iter;
	for (iter = m_mapSound.begin(); iter != m_mapSound.end(); ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}

	m_pSystem->close();
	m_pSystem->release();
}

bool SoundManager::Init()
{
	// FMOD System을 생성한다.
	System_Create(&m_pSystem);

	// FMOD를 초기화한다.
	m_pSystem->init(256, FMOD_INIT_NORMAL,NULL);

	m_pSystem->getMasterChannelGroup(&m_pMasterGroup);

	CreateChannelGroup("BGM");
	CreateChannelGroup("Effect");
	CreateChannelGroup("UI");

	return true;
}

void SoundManager::Update()
{
	m_pSystem->update();
}

bool SoundManager::LoadSound(const string & strKey, bool bLoop, 
	const char * pFileName, const string & strPathKey)
{
	if (FindSound(strKey))
		return true;

	const char* pPath = GET_SINGLE(PathManager)->FindPathMultiByte(strPathKey);
	string strFullPath;
	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	FMOD_MODE	eMode = FMOD_LOOP_NORMAL;
	if (!bLoop)
		eMode = FMOD_DEFAULT;

	PSOUNDINFO	pSoundInfo = new SOUNDINFO;

	pSoundInfo->bLoop = bLoop;
	m_pSystem->createSound(strFullPath.c_str(), eMode, NULL, &pSoundInfo->pSound);
	m_mapSound.insert(make_pair(strKey, pSoundInfo));
	
	return true;
}

bool SoundManager::Play(const string & strKey)
{
	PSOUNDINFO pSound = FindSound(strKey);

	if (!pSound)
		return false;
	m_pSystem->update();

	SOUND_TYPE eType = ST_BGM;
	if (pSound->bLoop)
		eType = ST_EFFECT;

	m_pSystem->playSound(pSound->pSound, NULL, false, &m_pChannel[eType]);
	return true;
}

void SoundManager::Stop(SOUND_TYPE eType)
{
	m_pChannel[eType]->stop();
}

void SoundManager::Volume(SOUND_TYPE eType, float fVolume)
{
	m_pChannel[eType]->setVolume(fVolume);
}

bool SoundManager::CreateChannelGroup(const string& strName)
{
	ChannelGroup* pGroup = FindChannelGroup(strName);

	if (pGroup)
		return false;

	m_pSystem->createChannelGroup(strName.c_str(), &pGroup);

	m_pMasterGroup->addGroup(pGroup, false, 0);

	m_mapChannelGroup.insert(make_pair(strName, pGroup));

	return true;
}

void SoundManager::ReleaseChannelGroup(const string& strName)
{
	auto iter = m_mapChannelGroup.find(strName);

	if (iter == m_mapChannelGroup.end())
		return;

	iter->second->release();
	m_mapChannelGroup.erase(iter);
}

PSOUNDINFO SoundManager::FindSound(const string & strKey)
{
	unordered_map<string, PSOUNDINFO>::iterator iter = m_mapSound.find(strKey);
	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}

ChannelGroup* SoundManager::FindChannelGroup(const string& strName)
{
	auto iter = m_mapChannelGroup.find(strName);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}
