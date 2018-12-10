#pragma once
#include <ctime>

namespace AlgTheoryLab2
{
	class Clock
	{
	public:
		Clock();
		void Start();
		time_t Elapsed();

	private:
		time_t _timeStart;
	};

	Clock::Clock() :
		_timeStart(0)
	{
	}

	void Clock::Start()
	{
		_timeStart = clock();
	}

	time_t Clock::Elapsed()
	{
		return clock() - _timeStart;
	}
}
