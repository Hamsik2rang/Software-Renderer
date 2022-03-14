#include "RenderObject.h"
#include <iostream>
#include <fstream>
#include <sstream>


RenderObject::RenderObject()
{}

RenderObject::~RenderObject()
{}

bool RenderObject::Load(std::string filename)
{
	std::ifstream ifs(filename, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);
		std::string type = line.substr(0, 2);
		std::istringstream iss(line);
		char delim;
		// vertex
		if (type == "v ")
		{
			std::string type;
			float vx, vy, vz;
			iss >> type >> vx >> delim >> vy >> delim >> vz;
			m_vertices.push_back({ vx, vy, vz });
		}
		// vertex texture
		else if (type == "vt")
		{
			std::string type;
			float tx, ty, tz;
			iss >> type >> tx >> delim >> ty >> delim >> tz;
			m_textures.push_back({ tx, ty, tz });
		}
		// vertex normal
		else if (type == "vn")
		{
			std::string type;
			float nx, ny, nz;
			iss >> type >> nx >> delim >> ny >> delim >> nz;
			m_normals.push_back({ nx, ny, nz });
		}
		// face(index)
		else if (type == "f ")
		{
			std::string type;
			iss >> type;
			for (int i = 0; i < 3; i++)
			{
				uint32_t v, t, n;
				iss >> v >> delim >> t >> delim >> n;
				m_indices[i].push_back({ v - 1,t - 1,n - 1 });
			}
		}
		else
		{
			// e.g. '#' here
			std::cout << line << std::endl;
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
