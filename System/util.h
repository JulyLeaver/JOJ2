#pragma once

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

vector<string> split(const string& s, const char c);

string getTime();

class Mark;
std::ostream& operator<<(std::ostream& os, const Mark* mark);