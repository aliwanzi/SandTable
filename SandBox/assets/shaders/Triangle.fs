#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec3 position;
	vec4 color;
} fs_in;

void main()
{
    //FragColor = vec4(fs_in.position * 0.5 + 0.5,1.0f);
    FragColor = fs_in.color;
}