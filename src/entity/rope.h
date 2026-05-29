#ifndef __MY_ROPE_HEADER__
#define __MY_ROPE_HEADER__
/*****************************************************************************/
#include "cannonball.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsRope rope.h "src/entity/rope.h"
/// @brief This class will hold all of the functions for the rope.
/////////////////////////////////////////////////
/*****************************************************************************/
/** @enum RopeAttachment
  * Basically holds if the rope is attached to a ball or a spot on the wall.
  */
enum RopeAttachment {
  AttachmentNone = 0, /**< Rope is not attached. */
  AttachmentBall,     /**< Rope is attached to a ball. */
  AttachmentWall      /**< Rope is attached to a spot on the wall. */
};
/*****************************************************************************/
/** @struct stcOneTwoAttachment
    Holds values for one and two. Used to represent the two ends that the
    rope is attached to. */
struct stcOneTwoAttachment{
  uchar one;
  uchar two;
};

/** @struct stcOneTwoPlaces
    Holds values for the places one and two. Used to represent the two ends that the
    rope is attached to. */
struct stcOneTwoPlaces{
  LOC one;
  LOC two;
};

/** @struct stcOneTwoBalls
    Holds points to balls for one and two. Used to represent the two ends that the
    rope is attached to. */
struct stcOneTwoBalls{
  clsCannonball* one;
  clsCannonball* two;
};

typedef struct stcOneTwoAttachment AttachSpots;
typedef struct stcOneTwoBalls BallsSpots;
typedef struct stcOneTwoPlaces PlacesSpots;
/*****************************************************************************/
class clsRope {
    public:
      clsRope();
      ~clsRope();
      void activate(void);
      //dblXY addTension(void);

      void update(void);
      void draw(void);

      bool isActive(void);

      void setAttachment(uchar,LOC);
      void setAttachment(uchar,clsCannonball*);
      bool isAttachedTo(clsCannonball*);

      bool blncheckphysics_; /**< if rope should be updated. */
    private:

      void update_spots(void);

      double length_; /**< Length of the rope in pixels. */

      PlacesSpots spot_; /**< Location of attachment spots. */
      AttachSpots attachments_; /**< Type of attachments. */
      BallsSpots balls_; /**< Point to the balls rope is attached to. */
};
/*****************************************************************************/
#endif // __ROPE_HEADER__
