// bdc 0x0894495c UiStaffCreditDraw
#include "bdc.h"

/* Draw (vtable slot 4) of the staff credits screen (task id 3004): submits its sprite layer and
   overlays to render packets. */

void UiStaffCreditDraw(UiScreen *screen)

{
  UiStaffCredit *credit = (UiStaffCredit *)screen;
  GfxSpriteLayer *layer;
  s32 i;

  layer = credit->base.spriteLayer;
  if (layer != (GfxSpriteLayer *)0x0) {
    GfxSpriteLayerDraw(layer, GfxNewRenderPacket(10.0f));
  }
  for (i = 0; i < 20; i++) {
    layer = credit->overlays[i];
    if (layer != (GfxSpriteLayer *)0x0) {
      GfxSpriteLayerDraw(layer, GfxNewRenderPacket(20.0f));
    }
  }
}
