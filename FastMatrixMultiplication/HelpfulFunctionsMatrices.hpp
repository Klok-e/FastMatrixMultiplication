#pragma once
#include "Matrix.hpp"
#include <iostream>

namespace AlgTheoryLab2
{
	template<class T>
	void printMatr(Matrix<T> const& m)
	{
		std::cout << "\n";
		for (int row = 0; row < m.Rows(); row++)
		{
			for (int col = 0; col < m.Columns(); col++)
			{
				std::cout << m.At(row, col) << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}

	template<class T>
	void fillWithRandom(Matrix<T>& matr, double min, double max, bool doFloor = false)
	{
		for (int col = 0; col < matr.Columns(); col++)
			for (int row = 0; row < matr.Rows(); row++)
			{
				double rand = random(min, max);
				matr.At(row, col) = doFloor ? std::floor(rand) : rand;
			}
	}
}