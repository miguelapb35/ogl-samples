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

#include <glf/glf.hpp>
#include <glf/compiler.hpp>

namespace
{
	glf::window Window("gl-320-draw-image-space");

	char const * VERT_SHADER_SOURCE("gl-320/draw-image-space.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/draw-image-space.frag");
	char const * TEXTURE_DIFFUSE("kueken3-bgr8.dds");

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
	GLuint ProgramName(0);
	GLuint VertexArrayName(0);
	GLuint TextureName(0);
	GLint UniformTransform(-1);
}//namespace

bool initProgram()
{
	bool Validated(true);
	
	if(Validated)
	{
		glf::compiler Compiler;
		ShaderName[shader::VERT] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, 
			"--version 150 --profile core");
		ShaderName[shader::FRAG] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE,
			"--version 150 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName = glCreateProgram();
		glAttachShader(ProgramName, ShaderName[shader::VERT]);
		glAttachShader(ProgramName, ShaderName[shader::FRAG]);

		glBindFragDataLocation(ProgramName, glf::semantic::frag::COLOR, "Color");
		glLinkProgram(ProgramName);
		Validated = Validated && glf::checkProgram(ProgramName);
	}

	if(Validated)
	{
		UniformTransform = glGetUniformBlockIndex(ProgramName, "transform");
	}

	return Validated;
}

bool initTexture()
{
	gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));
	assert(!Texture.empty());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &TextureName);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for(gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level)
	{
		glTexImage2D(
			GL_TEXTURE_2D,
			GLint(Level),
			GL_RGBA8,
			GLsizei(Texture[Level].dimensions().x),
			GLsizei(Texture[Level].dimensions().y),
			0,
			GL_BGR,
			GL_UNSIGNED_BYTE,
			Texture[Level].data());
	}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

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
	bool Validated(true);

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
	for(std::size_t i = 0; 0 < shader::MAX; ++i)
		glDeleteShader(ShaderName[i]);
	glDeleteProgram(ProgramName);
	glDeleteTextures(1, &TextureName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return glf::checkError("end");
}

void display()
{
	glUseProgram(ProgramName);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(VertexArrayName);

	glViewport(0, 0, Window.Size.x, Window.Size.y);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);


}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 3, 2);
}
