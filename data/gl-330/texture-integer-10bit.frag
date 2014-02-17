#version 330 core
#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

uniform usampler2D Diffuse;

in block
{
	vec2 Texcoord;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	uvec4 IntColor = texture(Diffuse, In.Texcoord);

	Color = vec4(IntColor.rgb, 1023.0) / 1023.0;
}
