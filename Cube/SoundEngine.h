#pragma once

#include <Windows.h>
#include "mmsystem.h"



#ifndef _SOUNDENGINE_H_
#define _SOUNDENGINE_H_

using namespace std;

#define SE_PLAY_LOOP 40100
#define SE_PLAY_ONCE 40101
#define SE_PAUSE 40102
#define SE_TERMINATE 40109


class SoundEngine
{
	HANDLE mEngineThreadHandle;
	DWORD mEngineThreadID;
	void Play(LPCSTR filePath, DWORD playParam);
	void Terminate();
	void WINAPI EngineThread();
public:
	
	SoundEngine();
	~SoundEngine();
};

#endif

