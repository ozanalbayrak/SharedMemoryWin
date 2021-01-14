#pragma once

#include <Windows.h>

class SharedMemory
{
public:
	SharedMemory();
	~SharedMemory();

	bool Create(int nKey, size_t nCapacity);
	bool Remove();
	bool IsOwner();
	char* GetBufferBasePtr();

private:
	HANDLE m_pHandle;
	char* m_pBuf;
	bool m_bOwner;

};

