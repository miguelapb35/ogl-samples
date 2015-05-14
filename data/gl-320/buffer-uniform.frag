#version 150 core

layout(std140) uniform;

uniform material
{
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	float Shininess;
} Material;

uniform light
{
	vec3 Position; // Camera space
} Light;

in block
{
	vec3 Normal;
	vec3 View;
	vec3 Color;
} In;

out vec4 Color;

void main()
{
	vec3 N = normalize(In.Normal);
	vec3 L = normalize(Light.Position + In.View);
	vec3 V = normalize(In.View);

	vec3 Diffuse = max(dot(N, L), 0.0) * Material.Diffuse.rgb;
	vec3 R = reflect(-L, N);
	vec3 Specular = pow(max(dot(R, V), 0.0), Material.Shininess) * Material.Specular.rgb;

	Color = vec4(Material.Ambient.rgb + Diffuse + Specular, 1.0);
}
