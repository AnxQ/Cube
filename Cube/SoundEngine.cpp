#include "SoundEngine.h"



SoundEngine::SoundEngine()
{
}


SoundEngine::~SoundEngine()
{
}

void SoundEngine::Play(LPCSTR filePath, DWORD playParam) {
	PostThreadMessage(mEngineThreadID, playParam, 0, (LPARAM)filePath);
}

void SoundEngine::Terminate() {
	PostThreadMessage(mEngineThreadID, SE_TERMINATE, 0, NULL);
}

void WINAPI SoundEngine::EngineThread() {
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER + 1000, PM_NOREMOVE);
	while (true) {
		if (PeekMessage(&msg, NULL, WM_USER, WM_USER + 1000, PM_REMOVE))
		{
			switch (msg.message) {
			case SE_PLAY_LOOP:
				PlaySound(TEXT((LPCSTR)msg.lParam), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				break;
			case SE_PLAY_ONCE:
				PlaySound(TEXT((LPCSTR)msg.lParam),NULL, SND_FILENAME | SND_ASYNC);
				break;
			case SE_PAUSE:
				break;
			case SE_TERMINATE:
				return;
			default:
				break;
			}
		}
	}
}