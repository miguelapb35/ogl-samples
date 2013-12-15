#ifndef TEST_DRAW_ELEMENTS_INCLUDED
#define TEST_DRAW_ELEMENTS_INCLUDED

#include "test.hpp"

class testDrawElements : public test
{
public:
	enum draw
	{
		DRAW_PACKED,
		DRAW_PARAMS
	};

private:
	enum buffer
	{
		BUFFER_FRAME,
		BUFFER_ARRAY,
		BUFFER_ELEMENT,
		BUFFER_MAX
	};

public:
	testDrawElements(
		int argc, char* argv[], profile Profile,
		draw const DrawType, std::size_t const DrawCount);
	virtual ~testDrawElements();

	virtual void render();

	bool initProgram();
	bool initBuffer();
	bool initVertexArray();

private:
	draw const DrawType;
	std::size_t const DrawCount;
	GLuint VertexArrayName;
	GLuint PipelineName;
	GLuint ProgramName;
	std::array<GLuint, BUFFER_MAX> BufferName;
	GLuint QueryName;
};

#endif//TEST_DRAW_ELEMENTS_INCLUDED
