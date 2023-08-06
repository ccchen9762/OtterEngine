#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
}

Vector3 Vector3::operator+(const Vector3& v2) const {
	Vector3 result = *this;
	result.x += v2.x, result.y += v2.y, result.z += v2.z;
	return result;
}

Vector3& Vector3::operator+=(const Vector3& v2) {
	x += v2.x, y += v2.y, z += v2.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v2) {
	x -= v2.x, y -= v2.y, z -= v2.z;
	return *this;
}

Vector3 Vector3::operator*(float scale) const {
	Vector3 result(
		x * scale, 
		y * scale,
		z * scale	
	);
	return result;
}

void Vector3::normalize() {
	float len = length();
	x /= len, y /= len, z /= len;
}

Vector3Int::Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {
}