#version 400 core

uniform sampler2D Diffuse;
uniform sampler2DShadow Shadow;

const float Spread = 1.5;
const int SampleCount = 8;
const vec2 Offsets[SampleCount] = vec2[](
	vec2( 0.7, 0.3),
	vec2( 0.3,-0.7),
	vec2(-0.7,-0.3),
	vec2(-0.3, 0.7),
	vec2( 0.7,-0.3),
	vec2(-0.3,-0.7),
	vec2(-0.7, 0.3),
	vec2( 0.3, 0.7));

in block
{
	vec4 Color;
	vec4 ShadowCoord;
} In;

out vec4 Color;

void main()
{
	vec4 ShadowCoord = In.ShadowCoord;
	ShadowCoord.z -= 0.005;

	vec4 Diffuse = In.Color;

	vec2 ShadowSize = vec2(textureSize(Shadow, 0));
	float Size = max(ShadowSize.x, ShadowSize.y);

	float Visibility = 0.0;
	for(int SampleIndex = 0; SampleIndex < 8; ++SampleIndex)
		Visibility += mix(0.5, 1.0, texture(Shadow, ShadowCoord.xyz + vec3(Offsets[SampleIndex] * Spread, 0.0) / Size));

	Color = Visibility / float(SampleCount) * Diffuse;
}
