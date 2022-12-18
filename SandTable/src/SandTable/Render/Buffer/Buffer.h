#pragma once
#include "SandTable/Core/Logger.h"

SAND_TABLE_NAMESPACE_BEGIN

class Buffer
{
public:
	Buffer() :m_uiRenderID(0) {}
	virtual ~Buffer() = default;
	virtual void Bind()const {};
	virtual void UnBind()const {};
protected:
	unsigned int m_uiRenderID;
};

SAND_TABLE_NAMESPACE_END