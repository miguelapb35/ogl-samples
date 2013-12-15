#include "csv.hpp"
#include <cstdio>

void csv::log(char const * String, float Convergent, float Min, float Max)
{
	this->Data.push_back(data(String, Convergent, Min, Max));
}

void csv::save(char const * Filename)
{
	FILE* File(fopen(Filename, "a+"));
	for(std::size_t i = 0; i < this->Data.size(); ++i)
	{
		fprintf(File, "%s, %2.5f, %2.5f, %2.5f\n",
			Data[i].String.c_str(),
			Data[i].Convergent,
			Data[i].Min, Data[i].Max);
	}
	fclose(File);
}

void csv::print()
{
	fprintf(stdout, "\n");
	for(std::size_t i = 0; i < this->Data.size(); ++i)
	{
		fprintf(stdout, "%s, %2.5f, %2.5f, %2.5f\n",
			Data[i].String.c_str(),
			Data[i].Convergent,
			Data[i].Min, Data[i].Max);
	}
}

