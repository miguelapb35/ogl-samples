#version 420 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

#define VERTEX		0
#define TRANSFORM0		1

#define SAMPLER_DIFFUSE				0 
#define SAMPLER_POSITION_INPUT		8 
#define SAMPLER_TEXCOORD_INPUT		9 
#define SAMPLER_COLOR_INPUT			10 
#define SAMPLER_POSITION_OUTPUT		11 
#define SAMPLER_TEXCOORD_OUTPUT		12 
#define SAMPLER_COLOR_OUTPUT		13 

layout(binding = TRANSFORM0) uniform transform
{
	mat4 MVP;
} Transform;

layout(binding = SAMPLER_POSITION_INPUT) uniform samplerBuffer Position;
layout(binding = SAMPLER_TEXCOORD_INPUT) uniform samplerBuffer Texcoord;
layout(binding = SAMPLER_COLOR_INPUT) uniform samplerBuffer Color;

out gl_PerVertex
{
	vec4 gl_Position;
};

out block
{
	vec4 Texcoord;
	vec4 Color;
} Out;

void main()
{	
	Out.Texcoord = texelFetch(Texcoord, gl_VertexID);
	if(gl_VertexID % 2 != 0)
		Out.Color = vec4(1.0);
	else
		Out.Color = texelFetch(Texcoord, gl_VertexID);
	gl_Position = texelFetch(Position, gl_VertexID);
}
