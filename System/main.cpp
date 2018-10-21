#include <iostream>
#include "Logger.h"
#include "Judge.h"

#include "util.h"
#include "Mark.h"
#include <thread>
#include <cstring>
#include <string>

using std::cout;
using std::cin;

int main()
{
	Logger* logger = new Logger(nullptr);
	Judge* judge = new Judge(logger);

	bool is = true;
	std::thread test = std::thread([&]() {
		const string PATH = "/mnt/c/Users/Jinho/Desktop/JOJ2/Users/testuser/";
		FILE* r = fopen((PATH + "test.c").c_str(), "rt");
		char buf[512] = { 0, };
		fread(buf, 1, 512, r);
		fclose(r);
		while (is)
		{
			std::string fileName = "9999.";
			fileName += getTime();
			fileName += ".c";
			FILE* w = fopen((PATH + fileName).c_str(), "wt");
			fwrite(buf, 1, strlen(buf), w);
			fclose(w);
			Mark* mark = new Mark("testuser", fileName, 9999);
			judge->judge(mark);

			std::this_thread::sleep_for(std::chrono::milliseconds(75));

		//	for (int i = 0; mark->compileCmd[i] != nullptr; ++i) logger->log(mark->compileCmd[i]);
		}
	});

	while (true)
	{
		int what;
		cin >> what;
		if (what == 0) break;
	}

	is = false;
	test.join();

	judge->shutdown();
	delete judge;

	logger->end();

	return 0;
}