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
  : node(nullptr), camera(nullptr), textures(), key_code(KEY_PLAY), jump(nullptr), lateral_speed(0), current_texture(0), key_pressed_down(false), player_state(nullptr)
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
      
      if(player_state->get_game_state() == start_screen)
	    player_state->set_game_state(in_game);
      key_code = event.KeyInput.Key;
      ic::vector3df rotation = node->getRotation();
      switch (event.KeyInput.Key)
      {
        case KEY_ESCAPE: //Quitter le jeu
          exit(0);
        break;

        case KEY_KEY_Z: // Saute
          if (player_state->get_game_state() == in_game)
            jump->jump();
        break;

      	case KEY_KEY_M: //Godmode
    	  player_state->godmode();
        break;

        case KEY_KEY_P:
        if(player_state->get_game_state() == in_game)
          player_state->set_game_state(paused);
        else if(player_state->get_game_state()==paused)
          player_state->set_game_state(in_game);
        break;

        default:;
      }
      node->setRotation(rotation);
    }

    // Gestion de l'animation "debout" lorsque le joueur n'appuie pas sur une touche
    if(event.EventType == EET_KEY_INPUT_EVENT &&
          !event.KeyInput.PressedDown && player_state->get_game_state() == in_game)
    {  
          if(player_state->get_character_state() != jumping)
        {
          jump->set_lateral_speed(0);
          player_state->set_character_state(standing);
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
          if(player_state->get_game_state() == in_game)
          {
            if(lateral_speed < 0  )
              lateral_speed = 0; 

            if(lateral_speed < 5 )
              lateral_speed += 0.2; //augmentation progressive de la vitesse

            if(player_state->get_character_state() != jumping)
              position.X += lateral_speed;

            jump->set_lateral_speed(lateral_speed);

            if(player_state->get_character_state() == standing)
            {
              player_state->set_character_state(walking);
              node->setMD2Animation(is::EMAT_RUN); // Animation de course si le personnage ne saute pas ou n'est pas deja en course
            }
            rotation.Y = 0;
            node->setPosition(position);
          }
          break;

        case KEY_KEY_Q: // Tourne à gauche
          if(player_state->get_game_state() == in_game)
          {
            if(lateral_speed > 0  )
              lateral_speed = 0;

            if(lateral_speed > -5  )
              lateral_speed -= 0.2;

            if(player_state->get_character_state() != jumping)
                position.X += lateral_speed;
            jump->set_lateral_speed(lateral_speed);

            if(player_state->get_character_state() == standing)
            {
              player_state->set_character_state(walking);
              node->setMD2Animation(is::EMAT_RUN); // Animation de course si le personnage ne saute pas ou n'est pas deja en course
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

/**************************************************************************\
 * EventReceiver::compute_keyboard                                         *
\**************************************************************************/
void EventReceiver::compute_camera()
{
  // Quand le joueur depasse la ligne d'arrivee, on enclenche le process de fin de partie..
  if(camera->getPosition().X >= 8080)
    player_state->set_game_state(finishing_run);

  // Reglage caméra afin qu'elle suive le joueur en X et en Y
  ic::vector3df camera_position = camera->getPosition();
  ic::vector3df camera_target   = camera->getTarget();
  ic::vector3df position        = node->getPosition();

  if(camera_position.X - position.X > 165 && camera_position.X < 8080.0f)
  {
    camera_position.X = position.X + 165;
    camera_target.X = position.X + 165;
  }
  else if(position.X > camera_position.X && camera_position.X < 8080.0f)
  {
    camera_position.X = position.X;
    camera_target.X   = position.X;  
  }

  if(node->getPosition().Y - camera_position.Y  > 70)
  {
    camera_position.Y = position.Y - 70;
    camera_target.Y = position.Y - 70;
  }

  if(camera->getPosition().Y > 50)
  {
    camera_position.Y = position.Y - 70;
    camera_target.Y = position.Y - 70;
  }

  //Effet ed style de camera en debut de partie
  if(position.X < 250)
  {
    camera_position.Y = position.Y + 350 - position.X;
  }

  if(position.X >= 250 && position.X < 260)
  {
    camera_position.Y = position.Y + 50;
    camera_target.Y = position.Y + 50;
  }

  camera->setPosition(camera_position);
  camera->setTarget(camera_target);
}
