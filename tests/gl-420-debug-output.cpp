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

namespace
{
	char const * VERT_SHADER_SOURCE("gl-420/debug-output.vert");
	char const * FRAG_SHADER_SOURCE("gl-420/debug-output.frag");

	GLsizei const VertexCount(4);
	GLsizeiptr const PositionSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const PositionData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f)
	};

	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLushort);
	GLushort const ElementData[ElementCount] =
	{
		0, 1, 2,
		2, 3, 0
	};

	namespace buffer
	{
		enum type
		{
			ELEMENT,
			VERTEX,
			TRANSFORM,
			MAX
		};
	}//namespace pipeline

	GLuint BufferName[buffer::MAX] = {0, 0, 0};
}//namespace

class instance : public test
{
public:
	instance(int argc, char* argv[])
		: test(argc, argv, "gl-420-debug-output", test::COMPATIBILITY, 4, 2, glm::vec2(0.25f, 0.25f), GENERATE_ERROR)
		, PipelineName(0)
		, ProgramName(0)
		, VertexArrayName(0)
	{}

private:
	GLuint PipelineName;
	GLuint ProgramName;
	GLuint VertexArrayName;

	bool initProgram()
	{
		bool Validated(true);

		if(Validated)
		{
			compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 420 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 420 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glProgramParameteri(ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName, VertShaderName);
			glAttachShader(ProgramName, FragShaderName);
			glLinkProgram(ProgramName);

			Validated = Validated && Compiler.check_program(ProgramName);
		}

		if(Validated)
		{
			glGenProgramPipelines(1, &PipelineName);
			glBindProgramPipeline(PipelineName);
			glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);
		}

		return Validated;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
			glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(semantic::attr::POSITION);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		return true;
	}

	bool initBuffer()
	{
		glGenBuffers(buffer::MAX, BufferName);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return true;
	}

	bool initDebugOutput()
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(&test::debugOutput, this);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		GLuint MessageId(4);
		glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageControlARB(GL_DEBUG_SOURCE_APPLICATION_ARB, GL_DEBUG_TYPE_OTHER_ARB, GL_DONT_CARE, 1, &MessageId, GL_FALSE);
		std::string Message1("Message 1");
		glDebugMessageInsertARB(
			GL_DEBUG_SOURCE_APPLICATION_ARB, 
			GL_DEBUG_TYPE_OTHER_ARB, 1, 
			GL_DEBUG_SEVERITY_MEDIUM_ARB,
			GLsizei(Message1.size()), Message1.c_str());
		std::string Message2("Message 2");
		glDebugMessageInsertARB(
			GL_DEBUG_SOURCE_THIRD_PARTY_ARB, 
			GL_DEBUG_TYPE_OTHER_ARB, 2, 
			GL_DEBUG_SEVERITY_MEDIUM_ARB,
			GLsizei(Message2.size()), Message2.c_str());
		glDebugMessageInsertARB(
			GL_DEBUG_SOURCE_APPLICATION_ARB, 
			GL_DEBUG_TYPE_OTHER_ARB, 2, 
			GL_DEBUG_SEVERITY_MEDIUM_ARB,
			-1, "Message 3");
		glDebugMessageInsertARB(
			GL_DEBUG_SOURCE_APPLICATION_ARB, 
			GL_DEBUG_TYPE_OTHER_ARB, MessageId, 
			GL_DEBUG_SEVERITY_MEDIUM_ARB,
			-1, "Message 4");

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated && this->checkExtension("GL_ARB_debug_output"))
			Validated = initDebugOutput();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initVertexArray();

		return Validated;
	}

	bool end()
	{
		bool Validated(true);

		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteVertexArrays(1, &VertexArrayName);
		glDeleteBuffers(buffer::MAX, BufferName);
		glDeleteProgram(ProgramName);

		return Validated;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);

			glm::mat4* Pointer = static_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER,
				0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

			glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.0f);
			glm::mat4 Model = glm::mat4(1.0f);
			*Pointer = Projection * this->view() * Model;

			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

		// Error added to test debug output
		glClearBufferfv(GL_UNIFORM_BUFFER, 0, &glm::vec4(0.0f, 0.5f, 1.0f, 1.0f)[0]);

		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);
		glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

		glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	instance Test(argc, argv);
	Error += Test();

	return Error;
}


