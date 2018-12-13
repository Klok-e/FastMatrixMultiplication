#include <iostream>
#include <ctime>
#include <vector>
#include <functional>
#include "Matrix.hpp"
#include "Clock.hpp"
#include "ArrayBuilder.hpp"
#include "HelpfulFunctionsMatrices.hpp"
#include "IReadWriteArray2D.hpp"

void testMultiplication(std::function<void()> func, int iters, AlgTheoryLab2::Clock& watch, int size, char const* name)
{
	unsigned long long arraysCreatedBefore = AlgTheoryLab2::ArrayBuilder::_createdArrays;
	unsigned long long arraysDeletedBefore = AlgTheoryLab2::ArrayBuilder::_deletedArrays;
	unsigned long long arraysCreatedSumSizeBefore = AlgTheoryLab2::ArrayBuilder::_createdArrSumSize;
	unsigned long long arraysDeletedSumSizeBefore = AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize;
	time_t sum = 0;
	for (int i = 0; i < iters; i++)
	{
		watch.Start();
		func();
		sum += watch.Elapsed();
	}
	std::cout << name << "\n";
	std::cout << "Time taken mean: " << (sum / iters) << "\n";
	std::cout << "Size: " << size << "\n";
	std::cout << "Arrays created: " << (AlgTheoryLab2::ArrayBuilder::_createdArrays - arraysCreatedBefore) / iters << "\n";
	std::cout << "Arrays deleted: " << (AlgTheoryLab2::ArrayBuilder::_deletedArrays - arraysDeletedBefore) / iters << "\n";
	std::cout << "Array sum size created: " << (AlgTheoryLab2::ArrayBuilder::_createdArrSumSize - arraysCreatedSumSizeBefore) / iters << "\n";
	std::cout << "Array sum size deleted: " << (AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize - arraysDeletedSumSizeBefore) / iters << "\n\n";
}

template<class T>
class MyClass : public AlgTheoryLab2::IReadWriteArray2D<T>
{
public:
	// Inherited via IReadWriteArray2D
	double const & At(int row, int column) const override
	{
		return 0.;
	}
	int Rows() const override
	{
		return 0;
	}
	int Columns() const override
	{
		return 0;
	}
	double & At(int row, int column) override
	{
		double t = 0.;
		return t;
	}
};

int main()
{
	srand(time(nullptr));
	{
		AlgTheoryLab2::Matrix<double> a1(8, 8);
		AlgTheoryLab2::Matrix<double> b1(8, 8);
		AlgTheoryLab2::fillWithRandom(a1, 0, 4, true);
		AlgTheoryLab2::fillWithRandom(b1, 0, 4, true);
		AlgTheoryLab2::printMatr(a1);
		AlgTheoryLab2::printMatr(b1);

		std::cout << "MultiplyStrassenVinogradNoAlloc: \n";
		AlgTheoryLab2::Matrix<double> c1 = a1.MultiplyStrassenVinogradNoAlloc(b1);
		AlgTheoryLab2::printMatr(c1);

		std::cout << "MultiplyNaive: \n";
		c1 = a1.MultiplyNaive(b1);
		AlgTheoryLab2::printMatr(c1);
	}

	std::vector<int> sizes{1024};

	const int iters = 3;
	AlgTheoryLab2::Clock watch;
	for (int sizeInd = 0; sizeInd < sizes.size(); sizeInd++)
	{
		int size = sizes[sizeInd];
		AlgTheoryLab2::Matrix<float> a(size, size);
		AlgTheoryLab2::Matrix<float> b(size, size);

		AlgTheoryLab2::fillWithRandom(a, -1000, 1000);
		AlgTheoryLab2::fillWithRandom(b, -1000, 1000);

		testMultiplication([&]()
		{
			auto c = a.MultiplyStrassenVinogradNoAlloc(b);
		}, iters, watch, size, "MultiplyStrassenVinogradNoAlloc");

		testMultiplication([&]()
		{
			auto c = a.MultiplyNaive(b);
		}, iters, watch, size, "MultiplyNaive");
	}

	std::cout << "Array sum size created: " << AlgTheoryLab2::ArrayBuilder::_createdArrSumSize << "\n";
	std::cout << "Array sum size deleted: " << AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize << "\n";
	if (AlgTheoryLab2::ArrayBuilder::_createdArrSumSize == AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize)
		std::cout << "No leaks. Hurray!\n";

	std::cin.ignore();
}
