// bdc 0x089f255c GfxFlushRenderPackets
#include "bdc.h"

/* End-of-frame render flush: collects all packets in `g_renderPacketList` into a NULL-terminated
   stack array of (packet, -sortKey) pairs, sorts it with `GfxCombSortByDepth`, and for each packet
   emits a GE BASE + CALL (0x10/0x0A commands) at `g_renderListCursor` to every display-list chunk
   in its `chunkHead` chain (then clears `chunkHead`), sets the libgu write pointer to the end of the
   calls (`sceGuSetMemory`), resets the packet pools/list (`GfxRenderPacketsReset`), closes the
   list (`sceGuFinish`), writes back the dcache from `g_renderListStart` over the list size and
   kicks it to the GE (`sceGuSendList`). Runs `GfxDumpRenderPackets` first when the one-shot
   flag `g_renderDumpRequest` is set (and clears it). */

typedef struct RenderPacketDepthPair {
  RenderPacket *packet;
  float depth;
} RenderPacketDepthPair;

void GfxFlushRenderPackets(void)
{
  RenderPacketDepthPair pairs[65]; /* 64 packets + NULL terminator */
  RenderPacket *packet;
  CoreNode *chunk;
  u32 addr;
  u32 count;
  u32 i;
  s32 size;

  packet = (RenderPacket *)g_renderPacketList.head;
  if (g_renderDumpRequest != 0) {
    GfxDumpRenderPackets();
    g_renderDumpRequest = 0;
  }
  count = 0;
  for (; packet != NULL; packet = (RenderPacket *)packet->node.next) {
    pairs[count].packet = packet;
    pairs[count].depth = -packet->sortKey;
    count++;
  }
  pairs[count].packet = NULL;
  GfxCombSortByDepth(pairs, count);

  i = 1;
  for (packet = pairs[0].packet; packet != NULL; packet = pairs[i++].packet) {
    for (chunk = packet->chunkHead; chunk != NULL; chunk = chunk->next) {
      addr = chunk->id;
      g_renderListCursor[0] = ((addr >> 24) & 0xf) << 16 | 0x10000000;
      g_renderListCursor[1] = (addr & 0xffffff) | 0x0a000000;
      g_renderListCursor += 2;
    }
    packet->chunkHead = NULL;
  }
  sceGuSetMemory(g_renderListCursor);
  GfxRenderPacketsReset();
  size = sceGuFinish();
  sceKernelDcacheWritebackRange(g_renderListStart, size);
  sceGuSendList(0, g_renderListStart, NULL, 0, NULL);
}
