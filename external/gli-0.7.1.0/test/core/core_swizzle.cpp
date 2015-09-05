///////////////////////////////////////////////////////////////////////////////////
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
/// @file gli/core/swizzle.cpp
/// @date 2015-09-05 / 2015-09-05
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/texture.hpp>

namespace swizzle
{
	int run()
	{
		int Error(0);

		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_RGBA8_UNORM, gli::texture::dim_type(1), 1, 1, 1);
			gli::texture::swizzles_type const Swizzles = Texture.swizzles();
			Error += Swizzles == gli::swizzles(gli::SWIZZLE_RED, gli::SWIZZLE_GREEN, gli::SWIZZLE_BLUE, gli::SWIZZLE_ALPHA) ? 0 : 1;
		}

		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_BGRA8_UNORM, gli::texture::dim_type(1), 1, 1, 1);
			gli::texture::swizzles_type const Swizzles = Texture.swizzles();
			Error += Swizzles == gli::swizzles(gli::SWIZZLE_BLUE, gli::SWIZZLE_GREEN, gli::SWIZZLE_RED, gli::SWIZZLE_ALPHA) ? 0 : 1;
		}

		{
			gli::texture Texture(gli::TARGET_2D, gli::FORMAT_BGRA8_UNORM, gli::texture::dim_type(1), 1, 1, 1, gli::swizzles(gli::SWIZZLE_BLUE, gli::SWIZZLE_GREEN, gli::SWIZZLE_RED, gli::SWIZZLE_ALPHA));
			gli::texture::swizzles_type const Swizzles = Texture.swizzles();
			Error += Swizzles == gli::swizzles(gli::SWIZZLE_RED, gli::SWIZZLE_GREEN, gli::SWIZZLE_BLUE, gli::SWIZZLE_ALPHA) ? 0 : 1;
		}

		return Error;
	}
}//namespace swizzle

int main()
{
	int Error(0);

	Error += swizzle::run();

	assert(!Error);

	return Error;
}

