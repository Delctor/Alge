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
	inline void vectorize(vector<T>& vector1, T(*func)(T))
	{
		size_t size = vector1._size;

		T* data = vector1._data;

		for (size_t i = 0; i < size; i++)
		{
			data[i] = (*func)(data[i]);
		}
	}

	template<typename T, typename T2 = std::conditional<std::is_same<T, double>::value, __m256d, std::conditional<std::is_same<T, float>::value, __m256, __m256i>>::type>
	inline void vectorize(vector<T>& vector1, T2(*func)(T2))
	{
		size_t size = vector1._size;

		size_t finalPos = vector1.finalPos;

		T* data = vector1._data;

		size_t step;

		if constexpr (sizeof(T) == 1)
		{
			step = 32;
		}
		else if constexpr (sizeof(T) == 4)
		{
			step = 8;
		}
		else if constexpr (sizeof(T) == 8)
		{
			step = 4;
		}

		for (size_t i = 0; i < finalPos; i += step)
		{
			if constexpr (std::is_same<T, double>::value)
			{
				_mm256_store_pd(&data[i], (*func)(_mm256_load_pd(&data[i])));
			}
			else if constexpr (std::is_same<T, float>::value)
			{
				_mm256_store_ps(&data[i], (*func)(_mm256_load_ps(&data[i])));
			}
			else if constexpr (std::is_same<T, uint64_t>::value)
			{
				_mm256_storeu_epi64(&data[i], (*func)(_mm256_loadu_epi64(&data[i])));
			}
			else if constexpr (std::is_same<T, int>::value)
			{
				_mm256_storeu_epi32(&data[i], (*func)(_mm256_loadu_epi32(&data[i])));
			}
			else if constexpr (std::is_same<T, uint8_t>::value)
			{
				_mm256_storeu_epi8(&data[i], (*func)(_mm256_loadu_epi8(&data[i])));
			}
		}
		for (size_t i = finalPos; i < size; i++)
		{
			if constexpr (std::is_same<T, double>::value)
			{
				_mm_store_sd(&data[i], _mm256_castpd256_pd128((*func)(_mm256_castpd128_pd256(_mm_load_sd(&data[i])))));
			}
			else if constexpr (std::is_same<T, float>::value)
			{
				_mm_store_ss(&data[i], _mm256_castps256_ps128((*func)(_mm256_castps128_ps256(_mm_load_ss(&data[i])))));
			}
			else if constexpr (std::is_same<T, uint64_t>::value)
			{
				_mm_store_sd(reinterpret_cast<double*>(&data[i]), _mm256_castpd256_pd128(_mm256_castsi256_pd((*func)(_mm256_castpd_si256(_mm256_castpd128_pd256(_mm_load_sd(reinterpret_cast<double*>(&data[i]))))))));
			}
			else if constexpr (std::is_same<T, int>::value)
			{
				_mm_store_ss(reinterpret_cast<float*>(&data[i]), _mm256_castps256_ps128(_mm256_castsi256_ps((*func)(_mm256_castps_si256(_mm256_castps128_ps256(_mm_load_ss(reinterpret_cast<float*>(&data[i]))))))));
			}
			else if constexpr (std::is_same<T, uint8_t>::value)
			{
				__m128i reg = _mm_undefined_pd();
				data[i] = _mm_extract_epi8((*func)(_mm_insert_epi8(reg, data[i], 0)), 0);
			}
		}
	}

}