// main.cpp

#include <irrlicht.h>

#include "events.h"
#include "jump.h"
#include "collision.h"
#include "entities.h"
#include "player_state.h"

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


  std::vector<iv::ITexture*> textures;

  // Création de la fenêtre et du système de rendu.
  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(640, 480),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver    *driver = device->getVideoDriver();
  is::ISceneManager   *smgr = device->getSceneManager();
  ig::IGUIEnvironment *gui  = device->getGUIEnvironment();



  // tableau contenant 35 kirbies de la classe Kirbies
  int nb_kirbies = 35;
  Kirbies kirbies[nb_kirbies];

  //tableau de position des kirbies
  std::vector<ic::vector3df> kirbies_positions = {ic::vector3df(2100, 0, 0),
                                                  ic::vector3df(2300, 0, 0),
                                                  ic::vector3df(2500, 45, 0),
                                                  ic::vector3df(2700, 0, 0),
                                                  ic::vector3df(2900, 45, 0),
                                                  ic::vector3df(3100, 0, 0),
                                                  ic::vector3df(200, 0, 0),
                                                  ic::vector3df(500, 0, 0),
                                                  ic::vector3df(800, 0, 0),
                                                  ic::vector3df(1150, 0, 0),
                                                  ic::vector3df(1930, 50, 0),
                                                  ic::vector3df(3300, 50, 0),
                                                  ic::vector3df(3500, 45, 0),
                                                  ic::vector3df(4600, 230, 0),
                                                  ic::vector3df(4800, 0, 0),
                                                  ic::vector3df(5000, 45, 0),
                                                  ic::vector3df(5200, 0, 0),
                                                  ic::vector3df(5370, 45, 0),
                                                  ic::vector3df(5550, 260, 0),
                                                  ic::vector3df(6300, 450, 0),
                                                  ic::vector3df(6500, 750, 0),
                                                  ic::vector3df(5630, 570, 0),
                                                  ic::vector3df(6950, 80, 0),
                                                  ic::vector3df(7200, 20, 0),
                                                  ic::vector3df(7500, 20, 0)
              };

  //Generation d'une file indienne de kirbies
  int pos_deb = 3780; 
  int pos_fin = 4650;
  while(pos_deb < pos_fin)
  {
     kirbies_positions.push_back(ic::vector3df(pos_deb, 0, 0));
     pos_deb += 80;
    
  };
  
  // Chargement des kirbies
  for(int k = 0; k < nb_kirbies; k++)
  {
    kirbies[k] = Kirbies();
    kirbies[k].set_smgr(smgr);
    kirbies[k].init(kirbies_positions[k], k + 5);
  }

  // Classe gérant les pièces
  int nb_coins = 133;
  Coins coins[nb_coins];
  int coins_id = 0;

  // Chargement de notre personnage
  ic::vector3df position  = ic::vector3df(-30, 0, 0);
  is::IAnimatedMesh *mesh = smgr->getMesh("data/tris.md2");

  // Attachement de notre personnage dans la scène
  is::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(mesh);

  node->setID(1);
  node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  node->setMD2Animation(irr::scene::EMAT_STAND);
  textures.push_back(driver->getTexture("data/blue_texture.pcx"));
  textures.push_back(driver->getTexture("data/ouch.png"));
  textures.push_back(driver->getTexture("data/godmode.png"));
  node->setMaterialTexture(0, textures[0]);
  node->setPosition(position);

   // Fond
  is::IAnimatedMesh *background = smgr->addHillPlaneMesh("background",
                                                          ic::dimension2d<irr::f32>(2000,1000),
                                                          ic::dimension2d<irr::u32>(10,1), 0, 0,
                                                          ic::dimension2d<irr::f32>(0,0),
                                                          ic::dimension2d<irr::f32>(10,1));

  is::IAnimatedMeshSceneNode *node_background = smgr->addAnimatedMeshSceneNode(background);

  node_background->setPosition(ic::vector3df(9000, 300, 500));
  node_background->setRotation(ic::vector3df(-90,node_background->getRotation().Y, node_background->getRotation().Z));
  node_background->setMaterialTexture( 0, driver->getTexture("data/background.png") );
  node_background->setMaterialFlag(irr::video::EMF_LIGHTING, false);


  // Maison (decor)
  irr::scene::IMeshSceneNode *house_node = smgr->addMeshSceneNode(smgr->getMesh("data/House/house.obj"),
                                      0,
                                      1000,
                                      ic::vector3df(-90.0f, -60.0f, 30.0f),
                                      ic::vector3df(0.0f, 90.0f, 0.0f),
                                      ic::vector3df(3.0f, 3.0f,3.0f),
                                      false);
  house_node->setMaterialFlag(iv::EMF_LIGHTING, true);
  house_node->setMaterialType(video::EMT_SOLID);

  // Drapeau d'arrivee (decor)
  irr::scene::IMeshSceneNode *flag_node = smgr->addMeshSceneNode(smgr->getMesh("data/Flag/flag.obj"),
                                      0,
                                      1001,
                                      ic::vector3df(8080.0f, -30.0f, 30.0f),
                                      ic::vector3df(0.0f, 0.0f, 0.0f),
                                      ic::vector3df(3.0f, 3.0f,3.0f),
                                      false);
  flag_node->setMaterialFlag(iv::EMF_LIGHTING, false);
  flag_node->setMaterialType(video::EMT_SOLID);

  //Mesh correspondant aux arbres en fond
  is::IAnimatedMesh *tree = smgr->addHillPlaneMesh("tree",
                                                    ic::dimension2d<irr::f32>(100,120),
                                                    ic::dimension2d<irr::u32>(1,1), 0, 0,
                                                    ic::dimension2d<irr::f32>(0,0),
                                                    ic::dimension2d<irr::f32>(1,1));

  is::IAnimatedMeshSceneNode*  tree_nodes[42];

  srand(time(NULL));

  //Generation aleatoire des arbres
  for(int i = 0; i < 42; i++)
  {
    tree_nodes[i] = smgr->addAnimatedMeshSceneNode(tree);

    tree_nodes[i]->setPosition(ic::vector3df(0, 400, 1000));
    tree_nodes[i]->setRotation(ic::vector3df(-90,tree_nodes[i]->getRotation().Y, tree_nodes[i]->getRotation().Z));
    tree_nodes[i]->setMaterialTexture( 0, driver->getTexture("data/tree.png") );
    tree_nodes[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    tree_nodes[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    tree_nodes[i]->setPosition(ic::vector3df(i*200, 20, rand()%300 + 200));
  }

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
  collision.set_kirbies(kirbies);
  collision.set_coins_number(nb_coins);
  collision.set_coins(coins);
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
  
 // Gestion des briques/cubes
  int n_cubes = 40;
  is::ISceneNode* cubes[n_cubes];
  int cube_id = 0; 
  
  // Generation des plateformes, 4 types :
  //  - unit pour les petits cubes
  //  - little pour les petites plateformes
  //  - mean pour les moyennes plateformes
  //  - big pour les grosses plateformes

  ic::vector3df cube_unit(1.0f, 1.0f,1.0f);
  ic::vector3df cube_little(3.0f, 1.0f,1.0f);
  ic::vector3df cube_mean(8.0f, 1.0f,1.0f);
  ic::vector3df cube_big(30.0f, 1.0f,1.0f);
  
  // Tableau des positions 
  std::vector<ic::vector3df> P_unit_c = {ic::vector3df(2200, 50, 0), ic::vector3df(2400, 50, 0), ic::vector3df(2600, 50, 0),
					 ic::vector3df(2800, 50, 0), ic::vector3df(3000, 50, 0), ic::vector3df(5300, 50, 0)};
  
  std::vector<ic::vector3df> P_little_c = {ic::vector3df(3690, 50, 0),  ic::vector3df(3770, 150, 0), ic::vector3df(3850, 250, 0), 
					   ic::vector3df(4500, 200, 0), ic::vector3df(5370, 150, 0), ic::vector3df(5480, 250, 0),
					   ic::vector3df(6240, 450, 0), ic::vector3df(6350, 550, 0), ic::vector3df(7540, 50, 0),
					   ic::vector3df(7660, 125, 0)};
  
  std::vector<ic::vector3df> P_mean_c = {ic::vector3df(400, 75, 0), ic::vector3df(4000, 325, 0),  ic::vector3df(4300, 325, 0),
					 ic::vector3df(6460, 650, 0), ic::vector3df(5700, 650, 0), ic::vector3df(4850, 375, 0),
					 ic::vector3df(6850, 50, 0), ic::vector3df(7880, 300, 0)};
  
  std::vector<ic::vector3df> P_big_c = {ic::vector3df(850, 150, 0), ic::vector3df(1530, 50, 0), ic::vector3df(5890, 350, 0),
					ic::vector3df(6110, 750, 0), ic::vector3df(5290, 550, 0)};
  
  iv::ITexture *brick = driver->getTexture("data/Brick.jpg");
  
  // Gestion des cubes unités
  for(int j=0; j<6; j++)
  {

    cubes[cube_id] = smgr->addCubeSceneNode(30);
    cubes[cube_id]->setPosition(P_unit_c[j]);
    cubes[cube_id]->setScale(cube_unit);
    cubes[cube_id]->setID(5 + nb_coins + nb_kirbies + cube_id);
    cubes[cube_id]->setMaterialTexture(0, brick);
    cube_id++;

    // gestion des pièces
    coins[coins_id] = Coins();
    coins[coins_id].set_smgr(smgr);
    ic::vector3df pos = P_unit_c[j];    
    pos.Y += 100;
    coins[coins_id].init(pos, coins_id + 5 + nb_kirbies);
    coins_id++;
  }
  
  
  
  irr::core::matrix4 mat_little;
  mat_little.setTextureScale(3.0F,1.0F);

  // Gestion des cubes little
  for(int k=0; k<10; k++)
  {
    cubes[cube_id] = smgr->addCubeSceneNode(20);
    cubes[cube_id]->setPosition(P_little_c[k]);
    cubes[cube_id]->setScale(cube_little);
    cubes[cube_id]->setID(5 + nb_coins + nb_kirbies + cube_id);
    cubes[cube_id]->setMaterialTexture(0, brick);
    cubes[cube_id]->getMaterial(0).setTextureMatrix(0,mat_little);
    cube_id++;
    
    // gestion des pièces
    for(int p=0; p<2; p++)
    {
      coins[coins_id] = Coins();
      coins[coins_id].set_smgr(smgr);
      ic::vector3df pos = P_little_c[k]; 
      pos.X = pos.X - 15 + p*30;
      pos.Y += 20;
      coins[coins_id].init(pos, coins_id + 5 + nb_kirbies);
      coins_id++;
    }
  }
  
  irr::core::matrix4 mat_mean;
  mat_mean.setTextureScale(8.0F,1.0F);
  
  // Gestion des cubes mean
  for(int l=0; l<8; l++)
  {
    cubes[cube_id] = smgr->addCubeSceneNode(20);
    cubes[cube_id]->setPosition(P_mean_c[l]);
    cubes[cube_id]->setScale(cube_mean);
    cubes[cube_id]->setID(5 + nb_coins + nb_kirbies + cube_id);
    cubes[cube_id]->setMaterialTexture(0, brick);
    cubes[cube_id]->getMaterial(0).setTextureMatrix(0,mat_mean);
    cube_id++;  
    
    // gestion des pièces
    for(int p=0; p<4; p++)
    {
      coins[coins_id] = Coins();
      coins[coins_id].set_smgr(smgr);
      ic::vector3df pos = P_mean_c[l]; 
      pos.X = pos.X - 60 + p*40;
      pos.Y += 20;
      coins[coins_id].init(pos, coins_id + 5 + nb_kirbies);
      coins_id++;
    }
  }
  
  
  irr::core::matrix4 mat_big;
  mat_big.setTextureScale(30.0F,1.0F);
  
  // Gestion des cubes big
  for(int m=0; m<5; m++)
  {
    cubes[cube_id] = smgr->addCubeSceneNode(20);
    cubes[cube_id]->setPosition(P_big_c[m]);
    cubes[cube_id]->setScale(cube_big);
    cubes[cube_id]->setID(5 + nb_coins + nb_kirbies + cube_id);
    cubes[cube_id]->setMaterialTexture(0, brick);
    cubes[cube_id]->getMaterial(0).setTextureMatrix(0,mat_big);
    cube_id++;  
    
    // gestion des pièces
    for(int p=0; p<15; p++)
    {
      coins[coins_id] = Coins();
      coins[coins_id].set_smgr(smgr);
      ic::vector3df pos = P_big_c[m]; 
      pos.X = pos.X - 280 + p*40;
      pos.Y += 20;
      coins[coins_id].init(pos, coins_id + 5 + nb_kirbies);
      coins_id++;
    }
  }
  

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
