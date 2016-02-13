///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Samples Pack (ogl-samples.g-truc.net)
///
/// Copyright (c) 2004 - 2014 G-Truc Creation (www.g-truc.net)
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
///////////////////////////////////////////////////////////////////////////////////

#include "test.hpp"
#include "test_buffer_interleaved.hpp"

#define TEST_U8VEC1				1
#define TEST_U8VEC2				2
#define TEST_U8VEC3				3
#define TEST_U8VEC4				4
#define TEST_U10VEC3_U2VEC1		5
#define TEST_F16VEC2			6
#define TEST_F16VEC3			7
#define TEST_F16VEC4			8
#define TEST_F32VEC2			9
#define TEST_F32VEC3			10
#define TEST_F32VEC4			11
#define TEST_F64VEC2			12
#define TEST_F64VEC3			13
#define TEST_F64VEC4			14
#define TEST TEST_U8VEC4

namespace
{
	char const * VERT_SHADER_SOURCE_F32("micro/test_buffer.vert");
	char const * VERT_SHADER_SOURCE_F64("micro/test_buffer_double.vert");
	char const * FRAG_SHADER_SOURCE("micro/test_buffer.frag");
	std::size_t const COLOR_ARRAY = 6;
	glm::uvec2 const SCALE(4, 2);

	struct attrib
	{
		GLint Size;
		GLenum Type;
		GLboolean Normalized;
		const void* Offset;
	};

#	if TEST == TEST_F64VEC2
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 0) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 1) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 2) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 3) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 4) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 5) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 6) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 7) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 8) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 9) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 10) },
			{ 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec2) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(1.0f, 0.0f)
				, ColorB(0.0f, 0.5f)
				, ColorC(0.0f, 0.0f)
				, ColorD(0.0f, 0.0f)
			{}

			glm::vec2 Position;
			glm::dvec2 ColorA;
			glm::dvec2 ColorB;
			glm::dvec2 ColorC;
			glm::dvec2 ColorD;
			glm::dvec2 Color[8];
		};

#	elif TEST == TEST_F64VEC3
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 0) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 1) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 2) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 3) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 4) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 5) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 6) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 7) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 8) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 9) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 10) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(1.0f, 0.0f, 0.0f)
				, ColorB(0.0f, 0.5f, 0.0f)
				, ColorC(0.0f, 0.0f, 0.0f)
				, ColorD(0.0f, 0.0f, 0.0f)
			{}

			glm::vec2 Position;
			glm::dvec3 ColorA;
			glm::dvec3 ColorB;
			glm::dvec3 ColorC;
			glm::dvec3 ColorD;
			glm::dvec3 Color[8];
		};

#	elif TEST == TEST_F64VEC4
		attrib const Attribs[] =
		{
			{ 4, GL_DOUBLE, GL_FALSE, 0},
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 0) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 1) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 2) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 3) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 4) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 5) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 6) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 7) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 8) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec4) + sizeof(glm::dvec4) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(glm::dvec4(Position, 0.0f, 1.0f))
				, ColorA(1.0f, 0.0f, 0.0f, 0.0f)
				, ColorB(0.0f, 0.5f, 0.0f, 0.0f)
				, ColorC(0.0f, 0.0f, 0.0f, 0.0f)
				, ColorD(0.0f, 0.0f, 0.0f, 1.0f)
			{}

			glm::dvec4 Position;
			glm::dvec4 ColorA;
			glm::dvec4 ColorB;
			glm::dvec4 ColorC;
			glm::dvec4 ColorD;
			glm::dvec4 Color[1];
		};

#	elif TEST == TEST_F32VEC2
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 0) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 1) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 2) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 3) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 4) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 5) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 6) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 7) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 8) },
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec2) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(1.0f, 0.0f)
				, ColorB(0.0f, 0.5f)
				, ColorC(0.0f, 0.0f)
				, ColorD(0.0f, 0.0f)
			{}

			glm::vec2 Position;
			glm::vec2 ColorA;
			glm::vec2 ColorB;
			glm::vec2 ColorC;
			glm::vec2 ColorD;
			glm::vec2 Color[8];
		};

