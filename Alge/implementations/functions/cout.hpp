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
	std::ostream& operator<<(std::ostream& os, const vector<double>& vector)
	{
		for (size_t i = 0; i < vector._size; i++)
		{
			std::cout << vector._data[i] << std::endl;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const vector<uint64_t>& vector)
	{
		for (size_t i = 0; i < vector._size; i++)
		{
			std::cout << vector._data[i] << std::endl;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const vector<uint8_t>& vector)
	{
		for (size_t i = 0; i < vector._size; i++)
		{
			std::cout << (vector._data[i] ? 1 : 0) << std::endl;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const vector<float>& vector)
	{
		for (size_t i = 0; i < vector._size; i++)
		{
			std::cout << vector._data[i] << std::endl;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const vector<int>& vector)
	{
		for (size_t i = 0; i < vector._size; i++)
		{
			std::cout << vector._data[i] << std::endl;
		}
		return os;
	}

	template<bool otherTransposed, bool otherContiguous>
	std::ostream& operator<<(std::ostream& os, const matrix<double, otherTransposed, otherContiguous>& matrix)
	{
		size_t rows = matrix._rows;
		size_t cols = matrix._cols;

		double* data1 = matrix._data;

		if constexpr (otherTransposed)
		{
			size_t actualRows = matrix.actualRows;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << data1[j * actualRows + i] << " ";
				}
				std::cout << std::endl;
			}
		}
		else
		{
			size_t actualCols = matrix.actualCols;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << data1[i * actualCols + j] << " ";
				}
				std::cout << std::endl;
			}
		}
		return os;
	}

	template<bool otherTransposed, bool otherContiguous>
	std::ostream& operator<<(std::ostream& os, const matrix<float, otherTransposed, otherContiguous>& matrix)
	{
		size_t rows = matrix._rows;
		size_t cols = matrix._cols;

		float* data1 = matrix._data;

		if constexpr (otherTransposed)
		{
			size_t actualRows = matrix.actualRows;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << data1[j * actualRows + i] << " ";
				}
				std::cout << std::endl;
			}
		}
		else
		{
			size_t actualCols = matrix.actualCols;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << data1[i * actualCols + j] << " ";
				}
				std::cout << std::endl;
			}
		}
		return os;
	}

	template<bool otherTransposed, bool otherContiguous>
	std::ostream& operator<<(std::ostream& os, const matrix<uint8_t, otherTransposed, otherContiguous>& matrix)
	{
		size_t rows = matrix._rows;
		size_t cols = matrix._cols;

		uint8_t* data1 = matrix._data;

		if constexpr (otherTransposed)
		{
			size_t actualRows = matrix.actualRows;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << (data1[j * actualRows + i] ? 1 : 0) << " ";
				}
				std::cout << std::endl;
			}
		}
		else
		{
			size_t actualCols = matrix.actualCols;

			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < cols; j++)
				{
					std::cout << (data1[i * actualCols + j] ? 1 : 0) << " ";
				}
				std::cout << std::endl;
			}
		}
		return os;
	}

}