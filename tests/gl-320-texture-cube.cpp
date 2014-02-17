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
	char const * VERT_SHADER_SOURCE("gl-320/texture-cube.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/texture-cube.frag");
	char const * TEXTURE_DIFFUSE("cube.dds");

	// With DDS textures, v texture coordinate are reversed, from top to bottom
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

	namespace shader
	{
		enum type
		{
			VERT,
			FRAG,
			MAX
		};
	}//namespace shader

	std::vector<GLuint> ShaderName(shader::MAX);
	GLuint VertexArrayName = 0;
	GLuint BufferName = 0;
	GLuint TextureName = 0;
	GLuint ProgramName = 0;
	GLint UniformMV = 0;
	GLint UniformMVP = 0;
	GLint UniformEnvironment = 0;
	GLint UniformCamera = 0;
	glm::ivec4 Viewport;
}//namespace

class gl_320_texture_cube : public test
{
public:
	gl_320_texture_cube(int argc, char* argv[]) :
		test(argc, argv, "gl-320-texture-cube", test::CORE, 3, 2)
	{}

private:
		bool initProgram()
	{
		bool Validated = true;
	
		if(Validated)
		{
			glf::compiler Compiler;
			ShaderName[shader::VERT] = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 150 --profile core");
			ShaderName[shader::FRAG] = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 150 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glAttachShader(ProgramName, ShaderName[shader::VERT]);
			glAttachShader(ProgramName, ShaderName[shader::FRAG]);

			glBindAttribLocation(ProgramName, glf::semantic::attr::POSITION, "Position");
			glBindFragDataLocation(ProgramName, glf::semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName);
			Validated = Validated && glf::checkProgram(ProgramName);
		}

		if(Validated)
		{
			UniformMV = glGetUniformLocation(ProgramName, "MV");
			UniformMVP = glGetUniformLocation(ProgramName, "MVP");
			UniformEnvironment = glGetUniformLocation(ProgramName, "Environment");
			UniformCamera = glGetUniformLocation(ProgramName, "Camera");
		}

		return Validated &&this->checkError("initProgram");
	}

	bool initBuffer()
	{
		glGenBuffers(1, &BufferName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return this->checkError("initBuffer");;
	}

	bool initTexture()
	{
		gli::textureCube Texture(gli::load_dds((getDataDirectory() + TEXTURE_DIFFUSE).c_str()));

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &TextureName);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureName);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, &glm::vec4(0.0f)[0]);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, -1000.f);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, 1000.f);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, 0.0f);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		for(std::size_t Face = 0; Face < 6; ++Face)
		for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
		{
			glCompressedTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + GLenum(Face),
				GLint(Level),
				GLenum(gli::internal_format(Texture.format())),
				GLsizei(Texture[Face][Level].dimensions().x), 
				GLsizei(Texture[Face][Level].dimensions().y), 
				0, 
				GLsizei(Texture[Face][Level].size()), 
				Texture[Face][Level].data());
		}

		return this->checkError("initTexture");
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName);
			glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), GLF_BUFFER_OFFSET(0));
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
			Validated = initTexture();

		return Validated && this->checkError("begin");
	}

	bool end()
	{
		glDeleteBuffers(1, &BufferName);
		glDeleteProgram(ProgramName);
		glDeleteTextures(1, &TextureName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return this->checkError("end");
	}

	bool render()
	{
		glm::ivec2 WindowSize(this->getWindowSize());

		glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 1000.0f);
		glm::mat4 View = this->view();
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;
		glm::mat4 MV = View * Model;

		glViewport(0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		// Bind the program for use
		glUseProgram(ProgramName);
		glUniformMatrix4fv(UniformMV, 1, GL_FALSE, &MV[0][0]);
		glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(UniformEnvironment, 0);
		glUniform3fv(UniformCamera, 1, &glm::vec3(0.0f, 0.0f, -this->cameraDistance())[0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureName);

		glBindVertexArray(VertexArrayName);
		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_320_texture_cube Test(argc, argv);
	Error += Test();

	return Error;
}

