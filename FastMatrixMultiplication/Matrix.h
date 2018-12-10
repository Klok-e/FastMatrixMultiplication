#pragma once
#include "Array.hpp"

namespace AlgTheoryLab2
{
	class Matrix
	{
	public:
		static const int _hybridFallbackThreshold = 64;

		// usual constructor
		Matrix(int rows, int columns, bool fillWithZeros = false);

		// construct with this array
		Matrix(int rows, int columns, double* data, bool fillWithZeros = false);

		// default constructor
		Matrix();

		// copy constructor
		Matrix(const Matrix& other);

		// move constructor
		Matrix(Matrix&& other);

		// assignment copy operator
		Matrix& operator=(const Matrix& other);

		// assignment move operator
		Matrix& operator=(Matrix&& other);

		Matrix operator+(const Matrix& other)const;
		Matrix operator-(const Matrix& other)const;

		double& At(int row, int column);
		double At(int row, int column) const;

		Matrix MultiplyStrassenVinogradNoAlloc(const Matrix& other) const;
		Matrix MultiplyStrassenVinogradNoAllocHybrid(const Matrix & other) const;
		Matrix MultiplyStrassenVinogradAlloc(const Matrix & other) const;
		Matrix MultiplyStrassenVinogradAllocHybrid(const Matrix& other) const;
		Matrix MultiplyNaive(const Matrix & other) const;

		int Rows() const;
		int Columns() const;

		Matrix CreateSubView(int rowStart, int colStart, int rowEnd, int colEnd) const;

	private:
		Array<double> _data;

		int _rows, _columns;

		int _rowStart, _colStart, _rowEnd, _colEnd;

		int _rowsOrig, _colsOrig;

		void CheckCompatibilityForMult(const Matrix& other) const;
		void CheckCompatibilityForAdd(const Matrix& other) const;

		bool IsSameDimensions(const Matrix& other) const;

		int GetIndex(int row, int column) const;

		void MultiplyStrassenVinogradNoAllocP(const Matrix& other, Matrix& writeTo, double* data) const;
		void MultiplyStrassenVinogradNoAllocHybridP(const AlgTheoryLab2::Matrix & other, AlgTheoryLab2::Matrix & result, double * aux) const;
		Matrix MultiplyStrassenVinogradAllocP(const Matrix & other) const;
		Matrix MultiplyStrassenVinogradAllocHybridP(const Matrix& other) const;
		void MultiplyNaiveP(const Matrix & other, Matrix & result) const;

		void Add(const Matrix& other, Matrix & writeTo) const;
		void Subtract(const Matrix& other, Matrix & writeTo) const;

		template<int auxCount, int stopAtDimensions>
		static int CalculateSizeOfAllAuxiliaryMatrices(const int n);

		static void CheckBounds(int row, int column, int rows, int columns);
	};

	template<int auxCount, int stopAtDimensions>
	int AlgTheoryLab2::Matrix::CalculateSizeOfAllAuxiliaryMatrices(const int n)
	{
#if _DEBUG
		if (!AlgTheoryLab2::isPowerOf2(stopAtDimensions))
			std::cerr << "\nError: not power of 2\n";
#endif

		if (n <= stopAtDimensions)
			return 0;
		return (n * n / 4) * (auxCount);
	}
}
