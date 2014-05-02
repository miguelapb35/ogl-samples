#version 150 core

#ifndef GEN_ERROR // If this is not declared, the compiler should generate an error
layout(triangles) in;
#endif

precision highp float;
precision highp int;
layout(std140, column_major) uniform;
layout(triangle_strip, max_vertices = 4) out;

/*
const vec4 Diffuse[] = {
	vec4(1.0, 0.0, 0.0, 1.0),
	vec4(0.0, 1.0, 0.0, 1.0),
	vec4(0.0, 0.0, 1.0, 1.0)
};
*/
in block
{
	vec4 Color;
} In[];

out block
{
	vec4 Color;
} Out;

void main()
{
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		Out.Color = In[i].Color;
		EmitVertex();
	}
	EndPrimitive();
}

