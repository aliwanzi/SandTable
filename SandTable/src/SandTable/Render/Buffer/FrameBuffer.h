#pragma once
#include "SandTable/Core/Core.h"
#include "SandTable/Render/Buffer/Buffer.h"

SAND_TABLE_NAMESPACE_BEGIN
struct FrameBufferSpecification
{
	unsigned int Width;
	unsigned int Height;
	unsigned int Samples;
	bool SwapChainTarget;
};

class FrameBuffer:public Buffer
{
public:
	static Ref<Buffer> Create(const Ref<FrameBufferSpecification>& spFrameBufferSpecification);
	const Ref<FrameBufferSpecification>& GetFrameBufferSpecification()const;
	unsigned int GetColorAttachment()const;
	unsigned int GetDepthAttachment()const;
private:
	FrameBuffer() = delete;
	FrameBuffer(FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;
protected:
	FrameBuffer(const Ref<FrameBufferSpecification>& spFrameBufferSpecification);
protected:
	Ref<FrameBufferSpecification> m_spFrameBufferSpecification;
	unsigned int m_uiRenderID;
	unsigned int m_uiColorAttachment;
	unsigned int m_uiDepthAttachment;
};
SAND_TABLE_NAMESPACE_END

