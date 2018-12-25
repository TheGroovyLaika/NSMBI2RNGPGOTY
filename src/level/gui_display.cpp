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
  : smgr(nullptr)
{
}

/**************************************************************************\
 * Gui_display::set_smgr                                           *
\**************************************************************************/
void Gui_display::set_smgr(irr::scene::ISceneManager *s)
{
	smgr = s;
}