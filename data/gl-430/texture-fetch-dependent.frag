#version 420 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

layout(binding = 0) uniform sampler2DArray Diffuse;
layout(binding = 1) uniform usampler2DArray Indirection;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

const vec2 WINDOW_SIZE = vec2(1280.0, 720.0);
const uint TableSize = 256;
const uint TextureFetchOffset = 1;
const uint TextureFetchCount = 32;

/*
void main()
{
	vec2 FragCoord = gl_FragCoord.xy;
	vec2 Texcoord = FragCoord / WINDOW_SIZE;
	float Layer = FragCoord.x + FragCoord.y * WINDOW_SIZE.x;

	uint IndirectionIndex = uint(Layer);
	for(int i = 0; i < TextureFetchCount; ++i)
	{
		IndirectionIndex = IndirectionIndex % TableSize;
		IndirectionIndex = texture(Indirection, vec3(0, 0, IndirectionIndex)).x;
	}

	Color = texture(Diffuse, vec3(Texcoord.st, float(IndirectionIndex)));
}
*/

void main()
{
	vec2 FragCoord = gl_FragCoord.xy;
	vec2 Texcoord = FragCoord / WINDOW_SIZE;
	float Layer = FragCoord.x + FragCoord.y * WINDOW_SIZE.x;

	uint IndirectionIndex = uint(mod(Layer, TableSize));
	uint IndirectionIndexTotal = IndirectionIndex;
	for(uint i = 0; i < TextureFetchCount; ++i)
	{
		uint FetchIndex = (IndirectionIndex + TextureFetchOffset * i) % TableSize;
		IndirectionIndexTotal += texture(Indirection, vec3(0, 0, FetchIndex)).x;
	}

	Color = texture(Diffuse, vec3(Texcoord.st, mod(float(IndirectionIndexTotal), TableSize)));
}

