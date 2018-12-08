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
}