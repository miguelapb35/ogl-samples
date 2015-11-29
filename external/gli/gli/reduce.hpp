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
/// @file gli/reduce.hpp
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
	struct reduce_func
	{
		typedef vec_type(*type)(vec_type const & A, vec_type const & B);
	};

	template <typename vec_type>
	vec_type reduce(texture1D const & In0, texture1D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(texture1DArray const & In0, texture1DArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(texture2D const & In0, texture2D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(texture2DArray const & In0, texture2DArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(texture3D const & In0, texture3D const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(textureCube const & In0, textureCube const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename vec_type>
	vec_type reduce(textureCubeArray const & In0, textureCubeArray const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

	template <typename texture_type, typename vec_type>
	vec_type reduce(texture_type const & In0, texture_type const & In1, typename reduce_func<vec_type>::type TexelFunc, typename reduce_func<vec_type>::type ReduceFunc);

}//namespace gli

#include "./core/reduce.inl"
