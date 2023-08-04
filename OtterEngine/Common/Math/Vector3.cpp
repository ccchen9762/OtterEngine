#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
}

void Vector3::normalize() {
	float len = length();
	x /= len, y /= len, z /= len;
}

Vector3Int::Vector3Int(int x, int y, int z) : x(x), y(y), z(z) {
}
