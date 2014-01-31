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
	glf::window Window("gl-330-blend-rtt");
	char const * VERT_SHADER_SOURCE1("gl-330/mrt.vert");
	char const * FRAG_SHADER_SOURCE1("gl-330/mrt.frag");
	char const * VERT_SHADER_SOURCE2("gl-330/image-2d.vert");
	char const * FRAG_SHADER_SOURCE2("gl-330/image-2d.frag");
	char const * TEXTURE_DIFFUSE("kueken3-bgr8.dds");
	glm::ivec2 const FRAMEBUFFER_SIZE(640, 480);
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);

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
	GLsizei const VertexCount(6);
	GLsizeiptr const VertexSize = VertexCount * sizeof(vertex);
	vertex const VertexData[VertexCount] =
	{
		vertex(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 0.0f)),
		vertex(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 0.0f)),
		vertex(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		vertex(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		vertex(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		vertex(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 0.0f))
	};

	enum texture_type
	{
		TEXTURE_RGB8,
		TEXTURE_R,
		TEXTURE_G,
		TEXTURE_B,
		TEXTURE_MAX
	};

	namespace shader
	{
		enum type
		{
			VERT1,
			FRAG1,
			VERT2,
			FRAG2,
			MAX
		};
	}//namespace shader

	std::vector<GLuint> ShaderName(shader::MAX);

	GLuint FramebufferName = 0;
	GLuint VertexArrayName = 0;
	GLuint ProgramNameSingle = 0;
	GLint UniformMVPSingle = 0;
	GLint UniformDiffuseSingle = 0;
	GLuint ProgramNameMultiple = 0;
	GLint UniformMVPMultiple = 0;
	GLint UniformDiffuseMultiple = 0;
	GLuint BufferName = 0;
	GLuint Texture2DName[TEXTURE_MAX];
	GLuint SamplerName = 0;
	glm::ivec4 Viewport[TEXTURE_MAX];

}//namespace

