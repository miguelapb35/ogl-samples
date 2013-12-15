#ifndef TEST_DRAW_RANGE_ELEMENTS_INCLUDED
#define TEST_DRAW_RANGE_ELEMENTS_INCLUDED

#include "test.hpp"

class testDrawRangeElements : public test
{
	enum draw
	{
		DRAW_MIN,
		DRAW_FULL
	};

	enum buffer
	{
		BUFFER_FRAME,
		BUFFER_ARRAY,
		BUFFER_ELEMENT,
		BUFFER_MAX
	};

public:
	testDrawRangeElements(
		int argc, char* argv[], profile Profile,
		draw const DrawType, std::size_t const DrawCount);
	virtual ~testDrawRangeElements();

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

#endif//TEST_DRAW_RANGE_ELEMENTS_INCLUDED
