#version 150 core
#extension GL_ARB_shader_texture_image_samples : require

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform sampler2DMS Diffuse;

in block
{
	vec2 Texcoord;
} In;

out vec4 Color;

void main()
{
	// integer UV coordinates, needed for fetching multisampled texture
	ivec2 Texcoord = ivec2(textureSize(Diffuse) * In.Texcoord);

	vec4 Temp = vec4(0.0);
	
	// For each of the samples
	for(int i = 0; i < textureSamples(Diffuse); ++i)
		Temp += texelFetch(Diffuse, Texcoord, i);

	Color = Temp * 0.25;
}
