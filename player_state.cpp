// player_state.cpp

#include <irrlicht.h>

#include "player_state.h"

#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * Player_state::Player_state                                           *
\**************************************************************************/
Player_state::Player_state()
  : node(nullptr), Alive(true), textures(), score(1000), looted_coin(0), dead_kirbies(0), hasGameStartedYetPleaseTellMeThanks(false), finishing_run(false), godMode(false), timer(0)
  {
}

/**************************************************************************\
 * Player_state::set_node                                           *
\**************************************************************************/
void Player_state::set_node(is::IAnimatedMeshSceneNode *n)
{
  node = n;
}

/**************************************************************************\
 * Player_state::get_started_game                                           *
\**************************************************************************/
bool Player_state::get_started_game()
{
  	return hasGameStartedYetPleaseTellMeThanks;
}

/**************************************************************************\
 * Player_state::start_game                                           *
\**************************************************************************/
void Player_state::start_game()
{
	// Debut de la partie
  	hasGameStartedYetPleaseTellMeThanks = true;
}

/**************************************************************************\
 * Player_state::compute_score                                           *
\**************************************************************************/
void Player_state::compute_score()
{
	//Gestion de la mort quand le score tombe a 0
	if(hasGameStartedYetPleaseTellMeThanks && Alive)
	{
		score--;
		if(score <= 0)
			game_over();
	}
}


/**************************************************************************\
 * Player_state::add_score                                           *
\**************************************************************************/
void Player_state::add_score(int added_score)
{
	score = std::min(score + added_score, 9999);
}

/**************************************************************************\
 * Player_state::loot_coin                                           *
\**************************************************************************/
void Player_state::loot_coin()
{
	looted_coin ++;
}

/**************************************************************************\
 * Player_state::add_a_dead_kirby                                           *
\**************************************************************************/
void Player_state::add_a_dead_kirby()
{
	dead_kirbies ++;
}

/**************************************************************************\
 * Player_state::increase_timer                                           *
\**************************************************************************/
void Player_state::increase_timer()
{
	timer ++;
}


/**************************************************************************\
 * Player_state::get_score                                           *
\**************************************************************************/
int Player_state::get_score()
{
  return score;
}


/**************************************************************************\
 * Player_state::get_coins                                           *
\**************************************************************************/
int Player_state::get_coins()
{
  return looted_coin;
}


/**************************************************************************\
 * Player_state::get_kirbies                                           *
\**************************************************************************/
int Player_state::get_kirbies()
{
  return dead_kirbies;
}

/**************************************************************************\
 * Player_state::get_alive                                           *
\**************************************************************************/
bool Player_state::get_alive()
{
  return Alive;
}

/**************************************************************************\
 * Player_state::get_finishing_run                                           *
\**************************************************************************/
bool Player_state::get_finishing_run()
{
  return finishing_run;
}

/**************************************************************************\
 * Player_state::godmode                                           *
\**************************************************************************/
void Player_state::godmode()
{
	//Changement de texture (dore ou normal)
  godMode = !godMode;
  if(godMode)
    node->setMaterialTexture(0, textures[2]);
  else
    node->setMaterialTexture(0, textures[0]);
}

/**************************************************************************\
 * Player_state::get_godmode                                           *
\**************************************************************************/
bool Player_state::get_godmode()
{
  return godMode;
}


/**************************************************************************\
 * Player_state::get_timer                                           *
\**************************************************************************/
int Player_state::get_timer()
{
  return timer;
}

/**************************************************************************\
 * Player_state::game_over                                           *
\**************************************************************************/
void Player_state::game_over()
{
	//Le personnage meurt
	if(Alive)
	{
		Alive = false;
		node->setMD2Animation(is::EMAT_DEATH_FALLBACKSLOW);
  		node->setMaterialTexture(0, textures[1]);
	}
}

/**************************************************************************\
 * Player_state::finish_line                                           *
\**************************************************************************/
void Player_state::finish_line()
{
	//Le personnage va continuer tout droit (dans le main)
	if(!finishing_run)
	{
		finishing_run = true;
	}
}