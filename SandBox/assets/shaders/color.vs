#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 ViewProjection;
uniform mat4 Transform;

void main()
{
	gl_Position = ViewProjection * Transform * vec4(aPos, 1.0);
}