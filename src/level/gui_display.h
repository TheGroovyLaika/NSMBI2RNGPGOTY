// gui_display.h

#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include <vector>

#include <irrlicht.h>
#include "player_state.h"
enum Token { s_s, p, i_g, g_i_o, f_r, e_s };

class Gui_display
{
 	irr::scene::ISceneManager *smgr; 
 	irr::gui::IGUIEnvironment *gui;
 	irr::video::IVideoDriver  *driver;
 	Player_state *player_state;
 	Token token;

 	std::vector<irr::gui::IGUIImage*> score_gui;
 	std::vector<irr::gui::IGUIImage*> final_score_gui;
 	std::vector<irr::gui::IGUIImage*> nb_coins;
 	std::vector<irr::gui::IGUIImage*> nb_kirbies;
 	irr::video::ITexture *digits[10];
	irr::gui::IGUIImage *start_game_screen;
	irr::gui::IGUIImage *game_over_screen;
	irr::gui::IGUIImage *end_level_screen;
	irr::gui::IGUIImage *pause_gui;

public:
	Gui_display();
	void set_smgr(irr::scene::ISceneManager *s){smgr = s;}
	void set_gui(irr::gui::IGUIEnvironment *g){gui = g;}
	void set_driver(irr::video::IVideoDriver *d){driver = d;}
	void set_player_state(Player_state *p_s){player_state = p_s;}
	void init();
  	void compute_gui();
};

#endif
