#pragma once
#include <Windows.h>
class SharedBarrier
{
public:
	SharedBarrier();
	~SharedBarrier();

	bool Create(int nKey, bool bInitSignaled = false);
	bool Remove();
	bool Wait();
	bool Signal();
private:
	HANDLE m_pHandle;


};

