#version 420 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0
#define TRANSFORM0	1
#define DIFFUSE		0

#define COUNT 24

layout(binding = TRANSFORM0) uniform transform
{
	mat4 MVP;
} Transform;

layout(location = POSITION) in vec3 Position;
layout(location = TEXCOORD) in vec2 Texcoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

out block
{
	vec2 Texcoord;
	vec4 Lumimance[COUNT];
} Out;

void main()
{
	int A = 0;
	uint B = A;
	float C = A;
	float D = B;
	ivec2 E = ivec2(0);
	uvec2 F = E;
	ivec3 G = ivec3(0);
	uvec3 H = G;
	ivec4 J = ivec4(0);
	uvec4 K = J;
	double L = A;
	double M = B;
	double N = C;

	for(int i = 0; i < A; ++i)
		Out.Lumimance[i] = vec4(1.0) / vec4(COUNT);

	Out.Texcoord = Texcoord;
	gl_Position = Transform.MVP * vec4(Position, 1.0);
}
