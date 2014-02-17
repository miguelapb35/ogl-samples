#version 320 core

uniform sampler2D Diffuse;

in block
{
	vec2 Texcoord;
} In;

out vec4 Color;

void main()
{
	Color = texture(Diffuse, In.Texcoord);
}
