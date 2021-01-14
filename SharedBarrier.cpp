#include "SharedBarrier.h"
#include <stdio.h>

SharedBarrier::SharedBarrier() :
	m_pHandle(0)
{
}

SharedBarrier::~SharedBarrier()
{
	this->Remove();
}

bool SharedBarrier::Create(int nKey, bool bInitSignaled)
{
    char barrierName[50];
    sprintf(barrierName, "Global\\ShMemBarrier-%d", nKey);
    m_pHandle = CreateEvent(NULL, TRUE, bInitSignaled, barrierName);

    if (m_pHandle == NULL)
    {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return false;
    }
	return true;
}

bool SharedBarrier::Remove()
{
	return CloseHandle(m_pHandle);
}

bool SharedBarrier::Wait()
{
	return WaitForSingleObject(m_pHandle, INFINITE);
}

bool SharedBarrier::Signal()
{
	return PulseEvent(m_pHandle);
}
