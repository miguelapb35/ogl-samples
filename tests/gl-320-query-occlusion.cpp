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
	char const * VERT_SHADER_SOURCE("gl-320/query-conditional.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/query-conditional.frag");

	GLsizei const VertexCount(6);
	GLsizeiptr const PositionSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const PositionData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f),
		glm::vec2(-1.0f,-1.0f)
	};

	GLuint VertexArrayName(0);
	GLuint ProgramName(0);
	GLuint BufferName(0);
	GLuint QueryName(0);
	GLint UniformMVP(0);
	GLint UniformColor(0);
}//namespace

class gl_320_query_occlusion : public test
{
public:
	gl_320_query_occlusion(int argc, char* argv[]) :
		test(argc, argv, "gl-320-query-occlusion", test::CORE, 3, 2)
	{}

private:
	bool initQuery()
	{
		glGenQueries(1, &QueryName);

		return this->checkError("initQuery");
	}

	bool initProgram()
	{
		bool Validated = true;
	
		if(Validated)
		{
			glf::compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 150 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 150 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glAttachShader(ProgramName, VertShaderName);
			glAttachShader(ProgramName, FragShaderName);

#			ifndef __APPLE__ // Workaround broken Apple driver, leak shader object or crash
				glDeleteShader(VertShaderName);
				glDeleteShader(FragShaderName);
#			endif
			
			glBindAttribLocation(ProgramName, glf::semantic::attr::POSITION, "Position");
			glBindFragDataLocation(ProgramName, glf::semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName);
			Validated = Validated && glf::checkProgram(ProgramName);
		}

		// Get variables locations
		if(Validated)
		{
			UniformMVP = glGetUniformLocation(ProgramName, "MVP");
			UniformColor = glGetUniformLocation(ProgramName, "Diffuse");
		}

		return Validated && this->checkError("initProgram");
	}

	// Buffer update using glBufferSubData
	bool initBuffer()
	{
		// Generate a buffer object
		glGenBuffers(1, &BufferName);

		// Bind the buffer for use
		glBindBuffer(GL_ARRAY_BUFFER, BufferName);

		// Reserve buffer memory but and copy the values
		glBufferData(GL_ARRAY_BUFFER, PositionSize, &PositionData[0][0], GL_STATIC_DRAW);

		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return this->checkError("initArrayBuffer");
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			// Bind vertex attribute
			glBindBuffer(GL_ARRAY_BUFFER, BufferName);
			glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glBindVertexArray(0);

		return this->checkError("initVertexArray");
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initVertexArray();
		if(Validated)
			Validated = initQuery();

		return Validated && this->checkError("begin");
	}

	bool end()
	{
		glDeleteBuffers(1, &BufferName);
		glDeleteProgram(ProgramName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return this->checkError("end");
	}

	bool render()
	{
		glm::ivec2 WindowSize(this->getWindowSize());

		glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * this->view() * Model;

		// Set the display viewport
		glViewport(0, 0, WindowSize.x, WindowSize.y);

		// Clear color buffer with black
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

		// Bind program
		glUseProgram(ProgramName);
		glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);
		glUniform4fv(UniformColor, 1, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		// Beginning of the samples count query
		glBeginQuery(GL_SAMPLES_PASSED, QueryName);

		glBindVertexArray(VertexArrayName);
		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);

		// End of the samples count query
		glEndQuery(GL_SAMPLES_PASSED);

		// Get the count of samples. 
		// If the result of the query isn't here yet, we wait here...
		GLuint SamplesCount = 0;
		glGetQueryObjectuiv(QueryName, GL_QUERY_RESULT, &SamplesCount);
		fprintf(stdout, "Samples count: %d\r", SamplesCount);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_320_query_occlusion Test(argc, argv);
	Error += Test();

	return Error;
}

