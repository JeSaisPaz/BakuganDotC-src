// bdc 0x088054b4 UiNameEntryAnimateKeyPress
#include "bdc.h"

/* Per-frame key-press pop animation of `UiNameEntry`: while `+0xbc` is set, grows
   the highlight sprite (sprite 35 of the layout array `+0x1c`) by 1/12 per frame and fades its
   alpha (`+0xbc` of the sprite) by 1/6; after 6 frames clears the flag and the frame counter
   `+0xc0`. Started by `UiNameEntryStartKeyPress`. */

void UiNameEntryAnimateKeyPress(UiNameEntry *self)

{
  GfxSprite **sprites;
  GfxSprite *sprite;
  float w;
  float h;
  float scale;
  
  if (self->keyPressActive != '\0') {
    self->keyPressFrame = self->keyPressFrame + 1;
    sprites = (GfxSprite **)self->base.data;
    sprites[35]->alpha = sprites[35]->alpha - 0.16666667f;
    scale = (float)self->keyPressFrame * 0.083333336f + 1.0f;
    w = GfxSpriteGetWidth(sprites[35]);
    sprite = sprites[35];
    h = GfxSpriteGetHeight(sprite);
    GfxSpriteSetSize(sprite,w * scale,h * scale);
    if (5 < self->keyPressFrame) {
      self->keyPressFrame = 0;
      self->keyPressActive = '\0';
    }
  }
  return;
}

