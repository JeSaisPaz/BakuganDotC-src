// bdc 0x089220d0 UiHologramGallerySetAttributeColor
#include "bdc.h"

/* Sets a sprite's colour (`+0xb0..+0xbc`) to the RGB of attribute `attr` from the table
   `g_hologramAttributeColors` (alpha 1). */

void UiHologramGallerySetAttributeColor(UiHologramGallery *self, GfxSprite *sprite, u32 attr)

{
  float colors[18];
  u32 idx;

  idx = attr & 0xff;
  memcpy(colors, g_hologramAttributeColors, 0x48);
  sprite->tint[0] = colors[idx * 3];
  sprite->tint[1] = colors[idx * 3 + 1];
  sprite->tint[2] = colors[idx * 3 + 2];
  sprite->alpha = 1.0f;
}
