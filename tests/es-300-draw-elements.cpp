//**********************************
// OpenGL Draw Elements
// 10/05/2010 - 12/03/2012
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
	glf::window Window("es-300-draw-elements", glm::ivec2(640, 480));

	char const * VERTEX_SHADER_SOURCE("es-300/flat-color.vert");
	char const * FRAGMENT_SHADER_SOURCE("es-300/flat-color.frag");

	GLsizei const ElementCount(6);
	GLsizeiptr const ElementSize = ElementCount * sizeof(glm::uint32);
	glm::uint32 const ElementData[ElementCount] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLsizei const VertexCount(4);
	GLsizeiptr const PositionSize = VertexCount * sizeof(glm::vec2);
	glm::vec2 const PositionData[VertexCount] =
	{
		glm::vec2(-1.0f,-1.0f),
		glm::vec2( 1.0f,-1.0f),
		glm::vec2( 1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f)
	};

	GLuint VertexArrayName = 0;
	GLuint ProgramName = 0;
	GLuint ArrayBufferName = 0;
	GLuint ElementBufferName = 0;
	GLint UniformMVP = 0;
	GLint UniformDiffuse = 0;
}//namespace

class es_300_draw_elements : public test
{
public:
	es_300_draw_elements(int argc, char* argv[]) :
		test(argc, argv, "es-300-draw-elements", test::ES, 3, 0)
	{}

private:
	bool initProgram()
	{
		bool Validated = true;
		
		// Create program
		if(Validated)
		{
			GLuint VertexShaderName = glf::createShader(GL_VERTEX_SHADER, glf::DATA_DIRECTORY + VERTEX_SHADER_SOURCE);
			GLuint FragmentShaderName = glf::createShader(GL_FRAGMENT_SHADER, glf::DATA_DIRECTORY + FRAGMENT_SHADER_SOURCE);
			glf::checkShader(VertexShaderName, VERTEX_SHADER_SOURCE);
			glf::checkShader(VertexShaderName, FRAGMENT_SHADER_SOURCE);

			ProgramName = glCreateProgram();
			glAttachShader(ProgramName, VertexShaderName);
			glAttachShader(ProgramName, FragmentShaderName);
			glDeleteShader(VertexShaderName);
			glDeleteShader(FragmentShaderName);

			glf::checkProgram(ProgramName);

			glBindAttribLocation(ProgramName, glf::semantic::attr::POSITION, "Position");
			glLinkProgram(ProgramName);
			Validated = glf::checkProgram(ProgramName);
		}

		// Get variables locations
		if(Validated)
		{
			UniformMVP = glGetUniformLocation(ProgramName, "MVP");
			UniformDiffuse = glGetUniformLocation(ProgramName, "Diffuse");
		}

		// Set some variables 
		if(Validated)
		{
			// Bind the program for use
			glUseProgram(ProgramName);

			// Set uniform value
			glUniform4fv(UniformDiffuse, 1, &glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)[0]);

			// Unbind the program
			glUseProgram(0);
		}

		return Validated && glf::checkError("initProgram");
	}

	bool initBuffer()
	{
		glGenBuffers(1, &ArrayBufferName);
		glBindBuffer(GL_ARRAY_BUFFER, ArrayBufferName);
		glBufferData(GL_ARRAY_BUFFER, PositionSize, PositionData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &ElementBufferName);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferName);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementSize, ElementData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return glf::checkError("initBuffer");
	}

	bool initVertexArray()
	{
		glGenVertexArrays(1, &VertexArrayName);
		glBindVertexArray(VertexArrayName);
			glBindBuffer(GL_ARRAY_BUFFER, ArrayBufferName);
				glVertexAttribPointer(glf::semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferName);

			glEnableVertexAttribArray(glf::semantic::attr::POSITION);
		glBindVertexArray(0);

		return glf::checkError("initVertexArray");
	}

	bool begin()
	{
		bool Validated(true);

		char const * Extensions = (char const *)glGetString(GL_EXTENSIONS);
		printf(Extensions);

		if(Validated)
			Validated = initProgram();
		if(Validated)
			Validated = initBuffer();
		if(Validated)
			Validated = initVertexArray();

		return Validated;
	}

	bool end()
	{
		// Delete objects
		glDeleteBuffers(1, &ArrayBufferName);
		glDeleteBuffers(1, &ElementBufferName);
		glDeleteProgram(ProgramName);

		return true;
	}

	bool render()
	{
		// Compute the MVP (Model View Projection matrix)
		glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -this->TranlationCurrent.y));
		glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, this->RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
		glm::mat4 View = glm::rotate(ViewRotateX, this->RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		// Set the display viewport
		glm::ivec2 WindowSize = this->getWindowSize();
		glViewport(0, 0, WindowSize.x, WindowSize.y);

		// Clear color buffer with black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepthf(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind program
		glUseProgram(ProgramName);

		// Set the value of MVP uniform.
		glUniformMatrix4fv(UniformMVP, 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VertexArrayName);

		glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, 0);

		return true;
	}
};

int main(int argc, char* argv[])
{
	int Error(0);

	es_300_draw_elements Test(argc, argv);
	Error += Test();

	return Error;
}



