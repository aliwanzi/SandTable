#version 450 core
out vec4 FragColor;

in vec2 texcoord;
uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, texcoord);
}