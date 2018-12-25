// level_creator.cpp

#include <irrlicht.h>

#include "level_creator.h"
#include <iostream>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * Level_Creator::create_platform                                           *
\**************************************************************************/
void Level_Creator::create_platform(irr::core::vector3df position, int mesh_id, irr::core::vector3df platform_size, irr::video::ITexture *texture, irr::core::matrix4 texture_mat)
{
	is::ISceneNode* cube_node;
	cube_node = smgr->addCubeSceneNode(20);
  cube_node->setPosition(position);
  cube_node->setScale(platform_size);
  cube_node->setID(mesh_id);
  cube_node->setMaterialTexture(0, texture);
	cube_node->getMaterial(0).setTextureMatrix(0,texture_mat);

  std::cout<<"---- ID : "<<mesh_id<<std::endl;
}

/**************************************************************************\
 * Level_Creator::create_coins                                           *
\**************************************************************************/
int Level_Creator::create_coins(irr::core::vector3df position, irr::core::vector3df platform_size, int coins_id, int platform_type, int nb_kirbies)
{
	switch (platform_type)
	{
		case 0:
			{
        Coins coin;
        (*coins).push_back(coin);
        (*coins)[coins_id] = Coins();
		    (*coins)[coins_id].set_smgr(smgr);
		    (*coins)[coins_id].init(ic::vector3df(position.X, position.Y + 80, position.Z), coins_id + 5 + nb_kirbies);
		    coins_id++;
			}			
			break;
		case 1:
			{
				float platform_width = 20 * platform_size.X;
				for(int k = 0; k < 2; k++)
				{
          Coins coin = Coins();
          (*coins).push_back(coin);
			    (*coins)[coins_id].set_smgr(smgr);
			    (*coins)[coins_id].init(ic::vector3df(position.X - platform_width/4 + k*platform_width/2, position.Y + 20, position.Z), coins_id + 5 + nb_kirbies);
			    coins_id++;
				}
			}			
			break;
		case 2:
			{
				float platform_width = 20 * platform_size.X;
				for(int k = 0; k < 4; k++)
				{
          Coins coin = Coins();
          (*coins).push_back(coin);
			    (*coins)[coins_id].set_smgr(smgr);
			    (*coins)[coins_id].init(ic::vector3df(position.X - platform_width/2 + platform_width/8 + k*platform_width/4, position.Y + 20, position.Z), coins_id + 5 + nb_kirbies);
			    coins_id++;
				}
			}	
		break;
		case 3:
			{
				float platform_width = 20 * platform_size.X;
				for(int k = 0; k < 15; k++)
				{
          Coins coin = Coins();
          (*coins).push_back(coin);
			    (*coins)[coins_id].set_smgr(smgr);
			    (*coins)[coins_id].init(ic::vector3df(position.X - platform_width/2 + platform_width/30 + k*platform_width/15, position.Y + 20, position.Z), coins_id + 5 + nb_kirbies);
			    coins_id++;
				}
			}
		break;
		default:
			{
        Coins coin = Coins();
        (*coins).push_back(coin);  
		    (*coins)[coins_id].set_smgr(smgr);
		    (*coins)[coins_id].init(ic::vector3df(position.X, position.Y + 80, position.Z), coins_id + 5 + nb_kirbies);
		    coins_id++;
			}
			break;
	}
	return coins_id;
}

/**************************************************************************\
 * Level_Creator::create_kirbies                                           *
\**************************************************************************/
void Level_Creator::create_kirbies(irr::core::vector3df position, int kirbies_id)
{
  Kirbies kirby = Kirbies();
  (*kirbies).push_back(kirby);
  (*kirbies)[kirbies_id].set_smgr(smgr);
  (*kirbies)[kirbies_id].init(position, kirbies_id + 5);
}

/**************************************************************************\
 * Level_Creator::Level_Creator                                           *
\**************************************************************************/
Level_Creator::Level_Creator()
  : smgr(nullptr), driver(nullptr), coins(nullptr), kirbies(nullptr), nb_kirbies(0), nb_coins(0), nb_platforms(0), levelIsMade(false)
{
}

/**************************************************************************\
 * Level_Creator::set_smgr                                           *
\**************************************************************************/
void Level_Creator::set_smgr(irr::scene::ISceneManager *s)
{
	smgr = s;
}

/**************************************************************************\
 * Level_Creator::set_driver                                           *
\**************************************************************************/
void Level_Creator::set_driver(irr::video::IVideoDriver *d)
{
	driver = d;
}

/**************************************************************************\
 * Level_Creator::set_coins                                           *
\**************************************************************************/
void Level_Creator::set_coins(std::vector<Coins> *c)
{
	coins = c;
}

/**************************************************************************\
 * Level_Creator::set_kirbies                                           *
\**************************************************************************/
void Level_Creator::set_kirbies(std::vector<Kirbies> *k)
{
	kirbies = k;
}

bool Level_Creator::get_level_state()
{
  return levelIsMade;
}

