// bdc 0x089ee0c8 GfxScreenFaderDraw
#include "bdc.h"

/* Draw method of the screen fader (vtable `g_gfxScreenFaderVtbl` slot `+0x2c`): when active and its alpha
   (`+0x2c`) is positive, draws the overlay rect into a new render packet at the fader's sort key
   (`GfxRectDrawInPacket`). */

void GfxScreenFaderDraw(GfxScreenFader *self)

{
  void *packet;
  
  if (((self->base).active != '\0') && !((self->base).color[3] <= 0.0f)) {
    packet = GfxNewRenderPacket((self->base).sortKey);
    GfxRectDrawInPacket(self->rect,packet);
  }
  return;
}

