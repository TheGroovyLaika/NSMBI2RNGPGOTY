// events.h

#ifndef EVENTS_H
#define EVENTS_H

#include <vector>

#include <irrlicht.h>
#include "jump.h"
#include "collision.h"
#include "../level/player_state.h"

class EventReceiver : public irr::IEventReceiver
{
  irr::scene::IAnimatedMeshSceneNode *node;
  irr::scene::ICameraSceneNode *camera;
  std::vector<irr::video::ITexture*> textures;
  irr::EKEY_CODE key_code;
  Jump *jump;
  Collision *collision;
  bool button_pressed;
  float lateral_speed;
  int current_texture;
  bool isWalking;
  bool key_pressed_down;
  Player_state *player_state;

  bool mouse(const irr::SEvent &event);
  bool keyboard(const irr::SEvent &event);
public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);

  void set_node(irr::scene::IAnimatedMeshSceneNode *n){node=n;}
  void set_camera(irr::scene::ICameraSceneNode *c){camera=c;}
  void set_textures(const std::vector<irr::video::ITexture *> &tex){textures=tex;}
  void set_jump(Jump *j){jump=j;}
  void set_collision(Collision *c){collision=c;}
  void set_player_state(Player_state *p_s){player_state=p_s;}

  void compute_keyboard();
  void compute_camera();
};

#endif
