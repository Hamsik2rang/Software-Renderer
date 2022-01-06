#include "model.h"

#include <iostream>

Model::Model(const char* filename)
{
	std::ifstream ifs(filename, std::ios::in);
	if (ifs.fail())
		return;

	std::string line = "";
	while (!ifs.eof())
	{
		std::getline(ifs, line);
		std::istringstream iss(line.c_str());
		char header;
		// vertices
		if (line.compare(0, 2, "v ") == 0)
		{
			iss >> header;

			Vec3f vertex;
			for (int i = 0; i < 3; i++)
				iss >> vertex.elem[i];
			_vertices.push_back(vertex);
		}
		//textures
		else if (line.compare(0, 3, "vt ") == 0)
		{
			// throw header away
			iss >> header;
			iss >> header;

			Vec2f texture;
			for (int i = 0; i < 2; i++)
				iss >> texture.elem[i];
			_textures.push_back(texture);
		}
		// normals
		else if (line.compare(0, 3, "vn ") == 0)
		{
			// throw header away
			iss >> header;
			iss >> header;

			Vec3f normal;
			for (int i = 0; i < 3; i++)
				iss >> normal.elem[i];
			_normals.push_back(normal);
		}
		// faces
		else if (line.compare(0, 2, "f ") == 0)
		{
			iss >> header;
			// [0] = vertex, [1] = texture, [2] = normal
			FaceElementIndex faceElem[3];
			char delimeter;
			for (int i = 0; i < 3; i++)
			{
				iss >> faceElem[0][i] >> delimeter >> faceElem[1][i]>> delimeter >> faceElem[2][i];
				// index in *.obj is started with 1.
				faceElem[0][i]--;
				faceElem[1][i]--;
				faceElem[2][i]--;
			}
			Face face{ faceElem[0], faceElem[1], faceElem[2] };
			_faces.push_back(face);
		}
	}
	std::cerr << "# v#" << _vertices.size() 
		<< " t#" << _textures.size() 
		<< " n#" << _normals.size() 
		<< " f#" << _faces.size() 
		<< std::endl;
	
	ifs.close();
}

Model::~Model() {}

Vec3f Model::vertex(int index) const
{
	return _vertices[index];
}

Vec2f Model::texture(int index) const
{
	return _textures[index];
}

Vec3f Model::normal(int index) const
{
	return _normals[index];
}

Face Model::face(int index) const
{
	return _faces[index];
}

size_t Model::vsize() const
{
	return _vertices.size();
}

size_t Model::tsize() const
{
	return _textures.size();
}

size_t Model::nsize() const
{
	return _normals.size();
}

size_t Model::fsize() const
{
	return _faces.size();
}

