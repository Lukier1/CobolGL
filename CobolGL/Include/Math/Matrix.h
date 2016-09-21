#pragma once

#include "../../stdafx.h"
#include "Vec.h"

class Matrix4x4 {
private:
	float v[16];
public:
	Matrix4x4();
	Matrix4x4(float _v[16]);

	static Matrix4x4 translate(float x, float y, float z);
	static Matrix4x4 scale(float x, float y, float z);
	static Matrix4x4 viewMatrix(const Vec<3> &forw, const Vec<3> &right, const Vec<3> &eye, const Vec<3> &up);
	static Matrix4x4 lookat(const Vec<3> &eyepos, const Vec<3> &point, const Vec<3> &up);
	static Matrix4x4 rotation(float x, float y, float z);
	static Matrix4x4 perspective(float near, float far, float fov, float aspect);
	const float * getMatrixData() const;
	Matrix4x4 operator +(const Matrix4x4& oth) const;
	Matrix4x4 operator -(const Matrix4x4& oth) const;
	Matrix4x4 operator *(const Matrix4x4& oth) const;

	Matrix4x4 traspose() const;
	Vec4 operator *(const Vec4& oth) const;
	//TODO: Get from smaller matrix

	Matrix4x4 inverse() const;
};