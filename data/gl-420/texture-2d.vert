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
	mediump int Value = mediump int(0);
	highp int HighValue = highp int(Value);

	for(int i = int(HighValue); i < int(COUNT); ++i)
		Out.Lumimance[i] = vec4(1.0) / vec4(COUNT);

	Out.Texcoord = Texcoord;
	gl_Position = Transform.MVP * vec4(Position, 1.0);
}
