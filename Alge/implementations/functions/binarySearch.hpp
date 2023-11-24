#pragma once
#include "../../definitions/vector/vectorDouble.h"
#include "../../definitions/vector/vectorFloat.h"
#include "../../definitions/vector/vectorUint8_t.h"
#include "../../definitions/vector/vectorUint64_t.h"
#include "../../definitions/vector/vectorInt.h"
#include "../../definitions/matrix/matrixDouble.h"
#include "../../definitions/matrix/matrixFloat.h"
#include "../../definitions/matrix/matrixUint8_t.h"

namespace alge
{
	template<typename T>
	inline size_t upper_bound(vector<T>& vector1, size_t left, size_t right, T num)
	{
		T* data1 = vector1._data;
		while (left <= right)
		{
			size_t mid = left + (right - left) / 2;

			if (data1[mid] <= num)
			{
				left = mid + 1;
			}
			else
			{
				right = mid - 1;
			}
		}
		return left;
	}

	template<typename T>
	inline size_t lower_bound(vector<T>& vector1, size_t left, size_t right, T num)
	{
		T* data1 = vector1._data;

		while (left <= right)
		{
			size_t mid = left + (right - left) / 2;

			if (data1[mid] < num)
			{
				left = mid + 1;
			}
			else
			{
				right = mid - 1;
			}
		}
		return left;
	}

}