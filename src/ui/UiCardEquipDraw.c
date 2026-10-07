// bdc 0x08969934 UiCardEquipDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiCardEquip screen (task id 303). When the sprite layer `+0x18`
   exists: draws the screen background (`UiScreenDrawBg`); if the fade flag is set, emits a
   full-screen 480x272 black rectangle with alpha `fade` into a render packet at depth 800 (screen
   camera, 2D state, alpha blend); then draws the sprite layer (mask 1) into a packet at depth 1000.
   Always finishes with `UiCardEquipDrawCardName` and `UiCardEquipDrawCardHelp`. */

void UiCardEquipDraw(UiCardEquip *self)
{
  RenderPacket *packet;
  u32 *list;
  ScePspFVector4 colour __attribute__((aligned(16)));
  float rect[4];

  if (self->base.spriteLayer != NULL) {
    UiScreenDrawBg(&self->base);
    if (self->fadeOn != 0) {
      rect[0] = 0.0f;
      rect[1] = 0.0f;
      rect[2] = 480.0f;
      rect[3] = 272.0f;
      packet = GfxNewRenderPacket(800.0f);
      list = GfxPacketBeginChunk(packet);
      list = GfxDlCall2DState(list);
      list = GfxCameraDlWrite(g_gfxScreenCamera, list, 0xffffffffu);
      list = GfxDlSetBlendState(list, &g_colorWhite, 0, 1);
      colour.x = 0.0f;
      colour.y = 0.0f;
      colour.z = 0.0f;
      colour.w = self->fade;
      list = GfxDlDrawColorRect(packet, list, rect, &colour);
      GfxPacketEndChunk(packet, list);
    }
    packet = GfxNewRenderPacket(1000.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  UiCardEquipDrawCardName(self);
  UiCardEquipDrawCardHelp(self);
}
