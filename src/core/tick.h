#ifndef __MY_TICK_HEADER__
#define __MY_TICK_HEADER__
/*****************************************************************************/
#include <chrono>
#include <cmath>
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsTick tick.h "src/core/tick.h"
/// @brief This class will hold functions related to time.
/////////////////////////////////////////////////
/*****************************************************************************/
class clsTick {
  public:
    clsTick();

    void startTimer(void);
    double getTimeDifference(void);
    void startHolding(void);
    double stopHolding(void);

	private:

		std::chrono::steady_clock::time_point lngTimerStart; /**< The time the ball positions were last updated at. */
		std::chrono::steady_clock::time_point lngHoldingStart; /**< The time of the start of the mouse being held down. */
};
/*****************************************************************************/
#endif
