#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "SandTable/Core/Core.h"
#include "SandTable/Core/Logger.h"
#include "SandTable/Debug/Instrumentor.h"
#include "SandTable/Core/TimeStep.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef SAND_TABLE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // SAND_TABLE_PLATFORM_WINDOWS
