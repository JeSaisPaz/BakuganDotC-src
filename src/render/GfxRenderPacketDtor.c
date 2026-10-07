// bdc 0x089f1264 GfxRenderPacketDtor
#include "bdc.h"

/* Destructor of a render packet (vtable `g_gfxRenderPacketVtbl`): `CoreNodeDtor`, frees the packet when
   `flags & 1` (packets normally live in the per-frame pool, see `GfxNewRenderPacket`). */

void GfxRenderPacketDtor(CoreNode *packet, u32 flags)

{
  if (packet != (CoreNode *)0x0) {
    packet->vtable = g_gfxRenderPacketVtbl;
    CoreNodeDtor(packet,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(packet,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

