//**********************************
// OpenGL Framebuffer Multisample
// 28/01/2013 - 28/01/2013
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
	char const * SAMPLE_NAME("OpenGL Framebuffer Multisample");	
	char const * VERTEX_SHADER_SOURCE("gl-330/fbo-integer-render.vert");
	char const * FRAGMENT_SHADER_SOURCE("gl-330/fbo-integer-render.frag");
	char const * TEXTURE_DIFFUSE("kueken3-bgr8.dds");
	glm::ivec2 const FRAMEBUFFER_SIZE(320, 240);
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(3);
	int const SAMPLE_MINOR_VERSION(3);

	glf::window Window(glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

	// With DDS textures, v texture coordinate are reversed, from top to bottom
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
			RENDER,
			SPLASH,
			MAX
		};
	}//namespace buffer

	GLuint VertexArrayName = 0;
	std::vector<GLuint> ProgramName(program::MAX);
	std::vector<GLuint> UniformMVP(program::MAX);
	std::vector<GLuint> UniformDiffuse(program::MAX);

	GLuint BufferName = 0;
	GLuint Texture2DName = 0;
	GLuint SamplerName = 0;
	
	GLuint MultisampleTextureName = 0;
	GLuint ColorTextureName = 0;
	
	GLuint FramebufferRenderName = 0;
	GLuint FramebufferResolveName = 0;
}//namespace

bool initDebugOutput()
{
	bool Validated(true);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageCallbackARB(&glf::debugOutput, NULL);

	return Validated;
}

bool initSampler()
{
	glGenSamplers(1, &SamplerName);

	// Parameters part of the sampler object:
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

bool initProgram()
{
	bool Validated = true;
	
	// Create program
	if(Validated)
	{
		GLuint VertexShaderName = glf::createShader(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERTEX_SHADER_SOURCE);
		GLuint FragmentShaderName = glf::createShader(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAGMENT_SHADER_SOURCE);

		Validated = Validated && glf::checkShader(VertexShaderName, VERTEX_SHADER_SOURCE);
		Validated = Validated && glf::checkShader(FragmentShaderName, FRAGMENT_SHADER_SOURCE);

		ProgramName[program::RENDER] = glCreateProgram();
		glAttachShader(ProgramName[program::RENDER], VertexShaderName);
		glAttachShader(ProgramName[program::RENDER], FragmentShaderName);
		glDeleteShader(VertexShaderName);
		glDeleteShader(FragmentShaderName);

		glLinkProgram(ProgramName[program::RENDER]);
		Validated = Validated && glf::checkProgram(ProgramName[program::RENDER]);
	}

	if(Validated)
	{
		UniformMVP[program::RENDER] = glGetUniformLocation(ProgramName[program::RENDER], "MVP");
		UniformDiffuse[program::RENDER] = glGetUniformLocation(ProgramName[program::RENDER], "Diffuse");
	}

	// Create program
	if(Validated)
	{
		GLuint VertexShaderName = glf::createShader(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERTEX_SHADER_SOURCE);
		GLuint FragmentShaderName = glf::createShader(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAGMENT_SHADER_SOURCE);

		Validated = Validated && glf::checkShader(VertexShaderName, VERTEX_SHADER_SOURCE);
		Validated = Validated && glf::checkShader(FragmentShaderName, FRAGMENT_SHADER_SOURCE);

		ProgramName[program::SPLASH] = glCreateProgram();
		glAttachShader(ProgramName[program::SPLASH], VertexShaderName);
		glAttachShader(ProgramName[program::SPLASH], FragmentShaderName);
		glDeleteShader(VertexShaderName);
		glDeleteShader(FragmentShaderName);

		glLinkProgram(ProgramName[program::SPLASH]);
		Validated = Validated && glf::checkProgram(ProgramName[program::SPLASH]);
	}

	if(Validated)
	{
		UniformMVP[program::SPLASH] = glGetUniformLocation(ProgramName[program::SPLASH], "MVP");
		UniformDiffuse[program::SPLASH] = glGetUniformLocation(ProgramName[program::SPLASH], "Diffuse");
	}

	return Validated && glf::checkError("initProgram");
}

bool initArrayBuffer()
{
	glGenBuffers(1, &BufferName);
	glBindBuffer(GL_ARRAY_BUFFER, BufferName);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, VertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return glf::checkError("initArrayBuffer");;
}

bool initTexture2D()
{
	glGenTextures(1, &Texture2DName);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture2DName);

	gli::texture2D Texture(gli::loadStorageDDS(glf::DATA_DIRECTORY + TEXTURE_DIFFUSE));
	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glTexImage2D(
			GL_TEXTURE_2D, 
			GLint(Level), 
			GL_RGB8, 
			GLsizei(Texture[Level].dimensions().x), 
			GLsizei(Texture[Level].dimensions().y), 
			0,
			GL_BGR, 
			GL_UNSIGNED_BYTE, 
			Texture[Level].data());
	}

	return glf::checkError("initTexture");
}

