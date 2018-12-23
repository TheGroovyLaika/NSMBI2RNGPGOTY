// collision.cpp

#include <irrlicht.h>

#include "collision.h"
#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * Collision::Collision                                           *
\**************************************************************************/
Collision::Collision()
  : node(nullptr), smgr(nullptr), jump(nullptr), kirbies(nullptr), coins(nullptr), mesh_size(0), isCollisioning(false),  isWalking(false), alreadyWalking(false), nb_kirbies(0), nb_coins(0), isGameOver(false)
{
}


/**************************************************************************\
 * Collision::set_node                                                *
\**************************************************************************/
void Collision::set_node(irr::scene::IAnimatedMeshSceneNode *n)
{
  node = n;
  mesh_size = node->getTransformedBoundingBox().MaxEdge.Y - node->getTransformedBoundingBox().MinEdge.Y;
}

/**************************************************************************\
 * Collision::set_smgr                                                *
\**************************************************************************/
void Collision::set_smgr(is::ISceneManager *s)
{
  smgr = s;
}

/**************************************************************************\
 * Collision::set_jump                                                *
\**************************************************************************/
void Collision::set_jump(Jump *j)
{
  jump = j;
}


/**************************************************************************\
 * Collision::set_walking                                                *
\**************************************************************************/
void Collision::set_walking(bool iw)
{
  isWalking = iw;
}

/**************************************************************************\
 * Collision::set_kirbies_number                                                *
\**************************************************************************/
void Collision::set_kirbies_number(int k)
{
  nb_kirbies = k;
}

/**************************************************************************\
 * Collision::set_kirbies                                               *
\**************************************************************************/
void Collision::set_kirbies(Kirbies *k)
{
  kirbies = k;
}

/**************************************************************************\
 * Collision::set_coins_number                                                *
\**************************************************************************/
void Collision::set_coins_number(int c)
{
  nb_coins = c;
}

/**************************************************************************\
 * Collision::set_coins                                               *
\**************************************************************************/
void Collision::set_coins(Coins *c)
{
  coins = c;
}

/**************************************************************************\
 * Collision::set_player_state                                               *
\**************************************************************************/
void Collision::set_player_state(Player_state *ps)
{
  player_state = ps;
}

/**************************************************************************\
 * Collision::compute_collision                                                *
\**************************************************************************/
void Collision::compute_collision()
{
  //Collision avec les plateformes
  ic::array< irr::scene::ISceneNode * > all_cubes;
  smgr->getSceneNodesFromType(irr::scene::ESNT_CUBE, all_cubes);
  irr::core::aabbox3d<float> nodeBox = node->getTransformedBoundingBox();
  isCollisioning = false;

  // On regarde pour tous les cubes s'il y a collision avec le personnage grace aux bounding boxes
  for(int i = 0; i < int(all_cubes.size()); i++)
  {
    irr::core::aabbox3d<float> cubeBox = all_cubes[i]->getTransformedBoundingBox();

    if(cubeBox.intersectsWithBox(nodeBox))
    {
      //Si le personnage va vers le bas et est en plein saut
      if(jump->get_speed() < 0.0f && jump->get_jumping())
      {
        //S'il est au dessus de la plateforme
        if( nodeBox.getCenter().X < cubeBox.MaxEdge.X && nodeBox.getCenter().X > cubeBox.MinEdge.X && ( nodeBox.getCenter().Y > cubeBox.MaxEdge.Y + mesh_size/8 ))
        {
          //Sa chute devient nulle
          jump->set_speed(-0.0000001);
          jump->set_collision(true);

          node->setPosition(ic::vector3df(node->getPosition().X, cubeBox.MaxEdge.Y + mesh_size/2 - 5, node->getPosition().Z));
          isCollisioning = true;

          if(isWalking)
          {
            //Gestion de l'animation de course quand on tombe sur la plateforme et qu'on bouge
            if(!alreadyWalking)
              node->setMD2Animation(is::EMAT_RUN);
            alreadyWalking = true;
          }

          else
          {
            //Gestion de l'animation debout quand on tombe sur la plateforme et qu'on ne bouge plus
            alreadyWalking = false;
            node->setMD2Animation(is::EMAT_STAND);
            jump->set_lateral_speed(0);
          }
        }
      }
    }

  }
  if(!isCollisioning)
    alreadyWalking = false;
  jump->set_collision(isCollisioning);

  //Collision avec les Kirbies
  for(int k = 0; k < nb_kirbies; k++)
  {
    if(kirbies[k].get_isAlive())
    {
      irr::scene::ISceneNode * kirby_node = smgr->getSceneNodeFromId(k + 5);
      irr::core::aabbox3d<float> kirbyBox = kirby_node->getTransformedBoundingBox();
      if(kirbyBox.intersectsWithBox(nodeBox))
      {
        if(nodeBox.getCenter().X < kirbyBox.MaxEdge.X && nodeBox.getCenter().X > kirbyBox.MinEdge.X)
        {
          // Si on est sous le Kirby et qu'on saute vers le haut -> Collision et mort
          if((jump->get_speed() >= 0.0f || isWalking) || !(nodeBox.getCenter().Y > kirbyBox.MaxEdge.Y - mesh_size/4) )
          {
      	    if(!player_state->get_godmode())
      	      player_state->game_over();
          }
          else // sinon c'est qu'on lui tombe dessus et qu'on le tue
          {
            jump->set_double_jumping(false);
            jump->set_jumping(false);
            jump->jump();
            player_state->add_score(300);
            player_state->add_a_dead_kirby();
            kirbies[k].kill();
          }
        }
        
      }
    }
    

  }

  //Collision avec les Pièces
  for(int k = 0; k < nb_coins; k++)
  {
    if(!coins[k].get_isBeingLooted())
    {
      irr::scene::ISceneNode * coin_node = smgr->getSceneNodeFromId(k + 5 + nb_kirbies);
      if(coin_node)
      {
        irr::core::aabbox3d<float> coinBox = coin_node->getTransformedBoundingBox();
        if(coinBox.intersectsWithBox(nodeBox))
        {
          coins[k].loot();
          player_state->add_score(50);
          player_state->loot_coin();
        }
      }
      
    }

  }

}
