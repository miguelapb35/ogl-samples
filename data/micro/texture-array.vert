#version 420 core

#define POSITION		0
#define COLOR			3
#define TEXCOORD		4
#define INSTANCE		7
#define FRAG_COLOR		0
#define TRANSFORM0	1

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

layout(binding = TRANSFORM0) uniform transform
{
	mat4 MVP;
} Transform;

layout(location = POSITION) in vec2 Position;
layout(location = TEXCOORD) in vec2 Texcoord;

out gl_PerVertex
{
	vec4 gl_Position;
	float gl_PointSize;
	float gl_ClipDistance[];
};

out block
{
	vec2 Texcoord;
	float Instance;
} Out;

mat4 ortho(float left, float right, float bottom, float top)
{
	mat4 m = mat4(1.0);
	
	m[0][0] = 2.0 / (right - left);
	m[1][1] = 2.0 / (top - bottom);
	m[2][2] = - 1.0;
	m[3][0] = - (right + left) / (right - left);
	m[3][1] = - (top + bottom) / (top - bottom);
	
	return m;
}

void main()
{	
	Out.Instance = gl_InstanceID;
	Out.Texcoord = Texcoord;
	//gl_Position = vec4(Position / vec2(160, 120) - vec2(1, 1), 0, 1);
	//gl_Position = ortho(0.0, 320.0, 0.0, 240.0) * vec4(Position, 0, 1);
	gl_Position = Transform.MVP * vec4(Position, 0, 1);
}
