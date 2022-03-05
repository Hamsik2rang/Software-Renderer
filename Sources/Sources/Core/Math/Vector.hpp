#pragma once

#include <cmath>

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

	float length() const
	{
		return (float)std::sqrt(x * x + y * y + z * z);
	}

	Vector3D<T>& normalize()
	{
		x /= length();
		y /= length();
		z /= length();

		return *this;
	}

	Vector2D<T> AffineToCartesian() const
	{
		assert(z != 0);
		Vector3D<T> ret = {x/z, y/z};
		return ret;
	}

	Vector4D<T> CartesianToAffine() const
	{
		Vector4D<T> ret(x, y, z, T(1));
		return ret;
	}
};

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
		: x{_x}, y{_y}, z{_z}, w{_w}
	{}


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
		Vector3D<T> ret = { x / w, y / w, z / w };
		return ret;
	}
};

union Color
{
	struct { unsigned char r; unsigned char g; unsigned char b; unsigned char a; };
	struct { unsigned char x; unsigned char y; unsigned char z; unsigned char w; };
	unsigned char elem[4];

	Color operator+(const Color& c) const
	{
		return Color{ unsigned char(r + c.r) , unsigned char(g + c.g), unsigned char(b + c.b), unsigned char(w + c.w) };
	}
	Color operator*(float f) const
	{
		return Color{ unsigned char(r * f) , unsigned char(g * f), unsigned char(b * f), unsigned char(w * f) };
	}
};
