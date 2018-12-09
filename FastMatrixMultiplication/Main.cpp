#include <iostream>
#include <ctime>
#include <vector>
#include <functional>
#include "Matrix.h"
#include "Clock.h"
#include "HelpfulFunctions.h"
#include "ArrayBuilder.hpp"

namespace my = AlgTheoryLab2;

void testMultiplication(std::function<void()> func, int iters, my::Clock& watch, int size, char const* name)
{
	unsigned long long arraysCreatedBefore = my::ArrayBuilder::_createdArrays;
	unsigned long long arraysDeletedBefore = my::ArrayBuilder::_deletedArrays;
	unsigned long long arraysCreatedSumSizeBefore = my::ArrayBuilder::_createdArrSumSize;
	unsigned long long arraysDeletedSumSizeBefore = my::ArrayBuilder::_deletedArrSumSize;
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
	std::cout << "Arrays created: " << (my::ArrayBuilder::_createdArrays - arraysCreatedBefore) / iters << "\n";
	std::cout << "Arrays deleted: " << (my::ArrayBuilder::_deletedArrays - arraysDeletedBefore) / iters << "\n";
	std::cout << "Array sum size created: " << (my::ArrayBuilder::_createdArrSumSize - arraysCreatedSumSizeBefore) / iters << "\n";
	std::cout << "Array sum size deleted: " << (my::ArrayBuilder::_deletedArrSumSize - arraysDeletedSumSizeBefore) / iters << "\n\n";
}

int main()
{
	srand(time(nullptr));
	{
		my::Matrix a1(8, 8);
		my::Matrix b1(8, 8);
		my::fillWithRandom(a1, 0, 4, true);
		my::fillWithRandom(b1, 0, 4, true);
		my::printMatr(a1);
		my::printMatr(b1);

		std::cout << "MultiplyStrassenVinogradNoAlloc: \n";
		my::Matrix c1 = a1.MultiplyStrassenVinogradNoAlloc(b1);
		my::printMatr(c1);

		std::cout << "MultiplyStrassenVinogradAlloc: \n";
		c1 = a1.MultiplyStrassenVinogradAlloc(b1);
		my::printMatr(c1);

		std::cout << "MultiplyStrassenVinogradNoAllocHybrid: \n";
		c1 = a1.MultiplyStrassenVinogradNoAllocHybrid(b1);
		printMatr(c1);

		std::cout << "MultiplyStrassenVinogradAllocHybrid: \n";
		c1 = a1.MultiplyStrassenVinogradAllocHybrid(b1);
		printMatr(c1);

		std::cout << "MultiplyNaive: \n";
		c1 = a1.MultiplyNaive(b1);
		printMatr(c1);
	}

	std::vector<int> sizes{1024};

	const int iters = 3;
	my::Clock watch;
	for (int sizeInd = 0; sizeInd < sizes.size(); sizeInd++)
	{
		int size = sizes[sizeInd];
		my::Matrix a(size, size);
		my::Matrix b(size, size);

		my::fillWithRandom(a, -1000, 1000);
		my::fillWithRandom(b, -1000, 1000);

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

	std::cout << "Array sum size created: " << my::ArrayBuilder::_createdArrSumSize << "\n";
	std::cout << "Array sum size deleted: " << my::ArrayBuilder::_deletedArrSumSize << "\n";
	if (my::ArrayBuilder::_createdArrSumSize == my::ArrayBuilder::_deletedArrSumSize)
		std::cout << "No leaks. Hurray!\n";

	std::cin.ignore();
}


