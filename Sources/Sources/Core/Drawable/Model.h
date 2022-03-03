#pragma once

#include "./Drawable.h"

#include <string>

class Model : public Drawable
{
private:

public:
	Model();
	virtual ~Model() override;

	bool Load(std::string filename);
};

