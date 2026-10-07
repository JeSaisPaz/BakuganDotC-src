// bdc 0x088c83c0 UiTalkBalloonFadeIconUpdate
#include "bdc.h"

/* Kind 4 update of a talk balloon sprite (`UiTalkBalloonSprite`: `GfxSprite` subclass, vtable
   `0x08af2d34`). Forces state 3 as soon as the owner's cursor state (`UiTalkBalloonGetCursorState`)
   is nonzero. State 0 fades in (+0.2 per frame, then state 1); state 1 fades out by 0.018 per frame
   while shrinking `size` by 0.25; state 3 shrinks by 1.0 (down to 1.0) and fades by 0.3. When the
   alpha reaches 0 in state 1 or 3 the sprite deletes itself (vtable slot 1, arg 3) and returns 1.
   Otherwise it eases its y (`scaleY`, 0.7 per frame) toward the owner's current choice line
   (`choiceTop + lineHeight * choiceIndex`), takes x from the owner's text origin, and circles it by
   6 px on an angle of `(-frame & 31) * 0.19635 - phase` (phase += 0.1 per frame); returns 0.
   The angle is in radians (`vcos.s`/`vsin.s` after the bank's 2/pi scale). */

s32 UiTalkBalloonFadeIconUpdate(GfxSprite *sprite)

{
  UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
  UiTalkBalloon *owner;
  const VtblEntry *entry;
  s32 state;
  float alpha;
  float size;
  float y;
  float phase;
  float angle;

  if (UiTalkBalloonGetCursorState(self->owner) != 0) {
    self->state = 3;
  }
  state = self->state;
  if (state == 0) {
    alpha = self->base.alpha;
    if (alpha < 1.0f) {
      self->base.alpha = alpha + 0.2f;
    }
    else {
      self->base.alpha = 1.0f;
      self->state = state + 1;
    }
  }
  else if (state == 1) {
    alpha = self->base.alpha - 0.018f;
    self->base.alpha = alpha;
    if (alpha <= 0.0f) {
      if (sprite != (GfxSprite *)0x0) {
        entry = &((const VtblEntry *)self->base.vtable)[1];
        ((void (*)(void *, int))entry->fn)((u8 *)sprite + entry->delta, 3);
      }
      return 1;
    }
    size = self->size - 0.25f;
    self->size = size;
    GfxSpriteSetSize(sprite, size, size);
  }
  else if (state == 3) {
    size = self->size;
    if (!(size <= 1.0f)) {
      size = size - 1.0f;
      self->size = size;
    }
    GfxSpriteSetSize(sprite, size, size);
    alpha = self->base.alpha;
    if (alpha <= 0.0f) {
      if (sprite != (GfxSprite *)0x0) {
        entry = &((const VtblEntry *)self->base.vtable)[1];
        ((void (*)(void *, int))entry->fn)((u8 *)sprite + entry->delta, 3);
      }
      return 1;
    }
    self->base.alpha = alpha - 0.3f;
  }
  y = self->base.scaleY;
  owner = self->owner;
  y = y + ((owner->choiceTop + owner->lineHeight * (float)owner->choiceIndex) - y) * 0.7f;
  self->base.scaleY = y;
  self->base.posX = owner->origin[0];
  self->base.posY = y;
  phase = self->phase + 0.1f;
  self->phase = phase;
  angle = (float)(-self->frame & 0x1f) * 0.19635f - phase;
  self->base.posX = self->base.posX + __builtin_cosf(angle) * 6.0f;
  self->base.posY = self->base.posY + __builtin_sinf(angle) * 6.0f;
  return 0;
}
