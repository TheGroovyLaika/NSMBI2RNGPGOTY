// jump.h

#ifndef JUMP_H
#define JUMP_H

#include <vector>

#include <irrlicht.h>
#include "../level/player_state.h"

class Jump
{

  irr::scene::IAnimatedMeshSceneNode *node;
  irr::scene::ICameraSceneNode *camera;
  Player_state *player_state;
  float speed;
  float lateral_speed;
  int jump_counts;
  
public:
  Jump();
  void set_node(irr::scene::IAnimatedMeshSceneNode *n){node=n;}
  void set_camera(irr::scene::ICameraSceneNode *c){camera=c;}
  void set_player_state(Player_state *p_s){player_state=p_s;}
  void set_speed(float s){speed=s;}
  void set_lateral_speed(float l_s){lateral_speed=l_s;}

  void jump();

  float get_speed();
  float get_lateral_speed();
  
  void update_jump();
  void reset_jumps();
};

#endif
