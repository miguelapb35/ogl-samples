//**********************************
// OpenGL Texture Derivative
// 18/11/2013 - 18/11/2013
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include <glf/glf.hpp>

namespace
{
	char const * SAMPLE_NAME("OpenGL Framebuffer Object");
	char const * VERT_SHADER_SOURCE_TEXTURE("gl-400/texture-derivative2.vert");
	char const * FRAG_SHADER_SOURCE_TEXTURE("gl-400/texture-derivative2.frag");
	char const * VERT_SHADER_SOURCE_SPLASH("gl-400/texture-derivative1.vert");
	char const * FRAG_SHADER_SOURCE_SPLASH("gl-400/texture-derivative1.frag");
	char const * TEXTURE_DIFFUSE("kueken1-dxt1.dds");
	int const FRAMEBUFFER_SIZE(8);
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(4);
	int const SAMPLE_MINOR_VERSION(0);

	glf::window Window(glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

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

	namespace texture
	{
		enum type
		{
			DIFFUSE,
			COLORBUFFER,
			RENDERBUFFER,
			MAX
		};
	}//namespace texture
	
	namespace program
	{
		enum type
		{
			TEXTURE,
			SPLASH,
			MAX
		};
	}//namespace program

	namespace shader
	{
		enum type
		{
			VERT_TEXTURE,
			FRAG_TEXTURE,
			VERT_SPLASH,
			FRAG_SPLASH,
			MAX
		};
	}//namespace shader

	GLuint FramebufferName(0);
	std::vector<GLuint> ShaderName(shader::MAX);
	std::vector<GLuint> ProgramName(program::MAX);
	std::vector<GLuint> VertexArrayName(program::MAX);
	std::vector<GLuint> BufferName(buffer::MAX);
	std::vector<GLuint> TextureName(texture::MAX);
	std::vector<GLint> UniformDiffuse(program::MAX);
	GLint UniformTransform(0);
	GLint UniformFramebufferSize(0);
}//namespace

bool initProgram()
{
	bool Validated(true);

	glf::compiler Compiler;

	if(Validated)
	{
		ShaderName[shader::VERT_TEXTURE] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_TEXTURE, "--version 400 --profile core");
		ShaderName[shader::FRAG_TEXTURE] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_TEXTURE, "--version 400 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[program::TEXTURE] = glCreateProgram();
		glAttachShader(ProgramName[program::TEXTURE], ShaderName[shader::VERT_TEXTURE]);
		glAttachShader(ProgramName[program::TEXTURE], ShaderName[shader::FRAG_TEXTURE]);

		glBindAttribLocation(ProgramName[program::TEXTURE], glf::semantic::attr::POSITION, "Position");
		glBindAttribLocation(ProgramName[program::TEXTURE], glf::semantic::attr::TEXCOORD, "Texcoord");
		glBindFragDataLocation(ProgramName[program::TEXTURE], glf::semantic::frag::COLOR, "Color");
		glLinkProgram(ProgramName[program::TEXTURE]);

		Validated = Validated && glf::checkProgram(ProgramName[program::TEXTURE]);
	}

	if(Validated)
	{
		UniformTransform = glGetUniformBlockIndex(ProgramName[program::TEXTURE], "transform");
		UniformDiffuse[program::TEXTURE] = glGetUniformLocation(ProgramName[program::TEXTURE], "Diffuse");
	}
		
	if(Validated)
	{
		ShaderName[shader::VERT_SPLASH] = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE_SPLASH, "--version 400 --profile core");
		ShaderName[shader::FRAG_SPLASH] = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE_SPLASH, "--version 400 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[program::SPLASH] = glCreateProgram();
		glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::VERT_SPLASH]);
		glAttachShader(ProgramName[program::SPLASH], ShaderName[shader::FRAG_SPLASH]);

		glBindFragDataLocation(ProgramName[program::SPLASH], glf::semantic::frag::COLOR, "Color");
		glLinkProgram(ProgramName[program::SPLASH]);

		Validated = Validated && glf::checkProgram(ProgramName[program::SPLASH]);
	}
	
	if(Validated)
	{
		UniformDiffuse[program::SPLASH] = glGetUniformLocation(ProgramName[program::SPLASH], "Diffuse");
		UniformFramebufferSize = glGetUniformLocation(ProgramName[program::SPLASH], "FramebufferSize");
	}

	return Validated && glf::checkError("initProgram");
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

	glGetIntegerv(
		GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
		&UniformBufferOffset);

	GLint UniformBlockSize = glm::max(GLint(sizeof(glm::mat4)), UniformBufferOffset);

	glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
	glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

