#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_TilingFactor;
layout (location = 5) in float a_EntityID;

uniform mat4 ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_TexIndex;
layout (location = 4) out flat float v_EntityID;

void main()
{
	gl_Position = ViewProjection * vec4(a_Position, 1.0);

	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;
}