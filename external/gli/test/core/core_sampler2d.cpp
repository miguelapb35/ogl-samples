//////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/core/texture_2d.cpp
/// @date 2011-10-11 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/sampler2d.hpp>
#include <gli/comparison.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/epsilon.hpp>
#include <ctime>
#include <limits>
#include <array>

namespace load
{
	int test()
	{
		int Error(0);

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4, 2), 1);
		*(Texture.data<glm::u8vec4>() + 0) = glm::u8vec4(255,   0,   0, 255);
		*(Texture.data<glm::u8vec4>() + 1) = glm::u8vec4(255, 128,   0, 255);
		*(Texture.data<glm::u8vec4>() + 2) = glm::u8vec4(255, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 3) = glm::u8vec4(128, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 4) = glm::u8vec4(  0, 255,   0, 255);
		*(Texture.data<glm::u8vec4>() + 5) = glm::u8vec4(  0, 255, 255, 255);
		*(Texture.data<glm::u8vec4>() + 6) = glm::u8vec4(  0,   0, 255, 255);
		*(Texture.data<glm::u8vec4>() + 7) = glm::u8vec4(255,   0, 255, 255);

		glm::u8vec4 Data0 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(0, 0), 0);
		glm::u8vec4 Data1 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(1, 0), 0);
		glm::u8vec4 Data2 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(2, 0), 0);
		glm::u8vec4 Data3 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(3, 0), 0);
		glm::u8vec4 Data4 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(0, 1), 0);
		glm::u8vec4 Data5 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(1, 1), 0);
		glm::u8vec4 Data6 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(2, 1), 0);
		glm::u8vec4 Data7 = Texture.load<glm::u8vec4>(gli::texture2D::dim_type(3, 1), 0);

		Error += Data0 == glm::u8vec4(255,   0,   0, 255) ? 0 : 1;
		Error += Data1 == glm::u8vec4(255, 128,   0, 255) ? 0 : 1;
		Error += Data2 == glm::u8vec4(255, 255,   0, 255) ? 0 : 1;
		Error += Data3 == glm::u8vec4(128, 255,   0, 255) ? 0 : 1;
		Error += Data4 == glm::u8vec4(  0, 255,   0, 255) ? 0 : 1;
		Error += Data5 == glm::u8vec4(  0, 255, 255, 255) ? 0 : 1;
		Error += Data6 == glm::u8vec4(  0,   0, 255, 255) ? 0 : 1;
		Error += Data7 == glm::u8vec4(255,   0, 255, 255) ? 0 : 1;

		return Error;
	}
}//namespace load

namespace fetch_rgb10a2_snorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::uint32 Packed[8];
		for (std::size_t i = 0; i < 8; ++i)
			Packed[i] = glm::packSnorm3x10_1x2(Colors[i]);

		gli::texture2D Texture(gli::FORMAT_RGB10A2_SNORM, gli::texture2D::dim_type(4, 2), 1);
		for (std::size_t i = 0; i < 8; ++i)
			*(Texture.data<glm::uint32>() + i) = Packed[i];

		glm::uint32 Loaded[8];
		Loaded[0] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 0), 0);
		Loaded[1] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 0), 0);
		Loaded[2] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 0), 0);
		Loaded[3] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 0), 0);
		Loaded[4] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 1), 0);
		Loaded[5] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 1), 0);
		Loaded[6] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 1), 0);
		Loaded[7] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 1), 0);

		for (std::size_t i = 0; i < 8; ++i)
			Error += Packed[i] == Loaded[i] ? 0 : 1;

		glm::vec4 Unpacked[8];
		for (std::size_t i = 0; i < 8; ++i)
			Unpacked[i] = glm::unpackSnorm3x10_1x2(Loaded[i]);

		for (std::size_t i = 0; i < 8; ++i)
			Error += glm::all(glm::epsilonEqual(Unpacked[i], Colors[i], 0.01f)) ? 0 : 1;

		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;

		Error += glm::all(glm::epsilonEqual(Data0, Colors[0], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data1, Colors[1], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data2, Colors[2], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data3, Colors[3], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data4, Colors[4], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data5, Colors[5], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data6, Colors[6], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data7, Colors[7], Epsilon)) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgb10a2_snorm

