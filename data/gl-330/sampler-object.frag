#version 330 core
#define FRAG_COLOR	0

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

struct material
{
	sampler2D Diffuse[2];
	vec4 Color;
};

uniform material Material;

in block
{
	vec2 Texcoord;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	vec4 Temp = Color * 0.33;

	if ((1.0 - In.Texcoord.y) / In.Texcoord.x < 1.0)
		Temp += texture(Material.Diffuse[0], In.Texcoord) * 0.66;
	else
		Temp += texture(Material.Diffuse[1], In.Texcoord) * 0.66;

	Color = Temp;
}
