#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 ViewProjection;
uniform mat4 Transform;
out vec2 texcoord;

void main()
{
	gl_Position = ViewProjection * Transform * vec4(aPos, 1.0);
	texcoord = aTex;
}