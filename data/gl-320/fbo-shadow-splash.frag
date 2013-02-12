#version 150 core

uniform sampler2D Diffuse;

layout(origin_upper_left) in vec4 gl_FragCoord;
out vec4 Color;

void main()
{
	Color = texture(Diffuse, vec2(gl_FragCoord.x, 1.0 - gl_FragCoord.y) / vec2(640, 480));
}
