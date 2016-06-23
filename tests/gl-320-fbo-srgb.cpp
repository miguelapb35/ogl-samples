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
	char const * VERT_SHADER_SOURCE_TEXTURE("gl-320/fbo-srgb.vert");
	char const * FRAG_SHADER_SOURCE_TEXTURE("gl-320/fbo-srgb.frag");
	char const * VERT_SHADER_SOURCE_SPLASH("gl-320/fbo-srgb-blit.vert");
	char const * FRAG_SHADER_SOURCE_SPLASH("gl-320/fbo-srgb-blit.frag");
	char const * TEXTURE_DIFFUSE("kueken7_rgba8_srgb.dds");

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v3fv4u8);
	glf::vertex_v3fv4u8 const VertexData[VertexCount] =
	{
		glf::vertex_v3fv4u8(glm::vec3(+1.0f, 0.0f,-0.70710678118f), glm::u8vec4(255,   0,   0, 255)),
		glf::vertex_v3fv4u8(glm::vec3(-1.0f, 0.0f,-0.70710678118f), glm::u8vec4(255, 255,   0, 255)),
		glf::vertex_v3fv4u8(glm::vec3(+0.0f,+1.0f,+0.70710678118f), glm::u8vec4(  0, 255,   0, 255)),
		glf::vertex_v3fv4u8(glm::vec3(+0.0f,-1.0f,+0.70710678118f), glm::u8vec4(  0,   0, 255, 255))
	};

	GLsizei const ElementCount(12);
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLushort);
	GLushort const ElementData[ElementCount] =
	{
		0, 2, 3,
		0, 3, 1,
		1, 2, 3,
		1, 2, 0
	};

	namespace buffer
	{
		enum type
		{
			VERTEX,
			ELEMENT,
			TRANSFORM,
			MAX
		};
	}//namespace buffer

	namespace texture
	{
		enum type
		{
			COLORBUFFER,
			RENDERBUFFER,
			MAX
		};
	}//namespace texture
	
	namespace program
	{
		enum type
		{
			RENDER,
			SPLASH,
			MAX
		};
	}//namespace program

	namespace shader
	{
		enum type
		{
			VERT_RENDER,
			FRAG_RENDER,
			VERT_SPLASH,
			FRAG_SPLASH,
			MAX
		};
	}//namespace shader
}//namespace

class gl_320_fbo_srgb : public test
{
public:
	gl_320_fbo_srgb(int argc, char* argv[]) :
		test(argc, argv, "gl-320-fbo-srgb", test::CORE, 3, 2),
		FramebufferName(0),
		FramebufferScale(2),
		UniformTransform(-1),
		UniformDiffuse(-1)
	{}

private:
	std::array<GLuint, program::MAX> ProgramName;
	std::array<GLuint, program::MAX> VertexArrayName;
	std::array<GLuint, buffer::MAX> BufferName;
	std::array<GLuint, texture::MAX> TextureName;
	GLint UniformDiffuse;
	GLint UniformTransform;
	GLuint FramebufferName;
	glm::uint FramebufferScale;

	bool initProgram()
	{
		bool Validated(true);

		compiler Compiler;

		std::array<GLuint, shader::MAX> ShaderName;

		if(Validated)
		{
			ShaderName[shader::VERT_RENDER] = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE_TEXTURE, "--version 150 --profile core");
			ShaderName[shader::FRAG_RENDER] = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE_TEXTURE, "--version 150 --profile core");

			ProgramName[program::RENDER] = glCreateProgram();
			glAttachShader(ProgramName[program::RENDER], ShaderName[shader::VERT_RENDER]);
			glAttachShader(ProgramName[program::RENDER], ShaderName[shader::FRAG_RENDER]);

