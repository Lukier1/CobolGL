#pragma once

#include "stdafx.h"

struct Vec4 {
	float v[4];

	Vec4(float x, float y, float z, float w) {
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	}
	Vec4(float x, float y, float z) : Vec4(x, y, z, 1)
	{


	}
	Vec4() : Vec4(0, 0, 0, 1)
	{

	}
	Vec4 operator +(const Vec4 &oth) {
		Vec4 res;
		for (int i = 4; i != 0; --i)
		{
			res.v[i-1] = v[i-1] + oth.v[i-1];
		}
		return res;
	}
	Vec4 operator -(const Vec4 &oth) {
		Vec4 res;
		for (int i = 4; i != 0; --i)
		{
			res.v[i - 1] = v[i - 1] - oth.v[i - 1];
		}
		return res;
	}
	Vec4 operator *(const Vec4 &oth) {
		Vec4 res;
		for (int i = 4; i != 0; --i)
		{
			res.v[i - 1] = v[i - 1] + oth.v[i - 1];
		}
	}
};

class Matrix4x4 {
private:
	float v[16];
public:
	Matrix4x4();
	Matrix4x4(float _v[16]);
	
	static Matrix4x4 translate(float x, float y, float z);
	static Matrix4x4 scale(float x, float y, float z);

	static Matrix4x4 rotation(float x, float y, float z);
	const float * getMatrixData() const;
	Matrix4x4 operator +(const Matrix4x4& oth) const;
	Matrix4x4 operator -(const Matrix4x4& oth) const;
	Matrix4x4 operator *(const Matrix4x4& oth) const;

	Vec4 operator *(const Vec4& oth) const;
	//TODO: Get from smaller matrix

};