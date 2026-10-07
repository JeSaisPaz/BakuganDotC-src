// bdc 0x0899c570 UiWorldMapStageSpriteDone
#include "bdc.h"

/* Advances the fade tween (flags 9, 16 frames) of stage-list sprite `index` of
   `UiWorldMap` (record `+0x74 + index * 0x28`); returns true when finished. */

bool UiWorldMapStageSpriteDone(UiScreen *screen, u8 out, int index)

{
  GfxSprite *sprite = ((GfxSprite **)screen->data)[index];
  UiTween *tween = &((UiWorldMap *)screen)->spriteTween[index];

  if (out == 0) {
    return UiTweenUpdate(1.0f, 1.0f, 16.0f, 0, sprite, tween, 9);
  }
  return UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprite, tween, 9);
}
