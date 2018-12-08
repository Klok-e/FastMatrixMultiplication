#include "HelpfulFunctions.h"

bool AlgTheoryLab2::isPowerOf2(int num)
{
	return (num != 0) && ((num &(num - 1)) == 0);
}

void AlgTheoryLab2::printMatr(AlgTheoryLab2::Matrix const & m)
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

double AlgTheoryLab2::random(double min, double max)
{
	double superRandomDouble = static_cast<double>(rand() + rand() + rand() + rand()) / (static_cast<double>(RAND_MAX)*4.);
	return superRandomDouble * (max - min) + min;
}

void AlgTheoryLab2::fillWithRandom(AlgTheoryLab2::Matrix & matr, double min, double max, bool doFloor)
{
	for (int col = 0; col < matr.Columns(); col++)
		for (int row = 0; row < matr.Rows(); row++)
		{
			double rand = AlgTheoryLab2::random(min, max);
			matr.At(row, col) = doFloor ? std::floor(rand) : rand;
		}
}