namespace fetch_rgb10a2_unorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::uint32 Packed[8];
		for(std::size_t i = 0; i < 8; ++i)
			Packed[i] = glm::packUnorm3x10_1x2(Colors[i]);

		gli::texture2D Texture(gli::FORMAT_RGB10A2_UNORM, gli::texture2D::dim_type(4, 2), 1);
		for(std::size_t i = 0; i < 8; ++i)
			*(Texture.data<glm::uint32>() + i) = Packed[i];

		glm::uint32 Loaded[8];
		Loaded[0] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 0), 0);
		Loaded[1] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 0), 0);
		Loaded[2] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 0), 0);
		Loaded[3] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 0), 0);
		Loaded[4] = Texture.load<glm::uint32>(gli::texture2D::dim_type(0, 1), 0);
		Loaded[5] = Texture.load<glm::uint32>(gli::texture2D::dim_type(1, 1), 0);
		Loaded[6] = Texture.load<glm::uint32>(gli::texture2D::dim_type(2, 1), 0);
		Loaded[7] = Texture.load<glm::uint32>(gli::texture2D::dim_type(3, 1), 0);

		for(std::size_t i = 0; i < 8; ++i)
			Error += Packed[i] == Loaded[i] ? 0 : 1;

		glm::vec4 Unpacked[8];
		for(std::size_t i = 0; i < 8; ++i)
			Unpacked[i] = glm::unpackUnorm3x10_1x2(Loaded[i]);

		for (std::size_t i = 0; i < 8; ++i)
			Error += glm::all(glm::epsilonEqual(Unpacked[i], Colors[i], 0.01f)) ? 0 : 1;

		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;

		Error += glm::all(glm::epsilonEqual(Data0, Colors[0], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data1, Colors[1], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data2, Colors[2], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data3, Colors[3], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data4, Colors[4], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data5, Colors[5], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data6, Colors[6], Epsilon)) ? 0 : 1;
		Error += glm::all(glm::epsilonEqual(Data7, Colors[7], Epsilon)) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgb10a2_unorm

