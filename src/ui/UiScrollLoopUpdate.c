// bdc 0x0892c298 UiScrollLoopUpdate
#include "bdc.h"

/* Scrolls the two sprites of a scroll record up by 1 px per frame (2 with frame skip), wrapping
   each from y <= -172 back to 372. */

void UiScrollLoopUpdate(UiScrollLoop *rec)

{
  float step;

  if (rec->on != 0) {
    if (g_gfxDisplay->frameSkip == 0) {
      step = 1.0f;
    }
    else {
      step = 2.0f;
    }
    rec->a->posY = rec->a->posY - step;
    if (rec->a->posY <= -172.0f) {
      rec->a->posY = 372.0f;
    }
    rec->b->posY = rec->b->posY - step;
    if (rec->b->posY <= -172.0f) {
      rec->b->posY = 372.0f;
    }
  }
  return;
}
