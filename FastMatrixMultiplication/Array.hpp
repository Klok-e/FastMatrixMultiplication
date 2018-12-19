#pragma once
#include <algorithm>
#include "ArrayBuilder.hpp"

namespace AlgTheoryLab2
{
	template<typename T>
	class Array
	{
	public:
		Array(int size, T* data);
		Array(int size);
		Array();
		~Array();

		Array(const Array<T>& other);
		Array(Array<T>&& other);
		Array<T>& operator=(const Array<T>& other);
		Array<T>& operator=(Array<T>&& other);

		T& operator[](int i);
		T const& operator[](int i) const;

		int Size() const;
		T const* Data() const;

	private:
		T* _data;
		int _size;
		bool _ownsData;

		void CheckIfOutOfBorders(int i) const;
	};

	template<typename T>
	inline Array<T>::Array(int size, T * data) :
		_data(data),
		_size(size),
		_ownsData(false)
	{
	}

	template<typename T>
	inline Array<T>::Array(int size) :
		_data(ArrayBuilder::CreateArray<T>(size)),
		_size(size),
		_ownsData(true)
	{
	}

	template<typename T>
	inline Array<T>::Array() :
		_data(nullptr),
		_size(0),
		_ownsData(false)
	{
	}

	template<typename T>
	inline Array<T>::~Array()
	{
		if (_ownsData)
			ArrayBuilder::DeleteArray(_data, _size);
	}

	template<typename T>
	inline Array<T>::Array(const Array<T>& other) :
		_data(nullptr),
		_size(0),
		_ownsData(false)
	{
		*this = other;
	}

	template<typename T>
	inline Array<T>::Array(Array<T>&& other) :
		_data(nullptr),
		_size(0),
		_ownsData(false)
	{
		*this = std::move(other);
	}

	template<typename T>
	inline Array<T> & Array<T>::operator=(const Array & other)
	{
		if (this != &other)
		{
			if (_ownsData)
				ArrayBuilder::DeleteArray(_data, _size);

			_data = nullptr;
			_size = 0;
			_ownsData = false;

			_data = ArrayBuilder::CreateArray<T>(other._size);
			_size = other._size;
			_ownsData = true;

			std::copy(other._data, other._data + other._size, _data);
		}
		return *this;
	}

	template<typename T>
	inline Array<T>& Array<T>::operator=(Array<T>&& other)
	{
		if (this != &other)
		{
			if (_ownsData)
				ArrayBuilder::DeleteArray(_data, _size);

			_data = other._data;
			_size = other._size;
			_ownsData = other._ownsData;

			other._data = nullptr;
			other._size = 0;
			other._ownsData = false;
		}
		return *this;
	}

	template<typename T>
	inline T & Array<T>::operator[](int i)
	{
#if _DEBUG
		CheckIfOutOfBorders(i);
#endif
		return _data[i];
	}

	template<typename T>
	inline T const & Array<T>::operator[](int i) const
	{
#if _DEBUG
		CheckIfOutOfBorders(i);
#endif
		return _data[i];
	}

	template<typename T>
	inline int Array<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline T const* Array<T>::Data() const
	{
		return _data;
	}

	template<typename T>
	inline void Array<T>::CheckIfOutOfBorders(int i) const
	{
		if (i < 0
			||
			i >= _size)
			std::cerr << "\nOut of borders!\n";
	}
}