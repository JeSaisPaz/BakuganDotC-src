// bdc 0x08929c90 UiHologramViewStartFade
#include "bdc.h"

/* Initialises the screen fade record `+0x6f4` of the hologram detail view (`UiHologramViewCtor`,
   task 392; view kind `+0x485`) to transparent (`out == 0`) or opaque, and resets the sprite
   layer's fade (`GfxSpriteLayerSetZoom`). */

void UiHologramViewStartFade(UiHologramView *self, char out)

{
  float scale;
  
  if (out == '\0') {
    scale = 0.0;
    self->screenFade[0] = 0.0;
    self->screenFade[1] = 0.0;
    self->screenFade[2] = 0.0;
    self->screenFade[3] = 0.0;
  }
  else {
    scale = 1.0;
    self->screenFade[2] = 0.0;
    self->screenFade[0] = 1.0;
    self->screenFade[1] = 1.0;
    self->screenFade[3] = 1.0;
  }
  GfxSpriteLayerSetZoom(scale,0.0,(self->base).spriteLayer,(float *)0x0);
  return;
}

