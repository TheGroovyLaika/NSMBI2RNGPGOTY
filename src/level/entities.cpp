// entities.cpp

#include <irrlicht.h>

#include "entities.h"
#include "math.h"
#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

/* ------------------ Player --------------------*/
/**************************************************************************\
 * Player::Player                                           *
\**************************************************************************/
Player::Player()
  : smgr(nullptr), player_node(nullptr)
{
}


/**************************************************************************\
 * Player::set_smgr                                                *
\**************************************************************************/
void Player::set_smgr(irr::scene::ISceneManager *s)
{
  smgr = s;
}

/**************************************************************************\
 * Player::init                                                *
\**************************************************************************/
is::IAnimatedMeshSceneNode* Player::init(ic::vector3df position)
{
  player_node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("data/tris.md2"),
                                               0,
                                               1,
                                               position,
                                               ic::vector3df(0.0f, 0.0f, 0.0f),
                                               ic::vector3df(1.0f, 1.0f,1.0f),
                                               false);

  player_node->setMaterialFlag(iv::EMF_LIGHTING, false);
  player_node->setMD2Animation(irr::scene::EMAT_STAND);

  return player_node;
}



/* ------------------ Kirbies --------------------*/
/**************************************************************************\
 * Kirbies::Kirbies                                           *
\**************************************************************************/
Kirbies::Kirbies()
  : smgr(nullptr), kirby_node(nullptr), falling(false), isAlive(true)
{
}

/**************************************************************************\
 * Kirbies::set_smgr                                                *
\**************************************************************************/
void Kirbies::set_smgr(irr::scene::ISceneManager *s)
{
  smgr = s;
}

/**************************************************************************\
 * Kirbies::get_isAlive                                                *
\**************************************************************************/
bool Kirbies::get_isAlive()
{
  return isAlive;
}

/**************************************************************************\
 * Kirbies::init                                                *
\**************************************************************************/
void Kirbies::init(ic::vector3df pos, int id)
{
  //creation des Kirbies en fonction de leur position et de l'id associee
 
  initial_position = pos;
  kirby_node = smgr->addMeshSceneNode(smgr->getMesh("data/Kirby/kirby.obj"),
                                      0,
                                      id,
                                      pos,
                                      ic::vector3df(0.0f, -90.0f, 0.0f),
                                      ic::vector3df(3.0f, 3.0f,3.0f),
                                      false);
  kirby_node->setMaterialFlag(iv::EMF_LIGHTING, false);
}

/**************************************************************************\
 * Kirbies::update_position                                                *
\**************************************************************************/
void Kirbies::update_position()
{
  // Si le Kirby est vivant, les kirbies flottent et suivent le personnage du regard
  if(isAlive)
  {
    ic::vector3df position = kirby_node->getPosition();
    ic::vector3df rotation = kirby_node->getRotation();
  
    if(position.Y > initial_position.Y + 2.0f && falling)
    {
      position.Y += (initial_position.Y + 1.0f - position.Y)/16;
      if(position.Y < initial_position.Y  + 2.0f)
        falling = false;
    }
    else
    {
      position.Y += (initial_position.Y  + 21.0f - position.Y)/16;
      if(position.Y > initial_position.Y  + 20.0f)
        falling = true;
    }
    
    irr::scene::ISceneNode * player = smgr->getSceneNodeFromId(1);
    
    if((player->getPosition().X - position.X) < 0)
    {
      rotation.Y = -85;
      rotation.Z = -asin((player->getPosition().Y - position.Y) / sqrt(pow((player->getPosition().Y - position.Y), 2) + pow((player->getPosition().X - position.X), 2)))  * 180 / 3.14158;
    }
    else
    {
      rotation.Y = 85;
      rotation.Z = asin((player->getPosition().Y - position.Y) / sqrt(pow((player->getPosition().Y - position.Y), 2) + pow((player->getPosition().X - position.X), 2)))  * 180 / 3.14158;
    }

    kirby_node->setPosition(position);
    kirby_node->setRotation(rotation);
  }
}

/**************************************************************************\
 * Kirbies::kill                                                *
\**************************************************************************/
void Kirbies::kill()
{
  //Gestion de la mort d'un Kirby :'(
  isAlive = false;
  kirby_node->remove();
}



/* ------------------ Coins --------------------*/
/**************************************************************************\
 * Coins::Coins                                           *
\**************************************************************************/
Coins::Coins()
  : smgr(nullptr), coin_node(nullptr), falling(false), isLooted(false), isBeingLooted(false), coin_speed(0)
{
}

/**************************************************************************\
 * Coins::set_smgr                                                *
\**************************************************************************/
void Coins::set_smgr(irr::scene::ISceneManager *s)
{
  smgr = s;
}

/**************************************************************************\
 * Coins::get_isLooted                                                *
\**************************************************************************/
bool Coins::get_isLooted()
{
  return isLooted;
}

/**************************************************************************\
 * Coins::get_isBeingLooted                                              *
\**************************************************************************/
bool Coins::get_isBeingLooted()
{
  return isBeingLooted;
}

/**************************************************************************\
 * Coins::init                                                *
\**************************************************************************/
void Coins::init(ic::vector3df pos, int id)
{
  //Creation des pieces
  initial_position = pos;
  coin_node = smgr->addMeshSceneNode(smgr->getMesh("data/Coin/coin.obj"),
                                      0,
                                      id,
                                      pos,
                                      ic::vector3df(0.0f, 90.0f, 0.0f),
                                      ic::vector3df(1.5f, 1.5f,1.0f),
                                      false);
  coin_node->setMaterialFlag(iv::EMF_LIGHTING, false);

  coin_node->setRotation(ic::vector3df(coin_node->getRotation().X, 90, coin_node->getRotation().Z));
  coin_node->setPosition(pos);
}

/**************************************************************************\
 * Coins::update_position                                                *
\**************************************************************************/
void Coins::update_position()
{

  ic::vector3df position;
  ic::vector3df rotation; 

  //Si la piece n'est pas recuperee par le personnage, on l'anime

  if(!isLooted)
  {
    position = coin_node->getPosition();
    rotation = coin_node->getRotation();
  }

  if(!isLooted && !isBeingLooted)
  {
  
    if(position.Y > initial_position.Y - 8.0f && falling)
    {
      position.Y += (initial_position.Y - 9.0f - position.Y)/32;
      if(position.Y < initial_position.Y - 8.0f)
        falling = false;
    }
    else
    {
      position.Y += (initial_position.Y + 1.0f - position.Y)/32;
      if(position.Y > initial_position.Y)
        falling = true;
    }
  
    rotation.Y += 0.5f;
  
    
  }
  else // Des que la piece est recuperee, elle s'anime et finit par disparaitre
  {
    rotation.Y += 5.0f;

    if(position.Y - initial_position.Y < 100.0f)
    {
      position.Y += coin_speed -5;
    if(coin_speed < -0.5)
      coin_speed *= 0.6;
    else
      coin_speed += 2.5;
    }
    else
    {
      isLooted = true;
      coin_node->remove();  
    }
  }

  if(!isLooted)
  {
    coin_node->setPosition(position);
    coin_node->setRotation(rotation);
  }
}

/**************************************************************************\
 * Coins::loot                                                *
\**************************************************************************/
void Coins::loot() //Gestion de la recuperation des pieces
{
  isBeingLooted = true;
  coin_speed = -5;
}