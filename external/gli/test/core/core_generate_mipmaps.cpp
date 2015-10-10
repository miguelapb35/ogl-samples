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
/// @file gli/core/core_generate_mipmaps.cpp
/// @date 2015-10-05 / 2015-10-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/comparison.hpp>
#include <gli/type.hpp>
#include <gli/view.hpp>
#include <gli/copy.hpp>
#include <gli/generate_mipmaps.hpp>

#include <glm/gtc/epsilon.hpp>

namespace mipmaps
{
	int test()
	{
		int Error = 0;

		gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(64));
		for(std::size_t j = 0, n = Texture.dimensions().y; j < n; ++j)
		for(std::size_t i = 0, m = Texture.dimensions().x; i < n; ++i)
			Texture.store<gli::u8vec4>(gli::dim2_t(i , j), 0, gli::u8vec4(i * 4, j * 4, 0, 255));

		gli::texture2D TextureView(gli::view(Texture, 0, 0));

		// Custom mipmaps generation using a sampler object
		gli::fsampler2D SamplerA(gli::texture2D(gli::copy(Texture)), gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		SamplerA.generate_mipmaps();

		gli::texture2D MipmapsA = SamplerA();
		gli::texture2D MipmapViewA(gli::view(MipmapsA, 0, 0));

		Error += TextureView == MipmapViewA ? 0 : 1;

		// Mipmaps generation using the wrapper function
		gli::texture2D MipmapsB = gli::generate_mipmaps(gli::texture2D(gli::copy(Texture)));
		gli::texture2D MipmapViewB(gli::view(MipmapsB, 0, 0));

		Error += TextureView == MipmapViewB ? 0 : 1;

		// Compare custom mipmaps generation and wrapper mipmaps generation
		Error += MipmapViewA == MipmapViewB ? 0 : 1;
		Error += MipmapsA == MipmapsB ? 0 : 1;

		return Error;
	}
}//namespace mipmaps

int main()
{
	int Error = 0;

	Error += mipmaps::test();

	return Error;
}

