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
  : node(nullptr), smgr(nullptr), jump(nullptr), kirbies(nullptr), coins(nullptr), mesh_size(0), nb_kirbies(0), nb_coins(0), isGameOver(false)
{
}

/**************************************************************************\
 * Collision::compute_collision                                                *
\**************************************************************************/
void Collision::compute_collision()
{
  mesh_size = node->getTransformedBoundingBox().MaxEdge.Y - node->getTransformedBoundingBox().MinEdge.Y;
  //Collision avec les plateformes
  ic::array< irr::scene::ISceneNode * > all_cubes;
  smgr->getSceneNodesFromType(irr::scene::ESNT_CUBE, all_cubes);
  irr::core::aabbox3d<float> nodeBox = node->getTransformedBoundingBox();

  // On regarde pour tous les cubes s'il y a collision avec le personnage grace aux bounding boxes
  for(int i = 0; i < int(all_cubes.size()); i++)
  {
    irr::core::aabbox3d<float> cubeBox = all_cubes[i]->getTransformedBoundingBox();

    if(cubeBox.intersectsWithBox(nodeBox))
    {
      if(jump->get_speed() < 0.0f)
      {
        if(nodeBox.getCenter().X < cubeBox.MaxEdge.X && nodeBox.getCenter().X > cubeBox.MinEdge.X && ( nodeBox.getCenter().Y > cubeBox.MaxEdge.Y + mesh_size/8))
        {
          if(player_state->get_character_state() == jumping)
          {
            node->setPosition(ic::vector3df(node->getPosition().X, cubeBox.MaxEdge.Y + mesh_size/2 - 2, node->getPosition().Z));
            player_state->set_character_state(standing);
            node->setMD2Animation(is::EMAT_STAND);

            jump->set_speed(-0.0000001);
            jump->reset_jumps();
          }
        }
        else
        {
          player_state->set_character_state(jumping);
        }
      }
    }

  }

  //Collision avec les Kirbies
  for(int k = 0; k < nb_kirbies; k++)
  {
    if((*kirbies)[k].get_isAlive())
    {
      irr::scene::ISceneNode * kirby_node = smgr->getSceneNodeFromId(k + 5);
      irr::core::aabbox3d<float> kirbyBox = kirby_node->getTransformedBoundingBox();
      if(kirbyBox.intersectsWithBox(nodeBox))
      {
        if(nodeBox.getCenter().X < kirbyBox.MaxEdge.X && nodeBox.getCenter().X > kirbyBox.MinEdge.X)
        {
          // Si on est sous le Kirby et qu'on saute vers le haut -> Collision et mort
          if((jump->get_speed() >= 0.0f) || !(nodeBox.getCenter().Y > kirbyBox.MaxEdge.Y - mesh_size/4) )
          {
      	    if(!player_state->get_godmode())
      	      player_state->game_over();
          }
          else // sinon c'est qu'on lui tombe dessus et qu'on le tue
          {
            // jump->set_double_jumping(false);
            // jump->set_jumping(false);
            jump->reset_jumps();
            jump->jump();
            player_state->add_score(300);
            player_state->add_a_dead_kirby();
            (*kirbies)[k].kill();
          }
        }
        
      }
    }
    

  }

  //Collision avec les Pièces
  for(int k = 0; k < nb_coins; k++)
  {
    if(!(*coins)[k].get_isBeingLooted())
    {
      irr::scene::ISceneNode * coin_node = smgr->getSceneNodeFromId(k + 5 + nb_kirbies);
      if(coin_node)
      {
        irr::core::aabbox3d<float> coinBox = coin_node->getTransformedBoundingBox();
        if(coinBox.intersectsWithBox(nodeBox))
        {
          (*coins)[k].loot();
          player_state->add_score(50);
          player_state->loot_coin();
        }
      }
      
    }

  }

}
