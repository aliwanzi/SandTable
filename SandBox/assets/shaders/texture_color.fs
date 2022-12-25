#version 420 core
out vec4 FragColor;

in vec2 texcoord;

uniform sampler2D Texture;
uniform vec4 Color;
uniform float Factor;

void main()
{
    FragColor = texture(Texture, texcoord * Factor) * Color;
}