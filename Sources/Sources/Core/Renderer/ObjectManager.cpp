#include "ObjectManager.h"
#include <fstream>
#include <sstream>

ObjectManager* ObjectManager::m_instance = nullptr;

ObjectManager::ObjectManager()
{
	
}

ObjectManager::~ObjectManager()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

ObjectManager* ObjectManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ObjectManager;
	}
	return m_instance;
}

Buffer* ObjectManager::GetBufferAddress(int index) const
{
	auto ptr = m_pBufferList.begin();
	while (index--)
	{
		ptr++;
	}

	return *ptr;
}

bool ObjectManager::Load(std::string filename)
{
	std::ifstream ifs(filename, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}
	Buffer* buf = new Buffer;
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
			iss >> type >> vx >> vy >> vz;
			buf->m_vertices.push_back({ vx, vy, vz });
		}
		// vertex texture
		else if (type == "vt")
		{
			std::string type;
			float tx, ty, tz;
			iss >> type >> tx >> ty >> tz;
			buf->m_textures.push_back({ tx, ty, tz });
		}
		// vertex normal
		else if (type == "vn")
		{
			std::string type;
			float nx, ny, nz;
			iss >> type >> nx >> ny >> nz;
			buf->m_normals.push_back({ nx, ny, nz });
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
				buf->m_indices[i].push_back({ v - 1,t - 1,n - 1 });
			}
		}
		else
		{
			// e.g. '#' here
			std::cout << line << std::endl;
			continue;
		}
	}
	m_pBufferList.push_back(buf);
	std::cout << "Load Success::"
		<< "vertex: " << buf->m_vertices.size()
		<< " texture: " << buf->m_textures.size()
		<< " normal: " << buf->m_normals.size()
		<< " faces: " << buf->m_indices[0].size()
		<< "\n";

	return true;
}
