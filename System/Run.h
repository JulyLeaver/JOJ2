#pragma once

#include "Event.h"
#include "Mark.h"
#include "Logger.h"
#include "WrapUp.h"

class Run : public Event<Mark*>
{
private:
	Logger* logger;
	WrapUp* wp;
	int CCU = 2; // ���� ä���� ��

	const int OUT_MAX = 128; // ��� ������ �� ���� �ִ� ����
	char* user_out, *ac_out;
public:
	Run(Logger* logger, WrapUp* wp);
	virtual ~Run();

	inline void run(Mark* mark)
	{
		Event<Mark*>::push(mark);
	}
	virtual void loop(std::unique_lock<std::mutex>& ul) override;
private:
	void executing(Mark* mark);
	bool grading(const string& user_out_data, const string& ac_out_data);
	bool cmp(const char* ac, const char* user);
};

