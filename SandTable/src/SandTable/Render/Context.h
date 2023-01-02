#pragma once
SAND_TABLE_NAMESPACE_BEGIN

class Context
{
public:
	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;
};

SAND_TABLE_NAMESPACE_END