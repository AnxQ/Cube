#include "MutexLock.h"

MutexLock::MutexLock()
{
	InitializeCriticalSection(&mCS);
}

MutexLock::~MutexLock()
{
	DeleteCriticalSection(&mCS);
}

void MutexLock::Lock() {
	EnterCriticalSection(&mCS);
}

void MutexLock::Unlock() {
	LeaveCriticalSection(&mCS)
}