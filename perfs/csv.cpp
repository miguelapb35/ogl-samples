#include "csv.hpp"
#include <cstdio>

std::string format(const char * Message, ...)
{
	assert(Message);
	
	char Text[1024];
	std::memset(Text, 0, sizeof(Text));

	va_list ap;
	va_start(ap, Message);
		std::vsprintf(Text, Message, ap);
	va_end(ap);

	return Text;
}

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

