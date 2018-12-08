#pragma once

namespace AlgTheoryLab2
{
	namespace ArrayBuilder
	{
		// extern because https://stackoverflow.com/questions/10422034/when-to-use-extern-in-c 
		// without it every file that includes it has its own variable with that name, we don't want this
		extern unsigned long long _createdArrays;
		extern unsigned long long _deletedArrays;

		extern unsigned long long _createdArrSumSize;
		extern unsigned long long _deletedArrSumSize;

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