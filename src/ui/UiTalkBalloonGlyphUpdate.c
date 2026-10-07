// bdc 0x088c7b1c UiTalkBalloonGlyphUpdate
#include "bdc.h"

/* Kind 0 update of a talk balloon glyph (`UiTalkBalloonSprite`, `GfxSprite` subclass). State 0
   fades in by 0.2 per frame (at once when `g_talkBalloonSkipAnim` is set), then advances to state
   1; it sets `size` 1.3 every frame. State 1, for a choice glyph (`holdTimer` = 1-based choice line)
   right of the owner's origin + 12: on the selected line while the owner is in state 3 it follows
   the cursor state (`UiTalkBalloonGetCursorState`): 0 flashes the glyph every 32 frames (red tint
   1.3, UV inset 0.5, `phase` = pi) and fades the red by 0.07, 1..3 fades `size` by 0.05 into a
   red-to-blue tint (tint and alpha clamped to [0, 1]); elsewhere the red fades by 0.2. A positive `phase`
   drops by pi/12 per frame and sizes the glyph 16 + 8 sin(phase) (undoing the inset at 16x16 when
   it reaches 0). State 100 deletes the sprite (vtable slot 1, arg 3) and returns 1; every other
   path returns 0. */

s32 UiTalkBalloonGlyphUpdate(GfxSprite *sprite)

{
  UiTalkBalloonSprite *self = (UiTalkBalloonSprite *)sprite;
  UiTalkBalloon *owner;
  const VtblEntry *entry;
  s32 state;
  s32 cursor;
  float tint;
  float size;
  float phase;

  state = self->state;
  if (state >= 2) {
    if (state != 100) {
      return 0;
    }
    if (sprite != (GfxSprite *)0x0) {
      entry = &((const VtblEntry *)self->base.vtable)[1];
      ((void (*)(void *, int))entry->fn)((u8 *)sprite + entry->delta, 3);
    }
    return 1;
  }
  if (state < 0) {
    return 0;
  }
  if (state == 0) {
    if (g_talkBalloonSkipAnim != 0) {
      self->base.alpha = 1.0f;
    }
    if (self->base.alpha < 1.0f) {
      self->base.alpha = self->base.alpha + 0.2f;
    }
    else {
      self->base.alpha = 1.0f;
      self->state = self->state + 1;
    }
    self->size = 1.3f;
    return 0;
  }

  /* state 1 */
  if (self->holdTimer <= 0) {
    return 0;
  }
  if (self->base.posX <= self->owner->origin[0] + 12.0f) {
    return 0;
  }
  if (self->holdTimer - 1 == self->owner->choiceIndex && self->owner->state == 3) {
    cursor = UiTalkBalloonGetCursorState(self->owner);
    if (cursor > 0) {
      if (cursor < 4) {
        if (!(self->size <= 0.0f)) {
          self->size = self->size - 0.05f;
        }
        size = self->size;
        self->base.tint[0] = 1.0f - size;
        self->base.tint[2] = size;
        self->base.tint[1] = 0.0f;
        /* tint[0..2] and alpha are one 16-byte vector: clamp all four to [0, 1] (vsat0.q) */
        self->base.tint[0] = VfSat0(self->base.tint[0]);
        self->base.tint[1] = VfSat0(self->base.tint[1]);
        self->base.tint[2] = VfSat0(self->base.tint[2]);
        self->base.alpha = VfSat0(self->base.alpha);
      }
    }
    else if (cursor == 0) {
      owner = self->owner;
      if (((self->frame + owner->choiceFrame) & 0x1f) == 0) {
        phase = self->phase;
        self->base.tint[0] = 1.3f;
        if (phase == 0.0f) {
          GfxSpriteInsetUv(0.5f, sprite);
        }
        self->phase = 3.1415927f;
      }
      else {
        tint = self->base.tint[0] - 0.07f;
        self->base.tint[0] = tint;
        if (tint < 0.0f) {
          self->base.tint[0] = 0.0f;
        }
      }
    }
  }
  else {
    tint = self->base.tint[0] - 0.2f;
    self->base.tint[0] = tint;
    if (tint < 0.0f) {
      self->base.tint[0] = 0.0f;
    }
  }

  if (!(self->phase <= 0.0f)) {
    phase = self->phase - 0.2617994f;
    self->phase = phase;
    if (phase <= 0.0f) {
      self->phase = 0.0f;
      GfxSpriteInsetUv(-0.5f, sprite);
      GfxSpriteSetSize(sprite, 16.0f, 16.0f);
    }
    else {
      size = __builtin_sinf(self->phase) * 8.0f + 16.0f;
      GfxSpriteSetSize(sprite, size, size);
    }
  }
  return 0;
}
