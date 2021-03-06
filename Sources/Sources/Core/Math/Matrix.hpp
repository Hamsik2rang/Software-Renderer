#pragma once

#include "./Vector.hpp"
#include "../Renderer/Camera.h"
#include "../Drawable/RenderObject.h"

#include <cassert>
#include <cstring>

class Camera;
class Model;

template <typename T>
class Matrix22;
template <typename T>
class Matrix33;
template <typename T>
class Matrix44;

using Mat2f = Matrix22<float>;
using Mat2i = Matrix22<int>;
using Mat3f = Matrix33<float>;
using Mat3i = Matrix33<int>;
using Mat4f = Matrix44<float>;
using Mat4i = Matrix44<int>;

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
	static const Matrix22 Identity;

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
		m.Transpose();
		Matrix22<T> ret;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transpose();
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

	Matrix22<T>& Transpose()
	{
		auto temp = elem[0][1];
		elem[0][1] = elem[1][0];
		elem[1][0] = temp;
		return *this;
	}
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
	static const Matrix33 Identity;

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
		m.Transpose();
		Matrix33<T> ret;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transpose();
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

	Matrix33<T>& Transpose()
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

	static const Matrix44 Identity;

	Matrix44()
		:r1(), r2(), r3(), r4()
	{}

	Matrix44(Vector4D<T> _r1, Vector4D<T> _r2, Vector4D<T> _r3, Vector4D<T> _r4)
		: r1(_r1), r2(_r2), r3(_r3), r4(_r4)
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
		m.Transpose();
		Matrix44<T> ret;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				ret[i][j] = v[i] * m.v[j];
			}
		}
		m.Transpose();
		return ret;
	}

	Vector4D<T> operator*(const Vector4D<T>& v) const
	{
		Vector4D<T> ret = { r1 * v, r2 * v, r3 * v ,r4 * v };
		return ret;
	}


	T* operator[](int index)
	{
		assert(index >= 0 && index < 4);
		return elem[index];
	}

	Matrix44<T>& Transpose()
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
		return *this;
	}
};

template <typename T>
const Matrix22<T> Matrix22<T>::Identity = {
	{1,0},
	{0,1} };

template <typename T>
const Matrix33<T> Matrix33<T>::Identity = {
	{1,0,0},
	{0,1,0},
	{0,0,1} };

template <typename T>
const Matrix44<T> Matrix44<T>::Identity = {
	{1,0,0,0},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,0,1} };


template <typename T>
Matrix44<T> GetTransform(T x, T y, T z)
{
	Matrix44<T> transform = { Vector4D<T>(1, 0, 0, x), Vector4D<T>(0, 1, 0, y), Vector4D<T>(0, 0, 1, z), Vector4D<T>(0, 0, 0, 1) };
	return transform;
}

template <typename T>
Matrix44<T> GetRotate(T x, T y, T z)
{
	// TODO: Implement This.
	Matrix44<T> rotate = Matrix44<T>::Identity;
	rotate = Matrix44<T>(
		{ 1.0f ,0.0f, 0.0f, 0.0f },
		{ 0.0f, std::cos(x), -std::sin(x), 0.0f },
		{ 0.0f, std::sin(x), std::cos(x), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }) * rotate;

	rotate = Matrix44<T>(
		{ std::cos(y),0.0f, std::sin(y), 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ -std::sin(y), 0.0f, std::cos(y), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }) * rotate;

	rotate = Matrix44<T>(
		{ std::cos(z), -std::sin(z), 0.0f, 0.0f },
		{ std::sin(z), std::cos(z), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }) * rotate;

	return rotate;
}

template <typename T>
Matrix44<T> GetScale(T x, T y, T z)
{
	Matrix44<T> scale = { Vector4D<T>(x, 0, 0, 0), Vector4D<T>(0, y, 0, 0), Vector4D<T>(0, 0, z, 0), Vector4D<T>(0, 0, 0, 1) };
	return scale;
}

inline Mat4f Model(const RenderObject* pObejct)
{
	Mat4f model = GetTransform(pObejct->m_position.x, pObejct->m_position.y, pObejct->m_position.z)
		* GetRotate(pObejct->m_rotation.x, pObejct->m_rotation.y, pObejct->m_rotation.z)
		* GetScale(pObejct->m_scale.x, pObejct->m_scale.y, pObejct->m_scale.z);
	return model;
}

inline Mat4f Projection(float fovY, float aspect, float nDist, float fDist)
{
	Mat4f proj(
		{ 1.0f / (std::tanf(DegreeToRadian(fovY / 2.0f)) * aspect), 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f / std::tanf(DegreeToRadian(fovY / 2.0f)), 0.0f, 0.0f },
		{ 0.0f, 0.0f, fDist / (fDist - nDist), nDist * fDist / (fDist - nDist) },
		{ 0.0f, 0.0f, -1.0f, 0.0f }
	);
	return proj;
}

inline Mat4f LookAt(const Vec3f eye, const Vec3f at, const Vec3f worldUp)
{
	Vec3f forward = (eye - at).normalize();
	Vec3f right = (worldUp ^ forward).normalize();
	Vec3f up = forward ^ right;

	Mat4f lookAt = Mat4f::Identity;
	lookAt = Mat4f(
		{ right.x, right.y, right.z, 0.0f },
		{ up.x,up.y,up.z, 0.0f },
		{ forward.x, forward.y, forward.z, 0.0f },
		{ 0.0f, 0.0f,0.0f, 1.0f })
		* GetTransform(-eye.x, -eye.y, -eye.z);

	return lookAt;
}
