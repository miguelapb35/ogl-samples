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
/// @file gli/sampler2d.hpp
/// @date 2015-09-29 / 2015-09-29
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "sampler.hpp"
#include "texture2d.hpp"
#include "core/convert.hpp"

namespace gli
{
	template <typename T, glm::precision P = glm::defaultp>
	class sampler2D : public sampler
	{
	public:
		typedef texture2D::size_type size_type;
		typedef texture2D::dim_type dim_type;
		typedef vec2 samplecoord_type;

		sampler2D(texture2D const & Texture, wrap Wrap, filter Mip, filter Min, glm::tvec4<T, P> const & BorderColor = glm::tvec4<T, P>(0, 0, 0, 1));

		texture2D const & operator()() const;

		glm::tvec4<T, P> texel_fetch(dim_type const & TexelCoord, size_type const & Level) const;

		void texel_write(dim_type const & TexelCoord, size_type const & Level, glm::tvec4<T, P> const & Texel);

		void clear(glm::tvec4<T, P> const & Color);

		void clear(glm::tvec4<T, P> const & Color, dim_type const & TexelOffset, dim_type const & TexelDim, size_type Level);

		glm::tvec4<T, P> texture_lod(samplecoord_type const & Texcoord, float Level) const;

		/// 
		void generate_mipmaps();

		/// 
		void generate_mipmaps(size_type BaseLevel, size_type MaxLevel);

	private:
		glm::tvec4<T, P> texture_lod_nearest(samplecoord_type const & Texcoord, size_type Level) const;

		glm::tvec4<T, P> texture_lod_linear(samplecoord_type const & Texcoord, size_type Level) const;

		texture2D Texture;
		typename detail::convert<texture2D, T, P>::func Convert;
		glm::tvec4<T, P> BorderColor;
	};

	typedef sampler2D<float> fsampler2D;
	typedef sampler2D<double> dsampler2D;
	typedef sampler2D<unsigned int> usampler2D;
	typedef sampler2D<int> isampler2D;

}//namespace gli

#include "./core/sampler2d.inl"
