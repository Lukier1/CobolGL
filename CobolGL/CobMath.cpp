#include "stdafx.h"

#include "CobMath.h"
Matrix4x4::Matrix4x4() {
	v[0] = 1.0f;
	v[5] = 1.0f;
	v[10] = 1.0f;
	v[15] = 1.0f;
}
Matrix4x4::Matrix4x4(float _v[16])
{
	for (int i = 0; i < 16; ++i)
	{
		v[i] = _v[i];
	}
}
Matrix4x4 Matrix4x4::translate(float x, float y, float z)
{
	float _v[16] = { 1, 0, 0, x,
		0, 0, 1, y,
		0, 0, 1, z,
		0, 0, 0, 1 };
	return Matrix4x4(_v);
}
Matrix4x4 Matrix4x4::scale(float x, float y, float z)
{
	float _v[16] = { x, 0, 0, 0,
		0, 0, y, 0,
		0, 0, z, 0,
		0, 0, 0, 1 };

	return Matrix4x4(_v);
}

Matrix4x4 Matrix4x4::rotation(float x, float y, float z)
{
	float _v[16] = {
		cos(y)*cos(z), cos(z)*sin(x)*sin(y) - cos(x)*sin(z), cos(x)*cos(z)*sin(y) + sin(x)*sin(z), 0,
		cos(y)*sin(z), cos(x)*cos(z) + sin(x)*sin(y)*sin(z), -cos(z)*sin(x) + cos(x)*sin(y)*sin(z), 0,
		-sin(x), cos(y)*sin(x), cos(x)*cos(y), 0,
		0, 0, 0, 1 };
	return Matrix4x4(_v);
}
const float * Matrix4x4::getMatrixData() const {
	return v;
}


Matrix4x4 Matrix4x4::operator+(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	for (int i = 0; i < 16; ++i)
	{
		res.v[i] = v[i] + oth.v[i];
	}
	return res;
}
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	for (int i = 0; i < 16; ++i)
	{
		res.v[i] = v[i] - oth.v[i];
	}
	return res;
}
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& oth) const
{
	Matrix4x4 res;
	res.v[0] = v[0] * oth.v[0] + v[1] * oth.v[4] + v[2] * oth.v[8] + v[3] * oth.v[12];
	res.v[1] = v[0] * oth.v[1] + v[1] * oth.v[5] + v[2] * oth.v[9] + v[3] * oth.v[13];
	res.v[2] = v[0] * oth.v[2] + v[1] * oth.v[6] + v[2] * oth.v[10] + v[3] * oth.v[14];
	res.v[3] = v[0] * oth.v[3] + v[1] * oth.v[7] + v[2] * oth.v[11] + v[3] * oth.v[15];

	res.v[4] = v[4] * oth.v[0] + v[5] * oth.v[4] + v[6] * oth.v[8] + v[7] * oth.v[12];
	res.v[5] = v[4] * oth.v[1] + v[5] * oth.v[5] + v[6] * oth.v[9] + v[7] * oth.v[13];
	res.v[6] = v[4] * oth.v[2] + v[5] * oth.v[6] + v[6] * oth.v[10] + v[7] * oth.v[14];
	res.v[7] = v[4] * oth.v[3] + v[5] * oth.v[7] + v[6] * oth.v[11] + v[7] * oth.v[15];

	res.v[8] = v[8] * oth.v[0] + v[9] * oth.v[4] + v[10] * oth.v[8] + v[11] * oth.v[12];
	res.v[9] = v[8] * oth.v[1] + v[9] * oth.v[5] + v[10] * oth.v[9] + v[11] * oth.v[13];
	res.v[10] = v[8] * oth.v[2] + v[9] * oth.v[6] + v[10] * oth.v[10] + v[11] * oth.v[14];
	res.v[11] = v[8] * oth.v[3] + v[9] * oth.v[7] + v[10] * oth.v[11] + v[11] * oth.v[15];

	res.v[12] = v[12] * oth.v[0] + v[13] * oth.v[4] + v[14] * oth.v[8] + v[15] * oth.v[12];
	res.v[13] = v[12] * oth.v[1] + v[13] * oth.v[5] + v[14] * oth.v[9] + v[15] * oth.v[13];
	res.v[14] = v[12] * oth.v[2] + v[13] * oth.v[6] + v[14] * oth.v[10] + v[15] * oth.v[14];
	res.v[15] = v[12] * oth.v[3] + v[13] * oth.v[7] + v[14] * oth.v[11] + v[15] * oth.v[15];

	return res;
}

Vec4 Matrix4x4::operator*(const Vec4& oth) const
{
	return Vec4();
}