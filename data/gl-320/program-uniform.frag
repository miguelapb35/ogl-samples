#version 150 core

uniform vec4 Diffuse[2];

out vec4 Color;

void main()
{
	Color = Diffuse[0] * Diffuse[1];
}
