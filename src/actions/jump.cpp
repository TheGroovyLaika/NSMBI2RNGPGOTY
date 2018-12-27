// jump.cpp

#include <irrlicht.h>

#include "jump.h"
#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * Jump::Jump                                           *
\**************************************************************************/
Jump::Jump()
  : node(nullptr), camera(nullptr), player_state(nullptr), speed(0), lateral_speed(0), jump_counts(0)
{
}

/**************************************************************************\
 * Jump::jump

  Fonction gerant l'initialisation des valeurs du saut et verifiant s'il y a double jump ou non 
\**************************************************************************/
void Jump::jump()
{
  if(jump_counts < 2)
  {
    jump_counts++;
    player_state->set_character_state(jumping);
    node->setMD2Animation(is::EMAT_JUMP);
    speed = 90.0f;
  }
}

/**************************************************************************\
 * Jump::get_speed                                                *
\**************************************************************************/
float Jump::get_speed()
{
  return speed;
}


/**************************************************************************\
 * Jump::get_lateral_speed                                                *
\**************************************************************************/
float Jump::get_lateral_speed()
{
  return lateral_speed;
}

/**************************************************************************\
 * Jump::jumping_calculation                                                *
\**************************************************************************/
void Jump::update_jump()
{
  //Calcul de la trajectoire du saut en fonction de la gravite terrestre
  float position_Y = node->getPosition().Y;
  float position_X = node->getPosition().X;

  if(player_state->get_character_state() == jumping)
  {
    speed -= 35*9.8*0.01;
    if(speed < -70)
      speed = -70;
    position_Y += speed*0.1;
    position_X += lateral_speed;

    node->setPosition(ic::vector3df(position_X, position_Y, node->getPosition().Z));
  }
}

void Jump::reset_jumps()
{
  jump_counts = 0;
}