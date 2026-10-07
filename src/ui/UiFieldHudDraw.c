// bdc 0x088cefa0 UiFieldHudDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the field HUD (task id 3001): when not suspended (`UiFieldHudIsSuspended`), submits the
   sprite layer and the HUD overlay layer. */

void UiFieldHudDraw(UiFieldHud *self)

{
  void *packet;
  GfxSpriteLayer *layer;
  UiTextPrinter *printer;

  if (!UiFieldHudIsSuspended(self)) {
    packet = GfxNewRenderPacket(150.0f);
    layer = (self->base).spriteLayer;
    if (layer != (GfxSpriteLayer *)0x0) {
      GfxSpriteLayerDraw(layer,packet);
    }
    printer = self->hintPrinter;
    if (printer != (UiTextPrinter *)0x0) {
      packet = GfxNewRenderPacket(100.0f);
      GfxSpriteLayerDraw(&printer->layer,packet);
    }
  }
  return;
}
