#pragma once

#include "Constants.h"
#include "SharedMemory.h"
#include "SharedBarrier.h"

typedef struct ChannelHeader
{
	volatile size_t nWrittenOffset;
	volatile size_t nWriteOffset;
	volatile size_t nTotalSize;
} ChannelHeader;

typedef struct MessageHeader
{
	size_t nLength;
} MessageHeader;


class SharedMemoryChannel
{
public:
	SharedMemoryChannel();
	~SharedMemoryChannel();

	bool Create(int nKey);

	bool Remove();

	size_t Write(char* pBuf, size_t nSize);

	bool ReadBlock(char* pBuf, size_t nCap);

	bool ReadMessage(char* pDestBuf, char* pBuf, size_t nReadPtr);

	bool RunLoop();

private:
	
	ChannelHeader* m_pChannelHeader;
	size_t m_nReadOffset;
	char* m_pMemoryHead;
	SharedMemory* m_pSharedMemory;
	SharedBarrier* m_pSharedBarrier;
};

