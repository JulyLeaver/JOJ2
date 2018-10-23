#include <iostream>
#include "Logger.h"
#include "Judge.h"

using std::cout;
using std::cin;

int main()
{ 
	Logger* logger = new Logger(nullptr);
	Judge* judge = new Judge(logger);

	judge->shutdown();
	delete judge;

	logger->end();

	return 0;
}