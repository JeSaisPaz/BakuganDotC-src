// bdc 0x089f1218 GfxRenderPacketInit
#include "bdc.h"

/* Constructor of a render packet (0x34 bytes): runs the generic list-node ctor
   `CoreNodeCtor(packet, 0)`, then sets the render-packet vtable `g_gfxRenderPacketVtbl` at `+0x20`,
   the float sort key at `+0x24` and clears the display-list chunk chain at `+0x28`; returns `packet`. */

void *GfxRenderPacketInit(void *packet, float sortKey)

{
  RenderPacket *p = (RenderPacket *)packet;

  CoreNodeCtor(&p->node, (CoreNode *)0x0);
  p->node.vtable = g_gfxRenderPacketVtbl;
  p->sortKey = sortKey;
  p->chunkHead = (CoreNode *)0x0;
  return packet;
}
