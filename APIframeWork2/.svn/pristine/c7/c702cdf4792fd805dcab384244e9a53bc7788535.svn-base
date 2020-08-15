#pragma once


#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <conio.h>
#include "Sound/fmod.hpp"
#include <comdef.h>
#include <time.h>
#include <Commctrl.h>

using namespace std;

#include "macro.h"
#include "resource.h"
#include "types.h"
#include "Flag.h"

#pragma comment(lib, "msimg32")

#pragma comment(lib, "fmod_vc")

#define		GRAVITY		0.98f

#define ROOT_PATH		"RootPath"
#define TEXTURE_PATH	"TexturePath"
#define DATA_PATH		"DataPath"
#define SOUND_PATH		"SoundPath"

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	p.clear();
}

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}
	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	p.clear();
}