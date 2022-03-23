#pragma once

#include "./Vector.hpp"
#include <cmath>


template <class T>
inline constexpr T Lerp(T v0, T v1, float alpha)
{
	return v0 + (v1 - v0) * alpha;
}

template <class T>
inline constexpr Vector3D<T> Barycentric(Vector3D<T> v0, Vector3D<T> v1, Vector3D<T> v2, Vector3D<T> p)
{
	Vector3D<T> eqx(v2.x - v0.x, v1.x - v0.x, v0.x - p.x);
	Vector3D<T> eqy(v2.y - v0.y, v1.y - v0.y, v0.y - p.y);
	Vector3D<T> result = eqx ^ eqy;

	if (std::abs(result.z) < 1.0f)
	{
		return Vector3D<T>(-1.0f, -1.0f, -1.0f);
	}
	return Vector3D<T>(1 - (result.x + result.y) / result.z, result.y / result.z, result.x / result.z);
}

template <class T>
inline constexpr Vector3D<T> Bezier(Vector3D<T> v0, Vector3D<T> v1, Vector3D<T> v2, float alpha)
{
	Vector3D<T> t0 = Lerp(v0, v1, alpha);
	Vector3D<T> t1 = Lerp(v1, v2, alpha);

	return Lerp(t0, t1, alpha);
}

template <class T>
inline constexpr Vector3D<T> Bezier(Vector3D<T> v0, Vector3D<T> v1, Vector3D<T> v2, Vector3D<T> v3, float alpha)
{
	Vector3D<T> t00 = Lerp(v0, v1, alpha);
	Vector3D<T> t01 = Lerp(v1, v2, alpha);
	Vector3D<T> t02 = Lerp(v2, v3, alpha);

	Vector3D<T> t10 = Lerp(t00, t01, alpha);
	Vector3D<T> t11 = Lerp(t01, t02, alpha);

	return Lerp(t10, t11, alpha);
}