bool initProgram()
{
	bool Validated = true;

	glf::compiler Compiler;
	ShaderName[shader::VERT1] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE1, "--version 330 --profile core");
	ShaderName[shader::FRAG1] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE1, "--version 330 --profile core");
	ShaderName[shader::VERT2] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE2, "--version 330 --profile core");
	ShaderName[shader::FRAG2] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE2, "--version 330 --profile core");
	Validated = Validated && Compiler.check();

	if(Validated)
	{
		ProgramNameMultiple = glCreateProgram();
		glAttachShader(ProgramNameMultiple, ShaderName[shader::VERT1]);
		glAttachShader(ProgramNameMultiple, ShaderName[shader::FRAG1]);
		glLinkProgram(ProgramNameMultiple);
		Validated = Validated && glf::checkProgram(ProgramNameMultiple);
	}

	if(Validated)
	{
		UniformMVPMultiple = glGetUniformLocation(ProgramNameMultiple, "MVP");
		UniformDiffuseMultiple = glGetUniformLocation(ProgramNameMultiple, "Diffuse");
	}

	if(Validated)
	{
		ProgramNameSingle = glCreateProgram();
		glAttachShader(ProgramNameSingle, ShaderName[shader::VERT2]);
		glAttachShader(ProgramNameSingle, ShaderName[shader::FRAG2]);
		glLinkProgram(ProgramNameSingle);
		Validated = Validated && glf::checkProgram(ProgramNameSingle);
	}

	if(Validated)
	{
		UniformMVPSingle = glGetUniformLocation(ProgramNameSingle, "MVP");
		UniformDiffuseSingle = glGetUniformLocation(ProgramNameSingle, "Diffuse");
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

bool initSampler()
{
	glGenSamplers(1, &SamplerName);
	glSamplerParameteri(SamplerName, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(SamplerName, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(SamplerName, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(SamplerName, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(SamplerName, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glSamplerParameterfv(SamplerName, GL_TEXTURE_BORDER_COLOR, &glm::vec4(0.0f)[0]);
	glSamplerParameterf(SamplerName, GL_TEXTURE_MIN_LOD, -1000.f);
	glSamplerParameterf(SamplerName, GL_TEXTURE_MAX_LOD, 1000.f);
	glSamplerParameterf(SamplerName, GL_TEXTURE_LOD_BIAS, 0.0f);
	glSamplerParameteri(SamplerName, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glSamplerParameteri(SamplerName, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	return glf::checkError("initSampler");
}

bool initTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(TEXTURE_MAX, Texture2DName);

	gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));

	glBindTexture(GL_TEXTURE_2D, Texture2DName[TEXTURE_RGB8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D, 
		GLint(0), 
		GL_RGB8,
		GLsizei(Texture.dimensions().x), 
		GLsizei(Texture.dimensions().y), 
		0,
		GL_BGR, 
		GL_UNSIGNED_BYTE, 
		Texture.data());

	glBindTexture(GL_TEXTURE_2D, Texture2DName[TEXTURE_R]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);

	glBindTexture(GL_TEXTURE_2D, Texture2DName[TEXTURE_G]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);

	glBindTexture(GL_TEXTURE_2D, Texture2DName[TEXTURE_B]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);

	for(int i = TEXTURE_R; i <= TEXTURE_B; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, Texture2DName[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 
			0,
			GL_R8,
			GLsizei(Texture.dimensions().x), 
			GLsizei(Texture.dimensions().y), 
			0,
			GL_RGB, 
			GL_UNSIGNED_BYTE, 
			0);
	}

	return glf::checkError("initTexture");
}

bool initFramebuffer()
{
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	for(std::size_t i = TEXTURE_R; i <= TEXTURE_B; ++i)
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + GLenum(i - TEXTURE_R), Texture2DName[i], 0);

	GLenum DrawBuffers[3];
	DrawBuffers[0] = GL_COLOR_ATTACHMENT0;
	DrawBuffers[1] = GL_COLOR_ATTACHMENT1;
	DrawBuffers[2] = GL_COLOR_ATTACHMENT2;

	glDrawBuffers(3, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return glf::checkError("initFramebuffer");
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

bool initBlend()
{
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_COLOR, GL_SRC_COLOR, GL_ZERO, GL_ZERO);

	glEnablei(GL_BLEND, 0);
	glEnablei(GL_BLEND, 1);
	glEnablei(GL_BLEND, 2);
	glEnablei(GL_BLEND, 3);

	glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	glColorMaski(2, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	glColorMaski(3, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);

	return glf::checkError("initBlend");
}

bool begin()
{
	Viewport[TEXTURE_RGB8] = glm::ivec4(0, 0, Window.Size >> 1);
	Viewport[TEXTURE_R] = glm::ivec4(Window.Size.x >> 1, 0, Window.Size >> 1);
	Viewport[TEXTURE_G] = glm::ivec4(Window.Size.x >> 1, Window.Size.y >> 1, Window.Size >> 1);
	Viewport[TEXTURE_B] = glm::ivec4(0, Window.Size.y >> 1, Window.Size >> 1);

	bool Validated = true;

	if(Validated)
		Validated = initBlend();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initBuffer();
	if(Validated)
		Validated = initVertexArray();
	if(Validated)
		Validated = initSampler();
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
	glDeleteBuffers(1, &BufferName);
	glDeleteProgram(ProgramNameMultiple);
	glDeleteProgram(ProgramNameSingle);
	glDeleteTextures(TEXTURE_MAX, Texture2DName);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteSamplers(1, &SamplerName);

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
		glViewport(0, 0, FRAMEBUFFER_SIZE.x >> 1, FRAMEBUFFER_SIZE.y >> 1);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

		glUseProgram(ProgramNameMultiple);
		glUniformMatrix4fv(UniformMVPMultiple, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(UniformDiffuseMultiple, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture2DName[TEXTURE_RGB8]);
		glBindSampler(0, SamplerName);
		glBindVertexArray(VertexArrayName);

		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
	}

	// Pass 2
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

	glUseProgram(ProgramNameSingle);

	{
		glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f);
		glm::mat4 View = glm::mat4(1.0f);
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(UniformMVPSingle, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(UniformDiffuseSingle, 0);
	}

	for(std::size_t i = 0; i < TEXTURE_MAX; ++i)
	{
		glViewport(Viewport[i].x, Viewport[i].y, Viewport[i].z, Viewport[i].w);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture2DName[i]);
		glBindSampler(0, SamplerName);
		glBindVertexArray(VertexArrayName);

		glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
	}

	glf::checkError("display");

}

int main(int argc, char* argv[])
{
	return glf::run(argc, argv, glf::CORE, 3, 3);
}
