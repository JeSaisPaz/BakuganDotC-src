// bdc 0x089293d4 UiHologramViewDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the hologram detail view screen (task id 392): when the sprite layer
   exists, draws the screen background, then (while `blinkOn`) a full-screen 480x272 black
   rectangle with alpha `blinkBaseA` in a packet at sort key 1000, then the sprite layer (mask 1)
   at sort key 2000; always ends with the text overlay `UiHologramViewDrawText`. */

void UiHologramViewDraw(UiHologramView *self)
{
  RenderPacket *packet;
  u32 *list;
  ScePspFVector4 colour __attribute__((aligned(16)));
  float rect[4];

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    if (self->blinkOn != 0) {
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = 480.0f;
      rect[3] = 272.0f;
      packet = GfxNewRenderPacket(1000.0f);
      list = GfxPacketBeginChunk(packet);
      list = GfxDlCall2DState(list);
      list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffff);
      list = GfxDlSetBlendState(list, &g_colorWhite, 0, 1);
      colour.w = self->blinkBaseA;
      colour.x = 0.0f;
      colour.y = 0.0f;
      colour.z = 0.0f;
      list = GfxDlDrawColorRect(packet, list, rect, &colour);
      GfxPacketEndChunk(packet, list);
    }
    packet = GfxNewRenderPacket(2000.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  UiHologramViewDrawText(self);
}
