#version 450 core
#extension GL_NV_shader_thread_group : require

#define POSITION	0
#define COLOR		3
#define TEXCOORD	4
#define FRAG_COLOR	0
#define TRANSFORM0	1
#define DIFFUSE		0

in block
{
	vec2 Texcoord;
} In;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	vec4 Helper = mix(vec4(0.0), vec4(1.0, 0.0, 0.0, 1.0) * 0.25, gl_HelperInvocation);
	
	vec4 HelperColor0 = quadSwizzle0NV(Helper);
	vec4 HelperColor1 = quadSwizzle1NV(Helper);
	vec4 HelperColor2 = quadSwizzle2NV(Helper);
	vec4 HelperColor3 = quadSwizzle3NV(Helper);

	Color = HelperColor0 + HelperColor1 + HelperColor2 + HelperColor3;
}
