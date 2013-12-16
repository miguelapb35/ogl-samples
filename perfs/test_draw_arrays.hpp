#ifndef TEST_DRAW_ARRAYS_INCLUDED
#define TEST_DRAW_ARRAYS_INCLUDED

#include "test.hpp"

class testDrawArrays : public test
{
public:
	enum draw
	{
		INSTANCED,
		MULTI_DISCARD,
		MULTI_DRAW,
		DRAW_PACKED,
		DRAW_PARAMS
	};

private:
	enum buffer
	{
		BUFFER_FRAME,
		BUFFER_ARRAY,
		BUFFER_INDIRECT,
		BUFFER_MAX
	};

public:
	testDrawArrays(
		int argc, char* argv[], profile Profile,
		draw const DrawType, std::size_t const DrawCount);
	virtual ~testDrawArrays();

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
};

#endif//TEST_DRAW_ARRAYS_INCLUDED
