#include "test.hpp"

namespace
{
	char const * VERT_SHADER_SOURCE("gl-320/texture-2d.vert");
	char const * FRAG_SHADER_SOURCE("gl-320/texture-2d.frag");
	char const * TEXTURE_DIFFUSE("kueken7_rgb8_unorm.ktx");

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

	class context_handle
	{
	public:
		context_handle()
			: Window(InvalidName)
		{
			assert(this->Window);
		}

		context_handle(GLFWwindow* window)
			: Window(reinterpret_cast<std::size_t>(window))
		{
			assert(this->Window);
		}

		friend bool operator==(context_handle const & A, context_handle const & B)
		{
			return A.Window == B.Window;
		}

		friend bool operator!=(context_handle const & A, context_handle const & B)
		{
			return A.Window != B.Window;
		}

		static const context_handle Invalidhandle;

	private:
		static const std::size_t InvalidName = static_cast<std::size_t>(-1);

		std::size_t Window;
	};

	context_handle const context_handle::Invalidhandle;

	enum object_type
	{
		OBJECT_BUFFER, ONJECT_FIRST = OBJECT_BUFFER,
		OBJECT_TEXTURE,
		OBJECT_SAMPLER,
		OBJECT_SHADER,
		OBJECT_PROGRAM,
		OBJECT_PIPELINE,
		OBJECT_FRAMEBUFFER,
		OBJECT_VERTEXARRAY,
		OBJECT_QUERY, ONJECT_LAST = OBJECT_QUERY
	};

	enum
	{
		OBJECT_COUNT = ONJECT_LAST - ONJECT_FIRST + 1
	};

	char const* get_name(object_type Type)
	{
		static char const* Table[]
		{
			"Buffer",			//OBJECT_BUFFER
			"Texture",			//OBJECT_TEXTURE
			"Sampler",			//OBJECT_SAMPLER
			"Shader",			//OBJECT_SHADER
			"Program",			//OBJECT_PROGRAM
			"Pipeline",			//OBJECT_PIPELINE
			"Framebuffer",		//OBJECT_FRAMEBUFFER
			"Vertexarray",		//OBJECT_VERTEXARRAY
			"Query"				//OBJECT_QUERY
		};
		static_assert(sizeof(Table) / sizeof(Table[0]) == OBJECT_COUNT, "Table needs to be updated");

		return Table[Type];
	}

	bool is_per_context(object_type Type)
	{
		static bool Table[]
		{
			false,			//OBJECT_BUFFER
			false,			//OBJECT_TEXTURE
			false,			//OBJECT_SAMPLER
			false,			//OBJECT_SHADER
			false,			//OBJECT_PROGRAM
			true,			//OBJECT_PIPELINE
			true,			//OBJECT_FRAMEBUFFER
			true,			//OBJECT_VERTEXARRAY
			true			//OBJECT_QUERY
		};
		static_assert(sizeof(Table) / sizeof(Table[0]) == OBJECT_COUNT, "Table needs to be updated");

		return Table[Type];
	}

	template <object_type Type>
	class handle
	{
	public:
		handle()
			: Name(InvalidName)
			, Context(context_handle::Invalidhandle)
		{}

		handle(context_handle Context, std::uint32_t Name)
			: Name(Name)
			, Context(Context)
		{
			assert(Name != InvalidName);
		}

		bool is_valid() const
		{
			return this->Name != InvalidName;
		}

		std::uint32_t get(context_handle Context) const
		{
			if(is_per_context(Type))
			{
				assert(this->Context == Context);
				if(this->Context != Context)
					return InvalidName;
			}

			return this->Name;
		}

		char const* name() const
		{
			return get_name(Type);
		}

		static handle<Type> const Invalidhandle;

	private:
		static const std::uint32_t InvalidName = 0xFFFFFFFF;

		std::uint32_t Name;
		context_handle Context;
	};

	template <object_type Type>
	handle<Type> const handle<Type>::Invalidhandle;

	typedef handle<OBJECT_BUFFER> buffer_handle;
	typedef handle<OBJECT_TEXTURE> texture_handle;
	typedef handle<OBJECT_SAMPLER> sampler_handle;
	typedef handle<OBJECT_SHADER> shader_handle;
	typedef handle<OBJECT_PROGRAM> program_handle;
	typedef handle<OBJECT_PIPELINE> pipeline_handle;
	typedef handle<OBJECT_FRAMEBUFFER> framebuffer_handle;
	typedef handle<OBJECT_VERTEXARRAY> vertexarray_handle;
	typedef handle<OBJECT_QUERY> query_handle;

	texture_handle create_texture(context_handle Context)
	{
		GLuint TextureName = 0;
		glGenTextures(1, &TextureName);
		return texture_handle(Context, TextureName);
	}

	void release_texture(context_handle Context, texture_handle& Texture)
	{
		GLuint TextureName = Texture.get(Context);
		glDeleteTextures(1, &TextureName);
		Texture = texture_handle::Invalidhandle;
	}

