#pragma once

#include "../Math/Vector.hpp"

#include <vector>

class Drawable
{
public:
	std::vector<Vec4f> m_vertices;
	std::vector<Vec4f> m_textures;
	std::vector<Vec4f> m_normals;
	std::vector<Vector3D<uint32_t>> m_indices[3];	// vertex/texture/normal index

	uint16_t m_stride = 3;

	Vec3f m_position{ 0.f, 0.f, 0.f };
	Vec3f m_rotation{ 0.f, 0.f, 0.f };	// degree
	Vec3f m_scale{ 1.f, 1.f, 1.f };
public:
	virtual ~Drawable() {};
};