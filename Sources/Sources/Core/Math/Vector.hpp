#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

template <typename T>
class Vector2D;
template <typename T>
class Vector3D;
template <typename T>
class Vector4D;

using Vec2f = Vector2D<float>;
using Vec2i = Vector2D<int>;
using Vec3f = Vector3D<float>;
using Vec3i = Vector3D<int>;
using Vec4f = Vector4D<float>;
using Vec4i = Vector4D<int>;

template <typename T>
class Vector2D
{
public:
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

	Vector2D<T> operator-() const { return Vector2D<T>(-x, -y); }
	Vector2D<T> operator+(const Vector2D<T>& v) const { return Vector2D(x + v.x, y + v.y); }
	Vector2D<T> operator-(const Vector2D<T>& v) const { return Vector2D(x - v.x, y - v.y); }
	T operator*(const Vector2D<T>& v) const { return x * v.x + y * v.y; }

	Vector2D<T> operator/(const T& scalar) const { return Vector2D(x / scalar, y / scalar); }
	Vector2D<T> operator*(T scalar) const { return Vector2D(x * scalar, y * scalar); }
	T& operator[](int index) { return elem[index]; }

	float	normalize() const { return (float)std::sqrt(x * x + y * y); }
	Vector2D<T> CartesianToPolar() const
	{
		T radius = std::sqrtf(x * x + y * y);
		T radian = std::atan2(y, x);
		return Vector2D<T>(radius, radian);
	}

	Vector2D<T> PolarToCartesian() const
	{
		T radius = x;
		T radian = y;
		int x = radius * std::cos(radian);
		int y = radius * std::sin(radian);
		return Vector2D<T>(x, y);
	}

	Vector3D<T> CartesianToAffine() const
	{
		Vector3D<T> ret(x, y, T(1));
		return ret;
	}
};

template <typename T>
class Vector3D
{
public:
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

	Vector3D<T> operator-() const
	{
		return Vector3D<T>(-x, -y, -z);
	}

	Vector3D<T> operator+(const Vector3D<T>& v) const
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D<T>& operator+=(const Vector3D<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vector3D<T> operator-(const Vector3D<T>& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D<T> operator*(const T& scalar) const
	{
		return Vector3D(x * scalar, y * scalar, z * scalar);
	}
	Vector3D<T> operator/(const T& scalar) const
	{
		return Vector3D(x / scalar, y / scalar, z / scalar);
	}

	Vector3D<T> operator^(const Vector3D<T>& v) const
	{
		return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	T operator*(const Vector3D<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	T& operator[](int index)
	{
		return elem[index];
	}

	template <typename T>
	friend std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v);

	float length() const
	{
		return (float)std::sqrt(x * x + y * y + z * z);
	}

	Vector3D<T>& normalize()
	{
		float denom = length();
		x /= denom;
		y /= denom;
		z /= denom;

		return *this;
	}

	Vector2D<T> AffineToCartesian() const
	{
		assert(z != 0);
		Vector3D<T> ret = { x / z, y / z };
		return ret;
	}

	Vector4D<T> CartesianToAffine() const
	{
		Vector4D<T> ret(x, y, z, T(1));
		return ret;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

template <typename T>
class Vector4D
{
public:
	union
	{
		struct { T x; T y; T z; T w; };
		struct { T r; T g; T b; T a; };
		T elem[4];
	};

	Vector4D()
		: x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
	{}

	Vector4D(T _x, T _y, T _z, T _w = T(1))
		: x{ _x }, y{ _y }, z{ _z }, w{ _w }
	{}

	Vector4D<T> operator-() const
	{
		return Vector4D<T>(-x, -y, -z, -w);
	}

	Vector4D<T> operator+(const Vector4D<T>& v) const
	{
		return Vector4D<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vector4D<T> operator-(const Vector4D<T>& v) const
	{
		return Vector4D<T>(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vector4D<T>& operator+=(const Vector4D<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vector4D<T>& operator-=(const Vector4D<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vector4D<T> operator*(const T& scalar) const
	{
		return Vector4D<T>(x * scalar, y * scalar, z * scalar, w * scalar);
	}


	Vector4D<T>& operator*=(const T& scalar) const
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vector4D<T> operator/(const T& scalar) const
	{
		return Vector4D<T>(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	Vector4D<T> operator/=(const T& scalar) const
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	T operator*(const Vector4D<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	T& operator[](int index)
	{
		return elem[index];
	}

	float length() const
	{
		return (float)std::sqrt(x * x + y * y + z * z + w * w);
	}

	Vector4D<T>& normalize()
	{
		*this /= length();

		return *this;
	}

	Vector3D<T> AffineToCartesian()
	{
		assert(w != 0);
		Vector3D<T> ret = { x / w, y / w, z / w};
		return ret;
	}
};

// Little-Endian으로 값이 저장되게 되므로 0xAARRGGBB-> BGRA 순으로 변수가 선언되어야 함.
union Color
{
	struct { unsigned char b; unsigned char g; unsigned char r; unsigned char a; };
	struct { unsigned char z; unsigned char y; unsigned char x; unsigned char w; };
	unsigned char elem[4];
	unsigned int bit;

	Color(unsigned char _r = 255, unsigned char _g = 255, unsigned char _b = 255, unsigned char _a = 255)
		:r(_r), g(_g), b(_b), a(_a)
	{}

	Color operator+(const Color& c) const
	{
		return Color{ unsigned char(r + c.r) , unsigned char(g + c.g), unsigned char(b + c.b), unsigned char(w + c.w) };
	}
	Color operator*(float f) const
	{
		return Color{ unsigned char(r * f) , unsigned char(g * f), unsigned char(b * f), unsigned char(w * f) };
	}
};