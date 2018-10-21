#include <ctime>
#include "util.h"

vector<string> split(const string & s, const char c)
{
	vector<string> r;
	r.push_back("");
	for (auto i : s)
	{
		if (i == c) r.push_back("");
		else r.back().push_back(i);
	}
	return r;
}

string getTime()
{
	time_t t;
	time(&t);
	struct tm* local = localtime(&t);

	string ret = "";
	ret += std::to_string(local->tm_year + 1900) + '-';
	ret += std::to_string(local->tm_mon + 1) + '-';
	ret += std::to_string(local->tm_mday) + '-';
	ret += std::to_string(local->tm_hour) + '-';
	ret += std::to_string(local->tm_min) + '-';
	ret += std::to_string(local->tm_sec);
	return ret;
}

std::ostream& operator<<(std::ostream& os, const Mark* mark)
{
	return os;
}
