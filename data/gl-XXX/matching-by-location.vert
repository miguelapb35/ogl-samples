#version 420 core

#define POSITION		0
#define COLOR			3
#define TEXCOORD		4
#define FRAG_COLOR		0

#define TRANSFORM0		1

layout(binding = TRANSFORM0) uniform transform
{
	mat4 MVP;
} Transform;

layout(location = POSITION) in vec2 Position;
layout(location = COLOR) in vec4 AttribColor;

out gl_PerVertex
{
	vec4 gl_Position;
};

out vec4 VertColor;

void main()
{	
	gl_Position = Transform.MVP * vec4(Position, 0.0, 1.0);
	VertColor = AttribColor;
}
