#include "test.hpp"
#include "test_buffer_separated.hpp"

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
#define TEST TEST_F32VEC4

namespace
{
	char const* VERT_SHADER_SOURCE_F32("micro/test_buffer.vert");
	char const* VERT_SHADER_SOURCE_F64("micro/test_buffer_double.vert");
	char const* FRAG_SHADER_SOURCE("micro/test_buffer.frag");
	std::size_t const COLOR_ARRAY = 6;
	glm::uvec2 const SCALE(6, 6);

	struct attrib
	{
		GLint Size;
		GLenum Type;
		GLboolean Normalized;
		GLsizei Stride;
		GLvoid const* Pointer;
	};

#	if TEST == TEST_F64VEC2
		attrib const Attribs[] = { 2, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec2) * 0) };

		struct vertex
		{
			vertex() :
				ColorA(1.0f * 0.1f, 0.5f * 0.1f)
			{}

			glm::dvec2 ColorA;
		};

#	elif TEST == TEST_F64VEC3
		attrib const Attribs[] =
		{
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 0) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 1) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 2) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 3) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 4) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 5) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 6) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 7) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 8) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 9) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 10) },
			{ 3, GL_DOUBLE, GL_FALSE, BUFFER_OFFSET(sizeof(glm::dvec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib), "Invalid table size");

		struct vertex
		{
			vertex() :
				ColorA(1.0f, 0.0f, 0.0f),
				ColorB(0.0f, 0.5f, 0.0f),
				ColorC(0.0f, 0.0f, 0.0f),
				ColorD(0.0f, 0.0f, 0.0f)
			{}

			glm::dvec3 ColorA;
			glm::dvec3 ColorB;
			glm::dvec3 ColorC;
			glm::dvec3 ColorD;
			glm::dvec3 Color[8];
		};

#	elif TEST == TEST_F64VEC4
		attrib const Attribs ={ 4, GL_DOUBLE, GL_FALSE, sizeof(glm::dvec4), NULL };

		struct color
		{
			color() :
				ColorA(0.1f, 0.05f, 0.0f, 0.0f)
			{}

			glm::dvec4 ColorA;
		};

#	elif TEST == TEST_F32VEC2
		attrib const Attribs = { 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL };

		struct color
		{
			color() :
				ColorA(0.10f, 0.05f)
			{}

			glm::vec2 ColorA;
		};

#	elif TEST == TEST_F32VEC3
		attrib const Attribs[] =
		{
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 0) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 1) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 2) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 3) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 4) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 5) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 6) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 7) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 8) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 9) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 10) },
			{ 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::vec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib), "Invalid table size");

		struct vertex
		{
			vertex() :
				ColorA(1.0f, 0.0f, 0.0f),
				ColorB(0.0f, 0.5f, 0.0f),
				ColorC(0.0f, 0.0f, 0.0f),
				ColorD(0.0f, 0.0f, 0.0f)
			{}

			glm::vec3 ColorA;
			glm::vec3 ColorB;
			glm::vec3 ColorC;
			glm::vec3 ColorD;
			glm::vec3 Color[8];
		};

#	elif TEST == TEST_F32VEC4
		attrib const Attribs = { 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL};

		struct color
		{
			color() :
				ColorA(1.0f * 0.1f, 0.5f * 0.1f, 0.0f, 0.0f)
			{}

			glm::vec4 ColorA;
		};

#	elif TEST == TEST_F16VEC4
		attrib const Attribs = { 4, GL_HALF_FLOAT, GL_FALSE, sizeof(glm::u16vec4), NULL};

		struct color
		{
			color() :
				ColorA(glm::packHalf1x16(1.0f * 0.1f), glm::packHalf1x16(0.5f * 0.1f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
			{}

			glm::u16vec4 ColorA;
		};

#	elif TEST == TEST_F16VEC3
		attrib const Attribs[] =
		{
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 0) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 1) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 2) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 3) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 4) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 5) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 6) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 7) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 8) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 9) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 10) },
			{ 3, GL_HALF_FLOAT, GL_FALSE, BUFFER_OFFSET(sizeof(glm::u16vec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib), "Invalid table size");

		struct vertex
		{
			vertex() :
				ColorA(glm::packHalf1x16(1.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
				ColorB(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.5f), glm::packHalf1x16(0.0f)),
				ColorC(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f)),
				ColorD(glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f), glm::packHalf1x16(0.0f))
			{}

			glm::u16vec3 ColorA;
			glm::u16vec3 ColorB;
			glm::u16vec3 ColorC;
			glm::u16vec3 ColorD;
			glm::u16vec3 Color[8];
		};

#	elif TEST == TEST_F16VEC2
		attrib const Attribs = { 2, GL_HALF_FLOAT, GL_FALSE, sizeof(glm::u16vec2), NULL };

		struct color
		{
			color() :
				ColorA(glm::packHalf1x16(0.10f), glm::packHalf1x16(0.05f))
			{}

			glm::u16vec2 ColorA;
		};

#	elif TEST == TEST_U10VEC3_U2VEC1
		attrib const Attribs = { 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_TRUE, sizeof(glm::uint32), NULL };

		struct color
		{
			color() :
				ColorA(glm::packUnorm3x10_1x2(glm::vec4(0.10f, 0.05f, 0, 0)))
			{}

			glm::uint32 ColorA;
		};

#	elif TEST == TEST_U8VEC4
		attrib const Attribs = { 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(glm::u8vec4), NULL };

		struct color
		{
			color() :
				ColorA(255 / 10, 127 / 10, 0, 0)
			{}

			glm::u8vec4 ColorA;
		};
		static_assert(sizeof(color) == sizeof(glm::u8vec4), "GNI");

