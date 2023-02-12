#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in float a_TexIndex;
layout(location = 5) in float a_Thickness;
layout(location = 6) in float a_Fade;
layout(location = 7) in float a_EntityID;

layout(std140) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	vec2 TexCoord;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 5) out flat float v_TexIndex;
layout (location = 6) out flat float v_EntityID;

void main()
{
	Output.LocalPosition = a_LocalPosition;
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.Thickness = a_Thickness;
	Output.Fade = a_Fade;

	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}
