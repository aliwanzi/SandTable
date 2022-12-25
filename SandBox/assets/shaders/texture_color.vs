#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTex;

uniform mat4 ViewProjection;
out vec2 texcoord;
out vec4 color;

void main()
{
	gl_Position = ViewProjection * vec4(aPos, 1.0);
	texcoord = aTex;
	color = aCol;
}