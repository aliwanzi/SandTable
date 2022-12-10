#include "..\..\SandTable\src\SandTable\Core\Application.h"
#include <SandTable.h>

class Sandbox :public SandTable::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

std::shared_ptr<SandTable::Application> SandTable::Application ::CreateApplication()
{
	static std::shared_ptr<Application> spSandBox(new Application());
	return spSandBox;
}
