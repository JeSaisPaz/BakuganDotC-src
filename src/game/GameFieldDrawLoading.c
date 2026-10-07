// bdc 0x088c2fe0 GameFieldDrawLoading
#include "bdc.h"

/* Draw handler of the field task (id 500, `GameFieldCtor`) for phase 0 (table `0x08a91b90`):
   renders the 2D sprite layer `+0x5f4` into a packet (`GfxSpriteLayerDraw`) and adds a framebuffer copy
   packet (`GfxPacketCopyFramebuffer`). */

void GameFieldDrawLoading(CoreTask *task)
{
  void *packet;

  packet = GfxNewRenderPacket(1.0f);
  GfxSpriteLayerDraw(((GameFieldTask *)task)->spriteLayer, packet);
  packet = GfxNewRenderPacket(2.0f);
  GfxPacketCopyFramebuffer(packet, (void *)0);
}
