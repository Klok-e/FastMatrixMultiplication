//#pragma once
#include "Matrix.h"
#include "HelpfulFunctions.h"
#include <algorithm>
#include <iostream>
#include "ArrayBuilder.hpp"

AlgTheoryLab2::Matrix::Matrix(int rows, int columns, bool fillWithZeros) :
	_data(rows * columns),
	_rows(rows),
	_columns(columns),
	_rowStart(0),
	_rowEnd(rows),
	_colStart(0),
	_colEnd(columns),
	_rowsOrig(rows),
	_colsOrig(columns)
{
	if (fillWithZeros)
	{
		int size = _data.Size();
		for (int i = 0; i < size; i++)
			_data[i] = 0;
	}
}

AlgTheoryLab2::Matrix::Matrix(int rows, int columns, double * data, bool fillWithZeros) :
	_data(rows * columns, data),
	_rows(rows),
	_columns(columns),
	_rowStart(0),
	_rowEnd(rows),
	_colStart(0),
	_colEnd(columns),
	_rowsOrig(rows),
	_colsOrig(columns)
{
	if (fillWithZeros)
	{
		int size = _data.Size();
		for (int i = 0; i < size; i++)
			_data[i] = 0;
	}
}

AlgTheoryLab2::Matrix::Matrix() :
	_data(),
	_rows(0),
	_columns(0),
	_rowStart(0),
	_rowEnd(0),
	_colStart(0),
	_colEnd(0),
	_rowsOrig(0),
	_colsOrig(0)
{
}

AlgTheoryLab2::Matrix::Matrix(const Matrix & other) :
	_data(),
	_rows(0),
	_columns(0),
	_rowStart(0),
	_rowEnd(0),
	_colStart(0),
	_colEnd(0),
	_rowsOrig(0),
	_colsOrig(0)
{
	*this = other;
}

AlgTheoryLab2::Matrix::Matrix(Matrix && other) :
	_data(),
	_rows(0),
	_columns(0),
	_rowStart(0),
	_rowEnd(0),
	_colStart(0),
	_colEnd(0),
	_rowsOrig(0),
	_colsOrig(0)
{
	*this = std::move(other);
}

AlgTheoryLab2::Matrix & AlgTheoryLab2::Matrix::operator=(const AlgTheoryLab2::Matrix & other)
{
	if (this != &other)
	{
		_rows = other._rows;
		_columns = other._columns;

		_rowStart = other._rowStart;
		_rowEnd = other._rowEnd;
		_colStart = other._colStart;
		_colEnd = other._colEnd;

		_rowsOrig = other._rowsOrig;
		_colsOrig = other._colsOrig;

		// copy some data
		_data = other._data;
	}
	return *this;
}

AlgTheoryLab2::Matrix & AlgTheoryLab2::Matrix::operator=(AlgTheoryLab2::Matrix && other)
{
	if (this != &other)
	{
		// steal some data
		_data = std::move(other._data);

		_rows = other._rows;
		_columns = other._columns;

		_rowStart = other._rowStart;
		_rowEnd = other._rowEnd;
		_colStart = other._colStart;
		_colEnd = other._colEnd;

		_rowsOrig = other._rowsOrig;
		_colsOrig = other._colsOrig;

		other._rows = 0;
		other._columns = 0;

		other._rowStart = 0;
		other._rowEnd = 0;
		other._colStart = 0;
		other._colEnd = 0;

		other._rowsOrig = 0;
		other._colsOrig = 0;
	}
	return *this;
}

double & AlgTheoryLab2::Matrix::At(int row, int column)
{
	return _data[GetIndex(row, column)];
}

