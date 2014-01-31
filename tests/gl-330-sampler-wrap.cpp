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

namespace
{
	glf::window Window("gl-330-sampler-wrap");

	char const * VERT_SHADER_SOURCE("gl-330/texture-2d.vert");
	char const * FRAG_SHADER_SOURCE("gl-330/texture-2d.frag");
	char const * TEXTURE_DIFFUSE("kueken1-dxt5.dds");

	struct vertex
	{
		vertex
		(
			glm::vec2 const & Position,
			glm::vec2 const & Texcoord
		) :
			Position(Position),
			Texcoord(Texcoord)
		{}

		glm::vec2 Position;
		glm::vec2 Texcoord;
	};

	// With DDS textures, v texture coordinate are reversed, from top to bottom
	GLsizei const VertexCount = 6;
	GLsizeiptr const VertexSize = VertexCount * sizeof(vertex);
	vertex const VertexData[VertexCount] =
	{
		vertex(glm::vec2(-1.0f,-1.0f), glm::vec2(-2.0f, 2.0f)),
		vertex(glm::vec2( 1.0f,-1.0f), glm::vec2( 2.0f, 2.0f)),
		vertex(glm::vec2( 1.0f, 1.0f), glm::vec2( 2.0f,-2.0f)),
		vertex(glm::vec2( 1.0f, 1.0f), glm::vec2( 2.0f,-2.0f)),
		vertex(glm::vec2(-1.0f, 1.0f), glm::vec2(-2.0f,-2.0f)),
		vertex(glm::vec2(-1.0f,-1.0f), glm::vec2(-2.0f, 2.0f))
	};

	namespace viewport
	{
		enum type
		{
			V00,
			V10,
			V11,
			V01,
			MAX
		};
	}//namespace viewport

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
	GLuint VertexArrayName;
	GLuint ProgramName;
	GLuint BufferName;
	GLuint Texture2DName;
	GLint UniformMVP;
	GLint UniformDiffuse;
	GLuint SamplerName[viewport::MAX];
	glm::ivec4 Viewport[viewport::MAX];
}//namespace

bool initProgram()
{
	bool Validated = true;
	
	if(Validated)
	{
		glf::compiler Compiler;
		ShaderName[shader::VERT] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, "--version 330 --profile core");
		ShaderName[shader::FRAG] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE, "--version 330 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName = glCreateProgram();
		glAttachShader(ProgramName, ShaderName[shader::VERT]);
		glAttachShader(ProgramName, ShaderName[shader::FRAG]);
		glLinkProgram(ProgramName);
		Validated = Validated && glf::checkProgram(ProgramName);
	}

	if(Validated)
	{
		UniformMVP = glGetUniformLocation(ProgramName, "MVP");
		UniformDiffuse = glGetUniformLocation(ProgramName, "Diffuse");
	}

	return Validated && glf::checkError("initProgram");
}

bool initArrayBuffer()
{
	glGenBuffers(1, &BufferName);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return glf::checkError("initArrayBuffer");
}

#define GL_MIRROR_CLAMP_TO_EDGE			0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT	0x8912

bool initSampler()
{
	glGenSamplers(viewport::MAX, SamplerName);

	for(std::size_t i = 0; i < viewport::MAX; ++i)
	{
		glSamplerParameteri(SamplerName[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(SamplerName[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glm::vec4 BorderColor(0.0f, 0.5f, 1.0f, 1.0f);
		glSamplerParameterfv(SamplerName[i], GL_TEXTURE_BORDER_COLOR, &BorderColor[0]);
	}

	glSamplerParameteri(SamplerName[viewport::V00], GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glSamplerParameteri(SamplerName[viewport::V10], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(SamplerName[viewport::V11], GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(SamplerName[viewport::V01], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glSamplerParameteri(SamplerName[viewport::V00], GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glSamplerParameteri(SamplerName[viewport::V10], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(SamplerName[viewport::V11], GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(SamplerName[viewport::V01], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return glf::checkError("initSampler");
}

bool initTexture2D()
{
	glGenTextures(1, &Texture2DName);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture2DName);

	gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));
	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glCompressedTexImage2D(
			GL_TEXTURE_2D,
			GLint(Level),
			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
			GLsizei(Texture[Level].dimensions().x), 
			GLsizei(Texture[Level].dimensions().y), 
			0, 
			GLsizei(Texture[Level].size()), 
			Texture[Level].data());
	}

	return glf::checkError("initTexture");
}

bool initVertexArray()
{
	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);
	glBindVertexArray(0);

	return glf::checkError("initVertexArray");
}

bool begin()
{
	Viewport[viewport::V00] = glm::ivec4(0, 0, Window.Size >> 1);
	Viewport[viewport::V10] = glm::ivec4(Window.Size.x >> 1, 0, Window.Size >> 1);
	Viewport[viewport::V11] = glm::ivec4(Window.Size.x >> 1, Window.Size.y >> 1, Window.Size >> 1);
	Viewport[viewport::V01] = glm::ivec4(0, Window.Size.y >> 1, Window.Size >> 1);

	bool Validated = true;

	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initArrayBuffer();
	if(Validated)
		Validated = initTexture2D();
	if(Validated)
		Validated = initSampler();
	if(Validated)
		Validated = initVertexArray();

	return Validated && glf::checkError("begin");
}

bool end()
{
	for(std::size_t i = 0; 0 < shader::MAX; ++i)
		glDeleteShader(ShaderName[i]);
	glDeleteBuffers(1, &BufferName);
	glDeleteProgram(ProgramName);
	glDeleteTextures(1, &Texture2DName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return glf::checkError("end");
}

void display()
{
	// Compute the MVP (Model View Projection matrix)
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 1000.0f);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;

	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

	// Bind the program for use
	glUseProgram(ProgramName);
	glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(UniformDiffuse, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture2DName);

	glBindVertexArray(VertexArrayName);

	for(std::size_t Index = 0; Index < viewport::MAX; ++Index)
	{
		glViewport(Viewport[Index].x, Viewport[Index].y, Viewport[Index].z, Viewport[Index].w);
		glBindSampler(0, SamplerName[Index]);
		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
	}
}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 3, 3);
}
