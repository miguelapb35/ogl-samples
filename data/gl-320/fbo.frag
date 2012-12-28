#version 150 core

uniform sampler2D Diffuse;

in vec4 gl_FragCoord;
out vec4 FragColor;

void main()
{
	vec2 TextureSize = vec2(textureSize(Diffuse, 0));

	FragColor = texture(Diffuse, gl_FragCoord.xy / TextureSize);
	//FragColor = texelFetch(Diffuse, ivec2(gl_FragCoord.xy), 0);
}
