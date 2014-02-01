#include "test_draw_indexing.hpp"

namespace
{
	GLsizei const VertexCount(6);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const VertexData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f),
		glm::vec2(-1.0f,-1.0f)
	};

	struct drawArraysIndirectCommand
	{
		GLuint count;
		GLuint instanceCount;
		GLuint first;
		GLuint baseInstance;
	};

	char const * VERT_SHADER_SOURCE[testDrawIndexing::INDEXING_MAX] = {"hz-430/draw.vert", "hz-430/draw-indexing-uniform.vert", "hz-430/draw-indexing-attrib.vert", "hz-430/draw-indexing-attrib.vert", "hz-430/draw-indexing-attrib.vert", "hz-430/draw-indexing-id.vert"};
	char const * FRAG_SHADER_SOURCE[testDrawIndexing::INDEXING_MAX] = {"hz-430/draw.frag", "hz-430/draw-indexing-uniform.frag", "hz-430/draw-indexing-attrib.frag", "hz-430/draw-indexing-attrib.frag", "hz-430/draw-indexing-attrib.frag", "hz-430/draw-indexing-id.frag"};

	GLint UniformDrawIndex(-1);
}//namespace

testDrawIndexing::testDrawIndexing(
	int argc, char* argv[], profile Profile,
	indexing const Indexing,
	std::size_t const DrawCount
) :
	test(argc, argv, Profile, DEFAULT_MAX_FRAME, DEFAULT_WINDOW_SIZE),
	Indexing(Indexing),
	DrawCount(DrawCount),
	VertexArrayName(0),
	PipelineName(0),
	ProgramName(0)
{
	bool Success = true;
	
	Success = Success && this->isExtensionSupported("GL_ARB_multi_draw_indirect");
	assert(Success);
	Success = Success && this->initProgram();
	assert(Success);
	Success = Success && this->initBuffer();
	assert(Success);
	Success = Success && this->initVertexArray();
	assert(Success);

	glEnable(GL_DEPTH_TEST);
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::PER_FRAME, this->BufferName[buffer::BUFFER_FRAME]);
	glBindProgramPipeline(this->PipelineName);
	glBindVertexArray(this->VertexArrayName);
	
	if(this->Indexing == DRAW || this->Indexing == ID_INDEXING || this->Indexing == DIVISOR_INDEXING || this->Indexing == DIVISOR_MULTI_INDEXING)
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, BufferName[BUFFER_INDIRECT]);
}

testDrawIndexing::~testDrawIndexing()
{
	glDeleteBuffers(static_cast<GLsizei>(BUFFER_MAX), &this->BufferName[0]);
	glDeleteProgramPipelines(1, &this->PipelineName);
	glDeleteProgram(this->ProgramName);
	glDeleteVertexArrays(1, &this->VertexArrayName);
}

bool testDrawIndexing::initProgram()
{
	bool Validated(true);
	
	glf::compiler Compiler;
	GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE[this->Indexing], "--version 420 --profile core");
	GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE[this->Indexing], "--version 420 --profile core");
	Validated = Validated && Compiler.check();

	if(Validated)
	{
		this->ProgramName = glCreateProgram();
		glProgramParameteri(this->ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(this->ProgramName, VertShaderName);
		glAttachShader(this->ProgramName, FragShaderName);
		glLinkProgram(this->ProgramName);
		Validated = Validated && glf::checkProgram(this->ProgramName);
	}

	if(Validated)
	{
		if(this->Indexing == UNIFORM_INDEXING)
			UniformDrawIndex = glGetUniformLocation(this->ProgramName, "DrawID");
	}

	glGenProgramPipelines(1, &this->PipelineName);
	if(Validated)
		glUseProgramStages(this->PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, this->ProgramName);

	return Validated;
}