#	elif TEST == TEST_F32VEC3
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 0) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 1) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 2) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 3) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 4) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 5) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 6) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 7) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 8) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec3) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(1.0f, 0.0f, 0.0f)
				, ColorB(0.0f, 0.5f, 0.0f)
				, ColorC(0.0f, 0.0f, 0.0f)
				, ColorD(0.0f, 0.0f, 0.0f)
			{}

			glm::vec2 Position;
			glm::vec3 ColorA;
			glm::vec3 ColorB;
			glm::vec3 ColorC;
			glm::vec3 ColorD;
			glm::vec3 Color[8];
		};

#	elif TEST == TEST_F32VEC4
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 0) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 1) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 2) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 3) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 4) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 5) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 6) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 7) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 8) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(1.0f, 0.0f, 0.0f, 0.0f)
				, ColorB(0.0f, 0.5f, 0.0f, 0.0f)
				, ColorC(0.0f, 0.0f, 0.0f, 0.0f)
				, ColorD(0.0f, 0.0f, 0.0f, 1.0f)
			{}

			glm::vec2 Position;
			glm::vec4 ColorA;
			glm::vec4 ColorB;
			glm::vec4 ColorC;
			glm::vec4 ColorD;
			glm::vec4 Color[8];
		};

#	elif TEST == TEST_F16VEC4
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 0) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 1) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 2) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 3) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 4) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 5) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 6) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 7) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 8) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(1.0f))
			{}

			glm::vec2 Position;
			glm::u16vec4 ColorA;
			glm::u16vec4 ColorB;
			glm::u16vec4 ColorC;
			glm::u16vec4 ColorD;
			glm::u16vec4 Color[8];
		};

#	elif TEST == TEST_F16VEC3
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 0) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 1) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 2) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 3) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 4) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 5) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 6) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 7) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 8) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 9) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 10) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f), glm::packHalf1x16(0.0f))
				, ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
			{}

			glm::vec2 Position;
			glm::u16vec3 ColorA;
			glm::u16vec3 ColorB;
			glm::u16vec3 ColorC;
			glm::u16vec3 ColorD;
			glm::u16vec3 Color[8];
		};

#	elif TEST == TEST_F16VEC2
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 0) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 1) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 2) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 3) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 4) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 5) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 6) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 7) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 8) },
			{ 2, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec2) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f))
				, ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f))
				, ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
				, ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
			{}

			glm::vec2 Position;
			glm::u16vec2 ColorA;
			glm::u16vec2 ColorB;
			glm::u16vec2 ColorC;
			glm::u16vec2 ColorD;
			glm::u16vec2 Color[8];
		};

#	elif TEST == TEST_U10VEC3_U2VEC1
		attrib const Attribs[] =
		{
			{ 4, GL_FLOAT, GL_FALSE, 0},
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 0) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 1) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 2) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 3) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 4) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 5) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 6) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 7) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 8) },
			{ 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec4) + sizeof(glm::uint32) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(glm::vec4(Position, 0.0f, 1.0f))
				, ColorA(glm::packUnorm3x10_1x2(glm::vec4(1.0f, 0, 0, 0)))
				, ColorB(glm::packUnorm3x10_1x2(glm::vec4(0, 0.5f, 0, 0)))
				, ColorC(glm::packUnorm3x10_1x2(glm::vec4(0, 0, 0, 0)))
				, ColorD(glm::packUnorm3x10_1x2(glm::vec4(0, 0, 0, 1.0f)))
			{}

			glm::vec4 Position;
			glm::uint32 ColorA;
			glm::uint32 ColorB;
			glm::uint32 ColorC;
			glm::uint32 ColorD;
			glm::uint32 Color[COLOR_ARRAY];
		};

#	elif TEST == TEST_U8VEC4
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 0) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 1) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 2) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 3) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 4) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 5) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 6) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 7) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 8) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(255, 0, 0, 0)
				, ColorB(0, 127, 0, 0)
				, ColorC(0, 0, 0, 0)
				, ColorD(0, 0, 0, 255)
			{}

			glm::vec2 Position;
			glm::u8vec4 ColorA;
			glm::u8vec4 ColorB;
			glm::u8vec4 ColorC;
			glm::u8vec4 ColorD;
			glm::u8vec4 Color[8];
		};

#	elif TEST == TEST_U8VEC3
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 0) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 1) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 2) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 3) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 4) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 5) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 6) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 7) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 8) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 9) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 10) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(255, 0, 0)
				, ColorB(0, 127, 0)
				, ColorC(0, 0, 0)
				, ColorD(0, 0, 0)
			{}

			glm::vec2 Position;
			glm::u8vec3 ColorA;
			glm::u8vec3 ColorB;
			glm::u8vec3 ColorC;
			glm::u8vec3 ColorD;
			glm::u8vec3 Color[8];
		};

