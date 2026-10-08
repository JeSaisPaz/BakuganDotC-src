// bdc 0x089f12d8 GfxPacketBeginChunk
#include "bdc.h"

/* Opens a display-list chunk on a render packet: takes a 0x24-byte chunk node from the per-frame
   pool (`g_renderChunkPool`, count `g_renderChunkPoolCount`), links it into the packet's chunk
   chain (`+0x28`), reserves two words at the current list pointer (`g_renderListCursor`) for the
   BASE/JUMP that skips the chunk in the main list (packet `+0x30`), records the chunk start
   (`list + 8`) in the node (`+0xc`) and in the packet (`+0x2c`), and returns that start as the
   address where the caller writes GE commands. Close it with `GfxPacketEndChunk`. */

u32 *GfxPacketBeginChunk(RenderPacket *packet)
{
  CoreNode *slot;
  CoreNode *node;

  slot = g_renderChunkPool + g_renderChunkPoolCount++;
  node = NULL;
  if (slot != NULL) {
    CoreNodeCtor(slot, NULL);
    node = slot;
  }
  if (packet->chunkHead == NULL) {
    packet->chunkHead = node;
  } else {
    CoreNodeLink(node, packet->chunkHead, 0);
  }
  packet->jumpSlot = g_renderListCursor;
  node->id = PspAddr(g_renderListCursor + 2);
  packet->chunkLast = node;
  return packet->jumpSlot + 2;
}
