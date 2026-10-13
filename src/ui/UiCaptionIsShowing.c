// bdc 0x0882cd28 UiCaptionIsShowing
#include "bdc.h"

/* Returns 1 when the caption state `hintState` (`+0xbe8`) of the talk/HUD task (`BtlHud`) is
   strictly between 0 and 100, i.e. a caption is on screen and not yet being dismissed (100 is the
   dismiss request written by `UiCaptionSetText`). */

s32 UiCaptionIsShowing(BtlHud *win)

{
  if ((0 < win->hintState) && (win->hintState < 100)) {
    return 1;
  }
  return 0;
}
