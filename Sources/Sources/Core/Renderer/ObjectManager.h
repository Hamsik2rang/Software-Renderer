#pragma once

#include "../Drawable/Buffer.h"

#include <list>
#include <string>

class ObjectManager
{
private:
	std::list<Buffer*> m_pBufferList;
	
	ObjectManager();
	static ObjectManager* m_instance;
public:
	~ObjectManager();
	
	static ObjectManager* GetInstance();
	bool Load(std::string filename);
	Buffer* GetBufferAddress(int index) const;
};