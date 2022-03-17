#pragma once

#include "./Buffer.h"

#include <string>

class RenderObject 
{
private:

public:
	Buffer* m_pBuffer;
	Vec3f m_position{ 0.f, 0.f, 0.f };
	Vec3f m_rotation{ 0.f, 0.f, 0.f };	// degree
	Vec3f m_scale{ 1.f, 1.f, 1.f };

	void SetBuffer(Buffer* pBuffer);
	RenderObject();
	RenderObject(RenderObject& r);
	~RenderObject();
};

