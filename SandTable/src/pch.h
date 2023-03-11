#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include "SandTable/Core/Core.h"
#include "SandTable/Core/Logger.h"
#include "SandTable/Core/TimeStep.h"
#include "SandTable/Core/DataBuffer.h"

#include "SandTable/Statics/Instrumentor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#ifdef SAND_TABLE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // SAND_TABLE_PLATFORM_WINDOWS
