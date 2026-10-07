// bdc 0x089a548c UiTweenBeginSlide
#include "bdc.h"

/* `UiTweenBegin` plus a positional slide: additionally offsets the sprite's X (`flags & 4`) or Y
   (`flags & 8`) by `slideFrom` and records the start/end/delta positions that `UiTweenUpdate`
   interpolates. */

void UiTweenBeginSlide(float startScale, float slideFrom, float slideTo, u8 fadeOut, GfxSprite *sprite, UiTween *tween, u8 flags)
{
  if (fadeOut == 0) {
    tween->t = 0.0f;
    if ((flags & 1) != 0) {
      tween->startAlpha = sprite->alpha;
    }
    if ((flags & 2) != 0) {
      GfxSpriteCenterPivot(sprite);
      sprite->flags |= 0x20;
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
  if ((flags & 4) != 0) {
    tween->slideEnd = (s16)(s32)(sprite->posX + slideTo);
    sprite->posX = sprite->posX + slideFrom;
    tween->slideStart = (s16)(s32)sprite->posX;
    tween->slideDelta = tween->slideEnd - tween->slideStart;
  }
  else if ((flags & 8) != 0) {
    tween->slideEnd = (s16)(s32)(sprite->posY + slideTo);
    sprite->posY = sprite->posY + slideFrom;
    tween->slideStart = (s16)(s32)sprite->posY;
    tween->slideDelta = tween->slideEnd - tween->slideStart;
  }
}