bool testDrawIndexing::initBuffer()
{
	glGenBuffers(BUFFER_MAX, &this->BufferName[0]);

	glBindBuffer(GL_UNIFORM_BUFFER, this->BufferName[BUFFER_FRAME]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_ARRAY]);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, &VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::vector<short> DrawID;
	DrawID.resize(this->DrawCount);
	for(std::size_t i = 0; i < DrawID.size(); ++i)
		DrawID[i] = i % 2;
	glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_DIVISOR]);
	glBufferData(GL_ARRAY_BUFFER, this->DrawCount * sizeof(short), &DrawID[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	std::vector<drawArraysIndirectCommand> Commands;
	Commands.resize(this->DrawCount);
	for(std::size_t i = 0; i < Commands.size(); ++i)
	{
		Commands[i].count = static_cast<GLuint>(VertexCount);
		Commands[i].instanceCount = static_cast<GLuint>(1);
		Commands[i].first = static_cast<GLuint>(0);
		if(this->Indexing == DIVISOR_MULTI_INDEXING)
			Commands[i].baseInstance = static_cast<GLuint>(i % 2 ? 0 : 1);
		else
			Commands[i].baseInstance = 0;
	}

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, BufferName[BUFFER_INDIRECT]);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(drawArraysIndirectCommand) * Commands.size(), &Commands[0], GL_STATIC_DRAW);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

	return true;
}

bool testDrawIndexing::initVertexArray()
{
	glGenVertexArrays(1, &this->VertexArrayName);
	glBindVertexArray(this->VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_ARRAY]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glEnableVertexAttribArray(glf::semantic::attr::POSITION);

		if(this->Indexing == DIVISOR_INDEXING)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_DIVISOR]);
			glVertexAttribIPointer(glf::semantic::attr::DRAW_ID, 1, GL_SHORT, sizeof(short), 0);
			glVertexAttribDivisor(glf::semantic::attr::DRAW_ID, 1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glEnableVertexAttribArray(glf::semantic::attr::DRAW_ID);
		}
	glBindVertexArray(0);

	return true;
}

void testDrawIndexing::render()
{
	float Depth(1.0f);
	glClearBufferfv(GL_DEPTH, 0, &Depth);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

	{
		glBindBuffer(GL_UNIFORM_BUFFER, this->BufferName[buffer::BUFFER_FRAME]);
		glm::mat4* Pointer = reinterpret_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

		glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 2048.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -this->TranlationCurrent.y - 512));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, this->RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, this->RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);

		*Pointer = Projection * View * Model;
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glViewportIndexedfv(0, &glm::vec4(0, 0, this->getWindowSize())[0]);

	this->beginTimer();
	switch(this->Indexing)
	{
		case DRAW:
		case DIVISOR_MULTI_INDEXING:
		case ID_INDEXING:
		{
			int const DrawChunk = 2;
			for(int i = 0; i < DrawChunk; ++i)
				glMultiDrawArraysIndirect(GL_TRIANGLES, 0, static_cast<GLsizei>(DrawCount / DrawChunk), 0);
		}
		case DIVISOR_INDEXING:
		{
			for(std::size_t DrawIndex(0); DrawIndex < DrawCount; ++DrawIndex)
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 1, static_cast<GLuint>(DrawIndex));
		}
		break;
		case ATTRIB_INDEXING:
		{
			for(std::size_t DrawIndex(0); DrawIndex < DrawCount; ++DrawIndex)
			{
				glVertexAttribI1i(1, DrawIndex % 2 ? 0 : 1); // 1 is DRAW_ID attribute location
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 1, 0);
			}
		}
		break;
		case UNIFORM_INDEXING:
		{
			for(std::size_t DrawIndex(0); DrawIndex < DrawCount; ++DrawIndex)
			{
				glProgramUniform1i(this->ProgramName, UniformDrawIndex, DrawIndex % 2 ? 0 : 1);
				glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 1, 0);
			}
		}
		break;
	default:
		assert(0);
		break;
	}
	this->endTimer();
}
