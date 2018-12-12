#pragma once
#include <iostream>

namespace AlgTheoryLab2
{
	constexpr bool isPowerOf2(int num)
	{
		return (num != 0) && ((num &(num - 1)) == 0);
	}

	double random(double min, double max)
	{
		double superRandomDouble = static_cast<double>(rand() + rand() + rand() + rand()) / (static_cast<double>(RAND_MAX)*4.);
		return superRandomDouble * (max - min) + min;
	}

	void CheckBounds(int row, int column, int rows, int columns)
	{
		if (row < 0 || row >= rows
			||
			column < 0 || column >= columns)
			std::cerr << "\nAt bounds exceeded: row " << row << " col " << column << " but size is rows " << rows << " cols " << columns << "\n";
	}
}