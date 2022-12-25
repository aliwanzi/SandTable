#version 420 core
out vec4 FragColor;

in vec2 texcoord;
in vec4 color;

uniform sampler2D Texture;
uniform vec4 Color;
uniform float Factor;

void main()
{
    //FragColor = texture(Texture, texcoord * Factor) * Color;
    FragColor = color;
}