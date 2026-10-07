// bdc 0x089f2920 GfxDumpRenderPackets
#include "bdc.h"

/* Debug variant of the render-packet flush, run once by `GfxFlushRenderPackets` when its dump
   flag (`0x08ac5dd4`) is set: copies `g_renderPacketList` into a heap array of (packet, -sortKey)
   pairs, sorts it (`GfxCombSortByDepth`) and prints `"----------DumpRender-----------"`, then for each
   packet `"$%07x:"`, every display-list chunk `"($%07x,$%07x)"` (node, list start) and the sort key
   `" :%f"`. While walking it also emits the same BASE/CALL (`0x10`/`0x0a`) GE commands at
   `g_renderListCursor` as the normal flush, for every chunk whose `unk08` is 0. */

typedef struct RenderPacketDepthPair {
  RenderPacket *packet; /* +0x0 NULL terminates the array */
  float depth;          /* +0x4 -sortKey */
} RenderPacketDepthPair;

void GfxDumpRenderPackets(void)
{
  CoreNode *node;
  RenderPacketDepthPair *pairs;
  RenderPacketDepthPair *out;
  RenderPacket *packet;
  CoreNode *chunk;
  s32 count;
  s32 i;
  bool fromLow;
  u32 list;

  node = g_renderPacketList.head;
  count = CoreNodeChainCount(node);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(false);
  pairs = (RenderPacketDepthPair *)MemAlloc((count + 1) * sizeof(RenderPacketDepthPair), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();

  out = pairs;
  for (i = 0; node != NULL; node = node->next, i++) {
    out->packet = (RenderPacket *)node;
    pairs[i].depth = -((RenderPacket *)node)->sortKey;
    out++;
  }
  out->packet = NULL;
  GfxCombSortByDepth(pairs, count);

  packet = pairs[0].packet;
  /* Freed before the walk below still reads it (original bug, harmless on this allocator). */
  MemLock();
  MemFree(pairs, NULL, 0);
  MemUnlock();

  printf("----------DumpRender-----------\n");
  for (i = 1; packet != NULL; i++) {
    printf("$%07x:", (u32)(uintptr_t)packet);
    chunk = packet->chunkHead;
    if (chunk != NULL) {
      for (;;) {
        printf("($%07x,$%07x)", (u32)(uintptr_t)chunk, chunk->id);
        if (chunk->unk08 == 0) {
          list = chunk->id;
          g_renderListCursor[0] = ((list >> 24) & 0xf) << 16 | 0x10000000;
          g_renderListCursor[1] = (list & 0xffffff) | 0x0a000000;
          g_renderListCursor += 2;
        }
        chunk = chunk->next;
        if (chunk == NULL)
          break;
        printf(",");
      }
    }
    printf(" :%f\n", (double)packet->sortKey);
    packet = pairs[i].packet;
  }
}
