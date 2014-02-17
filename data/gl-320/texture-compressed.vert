#version 320 core

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0

uniform mat4 MVP;

in vec2 Position;
in vec2 Texcoord;

out block
{
	vec2 Texcoord;
} Out;

void main()
{	
	Out.Texcoord = Texcoord;
	gl_Position = MVP * vec4(Position, 0.0, 1.0);
}
