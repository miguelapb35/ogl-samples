#version 150 core

precision highp float;
precision highp int;

uniform vec4 Diffuse;

out vec4 Color;

void main()
{
	Color = Diffuse;
}
