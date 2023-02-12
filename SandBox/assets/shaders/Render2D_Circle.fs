#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	vec2 TexCoord;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 5) in flat float v_TexIndex;
layout (location = 6) in flat float v_EntityID;

void main()
{
	vec4 texColor = texture(u_Textures[int(v_TexIndex)], Input.TexCoord);
	if(texColor.a < 0.00001)
		discard;

    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
    float circle = smoothstep(0.0, Input.Fade, distance);
    circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_Color = texColor * Input.Color;
	o_Color.a *= circle;

	o_EntityID = int(v_EntityID);
}