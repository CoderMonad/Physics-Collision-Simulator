/*****************************************************************************/
#include "tick.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file tick.cpp
/// @brief Holds functions for timey wimey stuff
/////////////////////////////////////////////////
/*****************************************************************************/
clsTick::clsTick() {
  /////////////////////////////////////////////////
  /// @brief Default constructor, just sets lngTimerStart to now
  /////////////////////////////////////////////////

	lngTimerStart = std::chrono::steady_clock::now();
}
/*****************************************************************************/
void clsTick::startTimer() {
  /////////////////////////////////////////////////
  /// @brief Sets lngTimerStart to now.
  /////////////////////////////////////////////////

  lngTimerStart = std::chrono::steady_clock::now();
}
/*****************************************************************************/
double clsTick::getTimeDifference() {
  /////////////////////////////////////////////////
  /// @brief Returns the difference (in seconds) from now to when lngTimerStart
  /// @return Time in Seconds
  /////////////////////////////////////////////////

  auto now = std::chrono::steady_clock::now();
  double temp_time = std::chrono::duration<double>(now - lngTimerStart).count();
  startTimer();
  return temp_time;
}
/*****************************************************************************/
void clsTick::startHolding() {
  /////////////////////////////////////////////////
  /// @brief Sets lngHoldingStart to now.
  /////////////////////////////////////////////////
  lngHoldingStart = std::chrono::steady_clock::now();
}
/*****************************************************************************/
double clsTick::stopHolding() {
  /////////////////////////////////////////////////
  /// @brief Returns the difference (in seconds) from now to when lngHoldingStart
  /// @return Time in Seconds
  /////////////////////////////////////////////////

  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration<double>(now - lngHoldingStart).count();
}
/*****************************************************************************/
