// bdc 0x089f2534 GfxRenderPacketsReset
#include "bdc.h"

/* Empties the per-frame render packet state: clears `g_renderPacketCount`, the chunk pool counter
   `g_renderChunkPoolCount` and the packet list `g_renderPacketList` (head, tail, count). */

void GfxRenderPacketsReset(void)

{
  g_renderPacketCount = 0;
  g_renderChunkPoolCount = 0;
  g_renderPacketList.tail = 0;
  g_renderPacketList.head = 0;
  g_renderPacketList.count = 0;
  return;
}
