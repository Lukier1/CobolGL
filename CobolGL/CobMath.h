#pragma once

#include "stdafx.h"

template <unsigned int N>
struct Vec {
	float v[N];

	Vec(float x, float y, float z, float w) {
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
		for (int i = 4; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	
	Vec() 
	{
		for (int i = 0; i < N; ++i)
		{
			v[i] = 0;
		}
	}

	Vec(float _v[N])
	{
		for (int i = 0; i < N; ++i)
		{
			v[i] = _v[i];
		}
	}
	
	Vec operator +(const Vec<N> &oth) const {
		Vec res;
		for (int i = 0; i < N; ++i)
		{
			res.v[i] = v[i] + oth.v[i];
		}
		return res;
	}
	Vec operator -(const Vec &oth) const {
		Vec res;
		for (int i = 0; i < N; ++i)
		{
			res.v[i] = v[i] - oth.v[i];
		}
		return res;
	}
	Vec operator *(const Vec &oth) const {
		Vec res(1,0,0,0);
		for (int i = 0; i < N; ++i)
		{
			res.v[i] = v[i] + oth.v[i];
		}
		return res;
	}

	Vec normalizeVec3() const
	{
		float d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
		d = sqrt(d);
		Vec vec;
		for (int i = 0; i < 3; ++i)
		{
			vec.v[i] = v[i] / d;
		}
		for (int i = 4; i < N; ++i)
		{
			vec.v[i] = v[i];
		}
		return vec;
	}

	Vec normalize() const
	{
		Vec vec;
		float d = 0;
		for (int i = 0; i < N; ++i)
		{
			d += v[i]*v[i];
		}
		d = sqrt(d);
		for (int i = 0; i < N; ++i)
		{
			vec.v[i] = v[i]/d;
		}
		retirm vec;
	}

	float dot(const Vec<N> &oth) const {
		float ret = 0.0f;
		for (int i = 0; i < N; ++i)
		{
			ret += v[i] * oth.v[i];
		}
		return ret;
	}

	Vec<3> cross(const Vec<3> &oth) const {
		return Vector3f(
			v[1] * oth.v[2] - v[2] * oth.v[1], 
			v[0] * oth.v[2] - v[2] * oth.v[0],
			v[0] * oth.v[1] - v[1] * oth.v[0]
			);
	}
	
};

typedef Vec<3> Vec3;
typedef Vec<4> Vec4;

Vec3 Vector3f(float x, float y, float z);

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

};