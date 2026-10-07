// bdc 0x08910720 UiPauseDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the pause menu (task id 410): submits the sprite layer (if any) into a
   render packet at sort key 500, zoomed by `scaleX/scaleY` via `GfxSpriteLayerSetZoom`; then for
   each of the two hint text printers that exists, zooms it the same way, sets its outline alpha
   to `hintAlpha`, prints `hintText[i]` at (230, 220 + 17*i) through vtable slot 2 and draws it
   through slot 5 into a packet at sort key 550; finally `UiHelpLineDraw`. */

void UiPauseDraw(UiPause *self)
{
  void *packet;
  UiTextPrinter *printer;
  const VtblEntry *entry;
  int i;
  int y;

  packet = GfxNewRenderPacket(500.0f);
  if (self->base.spriteLayer != NULL) {
    GfxSpriteLayerSetZoom(self->scaleX, self->scaleY, self->base.spriteLayer, NULL);
    GfxSpriteLayerDraw(self->base.spriteLayer, packet);
  }
  y = 220;
  for (i = 0; i < 2; i++) {
    if (self->hintPrinters[i] != NULL) {
      GfxSpriteLayerSetZoom(self->scaleX, self->scaleY, &self->hintPrinters[i]->layer, NULL);
      self->hintPrinters[i]->outlineColor[3] = self->hintAlpha;
      printer = self->hintPrinters[i];
      entry = &printer->layer.vtbl[2];
      ((void (*)(float, float, float, void *, char *, s32, s32, s32))entry->fn)(
          230.0f, (float)y, 0.0f, (u8 *)printer + entry->delta, self->hintText[i], 1, 0, 0);
      printer = self->hintPrinters[i];
      entry = &printer->layer.vtbl[5];
      packet = GfxNewRenderPacket(550.0f);
      ((void (*)(void *, void *))entry->fn)((u8 *)printer + entry->delta, packet);
    }
    y += 17;
  }
  UiHelpLineDraw();
}
