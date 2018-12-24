// file_loader.h

#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <vector>
#include <irrlicht.h>

class File_loader
{
	int position_nb[5];
	std::vector<irr::core::vector3df> position_data;
public:
	File_loader();
	void position_loader(const char* file_name);
	int* getPosition_nb();
	std::vector<irr::core::vector3df> getPosition_data();
};

#endif