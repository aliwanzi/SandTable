#include "pch.h"
#include "OpenGLRenderAPI.h"
#include "GL/gl3w.h"
#include "SandTable/Statics/Instrumentor.h"

SAND_TABLE_NAMESPACE_BEGIN

namespace
{
	void OnDebug(unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_DEV_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_DEV_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_DEV_INFO(message); return;
		}
	}

	bool IsExtensionSupported(const std::string& sExtersionName)
	{
		GLint numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

		for (int i = 0; i < numExtensions; i++)
		{
			if (std::string((char const*)glGetStringi(GL_EXTENSIONS, i)) == sExtersionName)
			{
				return true;
			}
		}
		return false;
	}
}

void OpenGLRenderAPI::Init()
{
	SAND_TABLE_PROFILE_FUNCTION();
	
#ifdef SAND_TABLE_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OnDebug, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif


	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
}

void OpenGLRenderAPI::SetClearColor(const glm::vec4& vec4Color)
{
	glClearColor(vec4Color.r, vec4Color.g, vec4Color.b, vec4Color.a);
}

void OpenGLRenderAPI::SetViewPort(unsigned int uiWidth, unsigned int uiHeight)
{
	glViewport(0, 0, uiWidth, uiHeight);
}

void OpenGLRenderAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawTriangles(unsigned int uiIndexCount)
{
	glDrawElements(GL_TRIANGLES, uiIndexCount, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderAPI::DrawLines(unsigned int uiIndexCount)
{
	glDrawArrays(GL_LINES, 0, uiIndexCount);
}

void OpenGLRenderAPI::SetLineWidth(float fWidth)
{
	glLineWidth(fWidth);
}

SAND_TABLE_NAMESPACE_END