	std::uint32_t access_texture(context_handle Context, texture_handle Texture)
	{
		return Texture.get(Context);
	}
	
}//namespace

class gl_320_texture2d : public test
{
public:
	gl_320_texture2d(int argc, char* argv[]) :
		test(argc, argv, "gl-320-texture-2d", test::CORE, 3, 2),
		VertexArrayName(0),
		ProgramName(0)
	{}

private:
	std::array<GLuint, buffer::MAX> BufferName;
	GLuint VertexArrayName;
	GLuint ProgramName;
	texture_handle TextureHandle;
	context_handle ContextHandle;

	bool initProgram()
	{
		bool Validated(true);
	
		if(Validated)
		{
			compiler Compiler;
			std::vector<GLuint> ShaderName(shader::MAX);
			ShaderName[shader::VERT] = Compiler.create(GL_VERTEX_SHADER, getDataDirectory() + VERT_SHADER_SOURCE, "--version 150 --profile core");
			ShaderName[shader::FRAG] = Compiler.create(GL_FRAGMENT_SHADER, getDataDirectory() + FRAG_SHADER_SOURCE, "--version 150 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glAttachShader(ProgramName, ShaderName[shader::VERT]);
			glAttachShader(ProgramName, ShaderName[shader::FRAG]);

			glBindAttribLocation(ProgramName, semantic::attr::POSITION, "Position");
			glBindAttribLocation(ProgramName, semantic::attr::TEXCOORD, "Texcoord");
			glBindFragDataLocation(ProgramName, semantic::frag::COLOR, "Color");
			glLinkProgram(ProgramName);
			Validated = Validated && Compiler.check_program(ProgramName);
		}

		if(Validated)
		{
			glUniformBlockBinding(ProgramName, glGetUniformBlockIndex(ProgramName, "transform"), semantic::uniform::TRANSFORM0);

			glUseProgram(ProgramName);
			glUniform1i(glGetUniformLocation(ProgramName, "Diffuse"), 0);
			glUseProgram(0);
		}

		Validated = Validated && this->checkError("initProgram");
	
		return Validated;
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
		gli::gl GL(gli::gl::PROFILE_GL33);

		gli::texture TextureLoad(gli::load((getDataDirectory() + TEXTURE_DIFFUSE)));
		assert(!TextureLoad.empty());

		gli::texture2d Texture(TextureLoad);

		//gli::texture2d TextureLoaded(gli::FORMAT_RGB8_SRGB_PACK8, gli::texture2d::extent_type(256), 1);
		//for(int TexelCoordY = 0; TexelCoordY < TextureLoaded.extent().y; ++TexelCoordY)
		//for(int TexelCoordX = 0; TexelCoordX < TextureLoaded.extent().x; ++TexelCoordX)
		//{
		//	TextureLoaded.store(gli::texture2d::extent_type(TexelCoordX, TexelCoordY), 0, glm::u8vec3(TexelCoordX, TexelCoordY, TexelCoordY));
		//}

		//gli::fsampler2D Sampler(TextureLoaded, gli::WRAP_CLAMP_TO_EDGE);
		//Sampler.generate_mipmaps(gli::FILTER_LINEAR);
		//Sampler.clear(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		//gli::texture2d Texture = Sampler();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		this->TextureHandle = ::create_texture(this->ContextHandle);

		gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ::access_texture(this->ContextHandle, this->TextureHandle));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture.levels() == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, -1000.f);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 1000.f);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

		for(gli::texture2d::size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			gli::texture2d::extent_type const Extent = Texture[Level].extent();
			glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(Level),
				Format.Internal,
				static_cast<GLsizei>(Extent.x), static_cast<GLsizei>(Extent.y),
				0,
				Format.External, Format.Type,
				Texture[Level].data());
		}

		if(Texture.levels() == 1)
			glGenerateMipmap(GL_TEXTURE_2D);
	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		return true;
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

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initTexture();
		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initVertexArray();

		return Validated;
	}

	bool end()
	{
		glDeleteProgram(ProgramName);
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		::release_texture(this->ContextHandle, this->TextureHandle);
		glDeleteVertexArrays(1, &VertexArrayName);

		return true;
	}

	bool render()
	{
		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
			glm::mat4* Pointer = static_cast<glm::mat4*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

			//glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.0f);
			glm::mat4 Projection = glm::perspectiveFov(glm::pi<float>() * 0.25f, 640.f, 480.f, 0.1f, 100.0f);
			glm::mat4 Model = glm::mat4(1.0f);
		
			*Pointer = Projection * this->view() * Model;

			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		glDrawBuffer(GL_BACK);

		glm::uvec2 WindowSize = this->getWindowSize();

		glViewport(0, 0, WindowSize.x, WindowSize.y);
		glDisable(GL_FRAMEBUFFER_SRGB);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		glUseProgram(ProgramName);

		glDisable(GL_FRAMEBUFFER_SRGB);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, access_texture(this->ContextHandle, this->TextureHandle));
		glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
		glBindVertexArray(VertexArrayName);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_320_texture2d Test(argc, argv);
	Error += Test();

	return Error;
}
