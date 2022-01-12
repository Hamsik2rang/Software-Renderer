#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>

#include "vector.hpp"


class Model
{
private:
	std::vector<hs::Vec3f> _vertices;
	std::vector<std::vector<int> > _faces;
public:
	Model(const char* filename);
	~Model();
	int vsize();
	int fsize();
	hs::Vec3f vertex(int i);
	std::vector<int> face(int idx);
};
