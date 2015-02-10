#version 150 core

precision highp float;
precision highp int;
layout(std140, column_major) uniform;

uniform sampler2D Diffuse;

in block
{
	vec2 Texcoord;
} In;

out vec4 Color;

/////////////
// rgbToSrgb


vec3 detail_rgbToSrgb(in vec3 ColorRGB, in float GammaCorrection)
{
	vec3 ClampedColorRGB = clamp(ColorRGB, 0.0, 1.0);

	return mix(
		pow(ClampedColorRGB, vec3(GammaCorrection)) * 1.055 - 0.055,
		ClampedColorRGB * 12.92,
		lessThan(ClampedColorRGB, vec3(0.0031308)));
}

vec3 rgbToSrgb(in vec3 ColorRGB, in float Gamma)
{
	return detail_rgbToSrgb(ColorRGB, 1.0 / Gamma);
}

vec3 rgbToSrgb(in vec3 ColorRGB)
{
	return detail_rgbToSrgb(ColorRGB, 0.41666);
}

vec4 rgbToSrgb(in vec4 ColorRGB, in float Gamma)
{
	return rgbToSrgb(vec4(vec3(ColorRGB), ColorRGB.a), Gamma);
}

vec4 rgbToSrgb(in vec4 ColorRGB)
{
	return rgbToSrgb(vec4(vec3(ColorRGB), ColorRGB.a), 0.41666);
}

/////////////
// srgbToRgb

vec3 srgbToRgb(in vec3 ColorSRGB, in float Gamma)
{
	return mix(
		pow((ColorSRGB + 0.055) * 0.94786729857819905213270142180095, vec3(Gamma)),
		ColorSRGB * 0.07739938080495356037151702786378,
		lessThanEqual(ColorSRGB, vec3(0.04045)));
}

vec3 srgbToRgb(in vec3 ColorSRGB)
{
	return srgbToRgb(ColorSRGB, 2.4);
}

vec4 srgbToRgb(in vec4 ColorSRGB, in float Gamma)
{
	return vec4(srgbToRgb(ColorSRGB.rgb, Gamma), ColorSRGB.a);
}

vec4 srgbToRgb(in vec4 ColorSRGB)
{
	return vec4(srgbToRgb(ColorSRGB.rgb, 2.4), ColorSRGB.a);
}

void main()
{
	vec3 ColorRGB = texture(Diffuse, In.Texcoord).rgb;

	Color = vec4(rgbToSrgb(srgbToRgb(rgbToSrgb(ColorRGB))), 1.0);
	//Color = vec4(rgbToSrgb(ColorRGB), 1.0);
}
