// gui_display.cpp

#include <irrlicht.h>

#include "gui_display.h"

#include <iostream>
using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;


/**************************************************************************\
 * Gui_display::Gui_display                                           *
\**************************************************************************/
Gui_display::Gui_display()
  : smgr(nullptr), gui(nullptr), driver(nullptr), player_state(nullptr), token(s_s), start_game_screen(nullptr), game_over_screen(nullptr)
{
}

/**************************************************************************\
 * Gui_display::init                                           *
\**************************************************************************/
void Gui_display::init()
{
	score_gui.push_back(gui->addImage(ic::rect<s32>(50,10,  81,47)));
	score_gui.back()->setScaleImage(true);
	score_gui.push_back(gui->addImage(ic::rect<s32>(81,10,  112,47)));
	score_gui.back()->setScaleImage(true);
	score_gui.push_back(gui->addImage(ic::rect<s32>(112,10, 143,47)));
	score_gui.back()->setScaleImage(true);
	score_gui.push_back(gui->addImage(ic::rect<s32>(143,10, 174,47)));
	score_gui.back()->setScaleImage(true);

	digits[0] = driver->getTexture("data/0_score.png");
	digits[1] = driver->getTexture("data/1_score.png");
	digits[2] = driver->getTexture("data/2_score.png");
	digits[3] = driver->getTexture("data/3_score.png");
	digits[4] = driver->getTexture("data/4_score.png");
	digits[5] = driver->getTexture("data/5_score.png");
	digits[6] = driver->getTexture("data/6_score.png");
	digits[7] = driver->getTexture("data/7_score.png");
	digits[8] = driver->getTexture("data/8_score.png");
	digits[9] = driver->getTexture("data/9_score.png");

  	start_game_screen = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
  	start_game_screen->setScaleImage(true);
	start_game_screen->setImage(driver->getTexture("data/start_game.png"));

	pause_gui = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
  	pause_gui->setScaleImage(true);
	pause_gui->setImage(driver->getTexture("data/unpause.png"));
}

/**************************************************************************\
 * Gui_display::compute_gui                                           *
\**************************************************************************/
void Gui_display::compute_gui()
{
	switch(player_state->get_game_state())
	{
		case start_screen:
		break;
		case in_game:
		{
			if(token == p)
			{
				pause_gui->setImage(driver->getTexture("data/unpause.png"));
				token = i_g;
			}
			if(token == s_s)
			{
				start_game_screen->remove();
				token = i_g;
			}
			// Récupération du score :
		    int score = player_state->get_score();
		    // Mise à jour du score :
		    score_gui[0]->setImage(digits[(score / 1000) % 10]);
		    score_gui[1]->setImage(digits[(score / 100) % 10]);
		    score_gui[2]->setImage(digits[(score / 10) % 10]);
		    score_gui[3]->setImage(digits[(score / 1) % 10]);
		}
		break;
		case paused:
		{
			if(token == i_g)
			{
				token = p;
				pause_gui->setImage(driver->getTexture("data/pause.png"));
			}

		}
		break;
		case game_is_over:
		{
			if(token != g_i_o)
			{
				game_over_screen = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
				game_over_screen->setScaleImage(true);

				final_score_gui.push_back(gui->addImage(ic::rect<s32>(258,390,  289,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(289,390,  320,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(320,390, 351,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(351,390, 383 ,427)));
				final_score_gui.back()->setScaleImage(true);
	
				nb_coins.push_back(gui->addImage(ic::rect<s32>(70,115,  101,152)));
				nb_coins.back()->setScaleImage(true);
				nb_coins.push_back(gui->addImage(ic::rect<s32>(101,115, 132,152)));
				nb_coins.back()->setScaleImage(true);
				nb_coins.push_back(gui->addImage(ic::rect<s32>(132,115, 163 ,152)));
				nb_coins.back()->setScaleImage(true);
	
				nb_kirbies.push_back(gui->addImage(ic::rect<s32>(470,115, 501,152)));
				nb_kirbies.back()->setScaleImage(true);
				nb_kirbies.push_back(gui->addImage(ic::rect<s32>(501,115, 532 ,152)));
				nb_kirbies.back()->setScaleImage(true);
				game_over_screen->setImage(driver->getTexture("data/gameover.png"));
				int score = player_state->get_score();
		
				final_score_gui[0]->setImage(digits[(score / 1000) % 10]);
				final_score_gui[1]->setImage(digits[(score / 100) % 10]);
				final_score_gui[2]->setImage(digits[(score / 10) % 10]);
				final_score_gui[3]->setImage(digits[(score / 1) % 10]);
		
				int collected_coins = player_state->get_coins();
		
				nb_coins[0]->setImage(digits[(collected_coins / 100) % 10]);
				nb_coins[1]->setImage(digits[(collected_coins / 10) % 10]);
				nb_coins[2]->setImage(digits[(collected_coins / 1) % 10]);
		
				int killed_kirbies = player_state->get_kirbies();
		
				nb_kirbies[0]->setImage(digits[(killed_kirbies / 10) % 10]);
				nb_kirbies[1]->setImage(digits[(killed_kirbies / 1) % 10]);

				token = g_i_o;
			}
		}
		break;
		case end_screen:
		{
			if(token != e_s)
			{
				end_level_screen = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
				end_level_screen->setScaleImage(true);
				
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(258,390,  289,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(289,390,  320,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(320,390, 351,427)));
				final_score_gui.back()->setScaleImage(true);
				final_score_gui.push_back(gui->addImage(ic::rect<s32>(351,390, 383 ,427)));
				final_score_gui.back()->setScaleImage(true);
	
				nb_coins.push_back(gui->addImage(ic::rect<s32>(70,115,  101,152)));
				nb_coins.back()->setScaleImage(true);
				nb_coins.push_back(gui->addImage(ic::rect<s32>(101,115, 132,152)));
				nb_coins.back()->setScaleImage(true);
				nb_coins.push_back(gui->addImage(ic::rect<s32>(132,115, 163 ,152)));
				nb_coins.back()->setScaleImage(true);
	
				nb_kirbies.push_back(gui->addImage(ic::rect<s32>(470,115, 501,152)));
				nb_kirbies.back()->setScaleImage(true);
				nb_kirbies.push_back(gui->addImage(ic::rect<s32>(501,115, 532 ,152)));
				nb_kirbies.back()->setScaleImage(true);
				end_level_screen->setImage(driver->getTexture("data/endlevel.png"));
				int score = player_state->get_score();
		
				final_score_gui[0]->setImage(digits[(score / 1000) % 10]);
				final_score_gui[1]->setImage(digits[(score / 100) % 10]);
				final_score_gui[2]->setImage(digits[(score / 10) % 10]);
				final_score_gui[3]->setImage(digits[(score / 1) % 10]);
		
				int collected_coins = player_state->get_coins();
		
				nb_coins[0]->setImage(digits[(collected_coins / 100) % 10]);
				nb_coins[1]->setImage(digits[(collected_coins / 10) % 10]);
				nb_coins[2]->setImage(digits[(collected_coins / 1) % 10]);
		
				int killed_kirbies = player_state->get_kirbies();
		
				nb_kirbies[0]->setImage(digits[(killed_kirbies / 10) % 10]);
				nb_kirbies[1]->setImage(digits[(killed_kirbies / 1) % 10]);
				token = e_s;
			}
		}
		break;
		default:
		break;	
	}

}

