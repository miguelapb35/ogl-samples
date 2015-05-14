#version 150 core

layout(std140) uniform;

uniform transform
{
	mat4 P;
	mat4 MV;
	mat3 Normal;
} Transform;

in vec3 Position;
in vec3 Normal;
in vec4 Color;

out block
{
	vec3 Normal;
	vec3 View;
	vec3 Color;
} Out;

void main()
{
	vec4 P = Transform.MV * vec4(Position, 1.0);

	Out.Normal = mat3(Transform.MV) * Normal;
	Out.View = -P.xyz;
	Out.Color = Color.rgb;

	gl_Position = Transform.P * P;
}