			glBindAttribLocation(ProgramName[program::RENDER], semantic::attr::POSITION, "Position");
			glBindAttribLocation(ProgramName[program::RENDER], semantic::attr::COLOR, "Color");
			glBindFragDataLocation(ProgramName[program::RENDER], semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName[program::RENDER]);
		}

		if(Validated)
		{
			ShaderName[shader::VERT_SPLASH] = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE_SPLASH, "--version 150 --profile core");
			ShaderName[shader::FRAG_SPLASH] = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE_SPLASH, "--version 150 --profile core");

			ProgramName[program::SPLASH] = glCreateProgram();
			glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::VERT_SPLASH]);
			glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::FRAG_SPLASH]);

			glBindFragDataLocation(ProgramName[program::SPLASH], semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName[program::SPLASH]);
		}
	
		if(Validated)
		{
			Validated = Validated && Compiler.check();
			Validated = Validated && Compiler.check_program(ProgramName[program::RENDER]);
			Validated = Validated && Compiler.check_program(ProgramName[program::SPLASH]);
		}

		if(Validated)
		{
			UniformTransform = glGetUniformBlockIndex(ProgramName[program::RENDER], "transform");
			UniformDiffuse = glGetUniformLocation(ProgramName[program::SPLASH], "Diffuse");

			glUseProgram(ProgramName[program::RENDER]);
			glUniformBlockBinding(ProgramName[program::RENDER], UniformTransform, semantic::uniform::TRANSFORM0);

			glUseProgram(ProgramName[program::SPLASH]);
			glUniform1i(UniformDiffuse, 0);
		}

		return Validated && this->checkError("initProgram");
	}

	bool initBuffer()
	{
		glGenBuffers(buffer::MAX, &BufferName[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLint UniformBufferOffset(0);
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &UniformBufferOffset);
		GLint UniformBlockSize = glm::max(GLint(sizeof(glm::mat4)), UniformBufferOffset);

		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return true;
	}

	bool initTexture()
	{
		glm::ivec2 const WindowSize(this->getWindowSize() * this->FramebufferScale);

		glGenTextures(texture::MAX, &TextureName[0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_SRGB8_ALPHA8, GLsizei(WindowSize.x), GLsizei(WindowSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureName[texture::RENDERBUFFER]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_DEPTH_COMPONENT24, GLsizei(WindowSize.x), GLsizei(WindowSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		return true;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(program::MAX, &VertexArrayName[0]);
		glBindVertexArray(VertexArrayName[program::RENDER]);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
			glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v3fv4u8), BUFFER_OFFSET(0));
			glVertexAttribPointer(semantic::attr::COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(glf::vertex_v3fv4u8), BUFFER_OFFSET(sizeof(glm::vec3)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(semantic::attr::POSITION);
			glEnableVertexAttribArray(semantic::attr::COLOR);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		glBindVertexArray(VertexArrayName[program::SPLASH]);
		glBindVertexArray(0);

		return true;
	}

	bool initFramebuffer()
	{
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, TextureName[texture::COLORBUFFER], 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, TextureName[texture::RENDERBUFFER], 0);

		if(!this->checkFramebuffer(FramebufferName))
			return false;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GLint const EncodingLinear = GL_LINEAR;
		GLint const EncodingSRGB = GL_SRGB;

		GLint Encoding = 0;
		glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &Encoding);

		return true;
	}

	bool begin()
	{
		bool Validated(true);

		// Explicitly convert linear pixel color to sRGB color space, as FramebufferName is a sRGB FBO
		// Shader execution is done with linear color to get correct linear algebra working.
		glEnable(GL_FRAMEBUFFER_SRGB);

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

		return Validated;
	}

	bool end()
	{
		glDeleteFramebuffers(1, &FramebufferName);
		glDeleteProgram(ProgramName[program::SPLASH]);
		glDeleteProgram(ProgramName[program::RENDER]);
		
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteTextures(texture::MAX, &TextureName[0]);
		glDeleteVertexArrays(program::MAX, &VertexArrayName[0]);

		return true;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
			glm::mat4* Pointer = static_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER,
				0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

			//glm::mat4 Projection = glm::perspectiveFov(glm::pi<float>() * 0.25f, 640.f, 480.f, 0.1f, 100.0f);
			glm::mat4 const Projection = glm::perspective(glm::pi<float>() * 0.25f, WindowSize.x / WindowSize.y, 0.1f, 100.0f);
		
			*Pointer = Projection * this->view();

			// Make sure the uniform buffer is uploaded
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		// Render a textured quad to a sRGB framebuffer object.
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glViewport(0, 0, static_cast<GLsizei>(WindowSize.x) * this->FramebufferScale, static_cast<GLsizei>(WindowSize.y) * this->FramebufferScale);

			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
			// Explicitly convert linear pixel color to sRGB color space, as FramebufferName is a sRGB FBO
			// Shader execution is done with linear color to get correct linear algebra working.
			glEnable(GL_FRAMEBUFFER_SRGB);

			float Depth(1.0f);
			glClearBufferfv(GL_DEPTH, 0, &Depth);
			glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

			glUseProgram(ProgramName[program::RENDER]);

			glBindVertexArray(VertexArrayName[program::RENDER]);
			glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0);
		}

		// Blit the sRGB framebuffer to the default framebuffer back buffer.
		{
			glDisable(GL_DEPTH_TEST);

			glViewport(0, 0, static_cast<GLsizei>(WindowSize.x), static_cast<GLsizei>(WindowSize.y));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// glDisable(GL_FRAMEBUFFER_SRGB); // Uncomment to avoid the extra linear to sRGB conversion and hence get correct display

			glUseProgram(ProgramName[program::SPLASH]);
			glBindVertexArray(VertexArrayName[program::SPLASH]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
		}

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_320_fbo_srgb Test(argc, argv);
	Error += Test();

	return Error;
}

