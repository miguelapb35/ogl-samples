#version 400 core

#define FRAG_COLOR	0

uniform samplerCubeArray Environment;

in block
{
	vec3 Reflect;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = texture(Environment, vec4(In.Reflect, 0.0));
}
