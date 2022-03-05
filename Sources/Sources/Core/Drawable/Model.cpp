#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>


Model::Model()
{}

Model::~Model()
{}

bool Model::Load(std::string filename)
{
	std::ifstream ifs(filename, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}
	std::stringstream ss;
	ss << ifs.rdbuf();

	while (!ss.eof())
	{
		std::string type;
		ss >> type;

		char delim;
		// vertex
		if (type == "v")
		{
			float vx, vy, vz;
			ss >> vx >> delim >> vy >> delim >> vz;
			m_vertices.push_back({ vx, vy, vz });
		}
		// vertex texture
		else if (type == "vt")
		{
			float tx, ty, tz;
			ss >> tx >> delim >> ty >> delim >> tz;
			m_textures.push_back({ tx, ty, tz });
		}
		// vertex normal
		else if (type == "vn")
		{
			float nx, ny, nz;
			ss >> nx >> delim >> ny >> delim >> nz;
			m_normals.push_back({ nx, ny, nz });
		}
		// face(index)
		else if (type == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				uint32_t x, y, z;
				ss >> x >> delim >> y >> delim >> z;
				m_indices[i].push_back({ x,y,z });
			}
		}
		else
		{
			// e.g. '#' here
			continue;
		}
	}
	std::cout << "Load Success::" 
		<< "vertex: " << m_vertices.size() 
		<< " texture: " << m_textures.size()
		<< " normal: " << m_normals.size() 
		<< " faces: " << m_indices[0].size() 
		<< "\n";
	return true;
}