#	elif TEST == TEST_U8VEC3
		attrib const Attribs[] =
		{
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 0) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 1) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 2) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 3) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 4) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 5) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 6) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 7) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 8) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 9) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 10) },
			{ 3, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec3) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib), "Invalid table size");

		struct vertex
		{
			vertex() :
				ColorA(255, 0, 0),
				ColorB(0, 127, 0),
				ColorC(0, 0, 0),
				ColorD(0, 0, 0)
			{}

			glm::u8vec3 ColorA;
			glm::u8vec3 ColorB;
			glm::u8vec3 ColorC;
			glm::u8vec3 ColorD;
			glm::u8vec3 Color[8];
		};

#	elif TEST == TEST_U8VEC2
		attrib const Attribs[] =
		{
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 0) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 1) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 2) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 3) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 4) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 5) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 6) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 7) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 8) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 9) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 10) },
			{ 2, GL_UNSIGNED_BYTE, GL_TRUE, BUFFER_OFFSET(sizeof(glm::u8vec2) * 11) }
		};
		static_assert(sizeof(Attribs) / sizeof(attrib), "Invalid table size");

		struct vertex
		{
			vertex() :
				ColorA(255, 0),
				ColorB(0, 127),
				ColorC(0, 0),
				ColorD(0, 0)
			{}

			glm::u8vec2 ColorA;
			glm::u8vec2 ColorB;
			glm::u8vec2 ColorC;
			glm::u8vec2 ColorD;
			glm::u8vec2 Color[8];
		};

#	endif//TEST

	namespace buffer
	{
		enum type
		{
			POSITION,
			COLOR0,
			COLOR1,
			COLOR2,
			COLOR3,
			COLOR4,
			COLOR5,
			COLOR6,
			COLOR7,
			COLOR8,
			COLOR9,
			ELEMENT,
			TRANSFORM,
			MAX
		};
	}//namespace buffer
}//namespace

class test_buffer_separated : public framework
{
public:
	test_buffer_separated(int argc, char* argv[], std::size_t FrameCount, glm::uvec2 const & WindowSize)
		: framework(argc, argv, "test_buffer_separated", framework::CORE, 3, 3, FrameCount, RUN_ONLY, WindowSize)
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
	typedef std::vector<glm::vec2> positionData;
	typedef std::vector<color> colorData;
	positionData PositionData;
	colorData ColorData[buffer::COLOR9 - buffer::COLOR0 + 1];
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

	int const ColorCount = buffer::COLOR9 - buffer::COLOR0 + 1;

	bool initBuffer()
	{
		glm::uvec2 const WindowSize = glm::uvec2(this->getWindowSize());
		glm::vec2 const ScaleInvert = 1.0f / glm::vec2(SCALE);
		glm::uvec2 const Size = WindowSize * SCALE;

		std::size_t const VertexCount = Size.x * Size.y * 4;
		PositionData.resize(VertexCount);
		for (int i = 0; i < ColorCount; ++i)
			ColorData[i].resize(VertexCount);
		ElementData.resize(Size.x * Size.y * 6);
		this->ElementCount = static_cast<GLsizei>(ElementData.size());

		for(glm::uint32 j = 0; j < Size.y >> 1; ++j)
		for(glm::uint32 i = 0; i < Size.x >> 1; ++i)
		{
			glm::uint32 Index(i + j * (static_cast<glm::uint32>(Size.x) >> 1));
			PositionData[Index * 4 + 0] = glm::vec2(i * 2 + 0, j * 2 + 0) * ScaleInvert;
			PositionData[Index * 4 + 1] = glm::vec2(i * 2 + 2, j * 2 + 0) * ScaleInvert;
			PositionData[Index * 4 + 2] = glm::vec2(i * 2 + 2, j * 2 + 2) * ScaleInvert;
			PositionData[Index * 4 + 3] = glm::vec2(i * 2 + 0, j * 2 + 2) * ScaleInvert;
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementData.size() * sizeof(glm::uint32), &this->ElementData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::POSITION]);
		glBufferData(GL_ARRAY_BUFFER, PositionData.size() * sizeof(glm::vec2), &this->PositionData[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		for (int i = 0; i < ColorCount; ++i)
		{
			assert(!ColorData[i].empty());
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::COLOR0 + i]);
			glBufferData(GL_ARRAY_BUFFER, ColorData[i].size() * sizeof(color), &this->ColorData[i][0], GL_STATIC_DRAW);
		}
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
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::POSITION]);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), BUFFER_OFFSET(0));
			glEnableVertexAttribArray(0);

			for (int i = 0; i < ColorCount; ++i)
			{
				glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::COLOR0 + i]);
				if(Attribs.Type == GL_DOUBLE)
					glVertexAttribLPointer(static_cast<GLuint>(buffer::COLOR0 + i), Attribs.Size, Attribs.Type, 0, nullptr);
				else
					glVertexAttribPointer(static_cast<GLuint>(buffer::COLOR0 + i), Attribs.Size, Attribs.Type, Attribs.Normalized, 0, Attribs.Pointer);
				glEnableVertexAttribArray(static_cast<GLuint>(buffer::COLOR0 + i));
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

int main_buffer_separated(int argc, char* argv[])
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

	test_buffer_separated Test(argc, argv, 10000, glm::uvec2(256));

	Error += Test();
	Test.log(CSV, "GNI");

	CSV.save("../main_buffer.csv");

	return Error;
}

