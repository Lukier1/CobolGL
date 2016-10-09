#include "stdafx.h"
#include "Include\Math\Matrix.h"

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
	float _v[16] = {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1 };
	return Matrix4x4(_v);
}
Matrix4x4 Matrix4x4::scale(float x, float y, float z)
{
	float _v[16] = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1 };

	return Matrix4x4(_v);
}


Matrix4x4 Matrix4x4::viewMatrix(const Vec<3> &forw, const Vec<3> &right, const Vec<3> &eye, const Vec<3> &up) {
	float _v[16] =
	{
		right.v[0], right.v[1], right.v[2], -eye.v[0],
		up.v[0], up.v[1], up.v[2], -eye.v[1],
		-forw.v[0], -forw.v[1], -forw.v[2], -eye.v[2],
		0, 0, 0, 1
	};
	
	return Matrix4x4(_v);
}

Matrix4x4 Matrix4x4::lookat(const Vec<3> &eyepos, const Vec<3> &point, const Vec<3> &up) {
	Vec3 f;
	f = point - eyepos;
	f = f.normalizeVec3();
	Vec3 u = up.normalizeVec3();
	Vec3 s = f.cross(u);


	float T[16] =
	{
		1, 0, 0, -eyepos.v[0],
		0, 1, 0, -eyepos.v[1],
		0, 0, 1, -eyepos.v[2],
		0, 0, 0, 1
	};
	float M[16] =
	{
		s.v[0], s.v[1], s.v[2], 0,
		u.v[0], u.v[1], u.v[2], 0,
		-f.v[0], -f.v[1], -f.v[2], 0,
		0, 0, 0, 1
	};
	return Matrix4x4(M)*Matrix4x4(T);

}
Matrix4x4 Matrix4x4::rotation(float x, float y, float z)
//x - alpha
//y - beta
//z - gamma
{
	float _v[16] = {
		cos(y)*cos(z), cos(z)*sin(x)*sin(y) - cos(x)*sin(z), cos(x)*cos(z)*sin(y) + sin(x)*sin(z), 0,
		cos(y)*sin(z), cos(x)*cos(z) + sin(x)*sin(y)*sin(z), -cos(z)*sin(x) + cos(x)*sin(y)*sin(z), 0,
		-sin(y), cos(y)*sin(x), cos(x)*cos(y), 0,
		0, 0, 0, 1 };
	return Matrix4x4(_v);
}

Matrix4x4 Matrix4x4::perspective(float near, float far, float fov, float aspect) {
	//TODO: Check for range == 0
	float range = tan(fov*0.5f)* near;
	float _v[16] = {
		near / (range * aspect), 0, 0, 0,
		0, near / range, 0, 0,
		0, 0, -2 / (far - near),-(far + near) / (far - near),
		0, 0, -1, 0
	};
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
	return Vec4(v[0] * oth.v[0] + v[1] * oth.v[1] + v[2] * oth.v[2] + v[3] * oth.v[3],
		v[4] * oth.v[0] + v[5] * oth.v[1] + v[6] * oth.v[2] + v[7] * oth.v[3],
		v[8] * oth.v[0] + v[9] * oth.v[1] + v[10] * oth.v[2] + v[11] * oth.v[3],
		v[12] * oth.v[0] + v[13] * oth.v[1] + v[14] * oth.v[2] + v[15] * oth.v[3]);
}
Matrix4x4 Matrix4x4::traspose() const
{
	float out_v[] = { v[0], v[4], v[8], v[12], v[1], v[5], v[9], v[13], v[2], v[6], v[10], v[14], v[3], v[7], v[11], v[15] };
	return Matrix4x4(out_v);
}

