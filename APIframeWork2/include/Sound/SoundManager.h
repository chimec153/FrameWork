#pragma once

#include "../game.h"
#include "fmod.hpp"

using namespace FMOD;

#pragma comment(lib, "fmod_vc")
enum SOUND_TYPE
{
	ST_BGM,
	ST_EFFECT,
	ST_END
};
typedef struct _tagSoundInfo
{
	Sound*	pSound;
	bool	bLoop;
}SOUNDINFO, *PSOUNDINFO;

class SoundManager
{
private:
	System*									m_pSystem;
	Channel*								m_pChannel[ST_END];
	unordered_map<string, PSOUNDINFO>		m_mapSound;
	ChannelGroup*							m_pMasterGroup;
	unordered_map<string, ChannelGroup*>	m_mapChannelGroup;
	
public:
	bool Init();
	void Update();
	bool LoadSound(const string& strKey, bool bLoop, const char* pFileName,
		const string& strPathKey = SOUND_PATH);
	bool Play(const string& strKey);
	void Stop(SOUND_TYPE eType);
	void Volume(SOUND_TYPE eType, float fVolume);

	bool CreateChannelGroup(const string& strName);
	void ReleaseChannelGroup(const string& strName);

private:
	PSOUNDINFO	FindSound(const string& strKey);
	ChannelGroup* FindChannelGroup(const string& strName);
	DECLARE_SINGLE(SoundManager)
};

