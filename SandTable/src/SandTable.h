#pragma once

//For use by Application
#include "pch.h"

#include "SandTable/Core/Application.h"
#include "SandTable/Core/KeyCode.h"
#include "SandTable/Core/MouseCode.h"
#include "SandTable/Core/Input.h"
#include "SandTable/Core/FileSystem.h"

#include "SandTable/ImGui/ImGuiLayer.h"
#include "SandTable/ImGui/SceneHierarchyPanel.h"
#include "SandTable/ImGui/ContentBrowserPanel.h"

#include "SandTable/Render/Render/Render.h"
#include "SandTable/Render/Render/Render2D.h"
#include "SandTable/Render/Render/RenderImage.h"

#include "SandTable/Render/Buffer/VertexBuffer.h"
#include "SandTable/Render/Buffer/IndexBuffer.h"
#include "SandTable/Render/Buffer/FrameBuffer.h"

#include "SandTable/Render/RenderCommand.h"
#include "SandTable/Render/VertexArray.h"
#include "SandTable/Render/Shader.h"
#include "SandTable/Render/Texture/SubTexture2D.h"
#include "SandTable/Render/Camera/OrthoGraphicCameraController.h"
#include "SandTable/Render/Camera/EditorCamera.h"
#include "SandTable/Render/Image/Image.h"

#include "SandTable/Scene/Scene.h"
#include "SandTable/Scene/Components.h"
#include "SandTable/Scene/Entity.h"

#include "SandTable/Statics/DrawStatistics/QuadDrawStatics.h"
#include "SandTable/Statics/Timer.h"

#include "SandTable/System/ParticleSystem2D.h"

#include "SandTable/Math/MathUtils.h"
#include "SandTable/Math/Random.h"

#include "SandTable/Script/ScriptEngine.h"

#include "SandTable/Project/Project.h"

using namespace SandTable;

