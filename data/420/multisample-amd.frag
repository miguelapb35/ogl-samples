#version 410 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

layout(binding = 0) uniform sampler2D Diffuse;

layout(location = TEXCOORD) in vec2 Texcoord;
layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

void main()
{
	FragColor = texture(Diffuse, interpolateAtSample(Texcoord, gl_SampleID));
}
