#pragma once

class Matrix3x3
{
public:
	Matrix3x3(float matrix[]);
	~Matrix3x3() = default;

	float value[9];
};