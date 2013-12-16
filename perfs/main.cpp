#include "test_draw_arrays.hpp"
#include "test_draw_elements.hpp"

std::size_t const TEST_DUPLICATE_COUNT(1);

int drawArrays(int argc, char* argv[], csv & CSV)
{
	int Error(0);

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::DRAW_PACKED, testDrawArrays::SEPARATED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(PACKED, SEPARATED) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::DRAW_PACKED, testDrawArrays::SHARED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(PACKED, SHARED) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::DRAW_PARAMS, testDrawArrays::SHARED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(PARAMS) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::MULTI_DRAW, testDrawArrays::SHARED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(MULTI) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::MULTI_DISCARD, testDrawArrays::SHARED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(DISCARD) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			testDrawArrays::INSTANCED, testDrawArrays::SHARED_VERTEX_DATA, 100000);
		Error += Test();
		Test.log(CSV, "DrawArrays(INSTANCED) 100000");
	}

	return Error;
}

int drawElements(int argc, char* argv[], csv & CSV)
{
	int Error = 0;

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			testDrawElements::DRAW_PACKED, 100000);
		Error += Test();
		Test.log(CSV, "DrawElements(PACKED) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			testDrawElements::DRAW_PARAMS, 100000);
		Error += Test();
		Test.log(CSV, "DrawElements(PARAMS) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			testDrawElements::MULTI_DRAW, 100000);
		Error += Test();
		Test.log(CSV, "DrawElements(MULTI) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			testDrawElements::MULTI_DISCARD, 100000);
		Error += Test();
		Test.log(CSV, "DrawElements(DISCARD) 100000");
	}

	for(std::size_t i = 0; i < TEST_DUPLICATE_COUNT; ++i)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			testDrawElements::INSTANCED, 100000);
		Error += Test();
		Test.log(CSV, "DrawElements(INSTANCED) 100000");
	}

	return Error;
}

int main(int argc, char* argv[])
{
	int Error(0);

	csv CSV;

	Error += drawArrays(argc, argv, CSV);
	Error += drawElements(argc, argv, CSV);

	CSV.print();
	CSV.save("../draws.csv");

	return Error;
}
