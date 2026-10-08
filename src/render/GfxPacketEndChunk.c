// bdc 0x089f1398 GfxPacketEndChunk
#include "bdc.h"

/* Closes a chunk opened with `GfxPacketBeginChunk`: writes RET (`0x0b000000`) at `end`, advances
   the main list pointer (`g_renderListCursor`) to `end + 4`, and patches the two words reserved at
   packet `+0x30` with BASE (high address nibble) and JUMP to the new pointer so the main list skips
   over the chunk body. */

void GfxPacketEndChunk(RenderPacket *packet, u32 *end)
{
  u32 *slot;

  *end = 0xb000000;
  g_renderListCursor = end + 1;
  slot = packet->jumpSlot;
  slot[0] = ((PspAddr(g_renderListCursor) >> 0x18) & 0xf) << 0x10 | 0x10000000;
  slot[1] = (PspAddr(g_renderListCursor) & 0xffffff) | 0x8000000;
}
