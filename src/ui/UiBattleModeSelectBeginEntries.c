// bdc 0x089b00ec UiBattleModeSelectBeginEntries
#include "bdc.h"

/* Prepares the entry-panel animation of `UiBattleModeSelect` (panels =
   sprites 4..5, `data+0x10/0x14`; per-panel state `tweens[4..5]`). Opening: centres
   sprites 4..7, saves the home position of panel 0 (`+0x57c/+0x580`), shows the panels at x = -224
   tinted by state (selected: alpha 1, others 0.4; disabled 0.6 grey), staggered by 2 frames each,
   and records a slide toward x = 704. Closing: hides the glow of the selected entry (sprite 6 +
   cursor) and snapshots the selected panel's alpha/scale for the fade-out. */

void UiBattleModeSelectBeginEntries(UiBattleModeSelect *self, u8 closing)
{
  GfxSprite **sprites;
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  u8 delay;

  sprites = (GfxSprite **)self->base.data;
  if (closing == 0) {
    for (i = 4; i < 8; i++) {
      GfxSpriteCenterPivot(sprites[i]);
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 1.0f, 0.0f);
      sprites = (GfxSprite **)self->base.data;
    }
    self->panelHomeX = sprites[4]->posX;
    self->panelHomeY = sprites[4]->posY;
    delay = 0;
    for (i = 4; i < 6; i++) {
      tween = &self->tweens[i];
      sprites[i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->cursor == i - 4) {
        if (self->entryEnabled[i - 4] != 0) {
          sprite->tint[0] = 1.0f;
          sprite->tint[1] = 1.0f;
          sprite->tint[2] = 1.0f;
        } else {
          sprite->tint[0] = 0.6f;
          sprite->tint[1] = 0.6f;
          sprite->tint[2] = 0.6f;
        }
        sprite->alpha = 1.0f;
      } else {
        if (self->entryEnabled[i - 4] != 0) {
          sprite->tint[0] = 1.0f;
          sprite->tint[1] = 1.0f;
          sprite->tint[2] = 1.0f;
        } else {
          sprite->tint[0] = 0.6f;
          sprite->tint[1] = 0.6f;
          sprite->tint[2] = 0.6f;
        }
        sprite->alpha = 0.4f;
      }
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      ((GfxSprite **)self->base.data)[i]->posX = -224.0f;
      tween->delay0b = delay;
      sprites = (GfxSprite **)self->base.data;
      tween->toggle07 = 0;
      tween->t = 0.0f;
      tween->slideStart = (s16)(int)sprites[i]->posX;
      tween->slideDelta = (s16)(int)(704.0f - sprites[i]->posX);
      delay += 2;
    }
  } else {
    sprites[self->cursor + 6]->flags &= ~1u;
    i = self->cursor;
    sprites = (GfxSprite **)self->base.data;
    self->tweens[i + 4].t = 0.0f;
    self->tweens[i + 4].startAlpha = sprites[i + 4]->alpha;
    self->tweens[i + 4].startScale = sprites[i + 4]->scaleX;
  }
}
