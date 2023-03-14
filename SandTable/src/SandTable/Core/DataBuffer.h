#pragma once

SAND_TABLE_NAMESPACE_BEGIN

class DataBuffer
{
public:
	DataBuffer(uint64_t uiDataBufferSize, uint8_t uiChannel);
	DataBuffer() = default;
	DataBuffer(const DataBuffer&) = default;
	~DataBuffer();

	void Resize(uint64_t uiDataBufferSize);
	void SetDataBuffer(const void* pDataBuffer);

	uint64_t GetDataBufferSize()const;
	const Ref<uint8_t>& GetDataBuffer()const;

	template<typename T>
	T* As()
	{
		return (T*)m_spDataBuffer.get();
	}

	static Ref<DataBuffer> DepthCopy(Ref<DataBuffer> spDataBuffer);

	operator bool() const;

private:
	Ref<uint8_t> m_spDataBuffer;
	uint64_t m_uiDataBufferSize;
	uint8_t m_uiChannel;
};

SAND_TABLE_NAMESPACE_END
