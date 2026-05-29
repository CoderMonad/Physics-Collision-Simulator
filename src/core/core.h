#ifndef __CORE_HEADER__
#define __CORE_HEADER__
/*****************************************************************************/
#include "../ui/toolbar.h"
#include "tick.h"
#include "../entity/rope.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file core.h
/// @brief Header for core functions
/////////////////////////////////////////////////
/*****************************************************************************/
typedef std::vector<clsCannonball> VectorCannon;
typedef std::vector<clsRope> VectorRope;
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup CannonballNameSpace Namespace for cannonballs
/// @brief Holds functions that deal with the balls colliding, and creating new balls
///       that are currently in main.cpp. I wanted to get them out of the global scope.
/// @{
/////////////////////////////////////////////////
namespace cannonballs {
  void addNew(LOC, LOC, double);
  void clean_up(void);
  void checkCollisons(uint);
  bool checkOverlap(BOX, BOX);
  void doCollide(uint, uint);
  extern uint intCannonBallNum;
  extern uint intRopeNum;
  extern VectorCannon balls;
  extern VectorRope ropes;
}
/// @}
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup CoreNameSpace Namespace for core functions
/// @brief Holds functions some core functions for the main
/// @{
/////////////////////////////////////////////////
namespace core {
  void fireRandom(void);
  char handleEvent(SDL_Event*);
  int findSelectedBall(LOC);

  void doFireTool(SDL_Event*);
  void doDeleTool(SDL_Event*);
  void doDragTool(SDL_Event*);
  void doDropTool(SDL_Event*);
  void doInfoTool(SDL_Event*);
  void doRopeTool(SDL_Event*);

  extern LOC oldmouse;
  extern LOC currentmouse;
  extern bool holding;
  extern clsToolbar toolbar;
  extern clsTick tick;
  extern clsScreen cannonwindow;
}
/// @}
/*****************************************************************************/
#endif
