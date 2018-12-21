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
  void set_node(irr::scene::IAnimatedMeshSceneNode *node);
  void set_camera(irr::scene::ICameraSceneNode *camera);
  void set_speed(float speed);
  void set_lateral_speed(float lateral_speed);
  void set_jumping(bool jumping);
  void set_double_jumping(bool doublejumping);
  void set_collision(bool collision);

  void jump();

  float get_speed();
  float get_lateral_speed();
  bool get_collision();
  bool get_jumping();
  bool get_double_jumping();
  void update_jump();
};

#endif
