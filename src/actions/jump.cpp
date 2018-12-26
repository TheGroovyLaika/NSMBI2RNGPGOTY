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
  : node(nullptr), camera(nullptr), speed(0), lateral_speed(0), isJumping(false), isCollisioning(false), isDoubleJumping(false)
{
}

/**************************************************************************\
 * Jump::set_speed                                                *
\**************************************************************************/
void Jump::set_speed(float s)
{
  if(!isDoubleJumping)
    speed = s;
  if(speed > 0 && isJumping && !isCollisioning)
    isDoubleJumping = true;
}

/**************************************************************************\
 * Jump::set_collision                                                *
\**************************************************************************/
void Jump::set_collision(bool c)
{
  isCollisioning = c;
  if(c)
    isDoubleJumping = false;
}

/**************************************************************************\
 * Jump::jump

  Fonction gerant l'initialisation des valeurs du saut et verifiant s'il y a double jump ou non 
\**************************************************************************/
void Jump::jump()
{

  if(!isDoubleJumping)
  {
    node->setMD2Animation(is::EMAT_JUMP);
    speed = 90.0f;
    if(!isJumping || isCollisioning)
      isJumping = true;
    else
      isDoubleJumping = true;
    isCollisioning = false;
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
 * Jump::get_collision                                                *
\**************************************************************************/
bool Jump::get_collision()
{
  return isCollisioning;
}

/**************************************************************************\
 * Jump::get_jumping                                                *
\**************************************************************************/
bool Jump::get_jumping()
{
  return isJumping;
}


/**************************************************************************\
 * Jump::get_double_jumping                                                *
\**************************************************************************/
bool Jump::get_double_jumping()
{
  return isDoubleJumping;
}

/**************************************************************************\
 * Jump::jumping_calculation                                                *
\**************************************************************************/
void Jump::update_jump()
{
  //Calcul de la trajectoire du saut en fonction de la gravite terrestre
  float position_Y = node->getPosition().Y;
  float position_X = node->getPosition().X;

  if(isJumping && !isCollisioning)
  {
    speed -= 35*9.8*0.01;
    if(speed < -70)
      speed = -70;
    position_Y += speed*0.1;
    position_X += lateral_speed;
  }
  if(!isCollisioning)
  {
    node->setPosition(ic::vector3df(position_X, position_Y, node->getPosition().Z));
  }

}
