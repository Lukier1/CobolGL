#include "stdafx.h"

#include "Include/CobMath.h"

Vec3 Vector3f(float x, float y, float z) {
	float v[] = { x, y, z };
	return Vec3(v);
}

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
		up.v[0],	up.v[1],	up.v[2],	-eye.v[1],
		-forw.v[0], -forw.v[1], -forw.v[2], -eye.v[2],
		0,			0,			0,			1
	};
	float _v2[16] =
	{
		1, 0, 0, -eye.v[0],
		0, 1, 0, -eye.v[1],
		0, 0, 1, -eye.v[2],
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
		near / ( range * aspect ), 0, 0, 0,
		0, near / range, 0, 0,
		0, 0, -2 / (far - near), (far + near) / (far - near),
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

	det = 1.0 / det;

	for (int i = 0; i < 16; i++)
		inv[i] = inv[i] * det;

	return Matrix4x4(inv);
}



Quaterion::Quaterion(float angle_rad, float x, float y, float z) {
	q[0] = cos(angle_rad / 2.0f);
	q[1] = sin(angle_rad / 2.0f)*x;
	q[2] = sin(angle_rad / 2.0f)*y;
	q[3] = sin(angle_rad / 2.0f)*z;
}

Quaterion::Quaterion(float angle_rad, Vec3 axis) : Quaterion(angle_rad, axis.v[0], axis.v[1], axis.v[2])
{

}

Quaterion::Quaterion() {
	q[0] = 0;
	q[1] = 0;
	q[2] = 0;
	q[3] = 0;
}

Quaterion Quaterion::operator + (const Quaterion& oth) const {
	return Quaterion();
}
Quaterion Quaterion::operator - (const Quaterion& oth) const {
	return Quaterion();
}
Quaterion Quaterion::operator * (const Quaterion& oth) const {
	Quaterion t;
	t.q[0] = q[0] * oth.q[0] - q[1] * oth.q[1] - q[2] * oth.q[2] - q[3] * oth.q[3];
	t.q[1] = q[1] * oth.q[0] - q[0] * oth.q[1] - q[3] * oth.q[2] - q[2] * oth.q[3];
	t.q[2] = q[2] * oth.q[0] - q[3] * oth.q[1] - q[0] * oth.q[2] - q[1] * oth.q[3];
	t.q[3] = q[3] * oth.q[0] - q[2] * oth.q[1] - q[1] * oth.q[2] - q[0] * oth.q[3];

	return t;
}
float Quaterion::dot(const Quaterion& oth) const {
	return q[0] * oth.q[0] + q[1] * oth.q[1] + q[2] * oth.q[2] + q[3] * oth.q[3];
}
Quaterion Quaterion::slerp(const Quaterion& q, const Quaterion& r, float t) {
	float dp = q.dot(r);

	if (fabs(dp) >= 1.0f) {
		return q;
	}

	float sin_omega = sqrt(1.0f - dp*dp);
	Quaterion res;
	if (fabs(sin_omega) < 0.001f)
	{
		for (int i = 0; i < 4; ++i) {
			res.q[i] = (1.0f - t) * q.q[i] + t*r.q[i];
		}
		return res;
	}

	float omega = acos(dp);
	float a = sin(1.0f - t * omega) / sin_omega;
	float b = sin(t * omega) / sin_omega;
	for (int i = 0; i < 4; ++i)
	{
		res.q[i] = q.q[i] * a + r.q[i] * b;
	}

	return res;
}

Quaterion Quaterion::normalise(const Quaterion &v) {
	float m = v.length();
	Quaterion res;
	res.q[0] = v.q[0] / m;
	res.q[1] = v.q[1] / m;
	res.q[2] = v.q[2] / m;
	res.q[3] = v.q[3] / m;
	return res;
}

Quaterion Quaterion::normalise() const {
	return Quaterion::normalise(*this);
}

Matrix4x4  Quaterion::getMatrix() const {
	/*float _v[16] =
	{
		1 - 2 * y * y - 2 * z*z, 2 * x*y - 2 * w*z, 2 * x*z + 2 * w*y, 0,
		2 * x*y + 2 * w*z, 1 - 2 * x*x - 2 * z*z, 2*y*z - 2 * w*x, 0,
		2 * x*z - 2 * w*y, 2*y*z + 2 * w * x, 1- 2*x*x - 2*y*y, 0,
		0,0,0,1
	}; */
	float _v[16] =
	{
		1 - 2 * q[2] * q[2] - 2 * q[3]*q[3], 2 * q[1]*q[2] - 2 * q[0]*q[3], 2 * q[1]*q[3] + 2 * q[0]*q[2], 0,
		2 * q[1]*q[2] + 2 * q[0]*q[3], 1 - 2 * q[1]*q[1] - 2 * q[3]*q[3], 2 * q[2]*q[3] - 2 * q[0]*q[1], 0,
		2 * q[1]*q[3] - 2 * q[0]*q[2], 2 * q[2]*q[3] + 2 * q[0] * q[1], 1 - 2 * q[1]*q[1] - 2 * q[2]*q[2], 0,
		0, 0, 0, 1
	};
	return Matrix4x4(_v);
}

float Quaterion::length() const {
	return sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
}

