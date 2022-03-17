#include "RenderObject.h"


void RenderObject::SetBuffer(Buffer* pBuffer)
{
	m_pBuffer = pBuffer;
}

RenderObject::RenderObject()
	:m_pBuffer(nullptr)
{}

RenderObject::RenderObject(RenderObject& r)
	:m_position(r.m_position), m_rotation(r.m_rotation), m_scale(r.m_scale)
{
	m_pBuffer = new Buffer;
	m_pBuffer->m_vertices.assign(r.m_pBuffer->m_vertices.begin(), r.m_pBuffer->m_vertices.end());
	m_pBuffer->m_textures.assign(r.m_pBuffer->m_textures.begin(), r.m_pBuffer->m_textures.end());
	m_pBuffer->m_normals.assign(r.m_pBuffer->m_normals.begin(), r.m_pBuffer->m_normals.end());
	for (int i = 0; i < 3; i++)
	{
		m_pBuffer->m_indices[i].assign(r.m_pBuffer->m_indices[i].begin(), r.m_pBuffer->m_indices[i].end());
	}

	m_pBuffer->m_stride = r.m_pBuffer->m_stride;
}

RenderObject::~RenderObject()
{}