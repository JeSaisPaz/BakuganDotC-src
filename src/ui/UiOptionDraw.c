// bdc 0x0897007c UiOptionDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiOption screen (task id 304). When the sprite layer `+0x18` exists:
   draws the screen background (`UiScreenDrawBg`); while a fade is active, a full-screen black
   rectangle with alpha `fadeLevel` at depth 1100 (screen camera `g_gfxScreenCamera`, 2D state,
   alpha blend); then the sprite layer at depth 1200. Always ends with `UiOptionDrawHelp`. */

void UiOptionDraw(UiOption *self)
{
  void *packet;
  u32 *list;
  ScePspFVector4 colour __attribute__((aligned(16)));
  float rect[4];

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    if (self->fadeActive != 0) {
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = 480.0f;
      rect[3] = 272.0f;
      packet = GfxNewRenderPacket(1100.0f);
      list = GfxPacketBeginChunk(packet);
      list = GfxDlCall2DState(list);
      list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffff);
      list = GfxDlSetBlendState(list, &g_colorWhite, 0, 1);
      colour.w = self->fadeLevel;
      colour.x = 0.0f;
      colour.y = 0.0f;
      colour.z = 0.0f;
      list = GfxDlDrawColorRect(packet, list, rect, &colour);
      GfxPacketEndChunk(packet, list);
    }
    packet = GfxNewRenderPacket(1200.0f);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  UiOptionDrawHelp(self);
}
