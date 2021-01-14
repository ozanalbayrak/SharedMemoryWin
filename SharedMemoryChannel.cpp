#include "SharedMemoryChannel.h"

SharedMemoryChannel::SharedMemoryChannel():
	m_pChannelHeader(0),
	m_nReadOffset(0),
	m_pMemoryHead(0),
{
	m_pSharedMemory = new SharedMemory();
	m_pSharedBarrier = new SharedBarrier();
}

SharedMemoryChannel::~SharedMemoryChannel()
{
	this->Remove();
}

bool SharedMemoryChannel::Create(int nKey)
{
	bool bRetVal = true;
	bRetVal = bRetVal && m_pSharedMemory->Create(nKey, SHARED_MEMORY_SIZE + sizeof(ChannelHeader));
	bRetVal = bRetVal && m_pSharedBarrier->Create(nKey);

	m_pMemoryHead = m_pSharedMemory->GetBufferBasePtr();
	m_pChannelHeader = (ChannelHeader*) m_pMemoryHead;
	m_pMemoryHead += sizeof(ChannelHeader);

	if (m_pSharedMemory->IsOwner())
	{
		m_pChannelHeader->nTotalSize = SHARED_MEMORY_SIZE;
		m_pChannelHeader->nWriteOffset = 0;
		m_pChannelHeader->nWrittenOffset = 0;
	}

	return bRetVal;
}

bool SharedMemoryChannel::Remove()
{
	delete m_pSharedMemory;
	delete m_pSharedBarrier;
	m_pMemoryHead = 0;
	m_pChannelHeader = 0;
	return true;
}

size_t SharedMemoryChannel::Write(char* pBuf, size_t nSize)
{
	size_t nWrittenBytes = -1;

	if (m_pMemoryHead != 0)
	{
		size_t nMessageHeadOffset = InterlockedAdd((LONG*)&m_pChannelHeader->nWriteOffset, nSize + sizeof(MessageHeader));
		InterlockedAnd64((LONGLONG*)&m_pChannelHeader->nWriteOffset, m_pChannelHeader->nTotalSize - 1);
		// TODO: What happens if MessageHeader is needed to be fragmented?
		if (nMessageHeadOffset >= m_pChannelHeader->nTotalSize)
		{
			// Continue!
		}



		nMessageHeadOffset = nMessageHeadOffset - (nSize + sizeof(MessageHeader));
		MessageHeader* sMessageHeader = (MessageHeader*) (m_pMemoryHead + nMessageHeadOffset);
		sMessageHeader->nLength = nSize;
		nMessageHeadOffset += sizeof(MessageHeader);
		memcpy(m_pMemoryHead + nMessageHeadOffset, pBuf, nSize);
		if (m_pChannelHeader->nWrittenOffset < nMessageHeadOffset + nSize)
		{
			size_t nWrittenOffset = m_pChannelHeader->nWrittenOffset;
			InterlockedCompareExchange((LONG*)m_pChannelHeader->nWrittenOffset, nMessageHeadOffset + nSize, nWrittenOffset);
		}
		nWrittenBytes = nSize;
	}
	return nWrittenBytes;
}

size_t SharedMemoryChannel::ReadBlock(char* pBuf, size_t nCap)
{
	size_t nReadBytes = -1;
	if (m_pMemoryHead != 0)
	{
		
	}

	return false;
}