/**************************************************************************\
 * Level_Creator::load_level                                           *

 Creating each part of the level :

 1. Creating the 4 types of platforms
  1.1 : the unit ones,   plus the coins associated with them
  1.2 : the small ones,  plus the coins associated with them
  1.3 : the medium ones, plus the coins associated with them
  1.4 : the big ones,    plus the coins associated with them

 2. The Kirbies (ennemies)

\**************************************************************************/
void Level_Creator::load_level(int position_nb[], std::vector<irr::core::vector3df> position_data)
{
	std::vector<irr::core::vector3df>  platform_sizes = {ic::vector3df(1.5f , 1.5f, 1.5f),
														 ic::vector3df(3.0f , 1.0f, 1.0f),
														 ic::vector3df(8.0f , 1.0f, 1.0f),
														 ic::vector3df(30.0f, 1.0f, 1.0f)};
	int position_id = 0;
	int coins_id = 0;
 	iv::ITexture *brick = driver->getTexture("data/Brick.jpg");
	irr::core::matrix4 texture_mat;

  nb_coins = position_nb[0] + position_nb[1] * 2 + position_nb[2] * 4 + position_nb[3] * 15;
 	nb_kirbies = position_nb[4];
  nb_platforms = position_nb[0] + position_nb[1] + position_nb[2] + position_nb[3]; 

  for(int platform_type = 0; platform_type < 4; platform_type++)
  {
  	texture_mat.setTextureScale(platform_sizes[platform_type].X, platform_sizes[platform_type].Y);

  	for(int platform_nb = 0; platform_nb < position_nb[platform_type]; platform_nb++)
  	{
  		create_platform(position_data[position_id], 5 + nb_coins + nb_kirbies + position_id, platform_sizes[platform_type], brick, texture_mat);
	    coins_id = create_coins(position_data[position_id], platform_sizes[platform_type], coins_id, platform_type, nb_kirbies);
      position_id++;
  	}
  }

  for(int kirbies_id = 0; kirbies_id < nb_kirbies; kirbies_id++)
  {
  	create_kirbies(position_data[position_id + kirbies_id], kirbies_id);
  }
  levelIsMade = true;
}

/**************************************************************************\
 * Level_Creator::load_background                                           *
\**************************************************************************/
void Level_Creator::load_background()
{
   // Fond
  is::IAnimatedMesh *background = smgr->addHillPlaneMesh("background",
                                                          ic::dimension2d<irr::f32>(2000,1000),
                                                          ic::dimension2d<irr::u32>(10,1), 0, 0,
                                                          ic::dimension2d<irr::f32>(0,0),
                                                          ic::dimension2d<irr::f32>(10,1));

  is::IAnimatedMeshSceneNode *node_background = smgr->addAnimatedMeshSceneNode(background);

  node_background->setPosition(ic::vector3df(9000, 300, 500));
  node_background->setRotation(ic::vector3df(-90,node_background->getRotation().Y, node_background->getRotation().Z));
  node_background->setMaterialTexture( 0, driver->getTexture("data/background.png") );
  node_background->setMaterialFlag(irr::video::EMF_LIGHTING, false);

  // Maison (decor)
  irr::scene::IMeshSceneNode *house_node = smgr->addMeshSceneNode(smgr->getMesh("data/House/house.obj"),
                                      0,
                                      1000,
                                      ic::vector3df(-90.0f, -60.0f, 30.0f),
                                      ic::vector3df(0.0f, 90.0f, 0.0f),
                                      ic::vector3df(3.0f, 3.0f,3.0f),
                                      false);
  house_node->setMaterialFlag(iv::EMF_LIGHTING, true);
  house_node->setMaterialType(video::EMT_SOLID);

  // Drapeau d'arrivee (decor)
  irr::scene::IMeshSceneNode *flag_node = smgr->addMeshSceneNode(smgr->getMesh("data/Flag/flag.obj"),
                                      0,
                                      1001,
                                      ic::vector3df(8080.0f, -30.0f, 30.0f),
                                      ic::vector3df(0.0f, 0.0f, 0.0f),
                                      ic::vector3df(3.0f, 3.0f,3.0f),
                                      false);
  flag_node->setMaterialFlag(iv::EMF_LIGHTING, false);
  flag_node->setMaterialType(video::EMT_SOLID);

  //Mesh correspondant aux arbres en fond
  is::IAnimatedMesh *tree = smgr->addHillPlaneMesh("tree",
                                                    ic::dimension2d<irr::f32>(100,120),
                                                    ic::dimension2d<irr::u32>(1,1), 0, 0,
                                                    ic::dimension2d<irr::f32>(0,0),
                                                    ic::dimension2d<irr::f32>(1,1));
  int nb_trees = 42;
  is::IAnimatedMeshSceneNode*  tree_nodes[nb_trees];

  srand(time(NULL));

  //Generation aleatoire des arbres
  for(int i = 0; i < nb_trees; i++)
  {
    tree_nodes[i] = smgr->addAnimatedMeshSceneNode(tree);

    tree_nodes[i]->setPosition(ic::vector3df(0, 400, 1000));
    tree_nodes[i]->setRotation(ic::vector3df(-90,tree_nodes[i]->getRotation().Y, tree_nodes[i]->getRotation().Z));
    tree_nodes[i]->setMaterialTexture( 0, driver->getTexture("data/tree.png") );
    tree_nodes[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    tree_nodes[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    tree_nodes[i]->setPosition(ic::vector3df(i*200, 20, rand()%300 + 200));
  }
}

/**************************************************************************\
 * Level_Creator::remove_platforms                                           *
\**************************************************************************/
void Level_Creator::remove_level()
{
  for(int mesh_id = 5 + nb_coins + nb_kirbies; mesh_id < 5 + nb_coins + nb_kirbies + nb_platforms; mesh_id++)
  {
    std::cout<<"Removing platform ---- ID : "<<mesh_id<<std::endl;
    smgr->getSceneNodeFromId(mesh_id)->remove();
  }

  while(!(*coins).empty())
  {
    std::cout<<"Removing coins ---- size : "<<(*coins).size()<<std::endl;
    (*coins).back().remove_node();
    (*coins).pop_back();
  }

  while(!(*kirbies).empty())
  {
    std::cout<<"Removing kirbies ---- size : "<<(*kirbies).size()<<std::endl;
    (*kirbies).back().remove_node();
    (*kirbies).pop_back();
  }

  levelIsMade = false;
}