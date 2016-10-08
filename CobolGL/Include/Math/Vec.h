#pragma once

#include "../../stdafx.h"

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
		Vec res(1, 0, 0, 0);
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
			d += v[i] * v[i];
		}
		d = sqrt(d);
		for (int i = 0; i < N; ++i)
		{
			vec.v[i] = v[i] / d;
		}
		return vec;
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
	const float * getData() const {
		return v;
	}
};

typedef Vec<3> Vec3;
typedef Vec<4> Vec4;

Vec3 Vector3f(float x, float y, float z);
