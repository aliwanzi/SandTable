#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out VS_OUT {
    vec3 position;
	vec4 color;
} vs_out;

uniform mat4 ViewProjection;
uniform mat4 Transform;

void main()
{
	gl_Position = ViewProjection * Transform * vec4(aPos, 1.0);
	vs_out.position = aPos;
	vs_out.color = aColor;
}