bool initTexture()
{
	bool Validated(true);

	gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));
	assert(!Texture.empty());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(texture::MAX, &TextureName[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::DIFFUSE]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glCompressedTexImage2D(
			GL_TEXTURE_2D,
			GLint(Level),
			GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
			GLsizei(Texture[Level].dimensions().x), 
			GLsizei(Texture[Level].dimensions().y), 
			0, 
			GLsizei(Texture[Level].size()), 
			Texture[Level].data());
	}
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_RGBA8, GLsizei(Window.Size.x / FRAMEBUFFER_SIZE), GLsizei(Window.Size.y / FRAMEBUFFER_SIZE), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::RENDERBUFFER]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, GLint(0), GL_DEPTH_COMPONENT24, GLsizei(Window.Size.x / FRAMEBUFFER_SIZE), GLsizei(Window.Size.y / FRAMEBUFFER_SIZE), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return Validated;
}

bool initVertexArray()
{
	glGenVertexArrays(program::MAX, &VertexArrayName[0]);
	glBindVertexArray(VertexArrayName[program::TEXTURE]);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);

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

	if(glf::checkFramebuffer(FramebufferName))
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool initDebugOutput()
{
#	ifdef GL_ARB_debug_output
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallbackARB(&glf::debugOutput, NULL);
#	endif

	return true;
}

bool begin()
{
	bool Validated(true);
	Validated = Validated && glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);

	float MinFragmentInterpolationOffset(0);
	float MaxFragmentInterpolationOffset(0);
	float FragmentInterpolationOffsetBits(0);
	glGetFloatv(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET, &MinFragmentInterpolationOffset);
	glGetFloatv(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET, &MaxFragmentInterpolationOffset);
	glGetFloatv(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS, &FragmentInterpolationOffsetBits);

	if(Validated && glf::checkExtension("GL_ARB_debug_output"))
		Validated = initDebugOutput();
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
	for(std::size_t i = 0; 0 < shader::MAX; ++i)
		glDeleteShader(ShaderName[i]);
	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteProgram(ProgramName[program::SPLASH]);
	glDeleteProgram(ProgramName[program::TEXTURE]);
	glDeleteBuffers(buffer::MAX, &BufferName[0]);
	glDeleteTextures(texture::MAX, &TextureName[0]);
	glDeleteVertexArrays(program::MAX, &VertexArrayName[0]);

	return glf::checkError("end");
}

void display()
{
	// Update of the uniform buffer
	{
		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(
			GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y - 87.f, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x + 30.0f, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);

/* Flat
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y - 87.f, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x + 3.0f, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);
*/
		*Pointer = Projection * View * Model;

		// Make sure the uniform buffer is uploaded
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glViewport(0, 0, Window.Size.x/ FRAMEBUFFER_SIZE, Window.Size.y/ FRAMEBUFFER_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	float Depth(1.0f);
	glClearBufferfv(GL_DEPTH , 0, &Depth);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

	glUseProgram(ProgramName[program::TEXTURE]);
	glUniform1i(UniformDiffuse[program::TEXTURE], 0);
	glUniformBlockBinding(ProgramName[program::TEXTURE], UniformTransform, glf::semantic::uniform::TRANSFORM0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::DIFFUSE]);
	glBindVertexArray(VertexArrayName[program::TEXTURE]);
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

	glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 2, 0);

	glDisable(GL_DEPTH_TEST);

	glViewport(0, 0, Window.Size.x, Window.Size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(ProgramName[program::SPLASH]);
	glUniform1i(UniformDiffuse[program::SPLASH], 0);
	glUniform1f(UniformFramebufferSize, static_cast<float>(FRAMEBUFFER_SIZE));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VertexArrayName[program::SPLASH]);
	glBindTexture(GL_TEXTURE_2D, TextureName[texture::COLORBUFFER]);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);

	glf::swapBuffers();
}

int main(int argc, char* argv[])
{
	return glf::run(
		argc, argv,
		glm::ivec2(::SAMPLE_SIZE_WIDTH, ::SAMPLE_SIZE_HEIGHT), 
		glf::CORE,
		::SAMPLE_MAJOR_VERSION, ::SAMPLE_MINOR_VERSION);
}
