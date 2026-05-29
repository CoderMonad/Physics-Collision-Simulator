#ifndef __CANNONBALL_HEADER__
#define __CANNONBALL_HEADER__
/*****************************************************************************/
#include "../math/vector_math.h"
#include <cstdlib>
#include <vector>
#include <time.h>
#include <cmath>
#include "../ui/screen.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsCannonball cannonball.h "src/entity/cannonball.h"
/// @brief This class is used to create and control each of the balls.
/////////////////////////////////////////////////
/*****************************************************************************/
/////////////////////////////////////////////////
/// @defgroup BallStuff Stuff for Cannonballs
/// @brief Important items for the cannonballs
/// @{
/////////////////////////////////////////////////
/*****************************************************************************/
/** @struct stcPhysicsProperties
    Holds the physical properties of the cannonball. */
struct stcPhysicalProperties {
  double radius; /**< Radius in meters */
  double mass; /**< Mass of the ball in kilograms */
  double area; /**< HALF of Area of the ball in terms of m<sup>2</sup>, it is only half
                    because the drag equation only needs the area facing the wind,
                    and it saves me from having to divide. */
  double volume; /**< Volume of the ball in m^3 */
  double density; /**< Density of the ball in kg/m^3 see Global::Physics::uBallDensity */
};

/** @struct stcBox
    Holds a box which specifics the space that the ball occupies, used to see if
    it is colliding with another ball. The box is defined with the values of it
    left-most edge, its right-most edge, its top-most edge, and its bottom-most edge */
struct stcBox {
  int left; /**< Left-most edge in term of pixels from the left window edge */
  int right; /**< Right-most edge in term of pixels from the left window edge */
  int top; /**< Top-most edge in term of pixels from the top window edge */
  int bottom; /**< Bottom-most edge in term of pixels from the top window edge */
};

/** @struct stcColor
    The color based on red, green, and blue */
struct stcColor {
  Uint8 Red; /**< Amount of Red in Color, can vary from 0 to 255 */
  Uint8 Green; /**< Amount of Green in Color, can vary from 0 to 255 */
  Uint8 Blue;/**< Amount of Blue in Color, can vary from 0 to 255 */
};

typedef struct stcPhysicalProperties PP;
typedef struct stcBox BOX;
typedef struct stcColor clr;
//typedef struct stcXYZVector XYZ;

typedef std::vector<LOC> VPath;
/// @}
/*****************************************************************************/
class clsCannonball {
  public:
    clsCannonball();
    ~clsCannonball();
    void setValues(double, LOC, double, double, int);
    LOC getplace(void);
    LOC getScreenPlace(void);
    void setplace(LOC);
    void update(double);
    void updateForces(void);
    dblXY getVelocity(void);
    dblXY getForces(void);
    void setVelocity(dblXY);
    PP getPhysicalProps(void);
    void setPhysicalProps(PP);
    BOX getBOX(void);
    void writeInfo(void);
    void togglePause(void);
    bool isPaused(void);
    void checkEdges(void);
    void setEdgePosition(void);
    bool blnstarted_; /**< Whether or not the ball is "started" if it is, the program will update
        it and won't let a new ball replace it in its array spot. */

    bool blncheckphysics_; /**< If physics should be checked. It is disabled if the ball collides,
                                so that when looping through all of the balls, it doesn't mark
                                the same collision twice */
    void addForce(dblXY);
    dblXY getdbLOC(void);
    void  setdbLOC(dblXY);
  private:

    uint ballID_;  /**< The ball ID which is basically just its number in the array */
    bool blndragenabled_;  /**< If Drag has been enabled or not */
    clr color_; /**< The color of the ball, which is randomly generated */
    BOX collisionbox_; /**< The Box for this specific ball */
    SDL_Rect screen_place_; /**< The SDL Rect for the current place of the ball */
    LOC place_; /**< The ball's location in terms of X and Y */
    dblXY vel_; /**< The ball's velocity in terms of X and Y */
    dblXY dblLOC_; /**< The ball's location in terms of X and Y in double
        (because it is more accurate then we round to get the place
        that is used to put the ball on the screen */
    dblXY acc_; /**< The ball's current acceleration values in terms of X and Y */
    PP props_; /**< The Physical properties of the ball */
    VPath path_; /**< Past locations of the ball which will be shown if enabled */
    double deltat_; /**< The time in seconds since the last time it moved. It is variable
        based on how much time has past since the last update to keep
        the movements looking realistic. See clsTick::getTimeDifference */

    dblXY forces_; /**< Forces on the ball in x and y */
    bool paused_; /**< If the simulation is paused or not.  */
    uint pathUpdateCounter_; /**< Frames elapsed since the last path point was recorded. */
    uint pathHead_;          /**< Next write index into the circular path_ buffer. */

    void show(void);
    void drawPath(LOC);
    void dragCalcValues(void);
    void dragUpdateAcc(void);
    void doFriction(void);
    void updateCollisionBox(void);
};
/*****************************************************************************/
#endif // __CANNONBALL_HEADER__
