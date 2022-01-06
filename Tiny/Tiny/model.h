#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

#include "vector.hpp"

struct FaceElementIndex
{
	int index0;
	int index1;
	int index2;

	int& operator[](int index)
	{
		switch (index)
		{
		case 0:
			return index0;
			break;
		case 1:
			return index1;
			break;
		case 2:
			return index2;
			break;
		default:
			assert(false);
		}
	}
};

struct Face
{
	FaceElementIndex vIndex;
	FaceElementIndex tIndex;
	FaceElementIndex nIndex;
};

class Model
{
private:
	std::vector<Vec3f>	_vertices;
	std::vector<Vec2f>	_textures;
	std::vector<Vec3f>	_normals;
	std::vector<Face>	_faces;

public:
	Model(const char* filename);
	~Model();
	Vec3f vertex(int index) const;
	Vec2f texture(int index) const;
	Vec3f normal(int index) const;
	Face face(int index) const;

	int vsize() const;
	int tsize() const;
	int nsize() const;
	int fsize() const;

};