#pragma once

#include "Vector.hpp"
#include <cassert>
#include <cstring>

template <typename T>
class Matrix22
{
public:
	union
	{
		struct { Vector2D<T> r1; Vector2D<T> r2; };
		Vector2D<T> v[2];
		T elem[2][2];
	};

	Matrix22<T> operator+(const Matrix22<T> m) const
	{
		Matrix22<T> ret;
		ret.r1 = r1 + m.r1;
		ret.r2 = r2 + m.r2;
		return ret;
	}

	Matrix22<T>& operator+=(const Matrix22<T> m)
	{
		r1 += m.r1;
		r2 += m.r2;
		return *this;
	}

	Matrix22<T>& operator-(const Matrix22<T> m)
	{
		Matrix22<T> ret;
		ret.r1 = r1 - m.r2;
		ret.r2 = r2 - m.r2;
		return ret;
	}

	Matrix22<T>& operator-=(const Matrix22<T> m)
	{
		r1 -= m.r1;
		r2 -= m.r2;
		return *this;
	}

	Matrix22<T> operator*(const T& k)
	{
		Matrix22<T> ret;
		ret.r1 = r1 * k;
		ret.r2 = r2 * k;
		return ret;
	}

	Matrix22<T>& operator*=(const T& k)
	{
		r1 *= k;
		r2 *= k;
		return *this;
	}

	Matrix22<T> operator/(const T& k)
	{
		Matrix22<T> ret;
		ret.r1 = r1 / k;
		ret.r2 = r2 / k;
		return ret;
	}

	Matrix22<T>& operator/=(const T& k)
	{
		r1 /= k;
		r2 /= k;
		return *this;
	}

	Matrix22<T> operator*(Matrix22<T> m)
	{
		m.Transparent();
		Matrix22<T> ret;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transparent();
		return ret;
	}

	Vector2D<T> operator*(const Vector2D<T>& v) const
	{
		Vector2D<T> ret = { r1 * v, r2 * v };
		return ret;
	}

	T* operator[](int index)
	{
		assert(index >= 0 && index < 2);
		return elem[index];
	}

	Matrix22<T>& Transparent()
	{
		auto temp = elem[0][1];
		elem[0][1] = elem[1][0];
		elem[1][0] = temp;
		return *this;
	}


	static const Matrix22 Identity;
};

template <typename T>
class Matrix33
{
public:
	union
	{
		struct { Vector3D<T> r1; Vector3D<T> r2; Vector3D<T> r3; };
		Vector3D<T> v[3];
		T elem[3][3];
	};

	Matrix33<T> operator+(const Matrix33<T> m) const
	{
		Matrix33<T> ret;
		ret.r1 = r1 + m.r1;
		ret.r2 = r2 + m.r2;
		ret.r3 = r3 + m.r3;
		return ret;
	}

	Matrix33<T>& operator+=(const Matrix33<T> m)
	{
		r1 += m.r1;
		r2 += m.r2;
		r3 += m.r3;
		return *this;
	}

	Matrix33<T>& operator-(const Matrix33<T> m)
	{
		Matrix33<T> ret;
		ret.r1 = r1 - m.r2;
		ret.r2 = r2 - m.r2;
		ret.r3 = r3 - m.r3;
		return ret;
	}

	Matrix33<T>& operator-=(const Matrix33<T> m)
	{
		r1 -= m.r1;
		r2 -= m.r2;
		r3 -= m.r3;
		return *this;
	}

	Matrix33<T> operator*(const T& k)
	{
		Matrix33<T> ret;
		ret.r1 = r1 * k;
		ret.r2 = r2 * k;
		ret.r3 = r3 * k;
		return ret;
	}

	Matrix33<T>& operator*=(const T& k)
	{
		r1 *= k;
		r2 *= k;
		r3 *= k;
		return *this;
	}

	Matrix33<T> operator/(const T& k)
	{
		Matrix33<T> ret;
		ret.r1 = r1 / k;
		ret.r2 = r2 / k;
		ret.r3 = r3 / k;
		return ret;
	}

	Matrix33<T>& operator/=(const T& k)
	{
		r1 /= k;
		r2 /= k;
		r3 /= k;
		return *this;
	}

	Matrix33<T> operator*(Matrix33<T> m)
	{
		m.Transparent();
		Matrix33<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transparent();
		return ret;
	}

	Vector3D<T> operator*(const Vector3D<T>& v) const
	{
		Vector3D<T> ret = { r1 * v, r2 * v, r3 * v };
	}

