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
#include "test_buffer.hpp"

#define TEST_U8VEC4		1
#define TEST_F16VEC4	2
#define TEST_F16VEC3	3
#define TEST_F32VEC4	4
#define TEST_F64VEC4	5
#define TEST TEST_F16VEC3

namespace
{
	char const * VERT_SHADER_SOURCE_F32("micro/test_buffer.vert");
	char const * VERT_SHADER_SOURCE_F64("micro/test_buffer_double.vert");
	char const * FRAG_SHADER_SOURCE("micro/test_buffer.frag");

	struct vertex_u8color4
	{
		vertex_u8color4() {}

		vertex_u8color4(glm::vec2 const & Position) :
			Position(Position),
			ColorA(255, 0, 0, 0),
			ColorB(0, 127, 0, 0),
			ColorC(0, 0, 0, 0),
			ColorD(0, 0, 0, 255)
		{}

		glm::vec2 Position;
		glm::u8vec4 ColorA;
		glm::u8vec4 ColorB;
		glm::u8vec4 ColorC;
		glm::u8vec4 ColorD;
		glm::u8vec4 Color[8];
	};

	struct vertex_f16color3
	{
		vertex_f16color3() {}

		vertex_f16color3(glm::vec2 const & Position) :
			Position(Position),
			ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
			ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f), glm::packHalf1x16(0.0f)),
			ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
			ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
		{}

		glm::vec2 Position;
		glm::u16vec3 ColorA;
		glm::u16vec3 ColorB;
		glm::u16vec3 ColorC;
		glm::u16vec3 ColorD;
		glm::u16vec3 Color[8];
	};

	struct vertex_f16color4
	{
		vertex_f16color4() {}

		vertex_f16color4(glm::vec2 const & Position) :
			Position(Position),
			ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
			ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
			ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
			ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(1.0f))
		{}

		glm::vec2 Position;
		glm::u16vec4 ColorA;
		glm::u16vec4 ColorB;
		glm::u16vec4 ColorC;
		glm::u16vec4 ColorD;
		glm::u16vec4 Color[8];
	};

	struct vertex_f32color4
	{
		vertex_f32color4() {}

		vertex_f32color4(glm::vec2 const & Position) :
			Position(Position),
			ColorA(1.0f, 0.0f, 0.0f, 0.0f),
			ColorB(0.0f, 0.5f, 0.0f, 0.0f),
			ColorC(0.0f, 0.0f, 0.0f, 0.0f),
			ColorD(0.0f, 0.0f, 0.0f, 1.0f)
		{}

		glm::vec2 Position;
		glm::vec4 ColorA;
		glm::vec4 ColorB;
		glm::vec4 ColorC;
		glm::vec4 ColorD;
		glm::vec4 Color[8];
	};

	struct vertex_f64color4
	{
		vertex_f64color4() {}

		vertex_f64color4(glm::vec2 const & Position) :
			Position(Position),
			ColorA(1.0f, 0.0f, 0.0f, 0.0f),
			ColorB(0.0f, 0.5f, 0.0f, 0.0f),
			ColorC(0.0f, 0.0f, 0.0f, 0.0f),
			ColorD(0.0f, 0.0f, 0.0f, 1.0f)
		{}

		glm::vec2 Position;
		glm::dvec4 ColorA;
		glm::dvec4 ColorB;
		glm::dvec4 ColorC;
		glm::dvec4 ColorD;
		glm::dvec4 Color[8];
	};

	struct attrib
	{
		GLint Size;
		GLenum Type;
		GLboolean Normalized;
		const void* Offset;
	};

	std::size_t const AttribSize = 13;

#	if TEST == TEST_F64VEC4
		attrib const Attribs[] =
		{
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(0) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2)) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 1) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 2) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 3) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 4) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 5) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 6) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 7) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 8) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 9) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 10) },
			{ 4, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::dvec4) * 11) }
		};
		typedef vertex_f64color4 vertex;

#	elif TEST == TEST_F32VEC4
		attrib const Attribs[AttribSize] =
		{
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(0) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2)) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 1) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 2) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 3) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 4) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 5) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 6) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 7) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 8) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 9) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 10) },
			{ 4, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::vec4) * 11) }
		};
		typedef vertex_f32color4 vertex;

