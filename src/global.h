#ifndef __GLOBAL_H_INCLUDED__
#define __GLOBAL_H_INCLUDED__
/*****************************************************************************/
//This file will hold the global references that are need in one place
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file global.h
/// @brief This file holds all of the references to variables declared in the
///        Global namespace. See Main.cpp for more details
/////////////////////////////////////////////////
/*****************************************************************************/
#define DEFINED_USE_R2_VEL_MODDER 0
/**< If this is defined, then program will use unrealistic method that will
     increase the velocity of two colliding balls the closer they are together
     it will reduce the number of times that the balls stick together,
     but also causes them to get unrealistically high velocities. */
/*****************************************************************************/
#define DEFINED_COLLISION_NORMAL_FORCE 0
/**< If this is defined, then program will use apply a normal force to any
     colliding balls. The forces are along the same direction as the velocities
     are. It doesn't make sense in the real world. */
/*****************************************************************************/
class clsConfig;
/*****************************************************************************/
//global typedefs
typedef unsigned char uchar; /**< Rename unsigned char to uchar because I'm lazy. */
typedef unsigned int uint; /**< Rename unsigned int to uint because I'm lazy. */
typedef unsigned long ulong; /**< Rename unsigned long to ulong because I'm lazy. */
/*****************************************************************************/
//Global namespace
namespace cannonballs {
  extern uint intCannonBallNum;
} //end namespace cannonballs

/////////////////////////////////////////////////
/// @defgroup GlobalValues Values held globally
/// @brief Things that will need to be referenced by many
///        different functions.
/// @{
/////////////////////////////////////////////////

namespace global {
  extern const bool blnDebugMode;
  extern clsConfig config;
  extern bool blnPaused;
  namespace physics {
  /////////////////////////////////////////////////
  /// @defgroup ChangeablePhysics Defined Physics values
  /// @brief Physics values for how the program works.
  /// @{
  /////////////////////////////////////////////////
    extern const float kMeterPixelRatio;
    extern const float kBallDensity;
    extern const float kGravity;
    extern const float kDragCofficient;
    extern const float kKineticFriction;
    extern const float kDensityAir;
    extern const float kMinVelocity;
    extern const float kCoefficientRestitution;
    extern uchar collisionmethod;
    extern const double kAirDynViscosity;
  /// @}
  } // end physics namespace

  namespace equations {
    /////////////////////////////////////////////////
    /// @defgroup EquationValues
    /// @brief Values used for certain equations
    /// @{
    /////////////////////////////////////////////////
    extern const float kVelocityScalar;
    extern const float kTimeSizeRatio;
    extern const float kRadiusMin;
    extern const float kRadiusMax;
    extern const float kMassAlphaRatio;
    extern const float kMassAlphaOffset;
    extern const float kAlphaMinimum;
    /// @}
  } // end equations namespace
} // end global namespace
/// @}
/*****************************************************************************/
/** @enum Collisions
  * The method of the collision (I just wanted to play around with some options.)
  */
enum Collisions {
  CollideElastic = 0, /**< This is normal collision in a perfect world without
                           friction, they hit and bounce off, no energy is lost. */
  CollideInelastic, /**< This is the normal way things collide, Balls collide and
                         bounce off of each other losing some energy. */
  CollidePerfectInelastic, /**< In this method the balls collide and then
                                stick together. */
  CollideNone /**< No collisions, balls pass through each other. */
};
/*****************************************************************************/
/** @enum Tools
  * The current tool being used in the toolbox.
  */
enum Tools {
  ToolRope = 0, /**< Tool to add a rope between balls, or pin ball to wall. */
  ToolFire,     /**< The basic firing tool. */
  ToolDrop,     /**< Drops a ball without velocity. */
  ToolDele,     /**< Tool to delete selected ball. */
  ToolDrag,     /**< Tool that allows ball to be dragged around. */
  ToolInfo,     /**< Tool that writes information about the selected ball to screen */
  ToolPause     /**< Not really a tool, but the pause symbol */
};
/*****************************************************************************/
#endif // __GLOBAL_H_INCLUDED__
