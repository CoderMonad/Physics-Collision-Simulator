#ifndef __TOOLBAR_HEADER__
#define __TOOLBAR_HEADER__
/*****************************************************************************/
#include "screen.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @class clsToolbar toolbar.h "src/ui/toolbar.h"
/// @brief This class is used to handle showing the toolbar. The actual functions
///        of the tools are handled in core.cpp instead.
/////////////////////////////////////////////////
/*****************************************************************************/
class clsToolbar {
    public:
      clsToolbar();
      void show(void);
      void setTool(uchar);
      uchar getTool(void);
      void incrementTool(char);
      void toggleToolbox(void);

    private:

      uchar selected_tool_;
      bool show_toolbox_;
      SDL_Rect position_;

};
/*****************************************************************************/
#endif // __TOOLBAR_HEADER__
