// bdc 0x089f2148 GfxNewRenderPacket
#include "bdc.h"

/* Allocates the next 0x34-byte render packet from the per-frame pool `g_renderPacketPool`
   (`g_renderPacketCount`++), constructs it with `sortKey` (stored at `+0x24`, vtable
   `0x08af582c`, chunk list `+0x28` = NULL) and appends it to `g_renderPacketList`; returns the
   packet. Drawing code then opens a GE display-list chunk on it with `GfxPacketBeginChunk` and closes it
   with `GfxPacketEndChunk`; at end of frame `GfxFlushRenderPackets` sorts packets by key and CALLs
   their chunks. */

void *GfxNewRenderPacket(float sortKey)
{
    CoreNode *packet;
    CoreNode *node;

    packet = &((RenderPacket *)g_renderPacketPool)[g_renderPacketCount].node;
    g_renderPacketCount = g_renderPacketCount + 1;
    node = NULL;
    if (packet != NULL) {
        GfxRenderPacketInit(packet, sortKey);
        node = packet;
    }
    return CoreNodeGroupAppend(node, &g_renderPacketList);
}
