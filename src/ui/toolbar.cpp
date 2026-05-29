/*****************************************************************************/
#include "toolbar.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file toolbar.cpp
/// @brief Holds all of the functions for the toolbar class
/////////////////////////////////////////////////
/*****************************************************************************/
clsToolbar::clsToolbar() {
    //ctor

    selected_tool_ = ToolFire;
    show_toolbox_ = true;
    /** @todo (GamerMan7799#9#): Make toolbar bigger? */
    position_ = {0,0,28,28};
}
/*****************************************************************************/
void clsToolbar::show() {
  /////////////////////////////////////////////////
  /// @brief Displays the toolbox if enabled, and pause indicator if paused
  /////////////////////////////////////////////////

  // Draw pause indicator in top left if paused
  if (global::blnPaused) {
    SDL_Rect pause_rect = {2, 2, 24, 24};
    SDL_RenderCopy(screen::screenatt.ren, screen::screenatt.tools,
                   &screen::screenatt.toolclips[ToolPause], &pause_rect);
  }

  if(show_toolbox_) { // only show if enabled
    // Make SDL rect just for the tool picture inside the frame
    SDL_Rect tool_pic_box = {position_.x+2,position_.y+2,24,24};

    SDL_RenderCopy(screen::screenatt.ren,screen::screenatt.toolbox,
                 NULL,&position_); // draw toolbox frame

    // draw tool inside box
    SDL_RenderCopy(screen::screenatt.ren,screen::screenatt.tools,
                   &screen::screenatt.toolclips[selected_tool_], &tool_pic_box);
  }
}
/*****************************************************************************/
void clsToolbar::toggleToolbox() {
  /////////////////////////////////////////////////
  /// @brief Toggles showing the toolbox on/off.
  /////////////////////////////////////////////////

  show_toolbox_ = !(show_toolbox_);
}
/*****************************************************************************/
void clsToolbar::incrementTool(char dir) {
  /////////////////////////////////////////////////
  /// @brief Moves toolbox to the next or previous tool
  ///
  /// @param dir = value to increment tool box up or down
  /////////////////////////////////////////////////

  // TODO: ToolRope and ToolInfo are hardcoded as the lower/upper bounds. If the
  //       Tools enum is extended, this silently excludes new tools. Use the first
  //       and last enum values (or a ToolCount sentinel) to future-proof this.
  if (dir == -1 && selected_tool_ != ToolRope) { selected_tool_--; }
  else if (dir == 1 && selected_tool_ != ToolInfo) { selected_tool_++; }
}
/*****************************************************************************/
uchar clsToolbar::getTool() {
  /////////////////////////////////////////////////
  /// @brief Returns current selected tool
  ///
  /// @return Selected tool based on enumTools
  /////////////////////////////////////////////////

  return selected_tool_;
}
/*****************************************************************************/
void clsToolbar::setTool(uchar newtool) {
  /////////////////////////////////////////////////
  /// @brief Sets tool
  ///
  /// @param newtool = new tool
  /////////////////////////////////////////////////

  selected_tool_ = newtool;
}
/*****************************************************************************/
