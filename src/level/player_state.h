// player_state.h

#ifndef PLAYER_STATE_H
#define PLAYER_STATE_H

#include <vector>

#include <irrlicht.h>

class Player_state
{

  irr::scene::IAnimatedMeshSceneNode *node;
  bool Alive;
  std::vector<irr::video::ITexture*> textures;
  int score;
  int looted_coin;
  int dead_kirbies;
  bool hasGameStartedYetPleaseTellMeThanks;
  bool finishing_run;
  bool godMode;
  int timer;

public:
  Player_state();
  bool get_started_game();
  void start_game();
  void set_node(irr::scene::IAnimatedMeshSceneNode *node);
  void set_textures(const std::vector<irr::video::ITexture *> &tex){textures=tex;}
  void compute_score();
  void add_score(int added_score);
  void loot_coin();
  void add_a_dead_kirby();
  void increase_timer();
  int get_score();
  int get_coins();
  int get_kirbies();
  bool get_alive();
  bool get_finishing_run();
  void godmode();
  bool get_godmode();
  int get_timer();
  void game_over();
  void finish_line();
};

#endif
