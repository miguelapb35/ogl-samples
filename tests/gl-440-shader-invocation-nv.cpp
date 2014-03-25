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
	char const * VERT_SHADER_SOURCE("gl-440/shader-invocation.vert");
	char const * FRAG_SHADER_SOURCE("gl-440/shader-invocation.frag");

	namespace program
	{
		enum type
		{
			VERTEX,
			FRAGMENT,
			MAX
		};
	}//namespace program
}//namespace

#define GL_WARP_SIZE_NV                                    0x9339
#define GL_WARPS_PER_SM_NV                                 0x933A
#define GL_SM_COUNT_NV                                     0x933B

class gl_440_shader_invocation_nv : public test
{
public:
	gl_440_shader_invocation_nv(int argc, char* argv[]) :
		test(argc, argv, "gl-440-shader-invocation-nv", test::CORE, 4, 4),
		PipelineName(0),
		VertexArrayName(0)
	{}

private:
	GLuint PipelineName;
	GLuint VertexArrayName;
	std::array<GLuint, program::MAX> ProgramName;

	bool initProgram()
	{
		bool Validated(true);
	
		glGenProgramPipelines(1, &PipelineName);

		if(Validated)
		{
			compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 440 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 440 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName[program::VERTEX] = glCreateProgram();
			glProgramParameteri(ProgramName[program::VERTEX], GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName[program::VERTEX], VertShaderName);
			glLinkProgram(ProgramName[program::VERTEX]);
			Validated = Validated && Compiler.checkProgram(ProgramName[program::VERTEX]);

			ProgramName[program::FRAGMENT] = glCreateProgram();
			glProgramParameteri(ProgramName[program::FRAGMENT], GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName[program::FRAGMENT], FragShaderName);
			glLinkProgram(ProgramName[program::FRAGMENT]);
			Validated = Validated && Compiler.checkProgram(ProgramName[program::FRAGMENT]);
		}

		if(Validated)
		{
			glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT, ProgramName[program::VERTEX]);
			glUseProgramStages(PipelineName, GL_FRAGMENT_SHADER_BIT, ProgramName[program::FRAGMENT]);
		}

		return Validated;
	}

	bool initVertexArray()
	{
		bool Validated(true);

		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
		glBindVertexArray(0);

		return Validated;
	}

	bool begin()
	{
		bool Validated(true);
		Validated = Validated && this->checkExtension("GL_NV_shader_thread_group");

		GLint WrapSize(0);
		GLint WrapsPerSM(0);
		GLint SMCount(0);
		glGetIntegerv(GL_WARP_SIZE_NV, &WrapSize);
		glGetIntegerv(GL_WARPS_PER_SM_NV, &WrapsPerSM);
		glGetIntegerv(GL_SM_COUNT_NV, &SMCount);

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();

		return Validated;
	}

	bool end()
	{
		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteProgram(ProgramName[program::FRAGMENT]);
		glDeleteProgram(ProgramName[program::VERTEX]);
		glDeleteVertexArrays(1, &VertexArrayName);

		return true;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);

		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_440_shader_invocation_nv Test(argc, argv);
	Error += Test();

	return Error;
}
