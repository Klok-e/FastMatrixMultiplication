#include "Clock.h"

AlgTheoryLab2::Clock::Clock() :
	_timeStart(0)
{
}

void AlgTheoryLab2::Clock::Start()
{
	_timeStart = clock();
}

time_t AlgTheoryLab2::Clock::Elapsed()
{
	return clock() - _timeStart;
}
