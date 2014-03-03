#version 330 core
#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

uniform transform
{
	mat4 MVP;
} Transform;

layout(location = POSITION) in vec4 Position;

void main()
{	
	gl_Position = Transform.MVP * Position;
}

