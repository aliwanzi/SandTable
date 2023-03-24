#include "pch.h"
#include "DataBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

DataBuffer::DataBuffer(uint64_t uiDataBufferSize, uint8_t uiChannel) :
	m_uiChannel(uiChannel)
{
	Resize(uiDataBufferSize);
}

DataBuffer::~DataBuffer()
{
	m_uiDataBufferSize = 0;
}

void DataBuffer::Clear()
{
	memset(m_spDataBuffer.get(), 0, m_uiDataBufferSize * m_uiChannel);
}

void DataBuffer::Resize(uint64_t uiDataBufferSize)
{
	m_spDataBuffer = Ref<uint8_t>(new uint8_t[uiDataBufferSize * m_uiChannel], std::default_delete<uint8_t>());
	m_uiDataBufferSize = uiDataBufferSize;
}

void DataBuffer::SetDataBuffer(const void* pDataBuffer)
{
	memcpy(m_spDataBuffer.get(), pDataBuffer, m_uiDataBufferSize * m_uiChannel);
}

uint64_t DataBuffer::GetDataBufferSize() const
{
	return m_uiDataBufferSize;
}

const Ref<uint8_t>& DataBuffer::GetDataBuffer() const
{
	return m_spDataBuffer;
}

Ref<DataBuffer> DataBuffer::DepthCopy(Ref<DataBuffer> spDataBuffer)
{
	auto spCopyBuffer = CreateRef<DataBuffer>(spDataBuffer->m_uiDataBufferSize, spDataBuffer->m_uiChannel);
	memcpy(spCopyBuffer.get(), spDataBuffer->m_spDataBuffer.get(), spDataBuffer->m_uiDataBufferSize);
	return spCopyBuffer;
}

DataBuffer::operator bool() const
{
	return m_spDataBuffer != nullptr;
}

SAND_TABLE_NAMESPACE_END
