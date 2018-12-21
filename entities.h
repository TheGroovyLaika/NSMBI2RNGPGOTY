// entities.h

#ifndef ENTITIES_H
#define ENTITIES_H

#include <vector>

#include <irrlicht.h>

class Kirbies
{

  irr::scene::ISceneManager *smgr;
  irr::scene::IMeshSceneNode *kirby_node;
  irr::core::vector3df initial_position;
  bool falling;
  bool isAlive;

public:
  Kirbies();
  void set_smgr(irr::scene::ISceneManager *smgr);
  bool get_isAlive();
  void init(irr::core::vector3df pos, int id);
  void update_position();
  void kill();

};

class Coins
{

  irr::scene::ISceneManager *smgr;
  irr::scene::IMeshSceneNode *coin_node;
  irr::core::vector3df initial_position;
  bool falling;
  bool isLooted;
  bool isBeingLooted;
  float coin_speed;

public:
  Coins();
  void set_smgr(irr::scene::ISceneManager *smgr);
  bool get_isLooted();
  bool get_isBeingLooted();
  void init(irr::core::vector3df pos, int id);
  void update_position();
  void loot();

};

#endif
