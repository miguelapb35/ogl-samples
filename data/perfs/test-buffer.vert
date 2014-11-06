#version 330 core
#extension GL_ARB_shading_language_420pack : require

#define TRANSFORM0		1

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

layout(binding = TRANSFORM0) uniform transform
{
	mat4 MVP;
} Transform;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec4 Color;

out gl_PerVertex
{
	vec4 gl_Position;
};

out block
{
	vec4 Color;
} Out;

void main()
{
	Out.Color = Color;
	
	gl_Position = Transform.MVP * vec4(Position, 1);
}
