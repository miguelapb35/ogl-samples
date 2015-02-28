#version 450 core

#define POSITION	0
#define COLOR		3
#define FRAG_COLOR	0

#define MATERIAL	0
#define TRANSFORM0	1

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

layout(binding = MATERIAL) uniform material
{
	vec4 Diffuse;
} Material;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = Material.Diffuse;
}
