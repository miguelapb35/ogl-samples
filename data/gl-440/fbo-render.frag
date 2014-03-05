#version 420 core

#define DIFFUSE		0

in vec4 gl_FragCoord;

in block
{
	vec2 Texcoord;
} In;

layout(binding = DIFFUSE) uniform sampler2D Diffuse;
layout(binding = 0, rgba8) uniform coherent image2D Color;

void main()
{
	imageStore(Color, ivec2(gl_FragCoord.xy), texture(Diffuse, In.Texcoord));
}
