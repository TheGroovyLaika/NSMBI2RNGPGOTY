// file_loader.cpp

#include <irrlicht.h>

#include "file_loader.h"
#include <iostream>
#include <fstream>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

/**************************************************************************\
 * File_loader::File_loader                                           *
\**************************************************************************/
File_loader::File_loader()
{}

void File_loader::position_loader(const char* file_name)
{
	FILE * pFile;
	pFile = fopen(file_name,"r");
	ic::vector3df position;

	if(pFile != NULL)
	{
		if(fscanf(pFile, "[%i %i %i %i %i]\n", position_nb, position_nb + 1, position_nb + 2, position_nb + 3, position_nb + 4) < 5)
			std::cout<<"FILE LOADING PROBLEM - Maybe the file is not full?"<<std::endl;
		while(fscanf(pFile, "%f %f %f\n", &position.X, &position.Y, &position.Z) > 0)
			position_data.push_back(position);		
	}
}

/**************************************************************************\
 * File_loader::getPosition_nb                                           *
\**************************************************************************/
int* File_loader::getPosition_nb()
{
	return position_nb;
}

/**************************************************************************\
 * File_loader::getPosition_data                                           *
\**************************************************************************/
std::vector<irr::core::vector3df> File_loader::getPosition_data()
{
	return position_data;
}

