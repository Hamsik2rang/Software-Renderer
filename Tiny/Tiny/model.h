#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

#include "vector.hpp"

class Model
{
private:
	std::vector<Vec3f>				_vertices;
	std::vector<Vec2f>				_textures;
	std::vector<Vec3f>				_normals;
	std::vector<std::vector<int>>	_faces;
public:
	Model(const char* filename);
	~Model();
	Vec3f vertex(int index) const;
	Vec2f texture(int index) const;
	Vec3f normal(int index) const;
	std::vector<int> face(int index) const;

	size_t vsize() const;
	size_t tsize() const;
	size_t nsize() const;
	size_t fsize() const;
};