namespace fetch_rgba32_sfloat
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		gli::texture2D TextureA(gli::FORMAT_RGBA32_SFLOAT, gli::texture2D::dim_type(4, 2), 1);
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			*(TextureA.data<glm::vec4>() + i) = Colors[i];

		gli::fsampler2D SamplerA(TextureA, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 DataA[sizeof(Colors) / sizeof(Colors[0])];
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			DataA[i] = SamplerA.texel_fetch(gli::texture2D::dim_type(i % 4, i / 4), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::epsilonEqual(DataA[i], Colors[i], Epsilon)) ? 0 : 1;

		gli::texture2D TextureB(gli::FORMAT_RGBA32_SFLOAT, gli::texture2D::dim_type(8, 4));
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			TextureB.store<glm::vec4>(gli::dim2_t(i % 4, i / 4), 1, Colors[i]);

		gli::fsampler2D SamplerB(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 DataB[sizeof(Colors) / sizeof(Colors[0])];
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			DataB[i] = SamplerB.texel_fetch(gli::texture2D::dim_type(i % 4, i / 4), 1);

		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::epsilonEqual(DataB[i], Colors[i], Epsilon)) ? 0 : 1;

		gli::texture2D TextureC(TextureB, 1, 1);

		Error += TextureA == TextureC ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgba32_sfloat

namespace fetch_rgba64_sfloat
{
	int test()
	{
		int Error(0);

		glm::dvec4 Colors[] =
		{
			glm::dvec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::dvec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::dvec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::dvec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::dvec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::dvec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::dvec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::dvec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		gli::texture2D TextureA(gli::FORMAT_RGBA64_SFLOAT, gli::texture2D::dim_type(4, 2), 1);
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			*(TextureA.data<glm::dvec4>() + i) = Colors[i];

		gli::dsampler2D SamplerA(TextureA, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::dvec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::dvec4 DataA[sizeof(Colors) / sizeof(Colors[0])];
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			DataA[i] = SamplerA.texel_fetch(gli::texture2D::dim_type(i % 4, i / 4), 0);

		double const Epsilon = 1.f / 255.f * 0.5f;
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::epsilonEqual(DataA[i], Colors[i], Epsilon)) ? 0 : 1;

		gli::texture2D TextureB(gli::FORMAT_RGBA64_SFLOAT, gli::texture2D::dim_type(8, 4));
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			TextureB.store<glm::dvec4>(gli::dim2_t(i % 4, i / 4), 1, Colors[i]);

		gli::dsampler2D SamplerB(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::dvec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::dvec4 DataB[sizeof(Colors) / sizeof(Colors[0])];
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			DataB[i] = SamplerB.texel_fetch(gli::texture2D::dim_type(i % 4, i / 4), 1);

		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::epsilonEqual(DataB[i], Colors[i], Epsilon)) ? 0 : 1;

		gli::texture2D TextureC(TextureB, 1, 1);

		Error += TextureA == TextureC ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgba64_sfloat

namespace fetch_rgba8_unorm
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		gli::texture2D TextureA(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4, 2), 1);
		for (std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			*(TextureA.data<glm::u8vec4>() + i) = glm::u8vec4(Colors[i] * 255.f);

		gli::fsampler2D SamplerA(TextureA, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

		glm::vec4 Data[sizeof(Colors) / sizeof(Colors[0])];
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Data[i] = SamplerA.texel_fetch(gli::texture2D::dim_type(i % 4, i / 4), 0);

		float const Epsilon = 1.f / 255.f * 0.5f;
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::epsilonEqual(Data[i], Colors[i], Epsilon)) ? 0 : 1;

		gli::texture2D TextureB(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4, 2), 1);
		TextureB.clear(gli::u8vec4(0, 0, 0, 255));

		gli::fsampler2D SamplerB(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			SamplerB.texel_write(gli::texture2D::dim_type(i % 4, i / 4), 0, Data[i]);

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgba8_unorm

namespace fetch_rgba8_uint
{
	int test()
	{
		int Error(0);

		glm::u8vec4 Colors[] =
		{
			glm::u8vec4(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) * 255.f),
			glm::u8vec4(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) * 255.f)
		};

		gli::texture2D Texture(gli::FORMAT_RGBA8_UINT, gli::texture2D::dim_type(4, 2), 1);
		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			*(Texture.data<glm::u8vec4>() + i) = Colors[i];

		gli::usampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::u32vec4(0, 0, 0, 1));

		glm::u8vec4 Outputs[8];
		Outputs[0] = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
		Outputs[1] = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
		Outputs[2] = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
		Outputs[3] = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
		Outputs[4] = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
		Outputs[5] = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
		Outputs[6] = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
		Outputs[7] = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

		for(std::size_t i = 0, n = sizeof(Colors) / sizeof(Colors[0]); i < n; ++i)
			Error += glm::all(glm::equal(Outputs[i], Colors[i])) ? 0 : 1;

		return Error;
	}
}//namespace fetch_rgba8_uint

namespace fetch_rgba8_srgb
{
	int test()
	{
		int Error(0);

		glm::vec4 Colors[] =
		{
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.5f, 1.0f, 1.0f),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
		};

		glm::u8vec4 StoreSRGB00(glm::convertLinearToSRGB(Colors[0]) * 255.f);
		glm::u8vec4 StoreSRGB10(glm::convertLinearToSRGB(Colors[1]) * 255.f);
		glm::u8vec4 StoreSRGB20(glm::convertLinearToSRGB(Colors[2]) * 255.f);
		glm::u8vec4 StoreSRGB30(glm::convertLinearToSRGB(Colors[3]) * 255.f);
		glm::u8vec4 StoreSRGB01(glm::convertLinearToSRGB(Colors[4]) * 255.f);
		glm::u8vec4 StoreSRGB11(glm::convertLinearToSRGB(Colors[5]) * 255.f);
		glm::u8vec4 StoreSRGB21(glm::convertLinearToSRGB(Colors[6]) * 255.f);
		glm::u8vec4 StoreSRGB31(glm::convertLinearToSRGB(Colors[7]) * 255.f);

		gli::texture2D Texture(gli::FORMAT_RGBA8_SRGB, gli::texture2D::dim_type(4, 2), 1);
		Texture.store(gli::dim2_t(0, 0), 0, StoreSRGB00);
		Texture.store(gli::dim2_t(1, 0), 0, StoreSRGB10);
		Texture.store(gli::dim2_t(2, 0), 0, StoreSRGB20);
		Texture.store(gli::dim2_t(3, 0), 0, StoreSRGB30);
		Texture.store(gli::dim2_t(0, 1), 0, StoreSRGB01);
		Texture.store(gli::dim2_t(1, 1), 0, StoreSRGB11);
		Texture.store(gli::dim2_t(2, 1), 0, StoreSRGB21);
		Texture.store(gli::dim2_t(3, 1), 0, StoreSRGB31);

		{
			gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

			glm::vec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
			glm::vec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
			glm::vec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
			glm::vec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
			glm::vec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
			glm::vec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
			glm::vec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
			glm::vec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

			Error += glm::all(glm::epsilonEqual(Data0, Colors[0], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data1, Colors[1], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data2, Colors[2], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data3, Colors[3], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data4, Colors[4], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data5, Colors[5], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data6, Colors[6], 0.01f)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data7, Colors[7], 0.01f)) ? 0 : 1;
		}

		{
			gli::dsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::dvec4(0.0f, 0.5f, 1.0f, 1.0f));

			glm::dvec4 Data0 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 0), 0);
			glm::dvec4 Data1 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 0), 0);
			glm::dvec4 Data2 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 0), 0);
			glm::dvec4 Data3 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 0), 0);
			glm::dvec4 Data4 = Sampler.texel_fetch(gli::texture2D::dim_type(0, 1), 0);
			glm::dvec4 Data5 = Sampler.texel_fetch(gli::texture2D::dim_type(1, 1), 0);
			glm::dvec4 Data6 = Sampler.texel_fetch(gli::texture2D::dim_type(2, 1), 0);
			glm::dvec4 Data7 = Sampler.texel_fetch(gli::texture2D::dim_type(3, 1), 0);

			Error += glm::all(glm::epsilonEqual(Data0, glm::dvec4(Colors[0]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data1, glm::dvec4(Colors[1]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data2, glm::dvec4(Colors[2]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data3, glm::dvec4(Colors[3]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data4, glm::dvec4(Colors[4]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data5, glm::dvec4(Colors[5]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data6, glm::dvec4(Colors[6]), 0.01)) ? 0 : 1;
			Error += glm::all(glm::epsilonEqual(Data7, glm::dvec4(Colors[7]), 0.01)) ? 0 : 1;
		}

		return Error;
	}
}//namespace fetch_rgba8_srgb

/*
namespace sampler
{
	int test()
	{
		int Error(0);

		gli::texture2D::dim_type const Size(512);
		gli::texture2D TextureA(gli::FORMAT_RGBA8_UNORM, Size, 1);

		{
			std::clock_t TimeStart = std::clock();

			for(std::size_t y = 0; y < Size.y; ++y)
			for(std::size_t x = 0; x < Size.x; ++x)
			{
				gli::texel_write(TextureA, gli::texture2D::dim_type(x, y), 0, glm::u8vec4(255, 128,   0, 255));
			}

			std::clock_t TimeEnd = std::clock();

			printf("texel_write(texture2D) - Time: %lu\n", TimeEnd - TimeStart);
		}

		gli::texture2D TextureB(gli::FORMAT_RGBA8_UNORM, Size, 1);

		{
			gli::sampler2D<float> Sampler(TextureB, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

			{
				std::clock_t TimeStart = std::clock();

				for(std::size_t y = 0; y < Size.y; ++y)
				for(std::size_t x = 0; x < Size.x; ++x)
				{
					Sampler.texel_write<glm::tvec4, glm::u8>(gli::texture2D::dim_type(x, y), 0, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
				}

				std::clock_t TimeEnd = std::clock();

				printf("sampler2D::texel_write - Time: %lu\n", TimeEnd - TimeStart);
			}

			{
				std::clock_t TimeStart = std::clock();

				for(float y = -0.5f; y < 1.5f; y += 0.025f)
				for(float x = -0.5f; x < 1.5f; x += 0.025f)
				{
					glm::vec4 ColorA = Sampler.texture_lod<glm::tvec4, glm::u8>(gli::texture2D::texcoord_type(x, y), 0.0f);
					Error += glm::all(glm::equal(ColorA, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f))) ? 0 : 1;
				}

				std::clock_t TimeEnd = std::clock();

				printf("sampler2D::texture_lod - Time: %lu\n", TimeEnd - TimeStart);
			}
		}

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace sampler
*/
namespace wrap_clamp_to_border
{
	int test()
	{
		int Error(0);

		glm::vec4 const Orange(1.0f, 0.5f, 0.0f, 1.0f);
		glm::vec4 const Blue(0.0f, 0.5f, 1.0f, 1.0f);

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(32), 1);
		Texture.clear(glm::packUnorm4x8(Orange));

		gli::fsampler2D SamplerA(Texture, gli::WRAP_CLAMP_TO_BORDER, gli::FILTER_LINEAR, gli::FILTER_LINEAR, Blue);

		{
			std::array<gli::fsampler2D::samplecoord_type, 8> SampleCoord{
			{
				gli::fsampler2D::samplecoord_type( 0.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.1f),
				gli::fsampler2D::samplecoord_type( 0.5f, 0.1f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.5f, 0.9f),
				gli::fsampler2D::samplecoord_type( 0.9f, 0.9f),
				gli::fsampler2D::samplecoord_type( 0.9f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.9f)
			}};

			for(std::size_t i = 0, n = SampleCoord.size(); i < n; ++i)
			{
				gli::vec4 const Texel = SamplerA.texture_lod(SampleCoord[i], 0.0f);
				Error += glm::all(glm::epsilonEqual(Texel, Orange, 0.01f)) ? 0 : 1;
			}
		}
		{
			std::array<gli::fsampler2D::samplecoord_type, 8> SampleCoord{
			{
				gli::fsampler2D::samplecoord_type( 0.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 1.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 1.5f, 1.5f),
				gli::fsampler2D::samplecoord_type( 0.5f, 1.5f),
				gli::fsampler2D::samplecoord_type( 1.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f, 1.5f)
			}};

			for(std::size_t i = 0, n = SampleCoord.size(); i < n; ++i)
			{
				gli::vec4 const Texel = SamplerA.texture_lod(SampleCoord[i], 0.0f);
				Error += glm::all(glm::epsilonEqual(Texel, Blue, 0.01f)) ? 0 : 1;
			}
		}

		return Error;
	}
}//namespace wrap_clamp_to_border

namespace wrap_mirror
{
	int test()
	{
		int Error(0);

		glm::vec4 const Orange(1.0f, 0.5f, 0.0f, 1.0f);
		glm::vec4 const Blue(0.0f, 0.5f, 1.0f, 1.0f);

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(32), 1);
		Texture.clear(glm::packUnorm4x8(Orange));

		gli::fsampler2D Sampler(Texture, gli::WRAP_REPEAT, gli::FILTER_LINEAR, gli::FILTER_LINEAR, Blue);

		{
			std::array<gli::fsampler2D::samplecoord_type, 16> SampleCoord{
			{
				gli::fsampler2D::samplecoord_type( 0.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.1f),
				gli::fsampler2D::samplecoord_type( 0.5f, 0.1f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.5f, 0.9f),
				gli::fsampler2D::samplecoord_type( 0.9f, 0.9f),
				gli::fsampler2D::samplecoord_type( 0.9f, 0.5f),
				gli::fsampler2D::samplecoord_type( 0.1f, 0.9f),
				gli::fsampler2D::samplecoord_type( 0.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 1.5f, 0.5f),
				gli::fsampler2D::samplecoord_type( 1.5f, 1.5f),
				gli::fsampler2D::samplecoord_type( 0.5f, 1.5f),
				gli::fsampler2D::samplecoord_type( 1.5f,-0.5f),
				gli::fsampler2D::samplecoord_type(-0.5f, 1.5f)
			}};

			for(std::size_t i = 0, n = SampleCoord.size(); i < n; ++i)
			{
				gli::vec4 const Texel = Sampler.texture_lod(SampleCoord[i], 0.0f);
				Error += glm::all(glm::epsilonEqual(Texel, Orange, 0.01f)) ? 0 : 1;
			}
		}

		return Error;
	}
}//namespace wrap_mirror

namespace texture_lod
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4), 1);
			Texture.clear(gli::u8vec4(0, 0, 0, 255));
			Texture.store(gli::dim2_t(0, 0), 0, gli::u8vec4(255, 127,   0, 255));
			Texture.store(gli::dim2_t(1, 0), 0, gli::u8vec4(255, 127,   0, 255));
			Texture.store(gli::dim2_t(0, 1), 0, gli::u8vec4(255, 127,   0, 255));
			Texture.store(gli::dim2_t(1, 1), 0, gli::u8vec4(255, 127,   0, 255));
			Texture.store(gli::dim2_t(2, 2), 0, gli::u8vec4(  0, 127, 255, 255));
			Texture.store(gli::dim2_t(3, 2), 0, gli::u8vec4(  0, 127, 255, 255));
			Texture.store(gli::dim2_t(2, 3), 0, gli::u8vec4(  0, 127, 255, 255));
			Texture.store(gli::dim2_t(3, 3), 0, gli::u8vec4(  0, 127, 255, 255));

			gli::sampler2D<float> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f));

			gli::vec4 SampleA = Sampler.texture_lod(gli::fsampler2D::samplecoord_type(0.25f), 0.0f);
			Error += gli::all(gli::epsilonEqual(SampleA, gli::vec4(1.0f, 0.5f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;

			gli::vec4 SampleB = Sampler.texture_lod(gli::fsampler2D::samplecoord_type(0.8f), 0.0f);
			Error += gli::all(gli::epsilonEqual(SampleB, gli::vec4(0.0f, 0.5f, 1.0f, 1.0f), 0.01f)) ? 0 : 1;

			gli::vec4 SampleC = Sampler.texture_lod(gli::fsampler2D::samplecoord_type(0.20f, 0.8f), 0.0f);
			Error += gli::all(gli::epsilonEqual(SampleC, gli::vec4(0.0f, 0.0f, 0.0f, 1.0f), 0.01f)) ? 0 : 1;
		}

		return Error;
	}
}//namespace texture_lod

namespace sampler_type
{
	int test()
	{
		int Error = 0;

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4), 1);
			gli::sampler2D<float> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4), 1);
			gli::sampler2D<double> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(4), 1);
			gli::sampler2D<int> Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		}

		return Error;
	}
}//namespace sampler_type

int main()
{
	int Error(0);

	Error += texture_lod::test();
	Error += load::test();
	Error += sampler_type::test();

	Error += fetch_rgba32_sfloat::test();
	Error += fetch_rgba64_sfloat::test();
	Error += fetch_rgba8_unorm::test();
	Error += fetch_rgba8_uint::test();
	Error += fetch_rgba8_srgb::test();
	Error += fetch_rgb10a2_unorm::test();
	Error += fetch_rgb10a2_snorm::test();
	//Error += sampler::test();
	Error += wrap_clamp_to_border::test();
	Error += wrap_mirror::test();

	return Error;
}

