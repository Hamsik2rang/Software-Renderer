#pragma once

#include "./IDrawable.h"

class Mesh : public IDrawable
{
private:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
	~Mesh();
};