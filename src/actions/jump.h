// jump.h

#ifndef JUMP_H
#define JUMP_H

#include <vector>

#include <irrlicht.h>

class Jump
{

  irr::scene::IAnimatedMeshSceneNode *node;
  irr::scene::ICameraSceneNode *camera;
  float speed;
  float lateral_speed;
  bool isJumping;
  bool isCollisioning;
  bool isDoubleJumping;
  
public:
  Jump();
  void set_node(irr::scene::IAnimatedMeshSceneNode *n){node=n;}
  void set_camera(irr::scene::ICameraSceneNode *c){camera=c;}
  void set_speed(float s);
  void set_lateral_speed(float l_s){lateral_speed=l_s;}
  void set_jumping(bool iJ){isJumping=iJ;}
  void set_double_jumping(bool iDJ){isDoubleJumping=iDJ;}
  void set_collision(bool iC);

  void jump();

  float get_speed();
  float get_lateral_speed();
  bool get_collision();
  bool get_jumping();
  bool get_double_jumping();
  
  void update_jump();
};

#endif
