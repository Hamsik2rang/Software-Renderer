#pragma once

#include "./Drawable.h"

#include <string>

class RenderObject : public Drawable
{
private:

public:
	RenderObject();
	virtual ~RenderObject() override;

	bool Load(std::string filename);
};

