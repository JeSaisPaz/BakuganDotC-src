// bdc 0x089f28d4 GfxGetTopRenderPacket
#include "bdc.h"

/* Returns the render packet with the highest sort key (the one drawn last, so on top) from
   `g_renderPacketList`, or NULL when no packet exists this frame. Compares the float at packet
   `+0x24` (see `GfxRenderPacketInit`) starting from -infinity. */

void *GfxGetTopRenderPacket(void)
{
  RenderPacket *best = (RenderPacket *)0;
  RenderPacket *cur = (RenderPacket *)g_renderPacketList.head;
  float bestKey = -__builtin_inff();

  for (; cur != (RenderPacket *)0; cur = (RenderPacket *)cur->node.next) {
    if (bestKey < cur->sortKey) {
      bestKey = cur->sortKey;
      best = cur;
    }
  }
  return best;
}
