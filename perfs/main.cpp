#include "test_draw_range_arrays.hpp"

int draw_range(int argc, char* argv[])
{
	int Error = 0;

	csv CSV;

	{
		testDrawRangeArrays TestDrawRangeArrays(argc, argv, test::CORE, testDrawRangeArrays::DRAW_PACKED, 100000);
		Error += TestDrawRangeArrays();
		TestDrawRangeArrays.log(CSV, "DrawRangeArrays(PACKED) 100000");
	}

	{
		testDrawRangeArrays TestDrawRangeArrays(argc, argv, test::CORE, testDrawRangeArrays::DRAW_PARAMS, 100000);
		Error += TestDrawRangeArrays();
		TestDrawRangeArrays.log(CSV, "DrawRangeArrays(PARAMS) 100000");
	}

	CSV.print();

	return 0;
}

int main(int argc, char* argv[])
{
	int Error = 0;

	Error += draw_range(argc, argv);

	return 0;
}
