#pragma once

#include <cmath>

namespace hs {
	template <class T>
	static constexpr T Lerp(T v0, T v1, T alpha)
	{
		return v0 + alpha * (v1 - v0);
	}

	template <class T>
	static constexpr Vector3D<T> Barycentric(Vector3D<T> v0, Vector3D<T> v1, Vector3D<T> v2, Vector3D<T> p)
	{
		Vector3D<T> eqx(v2.x - v0.x, v1.x - v0.x, v0.x - p.x);
		Vector3D<T> eqy(v2.y - v0.y, v1.y - v0.y, v0.y - p.y);
		Vector3D<T> result = eqx ^ eqy;

		if (std::abs(result.z) < 1.0f)
		{
			return Vector3D<T>(-1.0f, -1.0f, -1.0f);
		}
		return Vector3D<T>(result.x / result.z, result.y / result.z, 1 - (result.x + result.y) / result.z);
	}
};