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
	char const * VERT_SHADER_SOURCE1("gl-320/fbo-rtt-multiple-output.vert");
	char const * FRAG_SHADER_SOURCE1("gl-320/fbo-rtt-multiple-output.frag");
	char const * VERT_SHADER_SOURCE2("gl-320/fbo-rtt-layer.vert");
	char const * FRAG_SHADER_SOURCE2("gl-320/fbo-rtt-layer.frag");
	char const * TEXTURE_DIFFUSE("kueken3-bgr8.dds");
	int const FRAMEBUFFER_SIZE(2);
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);

	glf::window Window("gl-320-fbo-rtt-texture-array", glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

	// With DDS textures, v texture coordinate are reversed, from top to bottom
	GLsizei const VertexCount(6);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 0.0f))
	};

	namespace texture
	{
		enum type
		{
			RED,
			GREEN,
			BLUE,
			MAX
		};
	}//namespace texture

	namespace program
	{
		enum type
		{
			MULTIPLE,
			SPLASH,
			MAX
		};
	}//namespace program

	namespace shader
	{
		enum type
		{
			VERT_MULTIPLE,
			FRAG_MULTIPLE,
			VERT_SPLASH,
			FRAG_SPLASH,
			MAX
		};
	}//namespace shader

	std::vector<GLuint> ShaderName(shader::MAX);
	GLuint FramebufferName(0);
	GLuint BufferName(0);
	GLuint TextureName(0);
	std::vector<GLuint> VertexArrayName(program::MAX);
	std::vector<GLuint> ProgramName(program::MAX);
	std::vector<GLint> UniformMVP(program::MAX);
	std::vector<GLint> UniformDiffuse(program::MAX);
	GLint UniformLayer(0);
	std::vector<glm::ivec4> Viewport(texture::MAX);
}//namespace

bool initProgram()
{
	bool Validated = true;

	glf::compiler Compiler;

	if(Validated)
	{
		ShaderName[shader::VERT_MULTIPLE] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE1, "--version 150 --profile core");
		ShaderName[shader::FRAG_MULTIPLE] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE1, "--version 150 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[program::MULTIPLE] = glCreateProgram();
		glAttachShader(ProgramName[program::MULTIPLE], ShaderName[shader::VERT_MULTIPLE]);
		glAttachShader(ProgramName[program::MULTIPLE], ShaderName[shader::FRAG_MULTIPLE]);
		glBindAttribLocation(ProgramName[program::MULTIPLE], glf::semantic::attr::POSITION, "Position");
		glBindFragDataLocation(ProgramName[program::MULTIPLE], glf::semantic::frag::RED, "Red");
		glBindFragDataLocation(ProgramName[program::MULTIPLE], glf::semantic::frag::GREEN, "Green");
		glBindFragDataLocation(ProgramName[program::MULTIPLE], glf::semantic::frag::BLUE, "Blue");

		glLinkProgram(ProgramName[program::MULTIPLE]);
		Validated = glf::checkProgram(ProgramName[program::MULTIPLE]);
	}

	if(Validated)
	{
		UniformMVP[program::MULTIPLE] = glGetUniformLocation(ProgramName[program::MULTIPLE], "MVP");
	}

	if(Validated)
	{
		ShaderName[shader::VERT_SPLASH] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE2, "--version 150 --profile core");
		ShaderName[shader::FRAG_SPLASH] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE2, "--version 150 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[program::SPLASH] = glCreateProgram();
		glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::VERT_SPLASH]);
		glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::FRAG_SPLASH]);
		glBindAttribLocation(ProgramName[program::SPLASH], glf::semantic::attr::POSITION, "Position");
		glBindAttribLocation(ProgramName[program::SPLASH], glf::semantic::attr::TEXCOORD, "Texcoord");
		glBindFragDataLocation(ProgramName[program::SPLASH], glf::semantic::frag::COLOR, "Color");

		glLinkProgram(ProgramName[program::SPLASH]);
		Validated = glf::checkProgram(ProgramName[program::SPLASH]);
	}

	if(Validated)
	{
		UniformMVP[program::SPLASH] = glGetUniformLocation(ProgramName[program::SPLASH], "MVP");
		UniformDiffuse[program::SPLASH] = glGetUniformLocation(ProgramName[program::SPLASH], "Diffuse");
		UniformLayer = glGetUniformLocation(ProgramName[program::SPLASH], "Layer");
	}

	return glf::checkError("initProgram");
}

