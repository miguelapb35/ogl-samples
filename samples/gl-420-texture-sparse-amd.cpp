//**********************************
// OpenGL Samples Pack 
// ogl-samples.g-truc.net
//**********************************
// OpenGL Sparse Texture
// 04/10/2012 - 04/10/2012
//**********************************
// Christophe Riccio
// ogl-samples@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include <glf/glf.hpp>
#include <glf/compiler.hpp>

namespace
{
	std::string const SAMPLE_NAME("OpenGL Sparse Texture");
	std::string const VERT_SHADER_SOURCE(glf::DATA_DIRECTORY + "gl-420/texture-sparse.vert");
	std::string const FRAG_SHADER_SOURCE(glf::DATA_DIRECTORY + "gl-420/texture-sparse.frag");
	std::string const TEXTURE_DIFFUSE(glf::DATA_DIRECTORY + "kueken1-bgr8.dds");
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(4);
	int const SAMPLE_MINOR_VERSION(2);

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
	 
	GLuint PipelineName(0);
	GLuint ProgramName[program::MAX] = {0, 0};
	GLuint VertexArrayName(0);
	GLuint BufferName[buffer::MAX] = {0, 0, 0};
	GLuint TextureName(0);

	#define GL_TEXTURE_STORAGE_SPARSE_BIT_AMD 0x00000001

	#define GL_VIRTUAL_PAGE_SIZE_X_AMD 0x9195
	#define GL_VIRTUAL_PAGE_SIZE_Y_AMD 0x9196
	#define GL_VIRTUAL_PAGE_SIZE_Z_AMD 0x9197

	#define GL_MAX_SPARSE_TEXTURE_SIZE_AMD 0x9198
	#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_AMD 0x9199
	#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_AMD 0x919A

	#define GL_MIN_SPARSE_LEVEL_AMD 0x919B
	#define GL_MIN_LOD_WARNING_AMD 0x919C

	typedef void (GLAPIENTRY * PFNGLTEXSTORAGESPARSEAMDPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);
	typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGESPARSEAMDPROC) (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags);

	PFNGLTEXSTORAGESPARSEAMDPROC glTexStorageSparseAMD = 0;
	PFNGLTEXTURESTORAGESPARSEAMDPROC glTextureStorageSparseAMD = 0;
}//namespace

bool initProgram()
{
	bool Validated(true);
	
	glGenProgramPipelines(1, &PipelineName);

	if(Validated)
	{
		glf::compiler Compiler;
		GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, VERT_SHADER_SOURCE, 
			"--version 420 --profile core");
		GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, FRAG_SHADER_SOURCE,
			"--version 430 --profile core");
		Validated = Validated && Compiler.check();

		ProgramName[program::VERTEX] = glCreateProgram();
		glProgramParameteri(ProgramName[program::VERTEX], GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(ProgramName[program::VERTEX], VertShaderName);
		glLinkProgram(ProgramName[program::VERTEX]);
		glDeleteShader(VertShaderName);

		ProgramName[program::FRAGMENT] = glCreateProgram();
		glProgramParameteri(ProgramName[program::FRAGMENT], GL_PROGRAM_SEPARABLE, GL_TRUE);
		glAttachShader(ProgramName[program::FRAGMENT], FragShaderName);
		glLinkProgram(ProgramName[program::FRAGMENT]);
		glDeleteShader(FragShaderName);

		Validated = Validated && glf::checkProgram(ProgramName[program::VERTEX]);
		Validated = Validated && glf::checkProgram(ProgramName[program::FRAGMENT]);
	}

	if(Validated)
	{
		glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT, ProgramName[program::VERTEX]);
		glUseProgramStages(PipelineName, GL_FRAGMENT_SHADER_BIT, ProgramName[program::FRAGMENT]);
	}

	return Validated;
}

bool initBuffer()
{
	bool Validated(true);

	glGenBuffers(buffer::MAX, BufferName);

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

	return Validated;
}

