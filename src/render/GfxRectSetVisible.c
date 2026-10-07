// bdc 0x089ed670 GfxRectSetVisible
#include "bdc.h"

/* Sets or clears bit 0 (visible) of the flags word at `rect+0x20` of an overlay rect
   (`GfxRectCtor`), tested by `GfxRectDraw`. */

void GfxRectSetVisible(GfxRect *rect, char visible)
{
  if (visible != '\0') {
    rect->flags = rect->flags | 1;
    return;
  }
  rect->flags = rect->flags & 0xfffffffe;
}