bool initBuffer()
{
	glGenBuffers(1, &BufferName);

	glBindBuffer(GL_ARRAY_BUFFER, BufferName);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return glf::checkError("initBuffer");
}

bool initTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &TextureName);

	glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage3D(
		GL_TEXTURE_2D_ARRAY, 
		0,
		GL_RGB8,
		GLsizei(Window.Size.x / FRAMEBUFFER_SIZE),
		GLsizei(Window.Size.y / FRAMEBUFFER_SIZE),
		GLsizei(3),//depth
		0,
		GL_BGR,
		GL_UNSIGNED_BYTE,
		NULL);

	return glf::checkError("initTexture");
}

bool initFramebuffer()
{
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	GLenum DrawBuffers[3];
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	DrawBuffers[1] = GL_COLOR_ATTACHMENT1;
	DrawBuffers[2] = GL_COLOR_ATTACHMENT2;
	glDrawBuffers(3, DrawBuffers);

	for(std::size_t i = texture::RED; i <= texture::BLUE; ++i)
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GLuint(i - texture::RED), TextureName, 0, GLint(i));

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return glf::checkError("initFramebuffer");
}

bool initVertexArray()
{
	glGenVertexArrays(program::MAX, &VertexArrayName[0]);

	glBindVertexArray(VertexArrayName[program::MULTIPLE]);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
	glBindVertexArray(0);

	glBindVertexArray(VertexArrayName[program::SPLASH]);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);
	glBindVertexArray(0);

	return glf::checkError("initVertexArray");
}

bool begin()
{
	Viewport[texture::RED] = glm::ivec4(Window.Size.x >> 1, 0, Window.Size / FRAMEBUFFER_SIZE);
	Viewport[texture::GREEN] = glm::ivec4(Window.Size.x >> 1, Window.Size.y >> 1, Window.Size / FRAMEBUFFER_SIZE);
	Viewport[texture::BLUE] = glm::ivec4(0, Window.Size.y >> 1, Window.Size / FRAMEBUFFER_SIZE);

	bool Validated = true;

	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initBuffer();
	if(Validated)
		Validated = initVertexArray();
	if(Validated)
		Validated = initTexture();
	if(Validated)
		Validated = initFramebuffer();

	return Validated && glf::checkError("begin");
}

bool end()
{
	for(std::size_t i = 0; 0 < shader::MAX; ++i)
		glDeleteShader(ShaderName[i]);
	for(std::size_t i = 0; i < program::MAX; ++i)
		glDeleteProgram(ProgramName[i]);
	glDeleteBuffers(1, &BufferName);
	glDeleteTextures(1, &TextureName);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteVertexArrays(program::MAX, &VertexArrayName[0]);

	return glf::checkError("end");
}

void display()
{
	// Pass 1
	{
		// Compute the MVP (Model View Projection matrix)
		glm::mat4 Projection = glm::ortho(-1.0f, 1.0f,-1.0f, 1.0f, -1.0f, 1.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 View = ViewTranslate;
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, Window.Size.x * FRAMEBUFFER_SIZE, Window.Size.y * FRAMEBUFFER_SIZE);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		glUseProgram(ProgramName[program::MULTIPLE]);
		glUniformMatrix4fv(UniformMVP[program::MULTIPLE], 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VertexArrayName[program::MULTIPLE]);
		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
	}

	// Pass 2
	{
		glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Window.Size.x, Window.Size.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		glUseProgram(ProgramName[program::SPLASH]);
		glUniformMatrix4fv(UniformMVP[program::SPLASH], 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(UniformDiffuse[program::SPLASH], 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, TextureName);

	glBindVertexArray(VertexArrayName[program::SPLASH]);

	for(std::size_t i = 0; i < texture::MAX; ++i)
	{
		glViewport(Viewport[i].x, Viewport[i].y, Viewport[i].z, Viewport[i].w);
		glUniform1i(UniformLayer, GLint(i));

		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
	}

	glf::checkError("display");

}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 3, 2);
}
