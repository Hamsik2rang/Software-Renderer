#pragma once

#include "Vector.hpp"
#include <cassert>

template <typename T>
class Matrix2D
{
public:
	union
	{
		struct { Vector2D<T> r1; Vector2D<T> r2 };
		T elem[2][2];
	};

	Matrix2D<T> operator+(const Matrix2D<T> m) const
	{
		Matrix2D<T> ret;
		ret.r1 = r1 + m.r1;
		ret.r2 = r2 + m.r2;
		return ret;
	}
	
	Matrix2D<T>& operator+=(const Matrix2D<T> m)
	{
		r1 += m.r1;
		r2 += m.r2;
		return *this;
	}

	Matrix2D<T>& operator-(const Matrix2D<T> m)
	{
		Matrix2D<T> ret;
		ret.r1 = r1 - m.r2;
		ret.r2 = r2 - m.r2;
		return ret;
	}

	Matrix2D<T>& operator-=(const Matrix2D<T> m)
	{
		r1 -= m.r1;
		r2 -= m.r2;
		return *this;
	}

	Matrix2D<T> operator*(const T& k)
	{
		Matrix2D<T> ret;
		ret.r1 = r1 * k;
		ret.r2 = r2 * k;
		return ret;
	}

	Matrix2D<T>& operator*=(const T& k)
	{
		r1 *= k;
		r2 *= k;
		return *this;
	}

	Matrix2D<T> operator/(const T& k)
	{
		Matrix2D<T> ret;
		ret.r1 = r1 / k;
		ret.r2 = r2 / k;
		return ret;
	}

	Matrix2D<T>& operator/=(const T& k)
	{
		r1 /= k;
		r2 /= k;
		return *this;
	}


	T* operator[](int index)
	{
		assert(index >= 0 && index <= 1);
		return elem[index];
	}
};

template <typename T>
class Matrix3D
{
public:
	union
	{
		struct { Vector3D<T> r1; Vector3D<T> r2; Vector3D<T> r3 };
		T elem[3][3];
	};

	T* operator[](int index)
	{
		assert(index >= 0 && index <= 2);
		return elem[index];
	}
};

template <typename T>
class Matrix4D
{
public:
	union
	{
		struct { Vector4D<T> r1; Vector4D<T> r2; Vector4D<T> r3; Vector4D<T> r4 };
		T elem[4][4];
	};

	Matrix4D()
		:r1(), r2(), r3(), r4()
	{}

	matrix44(const Vector4D<T>& _r1, const Vector4D<T>& _r2, const Vector4D<T>& _r3, const Vector4D<T>& _r4)
		:r1(_r1), r2(_r2), r3(_r3), r4(_r4)
	{}

	T* operator[](int index)
	{
		assert(index >= 0 && index <= 3);
		return elem[index];
	}
};