#	elif TEST == TEST_U8VEC2
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 0) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 1) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 2) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 3) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 4) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 5) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 6) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 7) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 8) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 9) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 10) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec2) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(255, 0)
				, ColorB(0, 127)
				, ColorC(0, 0)
				, ColorD(0, 0)
			{}

			glm::vec2 Position;
			glm::u8vec2 ColorA;
			glm::u8vec2 ColorB;
			glm::u8vec2 ColorC;
			glm::u8vec2 ColorD;
			glm::u8vec2 Color[8];
		};

#	elif TEST == TEST_U8VEC1
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, 0},
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 0) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 1) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 2) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 3) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 4) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 5) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 6) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 7) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 8) },
			{ 1, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec1) * 9) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib) == 5 + COLOR_ARRAY, "Invalid table size");

		struct vertex
		{
			vertex(){}
			vertex(glm::vec2 const & Position)
				: Position(Position)
				, ColorA(255)
				, ColorB(0)
				, ColorC(0)
				, ColorD(0)
			{}

			glm::vec2 Position;
			glm::u8vec1 ColorA;
			glm::u8vec1 ColorB;
			glm::u8vec1 ColorC;
			glm::u8vec1 ColorD;
			glm::u8vec1 Color[COLOR_ARRAY];
		};

#	endif//TEST

	namespace buffer
	{
		enum type
		{
			VERTEX,
			ELEMENT,
			TRANSFORM,
			MAX
		};
	}//namespace buffer
}//namespace

class test_buffer_interleaved : public test
{
public:
	test_buffer_interleaved(int argc, char* argv[], std::size_t FrameCount, glm::uvec2 const & WindowSize)
		: test(argc, argv, "test_buffer_interleaved", test::CORE, 3, 3, FrameCount, RUN_ONLY, WindowSize)
		, VertexArrayName(0)
		, ProgramName(0)
		, ElementCount(0)
	{}

private:
	std::array<GLuint, buffer::MAX> BufferName;
	GLuint VertexArrayName;
	GLuint PipelineName;
	GLuint ProgramName;
	GLsizei ElementCount;
	typedef std::vector<vertex> vertexData;
	vertexData VertexData;
	std::vector<glm::uint32> ElementData;

	bool initProgram()
	{
		bool Validated = true;
	
		if(Validated)
		{
			compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + (TEST == TEST_F64VEC4 ? VERT_SHADER_SOURCE_F64 : VERT_SHADER_SOURCE_F32));
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE);

			ProgramName = glCreateProgram();
			glProgramParameteri(this->ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(this->ProgramName, VertShaderName);
			glAttachShader(this->ProgramName, FragShaderName);
			glLinkProgram(this->ProgramName);

			Validated = Validated && Compiler.check();
			Validated = Validated && Compiler.check_program(this->ProgramName);
		}

		if(Validated)
		{
			glGenProgramPipelines(1, &this->PipelineName);
			glUseProgramStages(this->PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, this->ProgramName);
		}

