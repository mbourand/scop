#include "../inc/Utils.h"

std::list<std::string> utils::split(const std::string& str, const std::string& charset)
{
	std::list<std::string> ret;

	size_t i = str.find_first_not_of(charset, 0);
	while (i < str.size())
	{
		size_t word_start = i;
		i = str.find_first_of(charset, i);
		size_t word_end = i;
		i = str.find_first_not_of(charset, i);
		ret.push_back(str.substr(word_start, word_end - word_start));
	}
	return ret;
}