// bdc 0x089ed780 GfxRectSetSize
#include "bdc.h"

/* Sets an overlay rect's width and height (`+0x34`, `+0x38`). */

void GfxRectSetSize(GfxRect *rect, s32 w, s32 h)
{
  rect->width = w;
  rect->height = h;
}
