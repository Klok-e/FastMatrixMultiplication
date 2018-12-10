#include <iostream>
#include <ctime>
#include <vector>
#include <functional>
#include "Matrix.hpp"
#include "Clock.hpp"
#include "HelpfulFunctions.hpp"
#include "ArrayBuilder.hpp"
#include "HelpfulFunctionsMatrices.hpp"

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

int main()
{
	srand(time(nullptr));
	{
		AlgTheoryLab2::Matrix a1(8, 8);
		AlgTheoryLab2::Matrix b1(8, 8);
		AlgTheoryLab2::fillWithRandom(a1, 0, 4, true);
		AlgTheoryLab2::fillWithRandom(b1, 0, 4, true);
		AlgTheoryLab2::printMatr(a1);
		AlgTheoryLab2::printMatr(b1);

		std::cout << "MultiplyStrassenVinogradNoAlloc: \n";
		AlgTheoryLab2::Matrix c1 = a1.MultiplyStrassenVinogradNoAlloc(b1);
		AlgTheoryLab2::printMatr(c1);

		std::cout << "MultiplyStrassenVinogradAlloc: \n";
		c1 = a1.MultiplyStrassenVinogradAlloc(b1);
		AlgTheoryLab2::printMatr(c1);

		std::cout << "MultiplyStrassenVinogradNoAllocHybrid: \n";
		c1 = a1.MultiplyStrassenVinogradNoAllocHybrid(b1);
		AlgTheoryLab2::printMatr(c1);

		std::cout << "MultiplyStrassenVinogradAllocHybrid: \n";
		c1 = a1.MultiplyStrassenVinogradAllocHybrid(b1);
		AlgTheoryLab2::printMatr(c1);

		std::cout << "MultiplyNaive: \n";
		c1 = a1.MultiplyNaive(b1);
		AlgTheoryLab2::printMatr(c1);
	}

	std::vector<int> sizes{512};

	const int iters = 3;
	AlgTheoryLab2::Clock watch;
	for (int sizeInd = 0; sizeInd < sizes.size(); sizeInd++)
	{
		int size = sizes[sizeInd];
		AlgTheoryLab2::Matrix a(size, size);
		AlgTheoryLab2::Matrix b(size, size);

		AlgTheoryLab2::fillWithRandom(a, -1000, 1000);
		AlgTheoryLab2::fillWithRandom(b, -1000, 1000);

		testMultiplication([&]()
		{
			a.MultiplyStrassenVinogradNoAlloc(b);
		}, iters, watch, size, "MultiplyStrassenVinogradNoAlloc");

		testMultiplication([&]()
		{
			a.MultiplyStrassenVinogradAlloc(b);
		}, iters, watch, size, "MultiplyStrassenVinogradAlloc");

		testMultiplication([&]()
		{
			a.MultiplyStrassenVinogradNoAllocHybrid(b);
		}, iters, watch, size, "MultiplyStrassenVinogradNoAllocHybrid");

		testMultiplication([&]()
		{
			a.MultiplyStrassenVinogradAllocHybrid(b);
		}, iters, watch, size, "MultiplyStrassenVinogradAllocHybrid");

		testMultiplication([&]()
		{
			a.MultiplyNaive(b);
		}, iters, watch, size, "MultiplyNaive");
	}

	std::cout << "Array sum size created: " << AlgTheoryLab2::ArrayBuilder::_createdArrSumSize << "\n";
	std::cout << "Array sum size deleted: " << AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize << "\n";
	if (AlgTheoryLab2::ArrayBuilder::_createdArrSumSize == AlgTheoryLab2::ArrayBuilder::_deletedArrSumSize)
		std::cout << "No leaks. Hurray!\n";

	std::cin.ignore();
}
