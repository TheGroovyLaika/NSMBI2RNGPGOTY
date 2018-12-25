// gui_display.h

#ifndef GUI_DISPLAY_H
#define GUI_DISPLAY_H

#include <vector>

#include <irrlicht.h>

class Gui_display
{
 irr::scene::ISceneManager *smgr; 
public:
  Gui_display();
  void set_smgr(irr::scene::ISceneManager *smgr);
};

#endif
