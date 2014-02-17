#ifndef TEST_COMPILER_INCLUDED
#define TEST_COMPILER_INCLUDED

#include "test.hpp"

class testCompiler : public test
{
public:
	enum mode
	{
		MULTITHREADED,
		DUALTHREADED,
		SINGLETHREADED,
		MODE_MAX
	};

public:
	testCompiler(
		int argc, char* argv[], profile Profile,
		mode Mode);
	virtual ~testCompiler();

	virtual void render();

private:
	mode Mode;
};

#endif//TEST_COMPILER_INCLUDED
