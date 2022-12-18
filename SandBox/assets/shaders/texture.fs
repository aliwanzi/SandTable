#version 420 core
out vec4 FragColor;

in vec2 texcoord;
uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, texcoord);
}