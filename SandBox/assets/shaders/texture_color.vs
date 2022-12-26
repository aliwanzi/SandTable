#version 420 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_TilingFactor;

uniform mat4 ViewProjection;

out VS_OUT {
	vec4 v_Color;
	vec2 v_TexCoord;
	float v_TexIndex;
	float v_TilingFactor;
} vs_out;

void main()
{
	gl_Position = ViewProjection * vec4(a_Position, 1.0);
	vs_out.v_Color = a_Color;
	vs_out.v_TexCoord = a_TexCoord;
	vs_out.v_TexIndex = a_TexIndex;
	vs_out.v_TilingFactor = a_TilingFactor;
}