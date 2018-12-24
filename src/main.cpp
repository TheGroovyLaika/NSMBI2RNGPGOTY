// main.cpp

#include <irrlicht.h>

#include "actions/events.h"
#include "actions/jump.h"
#include "actions/collision.h"
#include "level/entities.h"
#include "level/player_state.h"
#include "level/level_creator.h"
#include "level/file_loader.h"

#include <iostream>
#include <time.h>

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

int main()
{
  bool hasGameSartedYet = false; //Booleen verifiant si la partie 

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

  // Chargement du niveau
  level_creator.set_smgr(smgr);                    
  level_creator.set_driver(driver);                    
  level_creator.set_coins(&coins);                    
  level_creator.set_kirbies(&kirbies);                    
  level_creator.load_level(position_nb, position_data);
  level_creator.load_background();

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


  // Initialisation de la classe player_state
  player_state.set_node(node);
  player_state.set_textures(textures);

  // Chargement des textures pour les chiffres
  iv::ITexture *digits[10];
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

  // Création des places pour les chiffres
  ig::IGUIImage *score_1000  = gui->addImage(ic::rect<s32>(50,10,  81,47)); score_1000->setScaleImage(true);
  ig::IGUIImage *score_100   = gui->addImage(ic::rect<s32>(81,10,  112,47)); score_100->setScaleImage(true);
  ig::IGUIImage *score_10    = gui->addImage(ic::rect<s32>(112,10, 143,47)); score_10->setScaleImage(true);
  ig::IGUIImage *score_1     = gui->addImage(ic::rect<s32>(143,10, 174,47)); score_1->setScaleImage(true);

  ig::IGUIImage *start_game   = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
  start_game->setScaleImage(true);
  start_game->setImage(driver->getTexture("data/start_game.png"));
  
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


  int score = 0;


  while(device->run())
  {
    // Si la partie commence, on supprime l'image de debut de partie
    if(player_state.get_started_game() && !hasGameSartedYet)
    {
      start_game->remove();
      hasGameSartedYet = true;
    }

    // Quand le joueur depasse la ligne d'arrivee, on enclenche le process de fin de partie..
    if(camera->getPosition().X >= 8080)
      player_state.finish_line();

    // Tant que le joueur est en vie et que le process de fin de partie n'est pas en cours, on gere camera, collisions, evenements, sauts, et gestion du score
    if(player_state.get_alive() && !player_state.get_finishing_run())
    {
      // Reglage caméra afin qu'elle suive le joueur en X et en Y
      camera_position = camera->getPosition();
      camera_target   = camera->getTarget();
      position        = node->getPosition();

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

      // Gestion de la position des kirbies et des pieces (qui flottent en l'air)
      for(int k = 0; k < nb_kirbies; k++)
      {
        kirbies[k].update_position();
      }

      for(int k = 0; k < nb_coins; k++)
      {
        coins[k].update_position();
      }

      // Calcul du saut :
      jump.update_jump();

      // Contrôle clavier :
      receiver.compute_keyboard();

      // Calcul de la collision :
      collision.compute_collision(); 

      //Calcul du score :
      player_state.compute_score();

      if(node->getPosition().X < -30)
        node->setPosition(ic::vector3df(-30, node->getPosition().Y, node->getPosition().Z));
    }  

    // Si le joueur est mort
    else if(!player_state.get_alive())
    {
      // On attend une petite duree (animation de mort)
      if(player_state.get_timer() < 60)
      {
        player_state.increase_timer();
      }
      else //Puis on affiche l'ecran de fin de partie
      {
        //Image de Game Over
        ig::IGUIImage *game_over   = gui->addImage(ic::rect<s32>(0,0, 640,480)); 
        game_over->setScaleImage(true);
        game_over->setImage(driver->getTexture("data/gameover.png"));

        // Score final
        ig::IGUIImage *final_score_1000  = gui->addImage(ic::rect<s32>(258,390,  289,427)); final_score_1000->setScaleImage(true);
        ig::IGUIImage *final_score_100   = gui->addImage(ic::rect<s32>(289,390,  320,427)); final_score_100->setScaleImage(true);
        ig::IGUIImage *final_score_10    = gui->addImage(ic::rect<s32>(320,390, 351,427)); final_score_10->setScaleImage(true);
        ig::IGUIImage *final_score_1     = gui->addImage(ic::rect<s32>(351,390, 383 ,427)); final_score_1->setScaleImage(true);

        score = player_state.get_score();

        // Mise à jour du score final :
        final_score_1000->setImage(digits[(score / 1000) % 10]);
        final_score_100->setImage(digits[(score / 100) % 10]);
        final_score_10->setImage(digits[(score / 10) % 10]);
        final_score_1->setImage(digits[(score / 1) % 10]);

        // Nombre de pieces collectees
        ig::IGUIImage *nb_coins_100   = gui->addImage(ic::rect<s32>(70,115,  101,152)); nb_coins_100->setScaleImage(true);
        ig::IGUIImage *nb_coins_10    = gui->addImage(ic::rect<s32>(101,115, 132,152)); nb_coins_10->setScaleImage(true);
        ig::IGUIImage *nb_coins_1     = gui->addImage(ic::rect<s32>(132,115, 163 ,152)); nb_coins_1->setScaleImage(true);

        int collected_coins = player_state.get_coins();

        nb_coins_100->setImage(digits[(collected_coins / 100) % 10]);
        nb_coins_10->setImage(digits[(collected_coins / 10) % 10]);
        nb_coins_1->setImage(digits[(collected_coins / 1) % 10]);

        // Nombre de Kirbies tues
        ig::IGUIImage *nb_kirbies_10    = gui->addImage(ic::rect<s32>(470,115, 501,152)); nb_kirbies_10->setScaleImage(true);
        ig::IGUIImage *nb_kirbies_1     = gui->addImage(ic::rect<s32>(501,115, 532 ,152)); nb_kirbies_1->setScaleImage(true);

        int killed_kirbies = player_state.get_kirbies();

        nb_kirbies_10->setImage(digits[(killed_kirbies / 10) % 10]);
        nb_kirbies_1->setImage(digits[(killed_kirbies / 1) % 10]);
      }
    }

    //Gestion du process de fin de partie
    if(player_state.get_finishing_run())
    {
      position = node->getPosition();
      node->setPosition(ic::vector3df(position.X + 5, position.Y, position.Z));
      if(position.X > 8300)
        player_state.game_over();
    }

    // Récupération du score :
    score = player_state.get_score();
    // Mise à jour du score :
    score_1000->setImage(digits[(score / 1000) % 10]);
    score_100->setImage(digits[(score / 100) % 10]);
    score_10->setImage(digits[(score / 10) % 10]);
    score_1->setImage(digits[(score / 1) % 10]);

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