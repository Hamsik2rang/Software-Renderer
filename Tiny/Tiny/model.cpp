#include "model.h"

Model::Model(const char* filename) : _vertices(), _faces()
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			hs::Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.elem[i];
			_vertices.push_back(v);
		}
		else if (!line.compare(0, 2, "f "))
		{
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash)
			{
				idx--; // in wavefront obj all indices start at 1, not zero
				f.push_back(idx);
			}
			_faces.push_back(f);
		}
	}
	std::cerr << "# v# " << _vertices.size() << " f# " << _faces.size() << std::endl;
}

Model::~Model()
{}

int Model::vsize()
{
	return (int)_vertices.size();
}

int Model::fsize()
{
	return (int)_faces.size();
}

std::vector<int> Model::face(int idx)
{
	return _faces[idx];
}

hs::Vec3f Model::vertex(int i)
{
	return _vertices[i];
}
