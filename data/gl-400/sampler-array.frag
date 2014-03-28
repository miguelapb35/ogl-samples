#version 400 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define INSTANCE	7
#define FRAG_COLOR	0

uniform sampler2DArray Diffuse[2];
uniform uint DiffuseIndex;

in block
{
	vec2 Texcoord;
	float Instance;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

vec4 sampling(in sampler2DArray Sampler[2], in int Layer, in vec2 Texcoord)
{
	return texture(Sampler[DiffuseIndex], vec3(Texcoord, Layer));
}

void main()
{
	Color = sampling(Diffuse, 0, In.Texcoord);
}
