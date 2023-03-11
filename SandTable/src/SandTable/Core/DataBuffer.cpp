#include "pch.h"
#include "DataBuffer.h"

SAND_TABLE_NAMESPACE_BEGIN

DataBuffer::DataBuffer(uint64_t uiDataBufferSize)
{
	Resize(uiDataBufferSize);
}

DataBuffer::~DataBuffer()
{
	m_uiDataBufferSize = 0;
}

void DataBuffer::Resize(uint64_t uiDataBufferSize)
{
	m_spDataBuffer = Ref<uint8_t>(new uint8_t[uiDataBufferSize], std::default_delete<uint8_t>());
	m_uiDataBufferSize = uiDataBufferSize;
}

void DataBuffer::SetDataBuffer(const void* pDataBuffer)
{
	memcpy(m_spDataBuffer.get(), pDataBuffer, m_uiDataBufferSize);
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
	auto spCopyBuffer = CreateRef<DataBuffer>(spDataBuffer->m_uiDataBufferSize);
	memcpy(spCopyBuffer.get(), spDataBuffer->m_spDataBuffer.get(), spDataBuffer->m_uiDataBufferSize);
	return spCopyBuffer;
}

DataBuffer::operator bool() const
{
	return m_spDataBuffer != nullptr;
}

SAND_TABLE_NAMESPACE_END
