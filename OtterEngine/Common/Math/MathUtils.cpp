#include "MathUtils.h"

Vector3 operator*(float scale, const Vector3& v1) {
	Vector3 result(
		v1.x * scale,
		v1.y * scale,
		v1.z * scale
	);
	return result;
}

Vector3 operator*(const Vector3& v1, const Matrix3x3& m1) {
	return Vector3{
		v1.x * m1.value[0] + v1.y * m1.value[3] + v1.z * m1.value[6],
		v1.x * m1.value[1] + v1.y * m1.value[4] + v1.z * m1.value[7],
		v1.x * m1.value[2] + v1.y * m1.value[5] + v1.z * m1.value[8]
	};
}

float DotProduct(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 CrossProduct(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
}

Vector3 RotateAroundAxis(const Vector3& v1, const Vector3& axis, float angle) {
	Vector3 result = v1;
	float coeff = 1 - cosf(angle);
	float matrix[9] = {
		coeff * axis.x * axis.x + cosf(angle), coeff * axis.x * axis.y - axis.z * sinf(angle), coeff * axis.x * axis.z + axis.y * sinf(angle),
		coeff * axis.y * axis.x + axis.z * sinf(angle), coeff * axis.y * axis.y + cosf(angle),	coeff * axis.y * axis.z - axis.x * sinf(angle),
		coeff * axis.z * axis.x - axis.y * sinf(angle), coeff * axis.z * axis.y + axis.x * sinf(angle), coeff * axis.z * axis.z + cosf(angle),
	};
	Matrix3x3 rotationMatrix(matrix);

	return result * rotationMatrix;
}
