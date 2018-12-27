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
  std::vector<Kirbies> *kirbies;
  std::vector<Coins> *coins;
  Player_state *player_state;
  float mesh_size;
  int nb_kirbies;
  int nb_coins;
  bool isGameOver;
  
public:
  Collision();
  void set_node(irr::scene::IAnimatedMeshSceneNode *n){node=n;}
  void set_smgr(irr::scene::ISceneManager *s){smgr=s;}
  void set_jump(Jump *j){jump=j;}
  void set_kirbies(std::vector<Kirbies> *k){kirbies=k;}
  void set_coins(std::vector<Coins> *c){coins=c;}
  void set_player_state(Player_state *p_s){player_state=p_s;}
  void set_kirbies_number(int n_k){nb_kirbies=n_k;}
  void set_coins_number(int n_c){nb_coins=n_c;}

  void compute_collision();
};

#endif
