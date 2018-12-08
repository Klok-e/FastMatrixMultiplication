#pragma once
#include "Matrix.h"
#include <iostream>

namespace AlgTheoryLab2
{
	bool isPowerOf2(int num);

	void printMatr(Matrix const& m);

	double random(double min, double max);

	void fillWithRandom(Matrix& matr, double min, double max, bool doFloor = false);
}