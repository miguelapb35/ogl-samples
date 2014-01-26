//**********************************
// OpenGL Direct State Access
// 20/02/2011 - 05/07/2012
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include "test.hpp"

namespace
{
	char const * TEXTURE_DIFFUSE("kueken3-bgr8.dds");
	glm::ivec2 const FRAMEBUFFER_SIZE(160, 120);

	GLsizei const VertexCount(6);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-4.0f,-3.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 4.0f,-3.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 4.0f, 3.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2( 4.0f, 3.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-4.0f, 3.0f), glm::vec2(0.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-4.0f,-3.0f), glm::vec2(0.0f, 1.0f))
	};

	namespace program
	{
		enum type
		{
			THROUGH,
			RESOLVE_BOX,
			RESOLVE_NEAR,
			MAX
		};
	}//namespace program

	namespace buffer
	{
		enum type
		{
			VERTEX,
			TRANSFORM,
			BLIT,
			MAX
		};
	}//namespace buffer

	namespace texture
	{
		enum type
		{
			DIFFUSE,
			MULTISAMPLE,
			DEPTH,
			MAX
		};
	}//namespace buffer

	char const * VERT_SHADER_SOURCE("gl-430/dsa-pass1.vert");
	char const * FRAG_SHADER_SOURCE[program::MAX] = 
	{
		"gl-430/dsa-pass1.frag",
		"gl-430/dsa-pass2-msaa-box.frag",
		"gl-430/dsa-pass2-msaa-near.frag",
	};
}//namespace

class gl_430_direct_state_access_ext : public test
{
public:
	gl_430_direct_state_access_ext(int argc, char* argv[]) :
		test(argc, argv, "gl-430-direct-state-access-ext", test::CORE, 4, 3),
		VertexArrayName(0),
		FramebufferName(0),
		SamplerName(0)
	{}

private:
	std::array<GLuint, program::MAX> PipelineName;
	std::array<GLuint, program::MAX> ProgramName;
	std::array<GLuint, buffer::MAX> BufferName;
	std::array<GLuint, texture::MAX> TextureName;
	GLuint VertexArrayName;
	GLuint FramebufferName;
	GLuint SamplerName;

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

