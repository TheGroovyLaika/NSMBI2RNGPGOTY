// collision.h

#ifndef COLLISION_H
#define COLLISION_H

#include <vector>

#include <irrlicht.h>
#include "jump.h"
#include "../level/entities.h"
#include "../level/player_state.h"

class Collision
{

  irr::scene::IAnimatedMeshSceneNode *node;
  irr::scene::ISceneManager *smgr;
  Jump *jump;
  Kirbies *kirbies;
  Coins *coins;
  Player_state *player_state;
  float mesh_size;
  bool isCollisioning;
  bool isWalking;
  bool alreadyWalking;
  int nb_kirbies;
  int nb_coins;
  bool isGameOver;
  
public:
  Collision();
  void set_node(irr::scene::IAnimatedMeshSceneNode *node);
  void set_smgr(irr::scene::ISceneManager *smgr);
  void set_jump(Jump *jump);
  void set_kirbies(Kirbies *kirbies);
  void set_coins(Coins *coins);
  void set_player_state(Player_state *player_state);
  void set_walking(bool isWalking);
  void set_kirbies_number(int nb_kirbies);
  void set_coins_number(int nb_coins);
  void compute_collision();
};

#endif
