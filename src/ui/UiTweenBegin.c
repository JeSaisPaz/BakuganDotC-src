// bdc 0x089a53c8 UiTweenBegin
#include "bdc.h"

/* Starts a UI sprite appear/disappear transition: resets the tween's progress and snapshots the
   sprite's alpha (flags bit 0) and/or scale (flags bit 1) into the tween state that `UiTweenUpdate`
   then animates each frame. When fadeOut is 0 (appear) and bit 1 is set, the sprite is first
   centre-pivoted and set to startScale; the snapshot of scaleX is taken after that. When fadeOut is
   non-zero, the current values are snapshotted unchanged. */

void UiTweenBegin(float startScale, u8 fadeOut, GfxSprite *sprite, UiTween *tween, u8 flags)

{
  if (fadeOut == 0) {
    tween->t = 0.0f;
    if ((flags & 1) != 0) {
      tween->startAlpha = sprite->alpha;
    }
    if ((flags & 2) != 0) {
      GfxSpriteCenterPivot(sprite);
      sprite->flags = sprite->flags | 0x20;
      UiSpriteSetScaleRotation(sprite, startScale, startScale, 0.0f);
      tween->startScale = sprite->scaleX;
    }
  }
  else {
    tween->t = 0.0f;
    if ((flags & 1) != 0) {
      tween->startAlpha = sprite->alpha;
    }
    if ((flags & 2) != 0) {
      tween->startScale = sprite->scaleX;
    }
  }
}
