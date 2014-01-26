//**********************************
// OpenGL Programmable bilinear filtering on images
// 13/08/2012 - 13/08/2012
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
	char const * VERT_SHADER_SOURCE("gl-430/image-sampling.vert");
	char const * FRAG_SHADER_SOURCE("gl-430/image-sampling.frag");
	char const * TEXTURE_DIFFUSE("kueken1-bgr8.dds");

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
}//namespace

class gl_430_image_sampling : public test
{
public:
	gl_430_image_sampling(int argc, char* argv[]) :
		test(argc, argv, "gl-430-image-sampling", test::CORE, 4, 3),
		VertexArrayName(0),
		PipelineName(0),
		ProgramName(0),
		TextureName(0),
		SamplerName(0)
	{}

private:
	std::array<GLuint, buffer::MAX> BufferName;
	GLuint VertexArrayName;
	GLuint PipelineName;
	GLuint ProgramName;
	GLuint TextureName;
	GLuint SamplerName;

	bool initProgram()
	{
		bool Validated = true;

		glGenProgramPipelines(1, &PipelineName);

		if(Validated)
		{
			glf::compiler Compiler;
			GLuint VertShaderName = Compiler.create(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERT_SHADER_SOURCE, 
				"--version 420 --profile core");
			GLuint FragShaderName = Compiler.create(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAG_SHADER_SOURCE,
				"--version 420 --profile core");
			Validated = Validated && Compiler.check();

			ProgramName = glCreateProgram();
			glProgramParameteri(ProgramName, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(ProgramName, VertShaderName);
			glAttachShader(ProgramName, FragShaderName);
			glLinkProgram(ProgramName);

			Validated = Validated && glf::checkProgram(ProgramName);
		}

		if(Validated)
			glUseProgramStages(PipelineName, GL_VERTEX_SHADER_BIT | GL_FRAGMENT_SHADER_BIT, ProgramName);

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

		glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return true;
	}

	bool initTexture()
	{
		gli::texture2D Texture(gli::load_dds((glf::DATA_DIRECTORY + TEXTURE_DIFFUSE).c_str()));
		assert(!Texture.empty());

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &TextureName);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureName);
		glTexStorage2D(GL_TEXTURE_2D, GLint(Texture.levels()), GL_RGBA8UI, GLsizei(Texture.dimensions().x), GLsizei(Texture.dimensions().y));
		for(gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level)
		{
			glTexSubImage2D(
				GL_TEXTURE_2D, 
				GLint(Level), 
				0, 0, 
				GLsizei(Texture[Level].dimensions().x), 
				GLsizei(Texture[Level].dimensions().y), 
				GL_BGR_INTEGER, GL_UNSIGNED_BYTE, 
				Texture[Level].data());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		return true;
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

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferName[buffer::ELEMENT]);
		glBindVertexArray(0);

		return true;
	}

	bool begin()
	{
		bool Validated = true;

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initTexture();
		if(Validated)
			Validated = initVertexArray();

		return Validated;
	}

	bool end()
	{
		glDeleteBuffers(buffer::MAX, &BufferName[0]);
		glDeleteTextures(1, &TextureName);
		glDeleteSamplers(1, &SamplerName);
		glDeleteVertexArrays(1, &VertexArrayName);
		glDeleteProgram(ProgramName);
		glDeleteProgramPipelines(1, &PipelineName);

		return true;
	}

	bool render()
	{
		glm::vec2 WindowSize(this->getWindowSize());

		{
			glBindBuffer(GL_UNIFORM_BUFFER, BufferName[buffer::TRANSFORM]);
			glm::mat4* Pointer = (glm::mat4*)glMapBufferRange(
				GL_UNIFORM_BUFFER, 0,	sizeof(glm::mat4),
				GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

			glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 1000.0f);
			glm::mat4 Model = glm::mat4(1.0f);
			*Pointer = Projection * this->view() * Model;

			// Make sure the uniform buffer is uploaded
			glUnmapBuffer(GL_UNIFORM_BUFFER);
		}

		glViewportIndexedf(0, 0, 0, WindowSize.x, WindowSize.y);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

		glBindProgramPipeline(PipelineName);
		glBindBufferBase(GL_UNIFORM_BUFFER, glf::semantic::uniform::TRANSFORM0, BufferName[buffer::TRANSFORM]);
		glBindImageTexture(0, TextureName, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(1, TextureName, 1, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
		glBindImageTexture(2, TextureName, 2, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

		glBindVertexArray(VertexArrayName);
		glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, ElementCount, GL_UNSIGNED_SHORT, nullptr, 3, 0, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	gl_430_image_sampling Test(argc, argv);
	Error += Test();

	return Error;
}