double AlgTheoryLab2::Matrix::At(int row, int column) const
{
	return _data[GetIndex(row, column)];
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradNoAlloc(const AlgTheoryLab2::Matrix & other) const
{
	int size = CalculateSizeOfAllAuxiliaryMatrices<14, 7, 2>(_rows);

	double* dataForAllAuxiliaryMatrices = AlgTheoryLab2::ArrayBuilder::CreateArray<double>(size);

	AlgTheoryLab2::Matrix m(_rows, _columns);
	MultiplyStrassenVinogradNoAllocP(other, m, dataForAllAuxiliaryMatrices);

	AlgTheoryLab2::ArrayBuilder::DeleteArray(dataForAllAuxiliaryMatrices, size);

	return m;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradNoAllocHybrid(const AlgTheoryLab2::Matrix & other) const
{
	int size = CalculateSizeOfAllAuxiliaryMatrices<14, 7, Matrix::_hybridFallbackThreshold>(_rows);

	double* dataForAllAuxiliaryMatrices = AlgTheoryLab2::ArrayBuilder::CreateArray<double>(size);

	AlgTheoryLab2::Matrix m(_rows, _columns);
	MultiplyStrassenVinogradNoAllocHybridP(other, m, dataForAllAuxiliaryMatrices);

	AlgTheoryLab2::ArrayBuilder::DeleteArray(dataForAllAuxiliaryMatrices, size);

	return m;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradAlloc(const AlgTheoryLab2::Matrix& other)const
{
	Matrix m = MultiplyStrassenVinogradAllocP(other);
	return m;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradAllocHybrid(const AlgTheoryLab2::Matrix & other) const
{
	Matrix m = MultiplyStrassenVinogradAllocHybridP(other);
	return m;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyNaive(const AlgTheoryLab2::Matrix & other) const
{
	AlgTheoryLab2::Matrix result(_rows, _columns);
	MultiplyNaiveP(other, result);
	return result;
}

void AlgTheoryLab2::Matrix::MultiplyStrassenVinogradNoAllocP(const AlgTheoryLab2::Matrix& other, AlgTheoryLab2::Matrix& result, double* aux)const
{
#ifdef _DEBUG
	CheckCompatibilityForMult(other);
#endif
	double* auxBefore = aux;

	if (_columns * _rows == 4)
	{
		//split this matrix into 4 scalars
		double a11 = At(0, 0);
		double a12 = At(0, 1);
		double a21 = At(1, 0);
		double a22 = At(1, 1);

		//split other matrix into 4 scalars
		double b11 = other.At(0, 0);
		double b12 = other.At(0, 1);
		double b21 = other.At(1, 0);
		double b22 = other.At(1, 1);

		double s1 = a21 + a22;
		double s2 = s1 - a11;
		double s3 = a11 - a21;
		double s4 = a12 - s2;
		double s5 = b12 - b11;
		double s6 = b22 - s5;
		double s7 = b22 - b12;
		double s8 = s6 - b21;

		double p1 = s2 * s6;
		double p2 = a11 * b11;
		double p3 = a12 * b21;
		double p4 = s3 * s7;
		double p5 = s1 * s5;
		double p6 = s4 * b22;
		double p7 = a22 * s8;

		double t1 = p1 + p2;
		double t2 = t1 + p4;

		result.At(0, 0) = p2 + p3;
		result.At(0, 1) = t1 + p5 + p6;
		result.At(1, 0) = t2 - p7;
		result.At(1, 1) = t2 + p5;
		return;
	}

	int size = _rows / 2;

	// split this matrix into 4 matrices
	Matrix const a11 = CreateSubView(0, 0, size, size);
	Matrix const a12 = CreateSubView(0, size, size, size + size);
	Matrix const a21 = CreateSubView(size, 0, size + size, size);
	Matrix const a22 = CreateSubView(size, size, size + size, size + size);

	// split other matrix into 4 matrices
	Matrix const b11 = other.CreateSubView(0, 0, size, size);
	Matrix const b12 = other.CreateSubView(0, size, size, size + size);
	Matrix const b21 = other.CreateSubView(size, 0, size + size, size);
	Matrix const b22 = other.CreateSubView(size, size, size + size, size + size);

	AlgTheoryLab2::Matrix s1(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s2(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s3(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s4(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s5(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s6(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s7(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s8(size, size, aux);
	aux += size * size;

	unsigned long long taken = aux - auxBefore;

	a21.Add(a22, s1);
	s1.Subtract(a11, s2);
	a11.Subtract(a21, s3);
	a12.Subtract(s2, s4);
	b12.Subtract(b11, s5);
	b22.Subtract(s5, s6);
	b22.Subtract(b12, s7);
	s6.Subtract(b21, s8);

	AlgTheoryLab2::Matrix p1(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p2(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p3(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p4(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p5(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p6(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p7(size, size, aux);
	aux += size * size;

	taken = aux - auxBefore;

	// all these recursive calls can use the same memory block because they aren't simultaneuous
	s2.MultiplyStrassenVinogradNoAllocP(s6, p1, aux);
	a11.MultiplyStrassenVinogradNoAllocP(b11, p2, aux);
	a12.MultiplyStrassenVinogradNoAllocP(b21, p3, aux);
	s3.MultiplyStrassenVinogradNoAllocP(s7, p4, aux);
	s1.MultiplyStrassenVinogradNoAllocP(s5, p5, aux);
	s4.MultiplyStrassenVinogradNoAllocP(b22, p6, aux);
	a22.MultiplyStrassenVinogradNoAllocP(s8, p7, aux);
	aux += CalculateSizeOfAllAuxiliaryMatrices<14, 7, 2>(size);

	AlgTheoryLab2::Matrix t1(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix t2(size, size, aux); aux += size * size;

	taken = aux - auxBefore;

	p1.Add(p2, t1);
	t1.Add(p4, t2);

	AlgTheoryLab2::Matrix c11(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c12(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c21(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c22(size, size, aux); aux += size * size;

	taken = aux - auxBefore;

	p2.Add(p3, c11);
	t1.Add(p5, c12);
	c12.Add(p6, c12);
	t2.Subtract(p7, c21);
	t2.Add(p5, c22);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, col) = c11.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, size + col) = c12.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, col) = c21.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, size + col) = c22.At(row, col);
}

void AlgTheoryLab2::Matrix::MultiplyStrassenVinogradNoAllocHybridP(const AlgTheoryLab2::Matrix& other, AlgTheoryLab2::Matrix& result, double* aux)const
{
#ifdef _DEBUG
	CheckCompatibilityForMult(other);
#endif
	double* auxBefore = aux;

	if (_columns * _rows <= Matrix::_hybridFallbackThreshold * Matrix::_hybridFallbackThreshold)
	{
		MultiplyNaiveP(other, result);
		return;
	}

	int size = _rows / 2;

	// split this matrix into 4 matrices
	Matrix const a11 = CreateSubView(0, 0, size, size);
	Matrix const a12 = CreateSubView(0, size, size, size + size);
	Matrix const a21 = CreateSubView(size, 0, size + size, size);
	Matrix const a22 = CreateSubView(size, size, size + size, size + size);

	// split other matrix into 4 matrices
	Matrix const b11 = other.CreateSubView(0, 0, size, size);
	Matrix const b12 = other.CreateSubView(0, size, size, size + size);
	Matrix const b21 = other.CreateSubView(size, 0, size + size, size);
	Matrix const b22 = other.CreateSubView(size, size, size + size, size + size);

	AlgTheoryLab2::Matrix s1(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s2(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s3(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s4(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s5(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s6(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s7(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix s8(size, size, aux);
	aux += size * size;

	unsigned long long taken = aux - auxBefore;

	a21.Add(a22, s1);
	s1.Subtract(a11, s2);
	a11.Subtract(a21, s3);
	a12.Subtract(s2, s4);
	b12.Subtract(b11, s5);
	b22.Subtract(s5, s6);
	b22.Subtract(b12, s7);
	s6.Subtract(b21, s8);

	AlgTheoryLab2::Matrix p1(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p2(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p3(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p4(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p5(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p6(size, size, aux);
	aux += size * size;

	AlgTheoryLab2::Matrix p7(size, size, aux);
	aux += size * size;

	taken = aux - auxBefore;

	// all these recursive calls can use the same memory block because they aren't simultaneuous
	s2.MultiplyStrassenVinogradNoAllocHybridP(s6, p1, aux);
	a11.MultiplyStrassenVinogradNoAllocHybridP(b11, p2, aux);
	a12.MultiplyStrassenVinogradNoAllocHybridP(b21, p3, aux);
	s3.MultiplyStrassenVinogradNoAllocHybridP(s7, p4, aux);
	s1.MultiplyStrassenVinogradNoAllocHybridP(s5, p5, aux);
	s4.MultiplyStrassenVinogradNoAllocHybridP(b22, p6, aux);
	a22.MultiplyStrassenVinogradNoAllocHybridP(s8, p7, aux);
	aux += CalculateSizeOfAllAuxiliaryMatrices<14, 7, Matrix::_hybridFallbackThreshold>(size);

	AlgTheoryLab2::Matrix t1(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix t2(size, size, aux); aux += size * size;

	taken = aux - auxBefore;

	p1.Add(p2, t1);
	t1.Add(p4, t2);

	AlgTheoryLab2::Matrix c11(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c12(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c21(size, size, aux); aux += size * size;
	AlgTheoryLab2::Matrix c22(size, size, aux); aux += size * size;

	taken = aux - auxBefore;

	p2.Add(p3, c11);
	t1.Add(p5, c12);
	c12.Add(p6, c12);
	t2.Subtract(p7, c21);
	t2.Add(p5, c22);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, col) = c11.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, size + col) = c12.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, col) = c21.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, size + col) = c22.At(row, col);
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradAllocP(const AlgTheoryLab2::Matrix & other) const
{
#ifdef _DEBUG
	if (_columns != other._columns || _rows != other._rows
		||
		!isPowerOf2(_columns) || !isPowerOf2(_rows))
		std::cerr << "!!!!!!!ERRRRROR!!!!!!!!!!!!!!!!";
#endif

	if (_columns * _rows == 4)
	{
		//split this matrix into 4 scalars
		double a11 = At(0, 0);
		double a12 = At(0, 1);
		double a21 = At(1, 0);
		double a22 = At(1, 1);

		//split other matrix into 4 scalars
		double b11 = other.At(0, 0);
		double b12 = other.At(0, 1);
		double b21 = other.At(1, 0);
		double b22 = other.At(1, 1);

		double s1 = a21 + a22;
		double s2 = s1 - a11;
		double s3 = a11 - a21;
		double s4 = a12 - s2;
		double s5 = b12 - b11;
		double s6 = b22 - s5;
		double s7 = b22 - b12;
		double s8 = s6 - b21;

		double p1 = s2 * s6;
		double p2 = a11 * b11;
		double p3 = a12 * b21;
		double p4 = s3 * s7;
		double p5 = s1 * s5;
		double p6 = s4 * b22;
		double p7 = a22 * s8;

		double t1 = p1 + p2;
		double t2 = t1 + p4;

		Matrix result(2, 2);
		result.At(0, 0) = p2 + p3;
		result.At(0, 1) = t1 + p5 + p6;
		result.At(1, 0) = t2 - p7;
		result.At(1, 1) = t2 + p5;
		return result;
	}

	int size = _rows / 2;

	// split this matrix into 4 matrices
	Matrix const a11 = CreateSubView(0, 0, size, size);
	Matrix const a12 = CreateSubView(0, size, size, size + size);
	Matrix const a21 = CreateSubView(size, 0, size + size, size);
	Matrix const a22 = CreateSubView(size, size, size + size, size + size);

	// split other matrix into 4 matrices
	Matrix const b11 = other.CreateSubView(0, 0, size, size);
	Matrix const b12 = other.CreateSubView(0, size, size, size + size);
	Matrix const b21 = other.CreateSubView(size, 0, size + size, size);
	Matrix const b22 = other.CreateSubView(size, size, size + size, size + size);

	Matrix s1 = a21 + a22;
	Matrix s2 = s1 - a11;
	Matrix s3 = a11 - a21;
	Matrix s4 = a12 - s2;
	Matrix s5 = b12 - b11;
	Matrix s6 = b22 - s5;
	Matrix s7 = b22 - b12;
	Matrix s8 = s6 - b21;

	Matrix p1 = s2.MultiplyStrassenVinogradAllocP(s6);
	Matrix p2 = a11.MultiplyStrassenVinogradAllocP(b11);
	Matrix p3 = a12.MultiplyStrassenVinogradAllocP(b21);
	Matrix p4 = s3.MultiplyStrassenVinogradAllocP(s7);
	Matrix p5 = s1.MultiplyStrassenVinogradAllocP(s5);
	Matrix p6 = s4.MultiplyStrassenVinogradAllocP(b22);
	Matrix p7 = a22.MultiplyStrassenVinogradAllocP(s8);

	Matrix t1 = p1 + p2;
	Matrix t2 = t1 + p4;

	Matrix c11 = p2 + p3;
	Matrix c12 = t1 + p5 + p6;
	Matrix c21 = t2 - p7;
	Matrix c22 = t2 + p5;

	Matrix result(_rows, _columns);
	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, col) = c11.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, size + col) = c12.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, col) = c21.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, size + col) = c22.At(row, col);

	return result;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::MultiplyStrassenVinogradAllocHybridP(const AlgTheoryLab2::Matrix& other)const
{
#ifdef _DEBUG
	CheckCompatibilityForMult(other);
#endif

	if (_columns * _rows <= Matrix::_hybridFallbackThreshold * Matrix::_hybridFallbackThreshold)
	{
		return MultiplyNaive(other);
	}

	int size = _rows / 2;

	// split this matrix into 4 matrices
	Matrix const a11 = CreateSubView(0, 0, size, size);
	Matrix const a12 = CreateSubView(0, size, size, size + size);
	Matrix const a21 = CreateSubView(size, 0, size + size, size);
	Matrix const a22 = CreateSubView(size, size, size + size, size + size);

	// split other matrix into 4 matrices
	Matrix const b11 = other.CreateSubView(0, 0, size, size);
	Matrix const b12 = other.CreateSubView(0, size, size, size + size);
	Matrix const b21 = other.CreateSubView(size, 0, size + size, size);
	Matrix const b22 = other.CreateSubView(size, size, size + size, size + size);

	Matrix s1 = a21 + a22;
	Matrix s2 = s1 - a11;
	Matrix s3 = a11 - a21;
	Matrix s4 = a12 - s2;
	Matrix s5 = b12 - b11;
	Matrix s6 = b22 - s5;
	Matrix s7 = b22 - b12;
	Matrix s8 = s6 - b21;

	Matrix p1 = s2.MultiplyStrassenVinogradAllocHybridP(s6);
	Matrix p2 = a11.MultiplyStrassenVinogradAllocHybridP(b11);
	Matrix p3 = a12.MultiplyStrassenVinogradAllocHybridP(b21);
	Matrix p4 = s3.MultiplyStrassenVinogradAllocHybridP(s7);
	Matrix p5 = s1.MultiplyStrassenVinogradAllocHybridP(s5);
	Matrix p6 = s4.MultiplyStrassenVinogradAllocHybridP(b22);
	Matrix p7 = a22.MultiplyStrassenVinogradAllocHybridP(s8);

	Matrix t1 = p1 + p2;
	Matrix t2 = t1 + p4;

	Matrix c11 = p2 + p3;
	Matrix c12 = t1 + p5 + p6;
	Matrix c21 = t2 - p7;
	Matrix c22 = t2 + p5;

	Matrix result(_rows, _columns);
	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, col) = c11.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(row, size + col) = c12.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, col) = c21.At(row, col);

	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			result.At(size + row, size + col) = c22.At(row, col);

	return result;
}

void AlgTheoryLab2::Matrix::Add(const AlgTheoryLab2::Matrix & other, AlgTheoryLab2::Matrix & writeTo) const
{
#if _DEBUG
	CheckCompatibilityForAdd(other);
#endif
	for (int col = 0; col < _columns; col++)
		for (int row = 0; row < _rows; row++)
			writeTo.At(row, col) = At(row, col) + other.At(row, col);
}

void AlgTheoryLab2::Matrix::Subtract(const AlgTheoryLab2::Matrix & other, Matrix & writeTo) const
{
#if _DEBUG
	CheckCompatibilityForAdd(other);
#endif
	for (int col = 0; col < _columns; col++)
		for (int row = 0; row < _rows; row++)
			writeTo.At(row, col) = At(row, col) - other.At(row, col);
}

void AlgTheoryLab2::Matrix::MultiplyNaiveP(const AlgTheoryLab2::Matrix & other, AlgTheoryLab2::Matrix& result) const
{
#if _DEBUG
	CheckCompatibilityForMult(other);
#endif
	for (int cColumn = 0; cColumn < result._columns; cColumn++)
		for (int cRow = 0; cRow < result._rows; cRow++)
		{
			result.At(cRow, cColumn) = 0;
			for (int i = 0; i < _columns; i++)
				result.At(cRow, cColumn) += At(cRow, i) * other.At(i, cColumn);
		}
}

int AlgTheoryLab2::Matrix::Rows() const
{
	return _rows;
}

int AlgTheoryLab2::Matrix::Columns() const
{
	return _columns;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::CreateSubView(int rowStart, int colStart, int rowEnd, int colEnd) const
{
	// construct manually cause i'm a bydlocoder
	Matrix res;
	res._data = std::move(Array<double>(_data.Size(), _data.Data()));

	res._rows = rowEnd - rowStart;
	res._columns = colEnd - colStart;

	res._rowStart = _rowStart + rowStart;
	res._colStart = _colStart + colStart;
	res._rowEnd = res._rows + _rowEnd - rowEnd;
	res._colEnd = res._columns + _colEnd - colEnd;

#if _DEBUG
	if (res._rowEnd <= 0
		||
		res._colEnd <= 0)
		std::cerr << "\nError: impossible values of res._rowEnd and res._colEnd\n";
#endif

	res._rowsOrig = _rowsOrig;
	res._colsOrig = _colsOrig;

	return res;
}

int AlgTheoryLab2::Matrix::GetIndex(int row, int column) const
{
	// check relative coords
#ifdef _DEBUG
	CheckBounds(row, column, _rows, _columns);
#endif

	row += _rowStart;
	column += _colStart;

	// check absolute coords
#ifdef _DEBUG
	CheckBounds(row, column, _rowsOrig, _colsOrig);
#endif

	return _rowsOrig * column + row;
}

bool AlgTheoryLab2::Matrix::IsSameDimensions(const AlgTheoryLab2::Matrix & other) const
{
	return _columns == other._columns && _rows == other._rows;
}

void AlgTheoryLab2::Matrix::CheckBounds(int row, int column, int rows, int columns)
{
	if (row < 0 || row >= rows
		||
		column < 0 || column >= columns)
		std::cerr << "\nAt bounds exceeded: row " << row << " col " << column << " but size is rows " << rows << " cols " << columns << "\n";
}

void AlgTheoryLab2::Matrix::CheckCompatibilityForMult(const AlgTheoryLab2::Matrix & other) const
{
	if (!IsSameDimensions(other)
		||
		!isPowerOf2(_columns) || !isPowerOf2(_rows))
		std::cerr << "\n!!!!!!!ERRRRROR!!!!!!!!!!!!!!!!\n";
}

void AlgTheoryLab2::Matrix::CheckCompatibilityForAdd(const AlgTheoryLab2::Matrix & other) const
{
	if (!IsSameDimensions(other))
		std::cerr << "\nNot compatible dimensions\n";
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::operator+(const Matrix& a)const
{
	AlgTheoryLab2::Matrix result(_rows, _columns);
	Add(a, result);
	return result;
}

AlgTheoryLab2::Matrix AlgTheoryLab2::Matrix::operator-(const AlgTheoryLab2::Matrix& a)const
{
	AlgTheoryLab2::Matrix result(_rows, _columns);
	Subtract(a, result);
	return result;
}
