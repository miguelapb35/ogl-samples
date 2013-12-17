#include "test_draw_arrays.hpp"
#include "test_draw_elements.hpp"

std::size_t const TEST_DUPLICATE_COUNT(1);

int drawArrays(int argc, char* argv[], csv & CSV)
{
	struct entry
	{
		entry(
			std::string const & String,
			testDrawArrays::drawType const & DrawType,
			testDrawArrays::vertexDataType const & VertexDataType,
			std::size_t const & DrawCount
		) :
			String(String),
			DrawType(DrawType),
			VertexDataType(VertexDataType),
			DrawCount(DrawCount)
		{}

		std::string const String;
		testDrawArrays::drawType const DrawType;
		testDrawArrays::vertexDataType const VertexDataType;
		std::size_t const DrawCount;
	};

	int Error(0);

	std::vector<entry> Entries;
	Entries.push_back(entry("DrawArrays(PACKED, SEPARATED)", testDrawArrays::DRAW_PACKED, testDrawArrays::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(PACKED, SHARED)", testDrawArrays::DRAW_PACKED, testDrawArrays::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(PARAMS, SEPARATED)", testDrawArrays::DRAW_PARAMS, testDrawArrays::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(PARAMS, SHARED)", testDrawArrays::DRAW_PARAMS, testDrawArrays::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(MULTI, SEPARATED)", testDrawArrays::MULTI_DRAW, testDrawArrays::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(MULTI, SHARED)", testDrawArrays::MULTI_DRAW, testDrawArrays::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(DISCARD, SEPARATED)", testDrawArrays::MULTI_DISCARD, testDrawArrays::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(DISCARD, SHARED)", testDrawArrays::MULTI_DISCARD, testDrawArrays::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawArrays(INSTANCED, SHARED)", testDrawArrays::INSTANCED, testDrawArrays::SHARED_VERTEX_DATA, 100000));

	for(std::size_t EntryIndex(0); EntryIndex < Entries.size(); ++EntryIndex)
	for(std::size_t TestIndex(0); TestIndex < TEST_DUPLICATE_COUNT; ++TestIndex)
	{
		testDrawArrays Test(argc, argv, test::CORE, 
			Entries[EntryIndex].DrawType, Entries[EntryIndex].VertexDataType, Entries[EntryIndex].DrawCount);
		Error += Test();
		Test.log(CSV, Entries[EntryIndex].String.c_str());
	}

	return Error;
}

int drawElements(int argc, char* argv[], csv & CSV)
{
	struct entry
	{
		entry(
			std::string const & String,
			testDrawElements::drawType const & DrawType,
			testDrawElements::vertexDataType const & VertexDataType,
			std::size_t const & DrawCount
		) :
			String(String),
			DrawType(DrawType),
			VertexDataType(VertexDataType),
			DrawCount(DrawCount)
		{}

		std::string const String;
		testDrawElements::drawType const DrawType;
		testDrawElements::vertexDataType const VertexDataType;
		std::size_t const DrawCount;
	};

	int Error(0);

	std::vector<entry> Entries;
	Entries.push_back(entry("DrawElements(PACKED, SEPARATED)", testDrawElements::DRAW_PACKED, testDrawElements::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(PACKED, SHARED)", testDrawElements::DRAW_PACKED, testDrawElements::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(PARAMS, SEPARATED)", testDrawElements::DRAW_PARAMS, testDrawElements::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(PARAMS, SHARED)", testDrawElements::DRAW_PARAMS, testDrawElements::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(MULTI, SEPARATED)", testDrawElements::MULTI_DRAW, testDrawElements::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(MULTI, SHARED)", testDrawElements::MULTI_DRAW, testDrawElements::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(DISCARD, SEPARATED)", testDrawElements::MULTI_DISCARD, testDrawElements::SEPARATED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(DISCARD, SHARED)", testDrawElements::MULTI_DISCARD, testDrawElements::SHARED_VERTEX_DATA, 100000));
	Entries.push_back(entry("DrawElements(INSTANCED, SHARED)", testDrawElements::INSTANCED, testDrawElements::SHARED_VERTEX_DATA, 100000));

	for(std::size_t EntryIndex(0); EntryIndex < Entries.size(); ++EntryIndex)
	for(std::size_t TestIndex(0); TestIndex < TEST_DUPLICATE_COUNT; ++TestIndex)
	{
		testDrawElements Test(argc, argv, test::CORE, 
			Entries[EntryIndex].DrawType, Entries[EntryIndex].VertexDataType, Entries[EntryIndex].DrawCount);
		Error += Test();
		Test.log(CSV, Entries[EntryIndex].String.c_str());
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
