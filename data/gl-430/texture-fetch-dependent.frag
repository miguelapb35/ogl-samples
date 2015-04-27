#version 420 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

layout(binding = 0) uniform sampler2DArray Diffuse;
layout(binding = 1) uniform usampler2DArray Indirection;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

const vec2 WINDOW_SIZE = vec2(1280.0, 720.0);

/*
void main()
{
	vec2 FragCoord = gl_FragCoord.xy;
	vec2 Texcoord = FragCoord / WINDOW_SIZE;
	float Layer = FragCoord.x + FragCoord.y * WINDOW_SIZE.x;

	uint IndirectionIndex = uint(mod(Layer, 2048));
	for(int i = 0; i < 256; ++i)
		IndirectionIndex = texture(Indirection, vec3(0, 0, IndirectionIndex)).x;

	Color = texture(Diffuse, vec3(Texcoord.st, float(IndirectionIndex)));
}
*/

void main()
{
	uint Offset = 1;
	vec2 FragCoord = gl_FragCoord.xy;
	vec2 Texcoord = FragCoord / WINDOW_SIZE;
	float Layer = FragCoord.x + FragCoord.y * WINDOW_SIZE.x;

	uint IndirectionIndex = uint(mod(Layer, 2048));
	uint IndirectionIndexTotal = IndirectionIndex;
	for(int i = 0; i < 32; ++i)
		IndirectionIndexTotal += texture(Indirection, vec3(0, 0, IndirectionIndex + Offset * i)).x;

	Color = texture(Diffuse, vec3(Texcoord.st, mod(float(IndirectionIndexTotal), 2048)));
}
