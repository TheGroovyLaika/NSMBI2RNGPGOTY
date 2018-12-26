// player_state.h

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <vector>

#include <irrlicht.h>
 enum Game_state { start_screen, in_game, game_is_over,finishing_run, end_screen };

class Player_state
{

  irr::scene::IAnimatedMeshSceneNode *node;
  std::vector<irr::video::ITexture*> textures;
  int score;
  int looted_coin;
  int dead_kirbies;
  Game_state game_state;
  bool godMode;
  int timer;

public:
  Player_state();

  Game_state get_game_state();
  int get_score();
  int get_coins();
  int get_kirbies();
  bool get_godmode();
  int get_timer();

  void set_node(irr::scene::IAnimatedMeshSceneNode *n){node=n;}
  void set_textures(const std::vector<irr::video::ITexture *> &tex){textures=tex;}
  void set_game_state(Game_state g_s){game_state=g_s;}

  void compute_score();
  void add_score(int added_score);
  void loot_coin();
  void add_a_dead_kirby();
  void increase_timer();
  void godmode();
  void game_over();
  void next_level();
};

#endif
