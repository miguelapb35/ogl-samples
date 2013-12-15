#ifndef CSV_INCLUDED
#define CSV_INCLUDED

#include <vector>
#include <string>

class csv
{
	struct data
	{
		data(
			std::string const & String,
			float Convergent, float Min, float Max) :
			String(String),
			Convergent(Convergent), Min(Min), Max(Max)
		{}

		std::string String;
		float Convergent;
		float Min;
		float Max;
	};

public:
	void log(char const * String, float Average, float Min, float Max);

	void save(char const * Filename);
	void print();

private:
	std::vector<data> Data;
};

#endif//CSV_INCLUDED
