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
#include <glm/gtc/noise.hpp>

namespace
{
	char const * VERT_SHADER_SOURCE("gl-320/texture-float.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/texture-float.frag");

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2(1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
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
			VERTEX,
			ELEMENT,
			TRANSFORM,
			MAX
		};
	}//namespace buffer

	namespace shader
	{
		enum type
		{
			VERT,
			FRAG,
			MAX
		};
	}//namespace shader
}//namespace

class gl_320_texture_float : public test
{
public:
	gl_320_texture_float(int argc, char* argv[]) :
		test(argc, argv, "gl-320-texture-float", test::CORE, 3, 2, glm::uvec2(1280)),
		VertexArrayName(0),
		ProgramName(0),
		TextureName(0)
	{}

private:
	std::array<GLuint, buffer::MAX> BufferName;
	GLuint VertexArrayName;
	GLuint ProgramName;
	GLuint TextureName;

	bool initProgram()
	{
		bool Validated = true;
	
		if(Validated)
		{
			compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 150 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 150 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glAttachShader(ProgramName, VertShaderName);
			glAttachShader(ProgramName, FragShaderName);

			glBindAttribLocation(ProgramName, semantic::attr::POSITION, "Position");
			glBindAttribLocation(ProgramName, semantic::attr::TEXCOORD, "Texcoord");
			glBindFragDataLocation(ProgramName, semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName);
			Validated = Validated && Compiler.checkProgram(ProgramName);
		}

		if (Validated)
		{
			glUniformBlockBinding(ProgramName, glGetUniformBlockIndex(ProgramName, "transform"), semantic::uniform::TRANSFORM0);

			glUseProgram(ProgramName);
			glUniform1i(glGetUniformLocation(ProgramName, "Diffuse"), 0);
			glUseProgram(0);
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

		return this->checkError("initBuffer");
	}

	bool initTexture()
	{
		glm::u8vec3 ColorSRGB[] =
		{
			glm::u8vec3(225,   0, 152), // Rhodamine Red C
			glm::u8vec3(239,  51,  64), // Red 032 C
			glm::u8vec3(254,  80,   0), // Orange 021 C
			glm::u8vec3(254, 221,   0), // Yellow C
			glm::u8vec3(  0, 172, 140), // Green C
			glm::u8vec3(  0, 131, 195), // Process Blue C
			glm::u8vec3( 63,  67, 173), // Blue 072 C
			glm::u8vec3(187,  41, 187)  // Purple C
		};

		glm::u8vec3 ColorRainbow[] =
		{
			glm::u8vec3(214,  37, 152), // Pink C
			//glm::u8vec3(225,   0, 152), // Rhodamine Red C
			//glm::u8vec3(249,  56,  34), // Bright Red 032 C
			glm::u8vec3(239,  51,  64), // Red 032 C
			//glm::u8vec3(255,  94,   0), // Bright Orange C
			glm::u8vec3(254,  80,   0), // Orange 021 C
			glm::u8vec3(255, 215,   0), // Yellow 012 C
			//glm::u8vec3(0, 171, 132),  // Green C
			glm::u8vec3(0, 132, 61),  // 348 C (Green)
			glm::u8vec3(0, 133, 202),  // Process Blue C
			glm::u8vec3(16,   6, 159), // Blue 072 C
			glm::u8vec3( 78,  0, 142)  // Medium Purple C
		};

		glm::u8vec3 Color[] =
		{
			glm::u8vec3(239,  51,  64), // Red 032 C
			glm::u8vec3(0, 132, 61),  // 348 C (Green)
			glm::u8vec3(214,  37, 152), // Pink C
			glm::u8vec3(0, 133, 202),  // Process Blue C
			glm::u8vec3(255, 215,   0), // Yellow 012 C
			glm::u8vec3(78,  0, 142),  // Medium Purple C
			glm::u8vec3(254,  80,   0), // Orange 021 C
			glm::u8vec3(16,   6, 159), // Blue 072 C
		};

		std::size_t const Size = 64;

		float const Scale = 0.1f;

		std::vector<glm::i8vec3> Data;
		Data.resize(Size * Size);
		for (std::size_t j = 0; j < Size; ++j)
		for (std::size_t i = 0; i < Size; ++i)
		{
			std::size_t const Index = i + j * Size;
			//Data[Index] = static_cast<float>(i) / 8.0f;//glm::simplex(glm::vec3(i, j, static_cast<std::size_t>(0)));
			float const Sample = glm::abs(glm::simplex(glm::vec3(i * Scale, j * Scale, static_cast<std::size_t>(0)))) * 1.7f;
			std::size_t const ColorIndex = static_cast<std::size_t>(glm::clamp(Sample * 7.0f, 0.0f, 7.0f));
			assert(ColorIndex >= 0 && ColorIndex <= 7);
			Data[Index] = Color[ColorIndex];
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &TextureName);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureName);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Size, Size, 0, GL_RGB, GL_UNSIGNED_BYTE, &Data[0]);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		return this->checkError("initTexture");
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), BUFFER_OFFSET(0));
		glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(semantic::attr::POSITION);
		glEnableVertexAttribArray(semantic::attr::TEXCOORD);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		return this->checkError("initVertexArray");
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initTexture();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initVertexArray();

		return Validated && this->checkError("begin");
	}

	bool end()
	{
		glDeleteProgram(ProgramName);
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteTextures(1, &TextureName);
		glDeleteVertexArrays(1, &VertexArrayName);

		return this->checkError("end");
	}

	bool render()
	{
		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
			glm::mat4* Pointer = reinterpret_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

			glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 4.0f, 0.1f, 100.0f);
			glm::mat4 Model = glm::mat4(1.0f);

			*Pointer = Projection * this->view() * Model;

			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		glDrawBuffer(GL_BACK);

		glm::uvec2 WindowSize = this->getWindowSize();

		glViewport(0, 0, WindowSize.x, WindowSize.y);
		glDisable(GL_FRAMEBUFFER_SRGB);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f)[0]);

		glUseProgram(ProgramName);

		glDisable(GL_FRAMEBUFFER_SRGB);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureName);
		glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
		glBindVertexArray(VertexArrayName);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_320_texture_float Test(argc, argv);
	Error += Test();

	return Error;
}