		return true;
	}

	bool initProgram()
	{
		bool Validated(true);
	
		for(int i = 0; i < program::MAX; ++i)
		{
			glf::compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, "--version 420 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE[i], "--version 420 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName[i] = glCreateProgram();
			glProgramParameteri(ProgramName[i], GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName[i], VertShaderName);
			glAttachShader(ProgramName[i], FragShaderName);
			glLinkProgram(ProgramName[i]);
			Validated = Validated && glf::checkProgram(ProgramName[i]);

			if(Validated)
			{
				glGenProgramPipelines(program::MAX, &PipelineName[0]);
				glUseProgramStages(PipelineName[i], GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName[i]);
			}
		}

		return Validated;
	}

	bool initBuffer()
	{
		glGenBuffers(buffer::MAX, &BufferName[0]);

		glNamedBufferDataEXT(BufferName[buffer::VERTEX], VertexSize, VertexData, GL_STATIC_DRAW);
		glNamedBufferDataEXT(BufferName[buffer::TRANSFORM], sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
		glNamedBufferDataEXT(BufferName[buffer::BLIT], sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

		return true;
	}

	bool initTexture()
	{
		gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));
		assert(!Texture.empty());

		glGenTextures(texture::MAX, &TextureName[0]);

		//glTextureImage2DEXT(TextureName[texture::DIFFUSE], GL_TEXTURE_2D, 0, GL_RGB8, GLsizei(Texture[0].dimensions().x), GLsizei(Texture[0].dimensions().y), 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
		glTextureStorage2DEXT(TextureName[texture::DIFFUSE], GL_TEXTURE_2D, GLint(Texture.levels()), GL_RGB8, GLsizei(Texture[0].dimensions().x), GLsizei(Texture[0].dimensions().y));
		for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
		{
			glTextureSubImage2DEXT(
				TextureName[texture::DIFFUSE],
				GL_TEXTURE_2D, 
				GLint(Level), 
				0, 0, 
				GLsizei(Texture[Level].dimensions().x), 
				GLsizei(Texture[Level].dimensions().y), 
				GL_BGR, 
				GL_UNSIGNED_BYTE, 
				Texture[Level].data());
		}
		if(Texture.levels() == 1)
			glGenerateTextureMipmapEXT(TextureName[texture::DIFFUSE], GL_TEXTURE_2D);

		// From GL_ARB_texture_storage_multisample
		glTextureStorage2DMultisampleEXT(TextureName[texture::MULTISAMPLE], GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, GL_TRUE);
		glTextureStorage2DMultisampleEXT(TextureName[texture::DEPTH], GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT24, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, GL_TRUE);

		return true;
	}

	bool initFramebuffer()
	{
		bool Validated(true);

		glGenFramebuffers(1, &FramebufferName);
		glNamedFramebufferTextureEXT(FramebufferName, GL_DEPTH_ATTACHMENT, TextureName[texture::DEPTH], 0);
		glNamedFramebufferTextureEXT(FramebufferName, GL_COLOR_ATTACHMENT0, TextureName[texture::MULTISAMPLE], 0);
		Validated = Validated && (glCheckNamedFramebufferStatusEXT(FramebufferName, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		return Validated;
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
			glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
			glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(glf::semantic::attr::POSITION);
			glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);
		glBindVertexArray(0);

/*
		glGenVertexArrays(1, &VertexArrayName);
		glVertexAttribBinding(glf::semantic::attr::POSITION, 0);
		glVertexAttribFormat(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0);

		glVertexAttribBinding(glf::semantic::attr::TEXCOORD, 0);
		glVertexAttribFormat(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2));

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);
*/
		return true;
	}

	void renderFBO()
	{
		{
			glm::mat4* Pointer = (glm::mat4*)glMapNamedBufferRangeEXT(
				BufferName[buffer::TRANSFORM], 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

			glm::mat4 Perspective = glm::perspective(glm::pi<float>() * 0.25f, float(FRAMEBUFFER_SIZE.x) / FRAMEBUFFER_SIZE.y, 0.1f, 100.0f);
			glm::mat4 Model = glm::mat4(1.0f);
			*Pointer = Perspective * this->view() * Model;

			glUnmapNamedBufferEXT(BufferName[buffer::TRANSFORM]);
			//glNamedBufferSubDataEXT(BufferName[buffer::TRANSFORM], 0, sizeof(glm::mat4), &MVP[0][0]);
		}

		glViewportIndexedf(0, 0, 0, float(FRAMEBUFFER_SIZE.x), float(FRAMEBUFFER_SIZE.y));
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		float Depth(1.0f);
		glClearBufferfv(GL_DEPTH, 0, &Depth);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);
		
		glBindProgramPipeline(PipelineName[program::THROUGH]);
		glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
		glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D, TextureName[texture::DIFFUSE]);
		glBindSampler(0, SamplerName);
		glBindVertexArray(VertexArrayName);
		//glBindVertexBuffer(0, BufferName[buffer::VERTEX], 0, sizeof(glf::vertex_v2fv2f));

		glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 5, 0);

		glDisable(GL_MULTISAMPLE);
		glDisable(GL_DEPTH_TEST);
	}

	void resolveMultisampling()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		{
			glm::mat4* Pointer = (glm::mat4*)glMapNamedBufferRangeEXT(
				BufferName[buffer::BLIT], 0, sizeof(glm::mat4),
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

			glm::mat4 Perspective = glm::scale(glm::perspective(glm::pi<float>() * 0.25f, WindowSize.x / WindowSize.y, 0.1f, 100.0f), glm::vec3(1.0f,-1.0f, 1.0f));
			glm::mat4 Model = glm::mat4(1.0f);
			glm::mat4 MVP = Perspective * this->view() * Model;

			*Pointer = MVP;
			glUnmapNamedBufferEXT(BufferName[buffer::BLIT]);
			//glNamedBufferSubDataEXT(BufferName[buffer::BLIT], 0, sizeof(glm::mat4), &MVP[0][0]);
		}

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);
		glEnable(GL_SCISSOR_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::BLIT]);
		glBindMultiTextureEXT(GL_TEXTURE0, GL_TEXTURE_2D_MULTISAMPLE, TextureName[texture::MULTISAMPLE]);
		glBindSampler(0, SamplerName);
		glBindVertexArray(VertexArrayName);

		// Box
		{
			glScissorIndexed(0, 1, 1, WindowSize.x  / 2 - 2, WindowSize.y - 2);
			glBindProgramPipeline(PipelineName[program::RESOLVE_BOX]);
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 5, 0);
		}

		// Near
		{
			glScissorIndexed(0, WindowSize.x / 2 + 1, 1, WindowSize.x / 2 - 2, WindowSize.y - 2);
			glBindProgramPipeline(PipelineName[program::RESOLVE_NEAR]);
			glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, VertexCount, 5, 0);
		}

		glDisable(GL_SCISSOR_TEST);
	}

	bool begin()
	{
		bool Validated = true;
		Validated = Validated && glf::checkExtension("GL_EXT_direct_state_access");
		Validated = Validated && glf::checkExtension("GL_ARB_texture_storage_multisample");
		Validated = Validated && glf::checkExtension("GL_ARB_vertex_attrib_binding");

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

		return Validated;
	}

	bool end()
	{
		glDeleteProgramPipelines(program::MAX, &PipelineName[0]);
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteSamplers(1, &SamplerName);
		glDeleteTextures(texture::MAX, &TextureName[0]);
		glDeleteFramebuffers(1, &FramebufferName);
		glDeleteVertexArrays(1, &VertexArrayName);
		for(int i = 0; i < program::MAX; ++i)
			glDeleteProgram(ProgramName[i]);

		return true;
	}

	bool render()
	{
		// Pass 1: Render the scene in a multisampled framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		renderFBO();

		// Pass 2: Resolved and render the colorbuffer from the multisampled framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);
		resolveMultisampling();

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_430_direct_state_access_ext Test(argc, argv);
	Error += Test();

	return Error;
}

