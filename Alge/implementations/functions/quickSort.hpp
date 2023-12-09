#pragma once

namespace alge
{
#define zeroI 0, 1  // 0
#define oneI 2, 3   // 1
#define twoI 4, 5   // 2
#define threeI 6, 7 // 3

    static __m256i lookUpTableLeftPermuteDouble[16]
    {
        _mm256_setr_epi32(zeroI, oneI, twoI, threeI), // 0b0000
        _mm256_setr_epi32(zeroI, oneI, twoI, threeI), // 0b0001
        _mm256_setr_epi32(oneI, zeroI, twoI, threeI), // 0b0010
        _mm256_setr_epi32(zeroI, oneI, twoI, threeI), // 0b0011
        _mm256_setr_epi32(twoI, oneI, zeroI, threeI), // 0b0100
        _mm256_setr_epi32(zeroI, twoI, oneI, threeI), // 0b0101
        _mm256_setr_epi32(oneI, twoI, zeroI, threeI), // 0b0110
        _mm256_setr_epi32(zeroI, oneI, twoI, threeI), // 0b0111
        _mm256_setr_epi32(threeI, oneI, twoI, zeroI), // 0b1000
        _mm256_setr_epi32(zeroI, threeI, twoI, oneI), // 0b1001
        _mm256_setr_epi32(oneI, threeI, twoI, zeroI), // 0b1010
        _mm256_setr_epi32(zeroI, oneI, threeI, twoI), // 0b1011
        _mm256_setr_epi32(twoI, threeI, zeroI, oneI), // 0b1100
        _mm256_setr_epi32(zeroI, threeI, twoI, oneI), // 0b1101
        _mm256_setr_epi32(threeI, oneI, twoI, zeroI), // 0b1110
        _mm256_setr_epi32(zeroI, oneI, twoI, threeI), // 0b1111
    };

    // ArgSort

	template<typename T>
	size_t partition(size_t* indices, T* arr, size_t low, size_t high)
	{
		T pivot = arr[indices[low]];
		size_t i = low - 1;
		size_t j = high + 1;

		while (true)
		{
			do
			{
				i++;
			} while (arr[indices[i]] < pivot);

			do
			{
				j--;
			} while (arr[indices[j]] > pivot);

			if (i >= j)
			{
				return j;
			}

			std::swap(indices[i], indices[j]);
		}
	}

	template<typename T>
	void quicksort(size_t* indices, T* arr, size_t low, size_t high)
	{
		if (low < high)
		{
			size_t pivotI = partition(indices, arr, low, high);
			quicksort(indices, arr, low, pivotI);
			quicksort(indices, arr, pivotI + 1, high);
		}
	}

