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
	char const * VERT_SHADER_SOURCE("gl-430/texture-fetch-dependent.vert");
	char const * FRAG_SHADER_SOURCE("gl-430/texture-fetch-dependent.frag");
	char const * TEXTURE_DIFFUSE("kueken7_srgba8_unorm.dds");

	namespace texture
	{
		enum type
		{
			DIFFUSE,
			INDIRECTION,
			MAX
		};
	}//namespace texture
}//namespace

class instance : public test
{
public:
	instance(int argc, char* argv[]) :
		test(argc, argv, "gl-430-texture-fetch-dependent", test::CORE, 4, 3, glm::uvec2(1280, 720)),
		PipelineName(0),
		ProgramName(0),
		VertexArrayName(0)
	{}

private:
	GLuint PipelineName;
	GLuint ProgramName;
	GLuint VertexArrayName;
	std::array<GLuint, texture::MAX> TextureName;

	bool initProgram()
	{
		bool Validated = true;
	
		if(Validated)
		{
			compiler Compiler;

			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 430 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 430 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glProgramParameteri(ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName, VertShaderName);
			glAttachShader(ProgramName, FragShaderName);
			glLinkProgram(ProgramName);
			Validated = Validated && Compiler.checkProgram(ProgramName);
		}

		if(Validated)
		{
			glGenProgramPipelines(1, &PipelineName);
			glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);
		}

		return Validated;
	}

	bool initTexture()
	{
		glGenTextures(texture::MAX, &TextureName[0]);

		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName[texture::DIFFUSE]);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			std::vector<glm::u8vec4> Colors;
			Colors.resize(2048);
			for(std::size_t ColorIndex = 0; ColorIndex < Colors.size(); ++ColorIndex)
				Colors[ColorIndex] = glm::u8vec4(glm::vec4(glm::linearRand(glm::vec3(0), glm::vec3(1)) * 255.f, 1.0f));

			glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 1, 1, static_cast<GLsizei>(Colors.size()));
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
					0, 0, 0,
					1, 1, static_cast<GLsizei>(Colors.size()),
					GL_RGBA, GL_UNSIGNED_BYTE, &Colors[0][0]);
		}

		{
			glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName[texture::INDIRECTION]);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			std::vector<glm::u32vec1> Data;
			Data.resize(2048);
			for(std::size_t Index = 0; Index < Data.size(); ++Index)
				Data[Index] = glm::u32vec1(glm::linearRand(glm::vec1(0), glm::vec1(1)) * glm::vec1(Data.size() - 1));

			glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R32UI, 1, 1, static_cast<GLsizei>(Data.size()));
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
					0, 0, 0,
					1, 1, static_cast<GLsizei>(Data.size()),
					GL_RED_INTEGER, GL_UNSIGNED_INT, &Data[0][0]);
		}

		return true;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
		glBindVertexArray(0);

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();
		if(Validated)
			Validated = initTexture();

		return Validated;
	}

	bool end()
	{
		glDeleteProgram(ProgramName);
		glDeleteProgramPipelines(1, &PipelineName);
		glDeleteVertexArrays(1, &VertexArrayName);
		glDeleteTextures(texture::MAX, &TextureName[0]);

		return true;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindProgramPipeline(PipelineName);
		glBindVertexArray(VertexArrayName);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName[texture::DIFFUSE]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName[texture::INDIRECTION]);

		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 3, 1, 0);

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


