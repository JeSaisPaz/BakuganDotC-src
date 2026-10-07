// bdc 0x089ee11c GfxScreenFaderSetVisible
#include "bdc.h"

/* Shows or hides the screen fader's overlay rect (`GfxRectSetVisible`); vtable `0x08af577c` slot
   `+0x14`. */

void GfxScreenFaderSetVisible(GfxScreenFader *self, bool visible)

{
  GfxRectSetVisible(self->rect,visible);
  return;
}

