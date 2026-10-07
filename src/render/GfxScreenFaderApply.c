// bdc 0x089ee0ac GfxScreenFaderApply
#include "bdc.h"

/* Apply method of the screen fader (vtable `0x08af577c` slot `+0x24`): copies the current RGBA
   (`base.color`) into the colour of its overlay rect (`rect->color`). */

void GfxScreenFaderApply(GfxScreenFader *self)
{
  GfxRect *rect = self->rect;
  int i;

  for (i = 0; i < 4; i++) {
    rect->color[i] = self->base.color[i];
  }
}