	T* operator[](int index)
	{
		assert(index >= 0 && index < 3);
		return elem[index];
	}

	Matrix33<T>& Transparent()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = i + 1; j < 3; j++)
			{
				auto temp = elem[i][j];
				elem[i][j] = elem[j][i];
				elem[j][i] = temp;
			}
		}
	}
	static const Matrix33<T> Identity;
};

template <typename T>
class Matrix44
{
public:
	union
	{
		struct { Vector4D<T> r1; Vector4D<T> r2; Vector4D<T> r3; Vector4D<T> r4; };
		Vector4D<T> v[4];
		T elem[4][4];
	};

	Matrix44()
		:r1(), r2(), r3(), r4()
	{}

	Matrix44(const Vector4D<T>& _r1, const Vector4D<T>& _r2, const Vector4D<T>& _r3, const Vector4D<T>& _r4)
		:r1(_r1), r2(_r2), r3(_r3), r4(_r4)
	{}

	Matrix44<T> operator+(const Matrix44<T> m) const
	{
		Matrix44<T> ret;
		ret.r1 = r1 + m.r1;
		ret.r2 = r2 + m.r2;
		ret.r3 = r3 + m.r3;
		ret.r4 = r4 + m.r4;
		return ret;
	}

	Matrix44<T>& operator+=(const Matrix44<T> m)
	{
		r1 += m.r1;
		r2 += m.r2;
		r3 += m.r3;
		r4 += m.r4;
		return *this;
	}

	Matrix44<T>& operator-(const Matrix44<T> m)
	{
		Matrix44<T> ret;
		ret.r1 = r1 - m.r2;
		ret.r2 = r2 - m.r2;
		ret.r3 = r3 - m.r3;
		ret.r4 = r4 - m.r4;
		return ret;
	}

	Matrix44<T>& operator-=(const Matrix44<T> m)
	{
		r1 -= m.r1;
		r2 -= m.r2;
		r3 -= m.r3;
		r4 -= m.r4;
		return *this;
	}

	Matrix44<T> operator*(const T& k)
	{
		Matrix44<T> ret;
		ret.r1 = r1 * k;
		ret.r2 = r2 * k;
		ret.r3 = r3 * k;
		ret.r4 = r4 * k;
		return ret;
	}

	Matrix44<T>& operator*=(const T& k)
	{
		r1 *= k;
		r2 *= k;
		r3 *= k;
		r4 *= k;
		return *this;
	}

	Matrix44<T> operator/(const T& k)
	{
		Matrix44<T> ret;
		ret.r1 = r1 / k;
		ret.r2 = r2 / k;
		ret.r3 = r3 / k;
		ret.r4 = r4 / k;
		return ret;
	}

	Matrix44<T>& operator/=(const T& k)
	{
		r1 /= k;
		r2 /= k;
		r3 /= k;
		r4 /= k;
		return *this;
	}

	Matrix44<T> operator*(Matrix44<T> m)
	{
		m.Transparent();
		Matrix44<T> ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transparent();
		return ret;
	}

	Vector4D<T> operator*(const Vector4D<T>& v) const
	{
		Vector4D<T> ret = { r1 * v, r2 * v, r3 * v ,r4 * v };
	}


	T* operator[](int index)
	{
		assert(index >= 0 && index < 4);
		return elem[index];
	}

	Matrix44<T>& Transparent()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = i + 1; j < 4; j++)
			{
				auto temp = elem[i][j];
				elem[i][j] = elem[j][i];
				elem[j][i] = temp;
			}
		}
	}

	static const Matrix44<T> Identity;
};

template <typename T>
Matrix44<T> GetTransform(T x, T y, T z)
{
	Matrix44<T> transform = { Vector4D<T>(1, 0, 0, x), Vector4D<T>(0, 1, 0, y), Vector4D<T>(0, 0, 1, z), Vector4D<T>(0, 0, 0, 1) };
	return transform;
}

template <typename T>
Matrix44<T> GetRotate(T x, T y, T z)
{
	return Matrix44<T>();
}

template <typename T>
Matrix44<T> GetScale(T x, T y, T z)
{
	Matrix44<T> scale = { Vector4D<T>(x, 0, 0, 0), Vector4D<T>(0, y, 0, 0), Vector4D<T>(0, 0, z, 0), Vector4D<T>(0, 0, 0, 1) };
	return scale;
}