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
/// @file gli/generate_mipmaps.inl
/// @date 2015-10-06 / 2015-10-06
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/sampler2d.hpp>

namespace gli
{
	inline gli::texture2D generate_mipmaps(gli::texture2D const & Texture, gli::texture2D::size_type BaseLevel, gli::texture2D::size_type MaxLevel)
	{
		gli::fsampler2D Sampler(Texture, gli::WRAP_CLAMP_TO_EDGE, gli::FILTER_LINEAR, gli::FILTER_LINEAR);
		Sampler.generate_mipmaps(BaseLevel, MaxLevel);

		return Sampler();
	}

	inline gli::texture2D generate_mipmaps(gli::texture2D const & Texture)
	{
		return generate_mipmaps(Texture, Texture.base_level(), Texture.max_level());
	}
}//namespace gli