    // Double

#define _mm256_mask_compressstore_pd(ptr, _mask, mask, _val) \
_permutationIndices = lookUpTableLeftPermuteDouble[mask]; \
_maskPermuted = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(_mm256_castpd_si256(_mask), _permutationIndices)); \
_valuePermuted = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(_mm256_castpd_si256(_val), _permutationIndices)); \
_mm256_maskstore_pd(ptr, _mm256_castpd_si256(_maskPermuted), _valuePermuted);

    inline size_t partitionNoAvx(double* arr, size_t l, size_t r)
    {
        double pivotValue = arr[r];
        size_t i = l;

        for (size_t j = l; j < r; j++)
        {
            if (arr[j] < pivotValue)
            {
                std::swap(arr[i], arr[j]);
                i++;
            }
        }
        std::swap(arr[i], arr[r]);
        return i;
    }

    size_t partitionAvx(double* arr, size_t l, size_t r)
    {
        if (r - l < 10000)
        {
            return partitionNoAvx(arr, l, r);
        }

        size_t pivotIndex = r;

        __m256d _pivot = _mm256_broadcast_sd(&arr[r]);

        __m256d _xorMask = _mm256_castsi256_pd(_mm256_set1_epi64x(UINT64_MAX));
        int xorMask = 0b1111;

        __m256d _leftValue = _mm256_load_pd(&arr[l]);

        size_t leftW = l;
        l += 4;

        size_t rightW = r;
        r -= 4;
        __m256d _rightValue = _mm256_load_pd(&arr[r]);

        __m256i _permutationIndices;
        __m256d _maskPermuted, _valuePermuted;

        while (l + 4 <= r)
        {
            const size_t freeLeft = l - leftW;
            const size_t freeRight = rightW - r;

            __m256d _val;
            if (freeLeft <= freeRight)
            {
                _val = _mm256_load_pd(&arr[l]);
                l += 4;
            }
            else
            {
                r -= 4;
                _val = _mm256_load_pd(&arr[r]);
            }

            __m256d _mask = _mm256_cmp_pd(_val, _pivot, _CMP_LT_OQ);
            int mask = _mm256_movemask_pd(_mask);

            const size_t nbLow = _mm_popcnt_u32(mask);
            const size_t nbHigh = 4 - nbLow;

            _mm256_mask_compressstore_pd(&arr[leftW], _mask, mask, _val);
            leftW += nbLow;

            rightW -= nbHigh;
            _mm256_mask_compressstore_pd(&arr[rightW], _mm256_xor_pd(_mask, _xorMask), mask ^ xorMask, _val);
        }
        {
            const size_t remaining = r - l;
            __m256d _val = _mm256_load_pd(&arr[l]);
            l = r;

            __m256d _mask = _mm256_cmp_pd(_val, _pivot, _CMP_LT_OQ);
            int mask = _mm256_movemask_pd(_mask);

            uint8_t maskLow = mask & ~(0xFF << remaining);
            uint8_t maskHigh = (~mask) & ~(0xFF << remaining);

            int expandedMask = _pdep_u64(maskLow, 0x0101010101010101);
            expandedMask = (expandedMask << 8) - expandedMask;
            __m256d _maskLow = _mm256_castsi256_pd(_mm256_cvtepi8_epi64(_mm_cvtsi64_si128(expandedMask)));

            expandedMask = _pdep_u64(maskHigh, 0x0101010101010101);
            expandedMask = (expandedMask << 8) - expandedMask;
            __m256d _maskHigh = _mm256_castsi256_pd(_mm256_cvtepi8_epi64(_mm_cvtsi64_si128(expandedMask)));

            const size_t nbLow = _mm_popcnt_u32(maskLow);
            const size_t nbHigh = _mm_popcnt_u32(maskHigh);

            _mm256_mask_compressstore_pd(&arr[leftW], _maskLow, maskLow, _val);
            leftW += nbLow;

            rightW -= nbHigh;
            _mm256_mask_compressstore_pd(&arr[rightW], _maskHigh, maskHigh, _val);
        }
        {
            __m256d _mask = _mm256_cmp_pd(_leftValue, _pivot, _CMP_LT_OQ);
            int mask = _mm256_movemask_pd(_mask);

            const size_t nbLow = _mm_popcnt_u32(mask);
            const size_t nbHigh = 4 - nbLow;

            _mm256_mask_compressstore_pd(&arr[leftW], _mask, mask, _leftValue);
            leftW += nbLow;

            rightW -= nbHigh;
            _mm256_mask_compressstore_pd(&arr[rightW], _mm256_xor_pd(_mask, _xorMask), mask ^ xorMask, _leftValue);
        }
        {
            __m256d _mask = _mm256_cmp_pd(_rightValue, _pivot, _CMP_LT_OQ);
            int mask = _mm256_movemask_pd(_mask);

            const size_t nbLow = _mm_popcnt_u32(mask);
            const size_t nbHigh = 4 - nbLow;

            _mm256_mask_compressstore_pd(&arr[leftW], _mask, mask, _rightValue);
            leftW += nbLow;

            rightW -= nbHigh;
            _mm256_mask_compressstore_pd(&arr[rightW], _mm256_xor_pd(_mask, _xorMask), mask ^ xorMask, _rightValue);
        }

        std::swap(arr[pivotIndex], arr[leftW]);

        return leftW;
    }

    void quickSortAvx(double* arr, size_t l, size_t r)
    {
        if (l < r)
        {
            size_t pivot = partitionAvx(arr, l, r);

            if (pivot > 0)
            {
                quickSortAvx(arr, l, pivot - 1);
            }
            quickSortAvx(arr, pivot + 1, r);
        }
    }

    // Float



}