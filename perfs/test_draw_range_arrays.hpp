#ifndef TEST_DRAW_RANGE_ARRAYS_INCLUDED
#define TEST_DRAW_RANGE_ARRAYS_INCLUDED

#include "test.hpp"

class testDrawRangeArrays : public test
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
		BUFFER_MAX
	};

public:
	testDrawRangeArrays(
		int argc, char* argv[], profile Profile,
		draw const DrawType, std::size_t const DrawCount);
	virtual ~testDrawRangeArrays();

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

#endif//TEST_DRAW_RANGE_ARRAYS_INCLUDED
