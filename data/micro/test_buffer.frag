#version 330 core
#extension GL_ARB_shading_language_420pack : require

#define FRAG_COLOR		0

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

in block
{
	vec4 Color;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = vec4(1.0, 0.5, 0.0, 1.0);//In.Color;
}