Matrix4x4 Matrix4x4::inverse() const
{
	float inv[16], det;

	inv[0] = v[5] * v[10] * v[15] -
		v[5] * v[11] * v[14] -
		v[9] * v[6] * v[15] +
		v[9] * v[7] * v[14] +
		v[13] * v[6] * v[11] -
		v[13] * v[7] * v[10];

	inv[4] = -v[4] * v[10] * v[15] +
		v[4] * v[11] * v[14] +
		v[8] * v[6] * v[15] -
		v[8] * v[7] * v[14] -
		v[12] * v[6] * v[11] +
		v[12] * v[7] * v[10];

	inv[8] = v[4] * v[9] * v[15] -
		v[4] * v[11] * v[13] -
		v[8] * v[5] * v[15] +
		v[8] * v[7] * v[13] +
		v[12] * v[5] * v[11] -
		v[12] * v[7] * v[9];

	inv[12] = -v[4] * v[9] * v[14] +
		v[4] * v[10] * v[13] +
		v[8] * v[5] * v[14] -
		v[8] * v[6] * v[13] -
		v[12] * v[5] * v[10] +
		v[12] * v[6] * v[9];

	inv[1] = -v[1] * v[10] * v[15] +
		v[1] * v[11] * v[14] +
		v[9] * v[2] * v[15] -
		v[9] * v[3] * v[14] -
		v[13] * v[2] * v[11] +
		v[13] * v[3] * v[10];

	inv[5] = v[0] * v[10] * v[15] -
		v[0] * v[11] * v[14] -
		v[8] * v[2] * v[15] +
		v[8] * v[3] * v[14] +
		v[12] * v[2] * v[11] -
		v[12] * v[3] * v[10];

	inv[9] = -v[0] * v[9] * v[15] +
		v[0] * v[11] * v[13] +
		v[8] * v[1] * v[15] -
		v[8] * v[3] * v[13] -
		v[12] * v[1] * v[11] +
		v[12] * v[3] * v[9];

	inv[13] = v[0] * v[9] * v[14] -
		v[0] * v[10] * v[13] -
		v[8] * v[1] * v[14] +
		v[8] * v[2] * v[13] +
		v[12] * v[1] * v[10] -
		v[12] * v[2] * v[9];

	inv[2] = v[1] * v[6] * v[15] -
		v[1] * v[7] * v[14] -
		v[5] * v[2] * v[15] +
		v[5] * v[3] * v[14] +
		v[13] * v[2] * v[7] -
		v[13] * v[3] * v[6];

	inv[6] = -v[0] * v[6] * v[15] +
		v[0] * v[7] * v[14] +
		v[4] * v[2] * v[15] -
		v[4] * v[3] * v[14] -
		v[12] * v[2] * v[7] +
		v[12] * v[3] * v[6];

	inv[10] = v[0] * v[5] * v[15] -
		v[0] * v[7] * v[13] -
		v[4] * v[1] * v[15] +
		v[4] * v[3] * v[13] +
		v[12] * v[1] * v[7] -
		v[12] * v[3] * v[5];

	inv[14] = -v[0] * v[5] * v[14] +
		v[0] * v[6] * v[13] +
		v[4] * v[1] * v[14] -
		v[4] * v[2] * v[13] -
		v[12] * v[1] * v[6] +
		v[12] * v[2] * v[5];

	inv[3] = -v[1] * v[6] * v[11] +
		v[1] * v[7] * v[10] +
		v[5] * v[2] * v[11] -
		v[5] * v[3] * v[10] -
		v[9] * v[2] * v[7] +
		v[9] * v[3] * v[6];

	inv[7] = v[0] * v[6] * v[11] -
		v[0] * v[7] * v[10] -
		v[4] * v[2] * v[11] +
		v[4] * v[3] * v[10] +
		v[8] * v[2] * v[7] -
		v[8] * v[3] * v[6];

	inv[11] = -v[0] * v[5] * v[11] +
		v[0] * v[7] * v[9] +
		v[4] * v[1] * v[11] -
		v[4] * v[3] * v[9] -
		v[8] * v[1] * v[7] +
		v[8] * v[3] * v[5];

	inv[15] = v[0] * v[5] * v[10] -
		v[0] * v[6] * v[9] -
		v[4] * v[1] * v[10] +
		v[4] * v[2] * v[9] +
		v[8] * v[1] * v[6] -
		v[8] * v[2] * v[5];

	det = v[0] * inv[0] + v[1] * inv[4] + v[2] * inv[8] + v[3] * inv[12];

	if (det == 0)
		return *this;

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return Matrix4x4(inv);
}