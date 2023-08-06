#include "Matrix3x3.h"

#include <algorithm>

Matrix3x3::Matrix3x3(float matrix[]) {
	std::copy(matrix, matrix + 9, value);
}