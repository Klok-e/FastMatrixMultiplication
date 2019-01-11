#pragma once
#include "HelpfulFunctions.hpp"
#include "Array.hpp"
#include "IReadWriteArray2D.hpp"

namespace AlgTheoryLab2
{
	template<class Numeric>
	class Matrix final :
		public AlgTheoryLab2::IReadWriteArray2D<Numeric>
	{
	public:
		static const int _hybridFallbackThreshold = 64;

		class MatrixView final :
			public IReadOnlyArray2D<Numeric>
		{
		public:
			// construct from matrix
			MatrixView(Matrix<Numeric> const* matrix, int rowStart, int colStart, int rowEnd, int colEnd) :
				_matrix(matrix),
				_rowStart(rowStart),
				_colStart(colStart),
				_rowEnd(rowEnd),
				_colEnd(colEnd),
				_rows(rowEnd - rowStart),
				_columns(colEnd - colStart)
			{
#if _DEBUG
				if (_rowEnd <= 0
					||
					_colEnd <= 0)
					std::cerr << "\nError: impossible values of res._rowEnd and res._colEnd\n";
#endif
			}

			// construct from another view
			MatrixView(MatrixView const* view, int rowStart, int colStart, int rowEnd, int colEnd) :
				_matrix(view->_matrix),
				_rows(rowEnd - rowStart),
				_columns(colEnd - colStart),
				_rowStart(view->_rowStart + rowStart),
				_colStart(view->_colStart + colStart),
				_rowEnd(_rows + _rowStart),
				_colEnd(_columns + _colStart)
			{
#if _DEBUG
				if (_rowEnd <= 0
					||
					_colEnd <= 0)
					std::cerr << "\nError: impossible values of res._rowEnd and res._colEnd\n";
#endif
			}

			MatrixView CreateSubView(int rowStart, int colStart, int rowEnd, int colEnd) const
			{
				return MatrixView(this, rowStart, colStart, rowEnd, colEnd);
			}

			Numeric const & At(int row, int column) const override
			{
				row += _rowStart;
				column += _colStart;

				return _matrix->At(row, column);
			}
			int Rows() const override
			{
				return _rows;
			}
			int Columns() const override
			{
				return _columns;
			}

		private:
			Matrix<Numeric> const* _matrix;
			int _rows, _columns;
			int _rowStart, _colStart, _rowEnd, _colEnd;
		};

		// usual constructor
		Matrix(int rows, int columns, bool fillWithZeros = false);

		// construct with this array
		Matrix(int rows, int columns, Numeric* data, bool fillWithZeros = false);

		// default constructor
		Matrix();

		// copy constructor
		Matrix(const Matrix<Numeric>& other);

		// move constructor
		Matrix(Matrix<Numeric>&& other);

		// assignment copy operator
		Matrix<Numeric>& operator=(const Matrix<Numeric>& other);

		// assignment move operator
		Matrix<Numeric>& operator=(Matrix<Numeric>&& other);

		Matrix<Numeric> MultiplyStrassenVinogradNoAlloc(const Matrix<Numeric>& other) const;
		Matrix<Numeric> MultiplyNaive(const Matrix<Numeric> & other) const;

		Numeric& At(int row, int column) override;
		Numeric const& At(int row, int column) const override;

		int Rows() const override;
		int Columns() const override;

		// if i move this out compiler gives weird errors i don't understand
		MatrixView CreateSubView(int rowStart, int colStart, int rowEnd, int colEnd) const
		{
			return MatrixView(this, rowStart, colStart, rowEnd, colEnd);
		}

	private:
		Array<Numeric> _data;
		int _rows, _columns;

		int GetIndex(int row, int column) const;

		template<class _2DimArray1, class _2DimArray2>
		static void CheckCompatibilityForMult(const _2DimArray1& left, const _2DimArray2& right);

		template<class _2DimArray1, class _2DimArray2>
		static void CheckCompatibilityForAdd(const _2DimArray1& left, const _2DimArray2& right);

		template<class _2DimArray1, class _2DimArray2>
		static bool IsSameDimensions(const _2DimArray1& left, const _2DimArray2& right);

		template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
		static void MultiplyStrassenVinogradNoAllocP(const _2DimArray1& left, const _2DimArray2& right, _2DimArray3& result, Numeric* aux);

		template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
		static void MultiplyNaiveP(const _2DimArray1 & left, const _2DimArray2 & right, _2DimArray3 & result);

		template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
		static void Add(const _2DimArray1& a, const _2DimArray2& b, _2DimArray3& writeTo);

		template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
		static void Subtract(const _2DimArray1& a, const _2DimArray2& b, _2DimArray3& writeTo);

		template<int auxCount, int stopAtDimensions>
		static int CalculateSizeOfAllAuxiliaryMatrices(const int n);
	};

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
	inline void Matrix<Numeric>::Add(const _2DimArray1 & a, const _2DimArray2 & b, _2DimArray3 & writeTo)
	{
#if _DEBUG
		CheckCompatibilityForAdd(a, b);
#endif
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray1>::value);
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray2>::value);
		static_assert(std::is_base_of<IWriteOnlyArray2D<Numeric>, _2DimArray3>::value);

		int cols = a.Columns();
		int rows = a.Rows();
		for (int col = 0; col < cols; col++)
			for (int row = 0; row < rows; row++)
				writeTo.At(row, col) = a.At(row, col) + b.At(row, col);
	}

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
	inline void Matrix<Numeric>::Subtract(const _2DimArray1 & a, const _2DimArray2 & b, _2DimArray3 & writeTo)
	{
#if _DEBUG
		CheckCompatibilityForAdd(a, b);
#endif
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray1>::value);
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray2>::value);
		static_assert(std::is_base_of<IWriteOnlyArray2D<Numeric>, _2DimArray3>::value);

		int cols = a.Columns();
		int rows = a.Rows();
		for (int col = 0; col < cols; col++)
			for (int row = 0; row < rows; row++)
				writeTo.At(row, col) = a.At(row, col) - b.At(row, col);
	}

	template<class Numeric>
	template<int auxCount, int stopAtDimensions>
	inline int Matrix<Numeric>::CalculateSizeOfAllAuxiliaryMatrices(const int n)
	{
		static_assert(isPowerOf2(stopAtDimensions));

		if (n <= stopAtDimensions)
			return 0;
		return (n * n / 4) * (auxCount)+CalculateSizeOfAllAuxiliaryMatrices<auxCount, stopAtDimensions>(n / 2);
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric>::Matrix(int rows, int columns, bool fillWithZeros) :
		_data(rows * columns),
		_rows(rows),
		_columns(columns)
	{
		if (fillWithZeros)
		{
			int size = _data.Size();
			for (int i = 0; i < size; i++)
				_data[i] = 0;
		}
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric>::Matrix(int rows, int columns, Numeric * data, bool fillWithZeros) :
		_data(rows * columns, data),
		_rows(rows),
		_columns(columns)
	{
		if (fillWithZeros)
		{
			int size = _data.Size();
			for (int i = 0; i < size; i++)
				_data[i] = 0;
		}
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric>::Matrix() :
		_data(),
		_rows(0),
		_columns(0)
	{
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric>::Matrix(const Matrix & other) :
		_data(),
		_rows(0),
		_columns(0)
	{
		*this = other;
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric>::Matrix(Matrix && other) :
		_data(),
		_rows(0),
		_columns(0)
	{
		*this = std::move(other);
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric> & AlgTheoryLab2::Matrix<Numeric>::operator=(const AlgTheoryLab2::Matrix<Numeric> & other)
	{
		if (this != &other)
		{
			_rows = other._rows;
			_columns = other._columns;

			// copy some data
			_data = other._data;
		}
		return *this;
	}

	template<class Numeric>
	inline AlgTheoryLab2::Matrix<Numeric> & AlgTheoryLab2::Matrix<Numeric>::operator=(AlgTheoryLab2::Matrix<Numeric> && other)
	{
		if (this != &other)
		{
			// steal some data
			_data = std::move(other._data);

			_rows = other._rows;
			_columns = other._columns;

			other._rows = 0;
			other._columns = 0;
		}
		return *this;
	}

	template<class Numeric>
	inline Numeric & AlgTheoryLab2::Matrix<Numeric>::At(int row, int column)
	{
		return _data[GetIndex(row, column)];
	}

	template<class Numeric>
	inline Numeric const& AlgTheoryLab2::Matrix<Numeric>::At(int row, int column) const
	{
		return _data[GetIndex(row, column)];
	}

	template<class Numeric>
	inline Matrix<Numeric> Matrix<Numeric>::MultiplyStrassenVinogradNoAlloc(const AlgTheoryLab2::Matrix<Numeric> & other) const
	{
		int size = CalculateSizeOfAllAuxiliaryMatrices<21, _hybridFallbackThreshold>(_rows);

		Numeric* dataForAllAuxiliaryMatrices = ArrayBuilder::CreateArray<Numeric>(size);
		Matrix<Numeric> m(_rows, _columns);
		MultiplyStrassenVinogradNoAllocP(*this, other, m, dataForAllAuxiliaryMatrices);

		ArrayBuilder::DeleteArray(dataForAllAuxiliaryMatrices, size);

		return m;
	}

	template<class Numeric>
	inline Matrix<Numeric> Matrix<Numeric>::MultiplyNaive(const Matrix<Numeric> & other) const
	{
		AlgTheoryLab2::Matrix<Numeric> result(_rows, _columns);
		MultiplyNaiveP(*this, other, result);
		return result;
	}

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2>
	inline void Matrix<Numeric>::CheckCompatibilityForMult(const _2DimArray1 & left, const _2DimArray2 & right)
	{
		if (!IsSameDimensions(left, right)
			||
			!isPowerOf2(left.Columns()) || !isPowerOf2(right.Rows()))
			std::cerr << "\n!!!!!!!ERRRRROR!!!!!!!!!!!!!!!!\n";
	}

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2>
	inline void Matrix<Numeric>::CheckCompatibilityForAdd(const _2DimArray1 & left, const _2DimArray2 & right)
	{
		if (!IsSameDimensions(left, right))
			std::cerr << "\nNot compatible dimensions\n";
	}

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2>
	inline bool Matrix<Numeric>::IsSameDimensions(const _2DimArray1 & left, const _2DimArray2 & right)
	{
		return left.Columns() == right.Columns() && left.Rows() == right.Rows();
	}

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
	inline void Matrix<Numeric>::MultiplyStrassenVinogradNoAllocP(const _2DimArray1& left, const _2DimArray2& right, _2DimArray3& result, Numeric* aux)
	{
#ifdef _DEBUG
		CheckCompatibilityForMult(left, right);
#endif
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray1>::value);
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray2>::value);
		static_assert(std::is_base_of<IWriteOnlyArray2D<Numeric>, _2DimArray3>::value);

		if (left.Columns() <= _hybridFallbackThreshold)
		{
			MultiplyNaiveP(left, right, result);
			return;
		}

		int size = left.Columns() / 2;

		// split this matrix into 4 matrices
		auto a11 = left.CreateSubView(0, 0, size, size);
		auto a12 = left.CreateSubView(0, size, size, size + size);
		auto a21 = left.CreateSubView(size, 0, size + size, size);
		auto a22 = left.CreateSubView(size, size, size + size, size + size);

		// split other matrix into 4 matrices
		auto b11 = right.CreateSubView(0, 0, size, size);
		auto b12 = right.CreateSubView(0, size, size, size + size);
		auto b21 = right.CreateSubView(size, 0, size + size, size);
		auto b22 = right.CreateSubView(size, size, size + size, size + size);

		AlgTheoryLab2::Matrix<Numeric> s1(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s2(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s3(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s4(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s5(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s6(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s7(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> s8(size, size, aux);
		aux += size * size;

		Add(a21, a22, s1);			 //a21 + a22;
		Subtract(s1, a11, s2);		 //s1 - a11;
		Subtract(a11, a21, s3);		 //a11 - a21;
		Subtract(a12, s2, s4);		 //a12 - s2;
		Subtract(b12, b11, s5);		 //b12 - b11;
		Subtract(b22, s5, s6);		 //b22 - s5;
		Subtract(b22, b12, s7);		 //b22 - b12;
		Subtract(s6, b21, s8);		 //s6 - b21;

		AlgTheoryLab2::Matrix<Numeric> p1(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p2(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p3(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p4(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p5(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p6(size, size, aux);
		aux += size * size;

		AlgTheoryLab2::Matrix<Numeric> p7(size, size, aux);
		aux += size * size;

		// all these recursive calls can use the same memory block because they aren't simultaneuous
		// also memory they were using is no longer needed so we can rewrite it with something actually useful (t1,t2,c11-c22)
		MultiplyStrassenVinogradNoAllocP(s2, s6, p1, aux);			//s2 * s6;
		MultiplyStrassenVinogradNoAllocP(a11, b11, p2, aux);		//a11 * b11;
		MultiplyStrassenVinogradNoAllocP(a12, b21, p3, aux);		//a12 * b21;
		MultiplyStrassenVinogradNoAllocP(s3, s7, p4, aux);			//s3 * s7;
		MultiplyStrassenVinogradNoAllocP(s1, s5, p5, aux);			//s1 * s5;
		MultiplyStrassenVinogradNoAllocP(s4, b22, p6, aux);			//s4 * b22;
		MultiplyStrassenVinogradNoAllocP(a22, s8, p7, aux);			//a22 * s8;

		AlgTheoryLab2::Matrix<Numeric> t1(size, size, aux); aux += size * size;
		AlgTheoryLab2::Matrix<Numeric> t2(size, size, aux); aux += size * size;

		Add(p1, p2, t1);	  //p1 + p2;
		Add(t1, p4, t2);	  //t1 + p4;

		AlgTheoryLab2::Matrix<Numeric> c11(size, size, aux); aux += size * size;
		AlgTheoryLab2::Matrix<Numeric> c12(size, size, aux); aux += size * size;
		AlgTheoryLab2::Matrix<Numeric> c21(size, size, aux); aux += size * size;
		AlgTheoryLab2::Matrix<Numeric> c22(size, size, aux); aux += size * size;

		Add(p2, p3, c11);		//p2 + p3;
		Add(t1, p5, c12);		//t1 + p5 + p6;
		Add(c12, p6, c12);		//t2 - p7;
		Subtract(t2, p7, c21);	//t2 + p5;
		Add(t2, p5, c22);		//

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

	template<class Numeric>
	template<class _2DimArray1, class _2DimArray2, class _2DimArray3>
	inline void Matrix<Numeric>::MultiplyNaiveP(const _2DimArray1 & left, const _2DimArray2 & right, _2DimArray3 & result)
	{
#if _DEBUG
		CheckCompatibilityForMult(left, right);
#endif
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray1>::value);
		static_assert(std::is_base_of<IReadOnlyArray2D<Numeric>, _2DimArray2>::value);
		static_assert(std::is_base_of<IWriteOnlyArray2D<Numeric>, _2DimArray3>::value);

		for (int cColumn = 0; cColumn < result.Columns(); cColumn++)
			for (int cRow = 0; cRow < result.Rows(); cRow++)
			{
				double sum = 0;
				for (int i = 0; i < left.Columns(); i++)
					sum += left.At(cRow, i) * right.At(i, cColumn);
				result.At(cRow, cColumn) = sum;
			}
	}

	template<class Numeric>
	inline int AlgTheoryLab2::Matrix<Numeric>::Rows() const
	{
		return _rows;
	}

	template<class Numeric>
	inline int AlgTheoryLab2::Matrix<Numeric>::Columns() const
	{
		return _columns;
	}

	template<class Numeric>
	inline int AlgTheoryLab2::Matrix<Numeric>::GetIndex(int row, int column) const
	{
		return _rows * column + row;
	}
}