// bdc 0x0898290c UiCollectionCardDraw
#include "bdc.h"

/* Draw method (vtable `0x08af4fac` slot +0x24) of the card collection screen (task 313,
   `maybe_UiScreen313Ctor`): when the sprite layer exists, draws the background
   (`UiScreenDrawBg`) and the sprite layer five times with masks 1/2/4/8/0x10 in render packets
   z 50/200/300/400/500; then for each active dim fade `dim[i]` (i = 0 in packet z 250, i = 1 in
   z 350) a full-screen black rectangle `{0, 0, 480, 272}` with alpha `dim[i].value` (2D state,
   screen camera `g_gfxScreenCamera`, blend state white/untextured/alpha); finally the card
   description (`UiCollectionCardDrawHelp`). */

void UiCollectionCardDraw(UiCollectionCard *self)

{
  void *packet;
  u32 *list;
  int i;
  ScePspFVector4 colour __attribute__((aligned(16)));
  float rect[4] __attribute__((aligned(16)));

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    packet = GfxNewRenderPacket(50.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(200.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(300.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 4);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(400.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 8);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(500.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 0x10);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  for (i = 0; i < 2; i++) {
    if (self->dim[i].active != 0) {
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = 480.0f;
      rect[3] = 272.0f;
      if (i == 0) {
        packet = GfxNewRenderPacket(250.0f);
      } else {
        packet = GfxNewRenderPacket(350.0f);
      }
      list = GfxPacketBeginChunk(packet);
      list = GfxDlCall2DState(list);
      list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffff);
      list = GfxDlSetBlendState(list, &g_colorWhite, 0, 1);
      colour.x = 0.0f;
      colour.y = 0.0f;
      colour.z = 0.0f;
      colour.w = self->dim[i].value;
      list = GfxDlDrawColorRect(packet, list, rect, &colour);
      GfxPacketEndChunk(packet, list);
    }
  }
  UiCollectionCardDrawHelp(self);
}
