// level_creator.h

#ifndef LEVEL_CREATOR_H
#define LEVEL_CREATOR_H

#include <vector>
#include <irrlicht.h>
#include "entities.h"

class Level_Creator
{
	irr::scene::ISceneManager *smgr;
	irr::video::IVideoDriver  *driver;
	std::vector<Coins> *coins;
	std::vector<Kirbies> *kirbies;
	int nb_kirbies;
	int nb_coins;
	int nb_platforms;
	bool levelIsMade;
	void create_platform(irr::core::vector3df position, int mesh_id, irr::core::vector3df platform_size, irr::video::ITexture *texture, irr::core::matrix4 texture_mat);
	int  create_coins(irr::core::vector3df position, irr::core::vector3df platform_size, int coins_id, int platform_type, int nb_kirbies);
	void create_kirbies(irr::core::vector3df position, int kirbies_id);

public:
	Level_Creator();
	void set_smgr(irr::scene::ISceneManager *s);
	void set_driver(irr::video::IVideoDriver *d);
	void set_coins(std::vector<Coins> *coins);
	void set_kirbies(std::vector<Kirbies> *kirbies);
	bool get_level_state();
	void load_level(int position_nb[], std::vector<irr::core::vector3df> position_data);
	void load_background();
	void remove_level();
};

#endif