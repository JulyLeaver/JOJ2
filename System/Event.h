#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <class T>
class Event
{
public:
	std::queue<T> _q;

private:
	bool isExit;
	std::thread _thread;
	std::mutex _mutex;
	std::condition_variable _cv;

	// 스레드 시작 확인
	bool isStart;
	std::mutex _mutexStart;
	std::condition_variable _cvStart;

public:
	Event() : isExit(false), isStart(false)
	{
		_thread = std::thread(&Event::process, this);

		std::unique_lock<std::mutex> ul(_mutexStart);
		_cvStart.wait(ul, [=]()->bool {
			return isStart;
		});
	}
	virtual ~Event()
	{
	}

	void end()
	{
		isExit = true;
		_cv.notify_all();
		_thread.join();

		delete this;
	}

private:
	void process()
	{
		isStart = true;
		_cvStart.notify_all();

		std::unique_lock<std::mutex> ul(_mutex);
		while (!isExit)
		{
			// lock
			_cv.wait(ul); // unlock
			// lock

			ul.unlock();

			/*
			while (true)
			{
				ul.lock();
				if (_q.empty()) break;
				std::cout << _q.front() << '\n';
				_q.pop();
				ul.unlock();
			}
			*/
			loop(ul); // lock 상태에서 탈출해야 함
		}
	}

protected:
	void push(T _t)
	{
		std::unique_lock<std::mutex> ul(_mutex);
		_q.push(_t);
		_cv.notify_all();
	}

	virtual void loop(std::unique_lock<std::mutex>& ul) = 0;
};