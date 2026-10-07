// bdc 0x0899cd58 UiWorldMapAreaListFadeDone
#include "bdc.h"

/* Advances the fade tweens (16 frames, `UiTweenUpdate` flags 1) of the area-selection sprites of
   `UiWorldMap` (area buttons 0..7, labels 8..0xf and 0x10..0x17, decorations
   0x2a..0x31) while switching to or from the stage list; returns true when at least one of those
   tweens reports finished (they run in lockstep, so in practice: when the fade is done). */

bool UiWorldMapAreaListFadeDone(UiScreen *screen, u8 out)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  u8 finished = 0;
  int i;

  for (i = 0; i < 8; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)screen->data)[i],
                              &map->spriteTween[i], 1);
  }
  for (i = 8; i < 0x10; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)screen->data)[i],
                              &map->spriteTween[i], 1);
  }
  for (i = 0x10; i < 0x18; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)screen->data)[i],
                              &map->spriteTween[i], 1);
  }
  for (i = 0x2a; i < 0x32; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, ((GfxSprite **)screen->data)[i],
                              &map->spriteTween[i], 1);
  }
  return finished != 0;
}
