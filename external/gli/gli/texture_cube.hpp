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
/// @file gli/texture_cube.hpp
/// @date 2011-04-06 / 2013-01-11
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture2d.hpp"

namespace gli
{
	/// textureCube
	class textureCube : public texture
	{
	public:
		typedef dim2_t dim_type;
		typedef vec3 samplecoord_type;

	public:
		/// Create an empty texture cube
		textureCube();

		/// Create a textureCube and allocate a new storage
		explicit textureCube(
			format_type Format,
			dim_type const & Dimensions,
			size_type Levels);

		/// Create a textureCube and allocate a new storage with a complete mipmap chain
		explicit textureCube(
			format_type Format,
			dim_type const & Dimensions);

		/// Create a textureCube view with an existing storage
		explicit textureCube(
			texture const & Texture);

		/// Create a textureCube view with an existing storage
		explicit textureCube(
			texture const & Texture,
			format_type Format,
			size_type BaseLayer, size_type MaxLayer,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a textureCube view, reference a subset of an existing textureCube instance
		explicit textureCube(
			textureCube const & Texture,
			size_type BaseFace, size_type MaxFace,
			size_type BaseLevel, size_type MaxLevel);

		/// Create a view of the texture identified by Face in the texture cube
		texture2D operator[](size_type Face) const;

		/// Return the dimensions of a texture instance: width and height where both should be equal.
		dim_type dimensions(size_type Level = 0) const;

		/// Fetch a texel from a texture. The texture format must be uncompressed.
		template <typename genType>
		genType load(textureCube::dim_type const & TexelCoord, textureCube::size_type Face, textureCube::size_type Level) const;

		/// Write a texel to a texture. The texture format must be uncompressed.
		template <typename genType>
		void store(textureCube::dim_type const & TexelCoord, textureCube::size_type Face, textureCube::size_type Level, genType const & Texel);

	private:
		struct cache
		{
			std::uint8_t* Data;
			textureCube::size_type Size;
			textureCube::dim_type Dim;
		};

		void build_cache();
		size_type index_cache(size_type Face, size_type Level) const;

		std::vector<cache> Caches;

	};
}//namespace gli

#include "./core/texture_cube.inl"
