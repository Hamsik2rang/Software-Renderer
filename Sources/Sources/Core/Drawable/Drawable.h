#pragma once

#include "../Math/Vector.hpp"
#include <vector>

class Drawable
{
protected:
	// Constant
	static const int VERTEX = 0;
	static const int TEXTURE = 1;
	static const int NORMAL = 2;

	std::vector<Vec3f> m_vertices;
	std::vector<Vec3f> m_textures;
	std::vector<Vec3f> m_normals;
	std::vector<Vector3D<uint32_t>> m_indices[3];	// vertex/texture/normal index
	

	uint16_t m_stride = 3;

	Vec3f m_posision{ 0.f, 0.f, 0.f };
	Vec3f m_rotation{ 0.f, 0.f, 0.f };
	Vec3f m_scale{ 1.f, 1.f, 1.f };
public:
	virtual ~Drawable() {};
};