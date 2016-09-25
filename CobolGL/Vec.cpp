#include "stdafx.h"

#include "Include\Math\Vec.h"

Vec3 Vector3f(float x, float y, float z) {
	float v[] = { x, y, z };
	return Vec3(v);
}

