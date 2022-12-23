#version 420 core
out vec4 FragColor;

in vec2 texcoord;

uniform sampler2D Texture;
uniform vec4 Color;

void main()
{
    FragColor = texture(Texture, texcoord*10.f) * Color;
}