bool initFramebuffer()
{
	GLint MaxSampleMaskWords(0);
	GLint MaxColorTextureSamples(0);
	GLint MaxDepthTextureSamples(0);
	GLint MaxIntegerSamples(0);

	glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, &MaxSampleMaskWords); 
	glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, &MaxColorTextureSamples); 
	glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &MaxDepthTextureSamples); 
	glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &MaxIntegerSamples); 

	glGenTextures(1, &MultisampleTextureName);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MultisampleTextureName);

	// The second parameter is the number of samples.
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MaxIntegerSamples, GL_RGBA8UI, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, GL_TRUE);

	glGenFramebuffers(1, &FramebufferRenderName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferRenderName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, MultisampleTextureName, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &ColorTextureName);
	glBindTexture(GL_TEXTURE_2D, ColorTextureName);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, 0);

	glGenFramebuffers(1, &FramebufferResolveName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferResolveName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ColorTextureName, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return glf::checkError("initFramebuffer");
}

bool initVertexArray()
{
	glGenVertexArrays(1, &VertexArrayName);
	glBindVertexArray(VertexArrayName);
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
	bool Validated = true;
	Validated = Validated && glf::checkGLVersion(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);

	if(Validated && glf::checkExtension("GL_ARB_debug_output"))
		Validated = initDebugOutput();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initArrayBuffer();
	if(Validated)
		Validated = initVertexArray();
	if(Validated)
		Validated = initSampler();
	if(Validated)
		Validated = initTexture2D();
	if(Validated)
		Validated = initFramebuffer();

	return Validated && glf::checkError("begin");
}

bool end()
{
	glDeleteBuffers(1, &BufferName);
	glDeleteProgram(ProgramName[program::RENDER]);
	glDeleteProgram(ProgramName[program::SPLASH]);
	glDeleteSamplers(1, &SamplerName);
	glDeleteTextures(1, &Texture2DName);
	glDeleteTextures(1, &ColorTextureName);
	glDeleteTextures(1, &MultisampleTextureName);
	glDeleteFramebuffers(1, &FramebufferRenderName);
	glDeleteFramebuffers(1, &FramebufferResolveName);
	glDeleteVertexArrays(1, &VertexArrayName);

	return glf::checkError("end");
}

void renderFBO(GLuint Framebuffer)
{
	glm::mat4 Perspective = glm::perspective(45.0f, float(FRAMEBUFFER_SIZE.x) / FRAMEBUFFER_SIZE.y, 0.1f, 100.0f);
	glm::mat4 ViewFlip = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f,-1.0f, 1.0f));
	glm::mat4 ViewTranslate = glm::translate(ViewFlip, glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y * 2.0));
	glm::mat4 View = glm::rotate(ViewTranslate,-15.f, glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Perspective * View * Model;

	glUseProgram(ProgramName[program::RENDER]);
	glUniform1i(UniformDiffuse[program::RENDER], 0);
	glUniformMatrix4fv(UniformMVP[program::RENDER], 1, GL_FALSE, &MVP[0][0]);

	glViewport(0, 0, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y);
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(0.0f, 0.5f, 1.0f, 1.0f)[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture2DName);
	glBindSampler(0, SamplerName);
	glBindVertexArray(VertexArrayName);

	glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);

	glf::checkError("renderFBO");
}

void renderFB(GLuint Texture2DName)
{
	glm::mat4 Perspective = glm::perspective(45.0f, float(Window.Size.x) / Window.Size.y, 0.1f, 100.0f);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y * 2.0));
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Perspective * View * Model;

	glUseProgram(ProgramName[program::SPLASH]);
	glUniform1i(UniformDiffuse[program::SPLASH], 0);
	glUniformMatrix4fv(UniformMVP[program::SPLASH], 1, GL_FALSE, &MVP[0][0]);

	glViewport(0, 0, Window.Size.x, Window.Size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture2DName);
	glBindSampler(0, SamplerName);
	glBindVertexArray(VertexArrayName);

	glDrawArraysInstanced(GL_TRIANGLES, 0, VertexCount, 1);
		
	glf::checkError("renderFB");
}

void display()
{
	// Clear the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

	// Pass 1
	// Render the scene in a multisampled framebuffer
	glEnable(GL_MULTISAMPLE);
	renderFBO(FramebufferRenderName);
	glDisable(GL_MULTISAMPLE);

	// Resolved multisampling
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FramebufferRenderName);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FramebufferResolveName);
	glBlitFramebuffer(
		0, 0, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, 
		0, 0, FRAMEBUFFER_SIZE.x, FRAMEBUFFER_SIZE.y, 
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Pass 2
	// Render the colorbuffer from the multisampled framebuffer
	renderFB(ColorTextureName);

	glf::checkError("display");
	glf::swapBuffers();
}

int main(int argc, char* argv[])
{
	if(glf::run(
		argc, argv,
		glm::ivec2(::SAMPLE_SIZE_WIDTH, ::SAMPLE_SIZE_HEIGHT), 
		GLF_CONTEXT_CORE_PROFILE_BIT, ::SAMPLE_MAJOR_VERSION, 
		::SAMPLE_MINOR_VERSION))
		return 0;
	return 1;
}
