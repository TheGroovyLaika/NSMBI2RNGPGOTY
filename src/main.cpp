// main.cpp

#include <irrlicht.h>

#include "actions/events.h"
#include "actions/jump.h"
#include "actions/collision.h"
#include "level/entities.h"
#include "level/player_state.h"
#include "level/level_creator.h"
#include "level/file_loader.h"
#include "level/gui_display.h"

#include <iostream>
#include <time.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

int main()
{
  // Le gestionnaire d'événements
  EventReceiver receiver = EventReceiver();

  //classe gérant les sauts
  Jump jump = Jump();

  //classe gérant les Collision
  Collision collision = Collision();

  //classe gérant le gameover
  Player_state player_state = Player_state();

  //Level Creator
  Level_Creator level_creator = Level_Creator();

  ///GUI Display
  Gui_display gui_display = Gui_display();

  std::vector<iv::ITexture*> textures;

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver    *driver = device->getVideoDriver();
  is::ISceneManager   *smgr = device->getSceneManager();
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();

  File_loader file_loader = File_loader();
  char const *filename = "./data/level1.txt";
  file_loader.position_loader(filename);

  int position_nb[5] = {*file_loader.getPosition_nb(),
                        *(file_loader.getPosition_nb() + 1),
                        *(file_loader.getPosition_nb() + 2),
                        *(file_loader.getPosition_nb() + 3),
                        *(file_loader.getPosition_nb() + 4)};
  std::vector<ic::vector3df> position_data = file_loader.getPosition_data();

  int nb_coins = position_nb[0] + position_nb[1] * 2 + position_nb[2] * 4 + position_nb[3] * 15;
  int nb_kirbies = position_nb[4];

  std::vector<Coins> coins;
  std::vector<Kirbies> kirbies;
  Player player = Player();
  
  // Chargement de notre personnage
  ic::vector3df position  = ic::vector3df(-30, 0, 0);

  // Attachement de notre personnage dans la scène
  textures.push_back(driver->getTexture("data/blue_texture.pcx"));
  textures.push_back(driver->getTexture("data/ouch.png"));
  textures.push_back(driver->getTexture("data/godmode.png"));
  player.set_smgr(smgr);
  is::IAnimatedMeshSceneNode *node = player.init(position);
  node->setMaterialTexture(0, textures[0]);

  // Initialisation de la camera
  ic::vector3df camera_position = ic::vector3df(30, 50, -200);
  ic::vector3df camera_target = ic::vector3df(0, 50, 0);
  is::ICameraSceneNode *camera = smgr->addCameraSceneNode(nullptr, camera_position, camera_target);


  // Initialisation de la classe receiver
  receiver.set_node(node);
  receiver.set_textures(textures);
  receiver.set_jump(&jump);
  receiver.set_collision(&collision);
  receiver.set_camera(camera);
  receiver.set_player_state(&player_state);
  
  // Initialisation de la classe jump
  jump.set_node(node);
  jump.set_camera(camera);
  
  // Initialisation de la classe collision
  collision.set_node(node);
  collision.set_smgr(smgr);
  collision.set_jump(&jump);
  collision.set_kirbies_number(nb_kirbies);
  collision.set_kirbies(&kirbies);
  collision.set_coins_number(nb_coins);
  collision.set_coins(&coins);
  collision.set_player_state(&player_state);

  // Chargement du niveau
  level_creator.set_smgr(smgr);
  level_creator.set_driver(driver);                    
  level_creator.set_coins(&coins);                    
  level_creator.set_kirbies(&kirbies);                    
  level_creator.load_level(position_nb, position_data);
  level_creator.load_background();

  //Chargement du GUI
  gui_display.set_smgr(smgr);
  gui_display.set_gui(gui);
  gui_display.set_driver(driver);
  gui_display.set_player_state(&player_state);
  gui_display.init();

  // Initialisation de la classe player_state
  player_state.set_node(node);
  player_state.set_textures(textures);

  
  //Generation du sol
  irr::core::matrix4 mat;
  mat.setTextureScale(50.0F,150.0F);

  irr::scene::ISceneNode* ground = smgr->addCubeSceneNode(30);
  ground->setScale(ic::vector3df(320.0f, 2.0f,50.0f));
  ground->setMaterialFlag(irr::video::EMF_LIGHTING, true);
  ground->setMaterialTexture( 0, driver->getTexture("data/grass.jpg") );
  ground->getMaterial(0).setTextureMatrix(0,mat);
  ground->setPosition(ic::vector3df(4000.0f, -65.0f, 20.0f));
  ground->getMaterial(0).Shininess = 20.0f;
  //lightnode = is::ILightSceneNode(nullptr, smgr, 0, ic::vector3df(0, 0, 0));
  irr::scene:: ILightSceneNode* light = smgr->addLightSceneNode(nullptr,ic::vector3df(),iv::SColorf(1.0f, 1.0f, 0.6f, 1.0f),1000.0f,0);
  light->setPosition(ic::vector3df(0, 60, -100));
  light->getLightData().DiffuseColor = iv::SColorf(1.0f, 1.0f, 0.6f, 0.0);

  // set ambient light
  smgr->setAmbientLight(iv::SColor(0,255,255,255));

  light->getLightData().SpecularColor = iv::SColorf(1.0f, 1.0f, 0.0, 0.0);

  while(device->run())
  {
    switch(player_state.get_game_state())
    {
      case in_game:
      // Gestion de la position des kirbies et des pieces (qui flottent en l'air)
      for(unsigned int k = 0; k < kirbies.size(); k++)
      {
        kirbies[k].update_position();
      }

      for(unsigned int k = 0; k < coins.size(); k++)
      {
        coins[k].update_position();
      }

      // Calcul du saut :
      jump.update_jump();

      // Contrôle clavier :
      receiver.compute_keyboard();

      // MaJ de la camera :
      receiver.compute_camera();

      // Calcul de la collision :
      collision.compute_collision(); 

      //Calcul du score :
      player_state.compute_score();

      if(node->getPosition().X < -30)
        node->setPosition(ic::vector3df(-30, node->getPosition().Y, node->getPosition().Z));
      break;
      case game_is_over:
      // On attend une petite duree (animation de mort)
      if(player_state.get_timer() < 60)
      {
        player_state.increase_timer();
      }
      else //Puis on affiche l'ecran de fin de partie
      {
        if(level_creator.get_level_state())
          level_creator.remove_level();
       
      }
      break;
      case finishing_run:
      position = node->getPosition();
      node->setPosition(ic::vector3df(position.X + 5, position.Y, position.Z));
      if(position.X > 8300)
        player_state.game_over();
      break;
      default:
      break;
    }

    gui_display.compute_gui();
    driver->beginScene(true, true, iv::SColor(0,50,100,255));

    // Dessin de la scène :
    smgr->drawAll();

    // Dessin de la GUI :
    gui->drawAll();
    driver->endScene();

  }
  device->drop();

  return 0;
}