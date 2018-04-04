#pragma once

// MutexLock Class

#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include <Windows.h>

class MutexLock
{
private:
	CRITICAL_SECTION mCS;
public:
	MutexLock();
	void Lock();
	void Unlock();
	~MutexLock();
};

#endif 

