#pragma once

#include "./Interface/IDrawable.h"
#include <vector>

enum class PRIMITIVE_TYPE
{
	POINT=1,
	LINE,
	TRIANGLE,
	SQUARE,
};

class Primitive : public IDrawable
{
private:
	std::vector<float> vertices;

public:
	Primitive();
	~Primitive();

	PRIMITIVE_TYPE GetType() const;
	void Draw() const;
};

