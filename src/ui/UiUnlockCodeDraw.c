// bdc 0x08992694 UiUnlockCodeDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the UiUnlockCode screen (task id 316): if it has a sprite layer, emits
   layer-mask groups 1, 2 and 4 at render depths 350, 320 and 360; then each existing text printer
   (`keyText`, `digitText`) gets its outline colour set to `g_colorWhite` and is drawn at depth 360. */

void UiUnlockCodeDraw(UiUnlockCode *self)
{
  void *packet;
  UiTextPrinter *text;

  if (self->base.spriteLayer != NULL) {
    packet = GfxNewRenderPacket(350.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 1);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(320.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 2);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
    packet = GfxNewRenderPacket(360.0f);
    GfxSpriteLayerSetLayerMask(self->base.spriteLayer, 4);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  if (self->keyText != NULL) {
    text = self->keyText;
    text->outlineColor[0] = g_colorWhite.x;
    text->outlineColor[1] = g_colorWhite.y;
    text->outlineColor[2] = g_colorWhite.z;
    text->outlineColor[3] = g_colorWhite.w;
    text = self->keyText;
    GfxSpriteLayerDraw(&text->layer, GfxNewRenderPacket(360.0f));
  }
  if (self->digitText != NULL) {
    text = self->digitText;
    text->outlineColor[0] = g_colorWhite.x;
    text->outlineColor[1] = g_colorWhite.y;
    text->outlineColor[2] = g_colorWhite.z;
    text->outlineColor[3] = g_colorWhite.w;
    text = self->digitText;
    GfxSpriteLayerDraw(&text->layer, GfxNewRenderPacket(360.0f));
  }
}
