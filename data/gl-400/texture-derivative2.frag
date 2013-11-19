#version 150 core

uniform sampler2D Diffuse;

in block
{
	vec2 Texcoord;
} In;

out vec4 Color;

void main()
{
	vec2 Texcoord00 = interpolateAtOffset(In.Texcoord, vec2(0.0, 0.0));
	vec2 Texcoord10 = interpolateAtOffset(In.Texcoord, vec2(1.0, 0.0));
	vec2 Texcoord11 = interpolateAtOffset(In.Texcoord, vec2(1.0, 1.0));
	vec2 Texcoord01 = interpolateAtOffset(In.Texcoord, vec2(0.0, 1.0));
	vec2 Derivative = abs(Texcoord10 - Texcoord00) + abs(Texcoord01 - Texcoord00);
	Color = vec4(Derivative, 0.0, 1.0);
	//Color = vec4(fwidth(Texcoord), 0.0, 1.0);
}
