#version 150 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform sampler2D Diffuse;

in vec2 gl_PointCoord;

in block
{
	vec4 Color;
} In;

out vec4 Color;

void main()
{
	if(length(gl_PointCoord - 0.5) > 0.5)
		discard;

	Color = In.Color * texture(Diffuse, gl_PointCoord);
}

