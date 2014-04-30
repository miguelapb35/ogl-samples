#version 430 core
#define FRAG_COLOR	0

#define MATERIAL 0

precision highp float;
precision highp int;
layout(std140, column_major) uniform;
layout(std430, column_major) buffer;

layout(binding = MATERIAL) uniform material
{
	vec4 Diffuse;
} Material;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = Material.Diffuse;
}

