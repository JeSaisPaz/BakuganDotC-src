// bdc 0x088bf07c GameFieldDrawModelListTwoPass
#include "bdc.h"

/* Draws the model list `list` twice through `GameFieldDrawModelListRelative` (pass 2, then pass
   1) with the `GmoSetDepthWriteOverride` toggle set, then resets the draw pass to 0. Returns the display-list
   cursor. */

u32 *GameFieldDrawModelListTwoPass(CoreTask *task, u32 *dl, void **list)

{
  u32 *cursor;

  GmoSetDepthWriteOverride(true);
  cursor = GameFieldDrawModelListRelative(dl,list,2);
  cursor = GameFieldDrawModelListRelative(cursor,list,1);
  GmoSetDepthWriteOverride(false);
  GfxSetModelDrawPass(0);
  return cursor;
}