bool initTexture()
{
	bool Validated(true);

	gli::texture2D Texture = gli::load(TEXTURE_DIFFUSE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLsizei const Size(4096);

	glGenTextures(1, &TextureName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(glm::log2(4096.f)) - GLint(glm::log2(256.f)));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexStorage2D(GL_TEXTURE_2D, GLint(glm::log2(4096.f)), GL_RGBA8, GLsizei(Size), GLsizei(Size));

	glTextureStorageSparseAMD(TextureName, GL_TEXTURE_2D, GL_RGBA8, Size, Size, GLsizei(1), GLsizei(1), GL_TEXTURE_STORAGE_SPARSE_BIT_AMD);

	GLint PageSizeX(0);
	GLint PageSizeY(0);
	GLint PageSizeZ(0);
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_X_AMD, 1, &PageSizeX);
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Y_AMD, 1, &PageSizeY);
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_VIRTUAL_PAGE_SIZE_Z_AMD, 1, &PageSizeZ);

	for(std::size_t Level = 0; Level < GLint(glm::log2(256.f)); ++Level)
	{
		GLsizei const Width = (Size >> Level) / Texture[0].dimensions().x;
		GLsizei const Height = (Size >> Level) / Texture[0].dimensions().y;

		for(GLsizei j = 0; j < Height; ++j)
		for(GLsizei i = 0; i < Width; ++i)
		{
			if(i % 2 && j % 2)
				continue;

			glTexSubImage2D(
				GL_TEXTURE_2D, 
				GLint(Level), 
				i * GLsizei(Texture[0].dimensions().x), 
				j * GLsizei(Texture[0].dimensions().y), 
				GLsizei(Texture[0].dimensions().x), 
				GLsizei(Texture[0].dimensions().y), 
				GL_BGR, GL_UNSIGNED_BYTE, 
				Texture[0].data());
		}
	}

	//glGenerateMipmap(GL_TEXTURE_2D);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return Validated;
}

bool initVertexArray()
{
	bool Validated(true);

	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
		glBindBuffer(GL_ARRAY_BUFFER, BufferName[buffer::VERTEX]);
		glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(0));
		glVertexAttribPointer(glf::semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(glf::vertex_v2fv2f), GLF_BUFFER_OFFSET(sizeof(glm::vec2)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glEnableVertexAttribArray(glf::semantic::attr::TEXCOORD);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
	glBindVertexArray(0);

	return Validated;
}

bool initDebugOutput()
{
	bool Validated(true);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(&glf::debugOutput, NULL);

	return Validated;
}

bool begin()
{
	bool Validated(true);
	Validated = Validated && glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);
	Validated = Validated && glf::checkExtension("GL_AMD_sparse_texture");

	glTexStorageSparseAMD = (PFNGLTEXSTORAGESPARSEAMDPROC)glutGetProcAddress("glTexStorageSparseAMD");
	glTextureStorageSparseAMD = (PFNGLTEXTURESTORAGESPARSEAMDPROC)glutGetProcAddress("glTextureStorageSparseAMD");

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

	return Validated;
}

bool end()
{
	bool Validated(true);

	glDeleteProgramPipelines(1, &PipelineName);
	glDeleteProgram(ProgramName[program::FRAGMENT]);
	glDeleteProgram(ProgramName[program::VERTEX]);
	glDeleteBuffers(buffer::MAX, BufferName);
	glDeleteTextures(1, &TextureName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return Validated;
}

void display()
{
	// Update of the uniform buffer
	{
		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(
			GL_UNIFORM_BUFFER, 0,	sizeof(glm::mat4),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.001f, 100.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);
		
		*Pointer = Projection * View * Model;

		// Make sure the uniform buffer is uploaded
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	glViewportIndexedf(0, 0, 0, GLfloat(Window.Size.x), GLfloat(Window.Size.y));
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

	// Bind rendering objects
	glBindProgramPipeline(PipelineName);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureName);
	glBindVertexArray(VertexArrayName);
	glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);

	glDrawElementsInstancedBaseVertexBaseInstance(
		GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, 0, 1, 0, 0);

	glf::swapBuffers();
}

int main(int argc, char* argv[])
{
	return glf::run(
		argc, argv,
		glm::ivec2(::SAMPLE_SIZE_WIDTH, ::SAMPLE_SIZE_HEIGHT), 
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 
		::SAMPLE_MAJOR_VERSION, ::SAMPLE_MINOR_VERSION);
}
