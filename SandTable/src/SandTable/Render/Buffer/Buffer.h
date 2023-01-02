#pragma once
SAND_TABLE_NAMESPACE_BEGIN

class Buffer
{
public:
	virtual ~Buffer() = default;
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
};

SAND_TABLE_NAMESPACE_END