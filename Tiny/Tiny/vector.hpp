#pragma once
#include <cmath>


// already be in source directory. if you should use this in a main project, you can't copy it.
template <typename T>
struct Vector2D
{
	union
	{
		struct { T x; T y; };
		struct { T u; T v; };
		T elem[2];
	};

	Vector2D()
		:x(0), y(0)
	{}
	Vector2D(T _x, T _y)
		:x(_x), y(_y)
	{}

	Vector2D<T> operator+(const Vector2D<T>& v) const { return Vector2D(x + v.x, y + v.y); }
	Vector2D<T> operator-(const Vector2D<T>& v) const { return Vector2D(x - v.x, y - v.y); }
	Vector2D<T> operator*(const Vector2D<T>& v) const { return Vector2D(x * v.x, y * v.y); }

	Vector2D<T> operator*(T scalar) const { return Vector2D(x * scalar, y * scalar); }
	T&			operator[](int index) { return elem[index]; }

	float		normalize() const { return (float)std::sqrt(x * x + y * y); }
};

template <typename T>
struct Vector3D
{
	union
	{
		struct { T x; T y; T z; };
		struct { T r; T g; T b; };
		struct { T vertex; T uv; T norm; };
		T elem[3];
	};

	Vector3D()
		:x(0), y(0), z(0)
	{}
	Vector3D(T _x, T _y, T _z)
		:x(_x), y(_y), z(_z)
	{}

	Vector3D<T> operator+(const Vector3D<T>& v) const { return Vector3D(x + v.x, y + v.y, z + v.z); }
	Vector3D<T> operator-(const Vector3D<T>& v) const { return Vector3D(x - v.x, y - v.y, z - v.z); }
	Vector3D<T> operator*(T scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
	Vector3D<T> operator^(const Vector3D<T>& v) const { return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

	T			operator*(const Vector3D<T>& v) const { return x * v.x + y * v.y + z * v.z; }
	T&			operator[](int index) { return elem[index]; }
	float		normalize() const { return (float)std::sqrt(x * x + y * y + z * z); }
};

using Vec2f = Vector2D<float>;
using Vec2i = Vector2D<int>;
using Vec3f = Vector3D<float>;
using Vec3i = Vector3D<int>;