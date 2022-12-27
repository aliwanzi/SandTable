#version 420 core
out vec4 FragColor;

uniform sampler2D u_Textures[32];

in VS_OUT {
	vec4 v_Color;
	vec2 v_TexCoord;
	float v_TexIndex;
	float v_TilingFactor;
} fs_in;

void main()
{
	vec4 texColor=texture(u_Textures[int(fs_in.v_TexIndex)], fs_in.v_TexCoord * fs_in.v_TilingFactor);
	if(texColor.a < 0.1)
		discard;
    FragColor = texColor * fs_in.v_Color;
}