#version 150 core

precision highp float;

in block
{
	vec4 Color;
} In;

out vec4 FragColor;

void main()
{
	FragColor = In.Color;
}

