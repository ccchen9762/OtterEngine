#pragma once

#include "Vector3.h"
#include "Matrix3x3.h"

Vector3 operator*(float scale, const Vector3& v1);
Vector3 operator*(const Vector3& v1, const Matrix3x3& m1);
float DotProduct(const Vector3& v1, const Vector3& v2);
Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);
Vector3 RotateAroundAxis(const Vector3& v1, const Vector3& axis, float angle);