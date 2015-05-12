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
/// @file gli/core/format_info.hpp
/// @date 2015-05-10 / 2015-05-10
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "format.hpp"
#include "type.hpp"

namespace gli{
namespace detail
{
	struct formatInfo
	{
		std::uint8_t BlockSize : 5;
		std::uint8_t BlockDimensionsX : 4;
		std::uint8_t BlockDimensionsY : 4;
		std::uint8_t Component : 3;
		std::uint16_t Flags : 16;
	};

	formatInfo const & getInfo(format const & Format);
}//namespace detail

	inline bool is_valid(format const & Format)
	{
		return Format >= FORMAT_FIRST && Format <= FORMAT_LAST;
	}

	inline bool is(format const & Format, info const & Info)
	{
		return (detail::getInfo(Format).Flags & Info) != 0;
	}

	inline size_t component_count(format const & Format)
	{
		return detail::getInfo(Format).Component;
	}

	inline size_t block_size(format const & Format)
	{
		return detail::getInfo(Format).BlockSize;
	}

	inline size3_t block_dimensions(format const & Format)
	{
		detail::formatInfo const & Info = detail::getInfo(Format);
		return size3_t(Info.BlockDimensionsX, Info.BlockDimensionsY, 1);
	}
}//namespace gli

#include "format_info.inl"
