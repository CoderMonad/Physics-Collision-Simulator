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
      // if both spots are attached to walls, delete it, since it is not going
      // to be doing anything
      if(global::blnDebugMode) {printf("Bad rope, killing it.\n");}

      blncheckphysics_ = false;
      return;
    }
    update_spots();
    double angle;
    LOC differencexy;
    dblXY tension;
    dblXY ball_one_forces, ball_two_forces;
    differencexy.x = abs (spot_.one.x - spot_.two.x);
    differencexy.y = abs (spot_.one.y - spot_.two.y);

    // get the angle measure from spot one to spot two.
    // some of these equations are to correct the angle value.
    angle = (differencexy.x != 0) ? atan ((double)differencexy.y / (double)differencexy.x) :
                                    M_PI / 2;

    if (angle == M_PI/2) {angle += (spot_.one.y > spot_.two.y) ? M_PI : 0;}
    angle += (spot_.two.x < spot_.one.x) ? M_PI : 0;

    // forces only happen when rope is "tense" aka at or greater than length
    if (sqrt(pow(differencexy.x,2) + pow(differencexy.y,2)) >= length_) {
      if (attachments_.one == AttachmentBall && attachments_.two == AttachmentWall) {
        ball_one_forces = balls_.one->getForces();
        tension = ballWallForces(ball_one_forces,angle);
        balls_.one->addForce(tension);
      } else if (attachments_.one == AttachmentWall && attachments_.two == AttachmentBall ) {
        ball_one_forces = balls_.two->getForces();
        tension = ballWallForces(ball_one_forces,angle);
        balls_.two->addForce(tension);
      } else {
        // rope attached to two balls
        ball_one_forces = balls_.one->getForces();
        ball_two_forces = balls_.two->getForces();
        PP props1 = balls_.one->getPhysicalProps();
        PP props2 = balls_.two->getPhysicalProps();

        // Unit vector from ball 1 to ball 2 in screen space (y-down).
        // Negate y to convert to physics space (y-up), where forces live.
        double dx = (double)(spot_.two.x - spot_.one.x);
        double dy = (double)(spot_.two.y - spot_.one.y);
        double dist = sqrt(dx * dx + dy * dy);

        if (dist >= 0.5) {
          dblXY n_phys = {dx / dist, -dy / dist};

          // Project each ball's net force onto the rope axis (physics space).
          double f1_along = ball_one_forces.x * n_phys.x + ball_one_forces.y * n_phys.y;
          double f2_along = ball_two_forces.x * n_phys.x + ball_two_forces.y * n_phys.y;

          // Inextensible-constraint tension: T = (m1*F2_along - m2*F1_along)/(m1+m2).
          // Derived from (a1 - a2)·n̂ = 0 with T applied along n̂.
          // Clamp to 0 — rope can pull but not push.
          // Note: this force-only model doesn't include centripetal or velocity-level
          // correction terms, consistent with the ball-wall rope implementation.
          double T = (props1.mass * f2_along - props2.mass * f1_along) /
                     (props1.mass + props2.mass);
          if (T > 0.0) {
            balls_.one->addForce({  T * n_phys.x,  T * n_phys.y });
            balls_.two->addForce({ -T * n_phys.x, -T * n_phys.y });
          }
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
dblXY clsRope::ballWallForces(dblXY ball_one_forces, double angle) {
  dblXY tension;
  const double kAngleEps = 1e-9;
  const bool is_vertical   = fabs(angle - M_PI / 2)       < kAngleEps ||
                              fabs(angle - 3.0 / 2.0 * M_PI) < kAngleEps;
  const bool is_lower_half = !is_vertical && angle > M_PI && angle < 2 * M_PI;
  const bool is_upper_half = !is_vertical && angle > 0    && angle < M_PI;

  if (is_vertical) {
    tension.x = 0;
    tension.y = ball_one_forces.y;
    if (!(std::signbit((double)spot_.one.y-(double)spot_.two.y) ^ std::signbit(ball_one_forces.y))) {
      // force and direction to spot two are both positive or both negative, therefore
      // reverse the force for tension
      tension.y *= -1;
    }
  } else if (is_lower_half) {
    tension.y = ball_one_forces.y;
    tension.x = tension.y / tan(angle);
  } else if (is_upper_half) {
    tension.y = -1 * ball_one_forces.y;
    tension.x = tension.y / tan(angle);
  } else {
    tension.y = 0;
    tension.x = ball_one_forces.x;
    if ((std::signbit((double)spot_.one.x-(double)spot_.two.x) ^ std::signbit(ball_one_forces.x))) {
      // force and direction to spot two are both positive or both negative, therefore
      // reverse the force for tension
      tension.x *= -1;
    }
  }
  return tension;
}
/*****************************************************************************/
