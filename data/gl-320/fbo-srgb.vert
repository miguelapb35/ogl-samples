#version 150 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform transform
{
	mat4 MVP;
} Transform;

in vec3 Position;
in vec4 Color;

out block
{
	vec4 Color;
} Out;

void main()
{	
	Out.Color = Color;
	gl_Position = Transform.MVP * vec4(Position, 1.0);
}
