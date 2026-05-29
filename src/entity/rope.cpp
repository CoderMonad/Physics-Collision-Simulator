/*****************************************************************************/
#include "rope.h"
#include "../core/core.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file entity/rope.cpp
/// @brief Holds all of the functions for the rope
/////////////////////////////////////////////////
/*****************************************************************************/
clsRope::clsRope() {
    //ctor

    blncheckphysics_ = false;
    length_ = 0;
    spot_ = {{0,0},{0,0}};
    attachments_ = {AttachmentNone, AttachmentNone};
    balls_ = {nullptr, nullptr};
}
/*****************************************************************************/
clsRope::~clsRope() {
  blncheckphysics_ = false;
  balls_ = {nullptr, nullptr};
  if(global::blnDebugMode) { printf("Rope deconstructor called.\n"); }
}
/*****************************************************************************/
void clsRope::update() {
  /////////////////////////////////////////////////
  /// @brief updates the rope.
  /// @return void
  /////////////////////////////////////////////////
  if (blncheckphysics_) { //skip everything if not checking physics
    // check conditions in which the rope has to be deleted.
    if ( (attachments_.one == AttachmentWall && attachments_.two == AttachmentWall ) ||
         (attachments_.one == AttachmentBall && balls_.one == nullptr) ||
         (attachments_.two == AttachmentBall && balls_.two == nullptr)
       ) {
      if(global::blnDebugMode) {printf("Bad rope, killing it.\n");}
      blncheckphysics_ = false;
      return;
    }
    update_spots();

    // Vector from spot 1 to spot 2 in screen space (y-down)
    double dx = (double)(spot_.two.x - spot_.one.x);
    double dy = (double)(spot_.two.y - spot_.one.y);
    double dist = sqrt(dx * dx + dy * dy);

    if (dist >= length_) {
      // Unit vector from spot 1 to spot 2 in screen space
      // Negate y for physics space (y-up)
      dblXY n_phys = {dx / dist, -dy / dist};

      PP props1, props2;
      dblXY f1, f2;
      bool p1 = false, p2 = false;

      // Handle properties and flags based on attachment types
      if (attachments_.one == AttachmentBall) {
        props1 = balls_.one->getPhysicalProps();
        f1 = balls_.one->getForces();
        p1 = balls_.one->isPaused();
      } else {
        // Wall acts as infinite mass (props1.mass = 1.0, f1 = 0.0)
        props1 = {1.0, 1.0, 1.0, 1.0};
        f1 = {0.0, 0.0};
        p1 = true;
      }

      if (attachments_.two == AttachmentBall) {
        props2 = balls_.two->getPhysicalProps();
        f2 = balls_.two->getForces();
        p2 = balls_.two->isPaused();
      } else {
        props2 = {1.0, 1.0, 1.0, 1.0};
        f2 = {0.0, 0.0};
        p2 = true;
      }

      // Project net forces onto rope axis (physics space)
      double f1_along = f1.x * n_phys.x + f1.y * n_phys.y;
      double f2_along = f2.x * n_phys.x + f2.y * n_phys.y;

      // Inextensible-constraint tension: T = (m1*F2_along - m2*F1_along)/(m1+m2)
      // For Wall (infinite mass), this simplifies to T = -F_along.
      double m1 = p1 ? 1e12 : props1.mass;
      double m2 = p2 ? 1e12 : props2.mass;

      double T = (m1 * f2_along - m2 * f1_along) / (m1 + m2);

      // Force relaxation factor (0.5) improves stability when multiple ropes
      // act on the same ball, preventing over-correction and jitter.
      const double kRelax = 0.5;

      if (T > 0.0) {
        if (attachments_.one == AttachmentBall && !p1) {
          balls_.one->addForce({  T * kRelax * n_phys.x,  T * kRelax * n_phys.y });
        }
        if (attachments_.two == AttachmentBall && !p2) {
          balls_.two->addForce({ -T * kRelax * n_phys.x, -T * kRelax * n_phys.y });
        }
      }

      // Position correction to prevent stretching (Verlet-style pass)
      if (dist > length_) {
        double ratio = (dist - length_) / dist;
        double pdx = dx / global::physics::kMeterPixelRatio;
        double pdy = -dy / global::physics::kMeterPixelRatio;

        if (p1 && !p2) {
          dblXY loc2 = balls_.two->getdbLOC();
          loc2.x -= pdx * ratio; loc2.y -= pdy * ratio;
          balls_.two->setdbLOC(loc2);
        } else if (!p1 && p2) {
          dblXY loc1 = balls_.one->getdbLOC();
          loc1.x += pdx * ratio; loc1.y += pdy * ratio;
          balls_.one->setdbLOC(loc1);
        } else if (!p1 && !p2) {
          double total_m = props1.mass + props2.mass;
          dblXY loc1 = balls_.one->getdbLOC();
          dblXY loc2 = balls_.two->getdbLOC();
          loc1.x += pdx * ratio * (props2.mass / total_m);
          loc1.y += pdy * ratio * (props2.mass / total_m);
          loc2.x -= pdx * ratio * (props1.mass / total_m);
          loc2.y -= pdy * ratio * (props1.mass / total_m);
          balls_.one->setdbLOC(loc1);
          balls_.two->setdbLOC(loc2);
        }
      }
    }
    draw();
  }
}
/*****************************************************************************/
void clsRope::activate() {
  /////////////////////////////////////////////////
  /// @brief activates the rope.
  /// @return void
  ///
  /////////////////////////////////////////////////
  blncheckphysics_ = true;
  update_spots();

  length_ = math::getVectorLength(math::vectorSub(spot_.one,spot_.two));
  draw();
}
/*****************************************************************************/
void clsRope::setAttachment(uchar num, LOC place) {
  /////////////////////////////////////////////////
  /// @brief Sets rope attachment to a wall spot at place
  ///
  /// @param place = Wall spot in X and Y
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (num == 1) {
    attachments_.one = AttachmentWall;
    spot_.one = place;
    //if(global::blnDebugMode) {printf("(%i,%i)\n",spot_.one.x,spot_.one.y);}
    balls_.one = nullptr;
  } else if (num == 2) {
    attachments_.two = AttachmentWall;
    spot_.two = place;
    balls_.two = nullptr;
  }
}
/*****************************************************************************/
void clsRope::setAttachment(uchar num, clsCannonball* new_ball) {
  /////////////////////////////////////////////////
  /// @brief Sets rope attachment one to ball new_ball
  ///
  /// @param new_ball = pointer to ball rope is attached to
  /// @return void
  ///
  /////////////////////////////////////////////////

  if (num == 1) {
    attachments_.one = AttachmentBall;
    spot_.one = {0,0};
    balls_.one = new_ball;
  } else if (num == 2) {
    attachments_.two = AttachmentBall;
    spot_.two = {0,0};
    balls_.two = new_ball;
  }
}
/*****************************************************************************/
void clsRope::draw() {
  /////////////////////////////////////////////////
  /// @brief Draws the rope as a gravity-sagging catenary curve.
  ///        When the rope is slack the midpoint droops downward using a
  ///        quadratic Bezier parabolic approximation of a catenary.
  /////////////////////////////////////////////////

  LOC A = spot_.one;
  LOC B = spot_.two;

  double dx = (double)(B.x - A.x);
  double dy = (double)(B.y - A.y);
  double span = sqrt(dx * dx + dy * dy);

  double slack = (span < length_) ? (length_ - span) : 0.0;

  if (span < 1.0 || slack <= 0.0) {
    // Taut rope or degenerate (endpoints on top of each other): straight line.
    core::cannonwindow.drawline(A, B);
    return;
  }

  // Gravity in SDL screen space is (0, +1) (y increases downward).
  // Find the component of gravity perpendicular to the rope chord — this is
  // the direction the rope sags toward.
  dblXY u = {dx / span, dy / span};
  double g_along = u.y;  // dot({0,1}, u)
  dblXY g_perp = {-g_along * u.x, 1.0 - g_along * u.y};
  double g_perp_len = sqrt(g_perp.x * g_perp.x + g_perp.y * g_perp.y);

  dblXY sag_dir;
  if (g_perp_len < 0.001) {
    // Rope is nearly vertical: gravity is along the rope, no natural perp.
    // Sag horizontally as a fallback so the slack is still visible.
    sag_dir = {1.0, 0.0};
  } else {
    sag_dir = {g_perp.x / g_perp_len, g_perp.y / g_perp_len};
  }

  // Parabolic sag at midpoint: h = sqrt(3 * span * slack / 8).
  // For a quadratic Bezier the actual midpoint displacement is half the
  // control-point offset, so the control point must be placed at 2*h.
  double sag = sqrt(3.0 * span * slack / 8.0);
  double ctrl_x = (A.x + B.x) / 2.0 + 2.0 * sag * sag_dir.x;
  double ctrl_y = (A.y + B.y) / 2.0 + 2.0 * sag * sag_dir.y;

  // Number of segments scales with rope span, clamped to [8, 32].
  int N = (int)(span / 5.0);
  if (N < 8)  N = 8;
  if (N > 32) N = 32;

  LOC prev = A;
  for (int k = 1; k <= N; ++k) {
    double t  = (double)k / (double)N;
    double t1 = 1.0 - t;
    LOC next;
    next.x = (int)round(t1*t1*(double)A.x + 2.0*t*t1*ctrl_x + t*t*(double)B.x);
    next.y = (int)round(t1*t1*(double)A.y + 2.0*t*t1*ctrl_y + t*t*(double)B.y);
    core::cannonwindow.drawline(prev, next);
    prev = next;
  }
}
/*****************************************************************************/
void clsRope::update_spots() {
  // if attachment one is a ball, update spot
  if (attachments_.one == AttachmentBall) {
      if (!(balls_.one->blnstarted_)) {
        blncheckphysics_= false;
        return;
      }
      spot_.one = balls_.one->getScreenPlace();
  }

  // if attachment one is a ball, update spot
  if (attachments_.two == AttachmentBall) {
    // TODO: Comment above is a copy-paste error — this block handles attachment TWO, not one.
    if (!(balls_.two->blnstarted_)) {
      blncheckphysics_= false;
      return;
    }
    spot_.two = balls_.two->getScreenPlace();
  }


  /*if(global::blnDebugMode) {printf("New spots are now (%i,%i) and (%i,%i).\n",
                                   spot_.one.x,spot_.one.y,spot_.two.x,spot_.two.y);} */
}
/*****************************************************************************/
bool clsRope::isAttachedTo(clsCannonball* ball) {
  /////////////////////////////////////////////////
  /// @brief Checks if the rope is attached to a specific ball
  /// @param ball = pointer to the ball to check
  /// @return true if attached to the ball, false otherwise
  /////////////////////////////////////////////////

  return ( (attachments_.one == AttachmentBall && balls_.one == ball) ||
           (attachments_.two == AttachmentBall && balls_.two == ball) );
}
/*****************************************************************************/
