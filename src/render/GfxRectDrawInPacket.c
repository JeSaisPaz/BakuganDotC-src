// bdc 0x089ed384 GfxRectDrawInPacket
#include "bdc.h"

/* Draws an overlay rect into a render packet: opens a chunk, writes the rect (`GfxRectDraw`) and
   closes the chunk. No-op for a NULL packet. */

void GfxRectDrawInPacket(void *rect, void *packet)
{
  u32 *list;

  if (packet != NULL) {
    list = GfxPacketBeginChunk(packet);
    GfxRectDraw(rect, list);
    GfxPacketEndChunk(packet, list);
  }
}
