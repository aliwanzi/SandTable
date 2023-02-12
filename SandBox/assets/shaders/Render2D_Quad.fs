#version 450 core
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) in VertexOutput Input;
layout (location = 3) in flat float v_TexIndex;
layout (location = 4) in flat float v_EntityID;

void main()
{
	vec4 texColor = texture(u_Textures[int(v_TexIndex)], Input.TexCoord * Input.TilingFactor);
	if(texColor.a < 0.00001)
		discard;
    o_Color = texColor * Input.Color;
	o_EntityID = int(v_EntityID);
}