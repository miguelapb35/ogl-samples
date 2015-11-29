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
/// @file gli/transform.hpp
/// @date 2015-11-27 / 2015-11-27
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "texture1d.hpp"
#include "texture1d_array.hpp"
#include "texture2d.hpp"
#include "texture2d_array.hpp"
#include "texture3d.hpp"
#include "texture_cube.hpp"
#include "texture_cube_array.hpp"

namespace gli
{
	template <typename vec_type>
	struct transform_func
	{
		typedef vec_type(*type)(vec_type const & A, vec_type const & B);
	};

	template <typename vec_type>
	void transform(texture1D & Out, texture1D const & In0, texture1D const & In1, typename transform_func<vec_type>::type TexelFunc);
	
	template <typename vec_type>
	void transform(texture1DArray & Out, texture1DArray const & In0, texture1DArray const & In1, typename transform_func<vec_type>::type Func);
	
	template <typename vec_type>
	void transform(texture2D & Out, texture2D const & In0, texture2D const & In1, typename transform_func<vec_type>::type Func);
	
	template <typename vec_type>
	void transform(texture2DArray & Out, texture2DArray const & In0, texture2DArray const & In1, typename transform_func<vec_type>::type Func);
	
	template <typename vec_type>
	void transform(texture3D & Out, texture3D const & In0, texture3D const & In1, typename transform_func<vec_type>::type Func);

	template <typename vec_type>
	void transform(textureCube & Out, textureCube const & In0, textureCube const & In1, typename transform_func<vec_type>::type Func);
	
	template <typename vec_type>
	void transform(textureCubeArray & Out, textureCubeArray const & In0, textureCubeArray const & In1, typename transform_func<vec_type>::type Func);
	
}//namespace gli

#include "./core/transform.inl"
