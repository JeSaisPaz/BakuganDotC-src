// bdc 0x089a51ec UiSpriteSetHighlight
#include "bdc.h"

/* Sets or clears a sprite's highlight tint: `on` adds 0.3 to red and green (colour-add
   `+0xc0/+0xc4`), off clears them; blue stays 0 and alpha-add 1. */

void UiSpriteSetHighlight(GfxSprite *sprite, u8 on)

{
  if (on != '\0') {
    sprite->addColor[2] = 0.0;
    sprite->addColor[0] = 0.3;
    sprite->addColor[1] = 0.3;
    sprite->addColor[3] = 1.0;
    return;
  }
  sprite->addColor[0] = 0.0;
  sprite->addColor[1] = 0.0;
  sprite->addColor[2] = 0.0;
  sprite->addColor[3] = 1.0;
  return;
}

