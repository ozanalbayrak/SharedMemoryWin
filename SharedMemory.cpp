#include "SharedMemory.h"
#include <stdio.h>

SharedMemory::SharedMemory() :
	m_pHandle(0),
	m_pBuf(0),
	m_bOwner(false)
{
}

SharedMemory::~SharedMemory()
{
	this->Remove();
}

bool SharedMemory::Create(int nKey, size_t nCapacity)
{
	DWORD lastError;
	char shMemFileName[50];
	sprintf(shMemFileName, "Global\\ShMemFileMapping-%d", nKey);
	m_pHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nCapacity, shMemFileName);
	lastError = GetLastError();
	if (m_pHandle != NULL)
	{
		if (lastError != ERROR_ALREADY_EXISTS)
		{
			m_bOwner = true;
		}
	}
	else
	{
		printf("Could not create file mapping object (%d).\n", GetLastError());
		return false;
	}

	m_pBuf = (char*) MapViewOfFile(m_pHandle, FILE_MAP_ALL_ACCESS, 0, 0, nCapacity);
	if (m_pBuf == NULL)
	{
		printf("Could not map view of file (%d).\n", GetLastError());
		CloseHandle(m_pHandle);
		m_bOwner = false;
		return false;
	}
	return true;
}

bool SharedMemory::Remove()
{
	UnmapViewOfFile(m_pBuf);
	CloseHandle(m_pHandle);
	return true;
}

bool SharedMemory::IsOwner()
{
	return m_bOwner;
}

char* SharedMemory::GetBufferBasePtr()
{
	return m_pBuf;
}