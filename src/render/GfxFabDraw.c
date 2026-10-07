// bdc 0x089f86f0 GfxFabDraw
#include "bdc.h"

/* Draws a `.fab` animation: when it has a root clip (`+0x94`), opens a render packet at depth
   `+0x98` (`GfxNewRenderPacket`, `GfxPacketBeginChunk`), sets 2D state (`GfxDlCall2DState`,
   `GfxCameraDlWrite`), additive-free alpha blend (`0xdf000032`), linear filtering, then draws the
   root clip (`GfxFabClipDraw`) with the fab transform `+0x30` and colour `+0xa0`, and closes the
   chunk. */

void GfxFabDraw(GfxFab *fab)

{
  void *packet;
  u32 *dl;
  
  if (fab->rootClip != 0) {
    packet = GfxNewRenderPacket(fab->depth);
    dl = GfxPacketBeginChunk(packet);
    dl = GfxDlCall2DState(dl);
    dl = GfxCameraDlWrite(g_gfxScreenCamera,dl,0xffffffff);
    *dl = 0xdf000032;
    dl[1] = 0xe0000000;
    dl[2] = 0xe1000000;
    dl[3] = 0xc6000101;
    dl = GfxDlCallSpriteState(dl + 4);
    dl = GfxFabClipDraw(fab->rootClip,dl,&fab->transform[0][0],
                            fab->color,&g_colorBlack);
    GfxPacketEndChunk(packet,dl);
  }
  return;
}

