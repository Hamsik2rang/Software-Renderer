#pragma once

#include "../Math/Vector.hpp"

#include <vector>

struct Buffer
{
	std::vector<Vec4f> m_vertices;
	std::vector<Vec4f> m_textures;
	std::vector<Vec4f> m_normals;
	std::vector<Vector3D<uint32_t>> m_indices[3];

	uint8_t m_stride = 3;
};