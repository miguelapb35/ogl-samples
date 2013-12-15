#include "test_draw_range_elements.hpp"

namespace
{
	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(glm::uint32);
	glm::uint32 const ElementData[ElementCount] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const VertexData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f)
	};

	char const * VERT_SHADER_SOURCE("hz-430/vertex-array-object.vert");
	char const * FRAG_SHADER_SOURCE("hz-430/vertex-array-object.frag");
}//namespace

testDrawRangeElements::testDrawRangeElements(
	int argc, char* argv[], profile Profile,
	draw const DrawType, std::size_t const DrawCount
) :
	test(argc, argv, Profile, DEFAULT_MAX_FRAME, DEFAULT_WINDOW_SIZE),
	DrawType(DrawType),
	DrawCount(DrawCount),
	VertexArrayName(0),
	PipelineName(0),
	ProgramName(0),
	QueryName(0)
{
	bool Success = true;
	
	Success = Success && this->isExtensionSupported("GL_ARB_draw_elements_base_vertex");
	assert(Success);
	Success = Success && this->initProgram();
	assert(Success);
	Success = Success && this->initBuffer();
	assert(Success);
	Success = Success && this->initVertexArray();
	assert(Success);

	glGenQueries(1, &this->QueryName);
	glEnable(GL_DEPTH_TEST);
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::PER_FRAME, this->BufferName[buffer::BUFFER_FRAME]);
	glBindProgramPipeline(this->PipelineName);
	glBindVertexArray(this->VertexArrayName);
}

testDrawRangeElements::~testDrawRangeElements()
{
	glDeleteQueries(1, &this->QueryName);
	glDeleteBuffers(static_cast<GLsizei>(BUFFER_MAX), &this->BufferName[0]);
	glDeleteProgramPipelines(1, &this->PipelineName);
	glDeleteProgram(this->ProgramName);
	glDeleteVertexArrays(1, &this->VertexArrayName);
}

bool testDrawRangeElements::initProgram()
{
	bool Validated(true);
	
	glGenProgramPipelines(1, &this->PipelineName);

	glf::compiler Compiler;
	GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, "--version 420 --profile core");
	GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE, "--version 420 --profile core");
	Validated = Validated && Compiler.check();

	this->ProgramName = glCreateProgram();
	glProgramParameteri(this->ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glAttachShader(this->ProgramName, VertShaderName);
	glAttachShader(this->ProgramName, FragShaderName);
	glLinkProgram(this->ProgramName);
	Validated = Validated && glf::checkProgram(this->ProgramName);

	if(Validated)
		glUseProgramStages(this->PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);

	return Validated;
}

bool testDrawRangeElements::initBuffer()
{
	glGenBuffers(BUFFER_MAX, &this->BufferName[0]);

	glBindBuffer(GL_UNIFORM_BUFFER, this->BufferName[BUFFER_FRAME]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_ARRAY]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * DrawCount, 0, GL_STATIC_DRAW);
	for(std::size_t DrawIndex = 0; DrawIndex < DrawCount; ++DrawIndex)
		glBufferSubData(GL_ARRAY_BUFFER, DrawIndex * sizeof(VertexData), sizeof(VertexData), &VertexData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[BUFFER_ELEMENT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool testDrawRangeElements::initVertexArray()
{
	glGenVertexArrays(1, &this->VertexArrayName);
	glBindVertexArray(this->VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, this->BufferName[BUFFER_ARRAY]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glEnableVertexAttribArray(glf::semantic::attr::POSITION);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[BUFFER_ELEMENT]); 
	glBindVertexArray(0);

	return true;
}

void testDrawRangeElements::render()
{
	float Depth(1.0f);
	glClearBufferfv(GL_DEPTH, 0, &Depth);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

	{
		// Update the transformation matrix
		glBindBuffer(GL_UNIFORM_BUFFER, this->BufferName[buffer::BUFFER_FRAME]);
		glm::mat4* Pointer = reinterpret_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 2048.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -this->TranlationCurrent.y - 512));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, this->RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, this->RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);

		*Pointer = Projection * View * Model;
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glViewportIndexedfv(0, &glm::vec4(0, 0, this->getWindowSize())[0]);

	glBeginQuery(GL_TIME_ELAPSED, this->QueryName);
	switch(this->DrawType)
	{
	case DRAW_MIN:
		for(std::size_t DrawIndex(0); DrawIndex < DrawCount; ++DrawIndex)
			glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, 0);
		break;
	case DRAW_FULL:
		for(std::size_t DrawIndex(0); DrawIndex < DrawCount; ++DrawIndex)
			glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, 0, 1, 0, 0);
		break;
	default:
		assert(0);
		break;
	}
	glEndQuery(GL_TIME_ELAPSED);

	GLuint QueryTime(0);
	glGetQueryObjectuiv(this->QueryName, GL_QUERY_RESULT, &QueryTime);

	this->updateTime(static_cast<double>(QueryTime) / 1000000.0);
}