#	elif TEST == TEST_F16VEC4
		attrib const Attribs[AttribSize] =
		{
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(0) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2)) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 1) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 2) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 3) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 4) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 5) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 6) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 7) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 8) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 9) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 10) },
			{ 4, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u16vec4) * 11) }
		};
		typedef vertex_f16color4 vertex;

#	elif TEST == TEST_F16VEC3
	attrib const Attribs[AttribSize] =
	{
		{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(0) },
		{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec2)) },
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
	typedef vertex_f16color3 vertex;

#	elif TEST == TEST_U8VEC4
		attrib const Attribs[AttribSize] =
		{
			{ 2, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(0) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2)) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 1) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 2) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 3) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 4) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 5) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 6) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 7) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 8) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 9) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 10) },
			{ 4, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::vec2) + sizeof(glm::u8vec4) * 11) }
		};
		typedef vertex_u8color4 vertex;

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

class test_buffer : public test
{
public:
	test_buffer(int argc, char* argv[], std::size_t FrameCount, glm::uvec2 const & WindowSize)
		: test(argc, argv, "test_buffer", test::CORE, 3, 3, FrameCount, RUN_ONLY, WindowSize)
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
			Validated = Validated && Compiler.checkProgram(this->ProgramName);
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
		glm::uvec2 const WindowSize = glm::uvec2(this->getWindowSize()) * glm::uvec2(4);

		std::size_t Size = sizeof(vertex);
		VertexData.resize(WindowSize.x * WindowSize.y * 4);
		ElementData.resize(WindowSize.x * WindowSize.y * 6);
		this->ElementCount = static_cast<GLsizei>(ElementData.size());

		for(glm::uint32 j = 0; j < WindowSize.y >> 1; ++j)
		for(glm::uint32 i = 0; i < WindowSize.x >> 1; ++i)
		{
			glm::uint32 Index(i + j * (static_cast<glm::uint32>(WindowSize.x) >> 1));
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
			glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.5f, 1.0f, 1.0f)[0]);

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
		glm::uvec2 const WindowSize(this->getWindowSize());
/*
		for(std::size_t j = 0; j < WindowSize.y >> 1; ++j)
		for(std::size_t i = 0; i < WindowSize.x >> 1; ++i)
		{
			std::size_t Index(i + j * (static_cast<std::size_t>(WindowSize.x) >> 1));
			VertexData[Index * 4 + 0] = vertex(glm::vec3(i * 2 + 0, j * 2 + 0, 0), this->generateColorU8(), this->generateColor(), this->generateColor(), this->generateColor());
			VertexData[Index * 4 + 1] = vertex(glm::vec3(i * 2 + 2, j * 2 + 0, 0), this->generateColorU8(), this->generateColor(), this->generateColor(), this->generateColor());
			VertexData[Index * 4 + 2] = vertex(glm::vec3(i * 2 + 2, j * 2 + 2, 0), this->generateColorU8(), this->generateColor(), this->generateColor(), this->generateColor());
			VertexData[Index * 4 + 3] = vertex(glm::vec3(i * 2 + 0, j * 2 + 2, 0), this->generateColorU8(), this->generateColor(), this->generateColor(), this->generateColor());
			ElementData[Index * 6 + 0] = Index * 4 + 0;
			ElementData[Index * 6 + 1] = Index * 4 + 1;
			ElementData[Index * 6 + 2] = Index * 4 + 2;
			ElementData[Index * 6 + 3] = Index * 4 + 2;
			ElementData[Index * 6 + 4] = Index * 4 + 3;
			ElementData[Index * 6 + 5] = Index * 4 + 0;
		}
*/
		this->beginTimer();
			glDrawElementsInstanced(GL_TRIANGLES, this->ElementCount, GL_UNSIGNED_INT, 0, 1);
		this->endTimer();

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

int main_buffer(int argc, char* argv[])
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

	test_buffer Test(argc, argv, 10000, glm::uvec2(256));

	Error += Test();
	Test.log(CSV, "GNI");

	CSV.save("../main_buffer.csv");

	return Error;
}

