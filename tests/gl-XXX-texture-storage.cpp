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
	char const * VERT_SHADER_SOURCE("gl-430/texture-storage.vert");
	char const * FRAG_SHADER_SOURCE("gl-430/texture-storage.frag");
	char const * TEXTURE_DIFFUSE("kueken1-bgr8.dds");

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLushort);
	GLushort const ElementData[ElementCount] =
	{
		0, 1, 2, 
		2, 3, 0
	};

	namespace program
	{
		enum type
		{
			VERTEX,
			FRAGMENT,
			MAX
		};
	}//namespace program

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
}//namespace

class gl_XXX_texture_storage : public test
{
public:
	gl_XXX_texture_storage(int argc, char* argv[]) :
		test(argc, argv, "gl-XXX-texture-storage", test::CORE, 9, 9),
		PipelineName(0),
		VertexArrayName(0),
		TextureName(0)
	{}

private:
	GLuint PipelineName;
	GLuint VertexArrayName;
	GLuint TextureName;
	std::array<GLuint, program::MAX> ProgramName;
	std::array<GLuint, buffer::MAX> BufferName;

	bool initProgram()
	{
		bool Validated(true);
	
		if(Validated)
		{
			std::string Source = loadFile(getDataDirectory() + SHADER_SOURCE);
			this->Program = glCreateProgram(Source.c_str());
		}

		if(Validated)
		{
			this->Pipeline = glCreatePipeline(GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT);
			glPipelineStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);
		}

		return Validated;
	}

	bool initBuffer()
	{
		bool Validated(true);

		this->Buffer[buffer::ELEMENT] = glCreateBuffer(ElementSize, ElementData, 0);
		this->Buffer[buffer::VERTEX] = glCreateBuffer(VertexSize, VertexData, 0);

		GLint UniformBufferOffset(0);
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &UniformBufferOffset);
		GLint UniformBlockSize = glm::max(GLint(sizeof(glm::mat4)), UniformBufferOffset);

		this->Buffer[buffer::TRANSFORM] = glCreateBuffer(UniformBlockSize, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

		return Validated;
	}

	bool initTexture()
	{
		bool Validated(true);

		gli::texture2D Texture(gli::load_dds((getDataDirectory() + TEXTURE_DIFFUSE).c_str()));

		this->Texture = glCreateTexture(Device, GL_TEXTURE_2D, static_cast<GLint>(Texture.levels()), gli::internal_format(Texture.format()), 
			GLsizei(Texture[0].dimensions().x), GLsizei(Texture[0].dimensions().y), GLsizei(Texture[0].dimensions().z), 0);

		for(gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			glCopyTextureSubImage(Queue[queue::TRANSFER], this->Texture, GLint(Level),
				0, 0, 0,
				GLsizei(Texture[Level].dimensions().x), GLsizei(Texture[Level].dimensions().y), 1,
				Texture[Level].data());
		}

		glTextureSwizzle(this->Texture, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA);
		glTextureLevels(this->Texture, 0, GLint(Texture.levels() - 1));

		return Validated;
	}

	bool begin()
	{
		bool Validated(true);
		Validated = Validated && this->checkExtension("GL_ARB_texture_query_levels");
		Validated = Validated && this->checkExtension("GL_ARB_texture_view");
		Validated = Validated && this->checkExtension("GL_ARB_fragment_layer_viewport");

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initTexture();

		return Validated;
	}

	bool end()
	{
		glDeletePipelines(PipelineName);
		glDeleteProgram(ProgramName[program::FRAGMENT]);
		glDeleteProgram(ProgramName[program::VERTEX]);
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteTextures(1, &TextureName);

		return true;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		{
			// Compute the MVP (Model View Projection matrix)
			float Aspect = (WindowSize.x * 0.33f) / (WindowSize.y * 0.50f);
			glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, Aspect, 0.1f, 100.0f);
			glm::mat4 MVP = Projection * this->test::view() * glm::mat4(1.0f);

			*UniformPointer = MVP;
		}

 		GLsync Sync = glFlushBuffer(Queue[queue::GRAPHICS], Buffer[buffer::TRANSFORM], 0, sizeof(glm::mat4));

		glViewportIndexedf(Queue[queue::GRAPHICS], 0, 0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(Queue[queue::GRAPHICS], GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		glBindPipeline(Queue[queue::GRAPHICS], PipelineName);
		glBindBufferBase(Queue[queue::GRAPHICS], GL_UNIFORM_BUFFER, 0, Buffer[buffer::TRANSFORM]);
		glBindBuffer(Queue[queue::GRAPHICS], GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);

		glFence(Queue[queue::GRAPHICS], Sync);
		glDrawElements(Queue[queue::GRAPHICS], GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0, 0);

		return true;
	}
};

/*
GLsync CopyTextureSubImage(Device
	HGLRC hSrcRC, uint srcName, enum srcTarget, int srcLevel,
	int srcX, int srcY, int srcZ,
	HGLRC hDstRC, uint dstName, enum dstTarget, int dstLevel,
	int dstX, int dstY, int dstZ,
	sizei width, sizei height, sizei depth);
*/

int main(int argc, char* argv[])
{
	int Error(0);

	gl_XXX_texture_storage Test(argc, argv);
	Error += Test();

	return Error;
}