		return Validated;
	}

	bool initBuffer()
	{
		glm::uvec2 const WindowSize = glm::uvec2(this->getWindowSize());
		glm::vec2 const ScaleInvert = 1.0f / glm::vec2(SCALE);
		glm::uvec2 const Size = WindowSize * SCALE;

		std::size_t const VertexCount = Size.x * Size.y * 4;
		VertexData.resize(VertexCount);
		ElementData.resize(Size.x * Size.y * 6);
		this->ElementCount = static_cast<GLsizei>(ElementData.size());

		for(glm::uint32 j = 0; j < Size.y >> 1; ++j)
		for(glm::uint32 i = 0; i < Size.x >> 1; ++i)
		{
			glm::uint32 Index(i + j * (static_cast<glm::uint32>(Size.x) >> 1));
			VertexData[Index * 4 + 0] = vertex(glm::vec2(i * 2 + 0, j * 2 + 0));
			VertexData[Index * 4 + 1] = vertex(glm::vec2(i * 2 + 2, j * 2 + 0));
			VertexData[Index * 4 + 2] = vertex(glm::vec2(i * 2 + 2, j * 2 + 2));
			VertexData[Index * 4 + 3] = vertex(glm::vec2(i * 2 + 0, j * 2 + 2));
			ElementData[Index * 6 + 0] = Index * 4 + 0;
			ElementData[Index * 6 + 1] = Index * 4 + 1;
			ElementData[Index * 6 + 2] = Index * 4 + 2;
			ElementData[Index * 6 + 3] = Index * 4 + 2;
			ElementData[Index * 6 + 4] = Index * 4 + 3;
			ElementData[Index * 6 + 5] = Index * 4 + 0;
		}

		glm::mat4 Perspective = glm::ortho(0.0f, static_cast<float>(WindowSize.x), 0.0f, static_cast<float>(WindowSize.y));

		glGenBuffers(buffer::MAX, &BufferName[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementData.size() * sizeof(glm::uint32), &ElementData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(vertex), &this->VertexData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Perspective), &Perspective[0][0], GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return true;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
			for (std::size_t i = 0; i < sizeof(Attribs) / sizeof(attrib); ++i)
			{
				if(Attribs[i].Type == GL_DOUBLE)
					glVertexAttribLPointer(static_cast<GLuint>(i), Attribs[i].Size, Attribs[i].Type, sizeof(vertex), Attribs[i].Offset);
				else
					glVertexAttribPointer(static_cast<GLuint>(i), Attribs[i].Size, Attribs[i].Type, Attribs[i].Normalized, sizeof(vertex), Attribs[i].Offset);
				glEnableVertexAttribArray(static_cast<GLuint>(i));
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		return true;
	}

	bool init()
	{
		bool Validated = true;
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initVertexArray();
		if(Validated)
			Validated = initProgram();
		return Validated;
	}

	bool begin()
	{
		bool Validated = this->init();

		if(Validated)
		{
			glm::vec2 WindowSize(this->getWindowSize());
			glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

			glBindProgramPipeline(this->PipelineName);
			glBindVertexArray(this->VertexArrayName);
			glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, this->BufferName[buffer::TRANSFORM]);
			//glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.2f, 0.7f, 0.3f, 1.0f)[0]);

			//glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LESS);
		}

		return Validated;
	}

	bool end()
	{
		glDeleteBuffers(buffer::MAX, &this->BufferName[0]);
		glDeleteProgramPipelines(1, &this->PipelineName);
		glDeleteProgram(this->ProgramName);
		glDeleteVertexArrays(1, &this->VertexArrayName);

		return true;
	}

	bool render()
	{
		glDrawElementsInstanced(GL_TRIANGLES, this->ElementCount, GL_UNSIGNED_INT, 0, 1);

		return true;
	}
};

struct entry
{
	entry(
		std::string const & String,
		glm::uvec2 const & WindowSize,
		glm::vec2 const & TileSize,
		std::size_t const & TrianglePairPerTile,
		std::size_t const & DrawPerTile
	) :
		String(String),
		WindowSize(WindowSize),
		TileSize(TileSize),
		TrianglePairPerTile(TrianglePairPerTile),
		DrawPerTile(DrawPerTile)
	{}

	std::string const String;
	glm::uvec2 const WindowSize;
	glm::vec2 const TileSize;
	std::size_t const TrianglePairPerTile;
	std::size_t const DrawPerTile;
};

int main_buffer_interleaved(int argc, char* argv[])
{
	std::vector<entry> Entries;

	for(glm::uint TileSizeIndex = 3; TileSizeIndex < 4; ++TileSizeIndex)
	{	
		for(std::size_t DrawPerTile = 1; DrawPerTile <= 512; DrawPerTile <<= 1)
			Entries.push_back(entry(
				message_format("window(%d), tile(%d), triangle-per-draw(%d)", 64 * (TileSizeIndex + 1), 8 * (TileSizeIndex + 1), 1024 / DrawPerTile),
				glm::uvec2(64) * (TileSizeIndex + 1), glm::vec2(glm::uvec2(8, 8) * (TileSizeIndex + 1)), 512, DrawPerTile));
	}

	csv CSV;
	int Error(0);

	test_buffer_interleaved Test(argc, argv, 10000, glm::uvec2(256));

	Error += Test();
	Test.log(CSV, "GNI");

	CSV.save("../main_buffer_interleaved.csv");

	return Error;
}

