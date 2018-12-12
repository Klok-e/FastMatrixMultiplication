#pragma once

namespace AlgTheoryLab2
{
	template<class T>
	class IReadOnlyArray2D
	{
	public:
		virtual T const& At(int row, int column) const = 0;
		virtual int Rows() const = 0;
		virtual int Columns() const = 0;
	};

	template<class T>
	class IWriteOnlyArray2D
	{
	public:
		virtual T& At(int row, int column) = 0;
		virtual int Rows() const = 0;
		virtual int Columns() const = 0;
	};

	template<class T>
	class IReadWriteArray2D :
		public virtual IReadOnlyArray2D<T>, IWriteOnlyArray2D<T>
	{
	};
}