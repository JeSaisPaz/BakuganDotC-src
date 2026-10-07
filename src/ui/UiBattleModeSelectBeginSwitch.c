// bdc 0x089b124c UiBattleModeSelectBeginSwitch
#include "bdc.h"

/* Starts the panel swap of `UiBattleModeSelect` after a cursor move:
   flashes the arrow on the move side (`UiFlashStart` on sprite 2 + `switchDir`), clears the
   `toggle07` byte of tween slot 0, hides the old entry's glow (`UiBattleModeSelectShowGlow`), and
   records the slides (start/end/delta in tween slot 4 + panel): the old panel (`prevCursor`) leaves
   to x = 704 (direction 0) or -224 (direction 1), while the new one (`cursor`) is placed on the
   opposite side (-224 or 704) and slides to the home X `panelHomeX`. */

void UiBattleModeSelectBeginSwitch(UiBattleModeSelect *self)
{
  GfxSprite **sprites;
  UiTween *slide;
  s32 entry;
  float dist;

  sprites = (GfxSprite **)self->base.data;
  UiFlashStart(2.0f, sprites[2 + self->switchDir], 0, 0);
  self->tweens[0].toggle07 = 0;
  UiBattleModeSelectShowGlow(self, 0, (u8)self->prevCursor);

  /* old panel slides out */
  entry = self->prevCursor;
  sprites = (GfxSprite **)self->base.data;
  slide = &self->tweens[entry + 4];
  slide->slideStart = (s16)(s32)sprites[entry + 4]->posX;
  slide->slideEnd = (self->switchDir == 0) ? 704 : -224;
  dist = UiAbsDiff(sprites[entry + 4]->posX, (float)slide->slideEnd);
  self->tweens[self->prevCursor + 4].slideDelta = (s16)(s32)dist;

  /* new panel enters from the other side */
  sprites = (GfxSprite **)self->base.data;
  if (self->switchDir == 0) {
    sprites[self->cursor + 4]->posX = -224.0f;
  } else {
    sprites[self->cursor + 4]->posX = 704.0f;
  }
  entry = self->cursor;
  sprites = (GfxSprite **)self->base.data;
  slide = &self->tweens[entry + 4];
  slide->slideEnd = (s16)(s32)self->panelHomeX;
  slide->slideStart = (s16)(s32)sprites[entry + 4]->posX;
  dist = UiAbsDiff(sprites[entry + 4]->posX, (float)slide->slideEnd);
  self->tweens[self->cursor + 4].slideDelta = (s16)(s32)dist;
}
