// events.cpp

#include <irrlicht.h>

#include "events.h"
#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * EventReceiver::EventReceiver                                           *
\**************************************************************************/
EventReceiver::EventReceiver()
  : node(nullptr), camera(nullptr), textures(), key_code(KEY_PLAY), jump(nullptr), collision(nullptr), button_pressed(false), lateral_speed(0), current_texture(0), isWalking(false), key_pressed_down(false), player_state(nullptr)
{
}

/*------------------------------------------------------------------------*\
 * EventReceiver::keyboard                                                *
\*------------------------------------------------------------------------*/
bool EventReceiver::keyboard(const SEvent &event)
{
    key_pressed_down = event.KeyInput.PressedDown;
    if (event.KeyInput.PressedDown)
    {
      
      if(!player_state->get_started_game())
	player_state->start_game();
      key_code = event.KeyInput.Key;
      ic::vector3df rotation = node->getRotation();
      switch (event.KeyInput.Key)
      {
        case KEY_ESCAPE: //Quitter le jeu
          exit(0);
	  break;
        case KEY_KEY_Z: // Saute
          if (player_state->get_alive())
            jump->jump();
	  break;
      	case KEY_KEY_M: //Godmode
      	  player_state->godmode();
          break;

        default:;
      }
      node->setRotation(rotation);
    }

    // Gestion de l'animation "debout" lorsque le joueur n'appuie pas sur une touche
    if(event.EventType == EET_KEY_INPUT_EVENT &&
          !event.KeyInput.PressedDown && player_state->get_alive())
    {        
        isWalking = false;
        collision->set_walking(false);
        if(!jump->get_jumping())
        {
          jump->set_lateral_speed(0);
          node->setMD2Animation(is::EMAT_STAND);
        }
    }
  return false;
  
}
/*------------------------------------------------------------------------*\
 * EventReceiver::mouse                                                   *
\*------------------------------------------------------------------------*/
bool EventReceiver::mouse(const SEvent &event)
{
   return false;
}

/**************************************************************************\
 * EventReceiver::OnEvent                                                 *
\**************************************************************************/
bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!node) return false;
  switch (event.EventType)
  {
    case EET_KEY_INPUT_EVENT:
      return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
      return mouse(event);
    default:;
  }

  return false;
}


/**************************************************************************\
 * EventReceiver::set_node                                                *
\**************************************************************************/
void EventReceiver::set_node(irr::scene::IAnimatedMeshSceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * EventReceiver::set_camera                                                *
\**************************************************************************/
void EventReceiver::set_camera(is::ICameraSceneNode *c)
{
  camera = c;
}

/**************************************************************************\
 * EventReceiver::set_jump                                                *
\**************************************************************************/
void EventReceiver::set_jump(Jump *j)
{
  jump = j;
}

/**************************************************************************\
 * EventReceiver::set_collision                                                *
\**************************************************************************/
void EventReceiver::set_collision(Collision *c)
{
  collision = c;
}


/**************************************************************************\
 * EventReceiver::set_player_state                                                *
\**************************************************************************/
void EventReceiver::set_player_state(Player_state *ps)
{
  player_state = ps;
}


/**************************************************************************\
 * EventReceiver::compute_keyboard                                                *
\**************************************************************************/
void EventReceiver::compute_keyboard()
{    

  //Gestion des vitesses laterales (gauche et droite) en fonction de la touche sur laquelle on a appuye plus tot
  if (key_pressed_down)
  {
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();
    switch (key_code)
      {
        case KEY_KEY_D: // Tourne à droite
          if(player_state->get_alive())
          {
            if(lateral_speed < 0  )
              lateral_speed = 0; 

            if(lateral_speed < 5 )
              lateral_speed += 0.2; //augmentation progressive de la vitesse

            if(!jump->get_jumping() || jump->get_collision())
              position.X += lateral_speed;

            jump->set_lateral_speed(lateral_speed);

            if(!isWalking){
              if(!jump->get_jumping())
              {
                node->setMD2Animation(is::EMAT_RUN); // Animation de course si le personnage ne saute pas ou n'est pas deja en course
              }
              if(jump->get_speed()>-0.2f)
              {
                // Si le personnage ne tombe pas, c'est qu'il est sur le plateforme
                collision->set_walking(true);
                isWalking = true;
              }

            }

            rotation.Y = 0;
            node->setPosition(position);
          }
          break;

        case KEY_KEY_Q: // Tourne à gauche
          if(player_state->get_alive())
          {
            if(lateral_speed > 0  )
              lateral_speed = 0;

            if(lateral_speed > -5  )
              lateral_speed -= 0.2;

            if(!jump->get_jumping() || jump->get_collision())
                position.X += lateral_speed;
            jump->set_lateral_speed(lateral_speed);

            if(!isWalking){
              if(!jump->get_jumping())
                    node->setMD2Animation(is::EMAT_RUN);
              if(jump->get_speed()>-0.2f)
              {
                collision->set_walking(true);
                isWalking = true;
              }
            }

            rotation.Y = 180 ;
            node->setPosition(position);
          }
          break;

        default:;
      }

    node->setRotation(rotation);

  }
  
}
