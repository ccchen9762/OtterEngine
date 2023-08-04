#pragma once

#include <cmath>

struct Vector3 {
public:
	Vector3() = default;
	Vector3(float x, float y, float z = 0.0f);
	~Vector3() = default;

	float length() { return sqrtf(x * x + y * y + z * z); }
	void normalize();

	float x;
	float y;
	float z;
};

struct Vector3Int {
public:
	Vector3Int() = default;
	Vector3Int(int x, int y, int z = 0);
	~Vector3Int() = default;

	int x;
	int y;
	int z;
};