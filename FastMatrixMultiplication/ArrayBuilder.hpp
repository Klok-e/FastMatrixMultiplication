#pragma once

namespace AlgTheoryLab2
{
	namespace ArrayBuilder
	{
		inline unsigned long long _createdArrays;
		inline unsigned long long _deletedArrays;

		inline unsigned long long _createdArrSumSize;
		inline unsigned long long _deletedArrSumSize;

		template<typename T>
		T* CreateArray(int size);

		template<typename T>
		void DeleteArray(T* arr, int size);

		template<typename T>
		T* CreateArray(int size)
		{
			_createdArrays += 1;
			_createdArrSumSize += size;
			T* newArr = new T[size];
#if _DEBUG
			std::memset(newArr, -1, sizeof(T)*size);
#endif
			return newArr;
		}

		template<typename T>
		void DeleteArray(T* arr, int size)
		{
			if (size > 0)
				_deletedArrays += 1;
			_deletedArrSumSize += size;
			delete[] arr;
		}
	}
}