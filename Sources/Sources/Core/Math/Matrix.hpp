#pragma once

#include "Vector.hpp"

template <typename T>
class Matrix22
{
public:
	union
	{
		struct { Vector2D<T> r1; Vector2D<T> r2 };
		T elem[2][2];
	};

};

template <typename T>
class Matrix33
{
public:
	union
	{
		struct { Vector3D<T> r1; Vector3D<T> r2; Vector3D<T> r3 };
		T elem[3][3];
	};


};

template <typename T>
class Matrix44
{
public:
	union
	{
		struct { Vector4D<T> r1; Vector4D<T> r2; Vector4D<T> r3; Vector4D<T> r4 };
		T elem[4][4];
	};

	Matrix44()
		:r1(), r2(), r3(), r4()
	{}

	matrix44(const Vector4D<T>& _r1, const Vector4D<T>& _r2, const Vector4D<T>& _r3, const Vector4D<T>& _r4)
		:r1(_r1), r2(_r2), r3(_r3), r4(_r4)
	{}

};