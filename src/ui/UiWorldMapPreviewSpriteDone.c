// bdc 0x0899b644 UiWorldMapPreviewSpriteDone
#include "bdc.h"

/* Advances the tween of preview sprite `index` of `UiWorldMap` (record `+0x74 +
   index * 0x28`; scale 1.5 → 1 coming in, 1 → 1.5 going out, 16 frames); returns true when
   finished. */

bool UiWorldMapPreviewSpriteDone(UiScreen *screen, u8 out, int index)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  GfxSprite *sprite = ((GfxSprite **)screen->data)[index];
  UiTween *tween = (UiTween *)((u8 *)map->spriteTween + index * sizeof(UiTween));

  if (out == 0) {
    return UiTweenUpdate(1.5f, 1.0f, 16.0f, 0, sprite, tween, 7);
  }
  return UiTweenUpdate(1.0f, 1.5f, 16.0f, out, sprite, tween, 7);
}
