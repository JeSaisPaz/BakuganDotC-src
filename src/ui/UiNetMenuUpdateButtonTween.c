// bdc 0x0894f0ec UiNetMenuUpdateButtonTween
#include "bdc.h"

/* Advances the host/join button tween of the network-play menu (task 1999, `UiNetMenuCtor`)
   started by `UiNetMenuStartButtonTween`. Opening (`out` = 0): each button sprite 4/5 waits
   `order` frames, then eases from `fromX` by `distance` (t += 1/8, t²) to X 704, resets to scale 1,
   waits 4 frames and eases back (t += 1/16) to `buttonX` (chosen button) or X −224 (the other),
   ending with tint 1 (enabled) or 0.6 (disabled) and alpha 1; returns true on a frame where both
   buttons have finished the second leg. Closing (`out` = 1): grows the chosen button's scale and
   fades its alpha by t² (t += 1/8); when t reaches 1 hides the sprite and returns true. */

bool UiNetMenuUpdateButtonTween(UiScreen *screen, bool out)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  UiNetMenuButtonSlide *slide;
  GfxSprite *sprite;
  u8 done;
  s32 i;
  s32 idx;
  float t;
  float u;
  float from;
  float dist;

  done = 0;
  if (!out) {
    for (i = 4; i < 6; i++) {
      slide = &menu->buttonSlide[i - 4];
      if (slide->order != 0) {
        slide->order--;
        continue;
      }
      from = (float)slide->fromX;
      dist = (float)slide->distance;
      if (slide->fadingOut == 0) {
        t = slide->t + 0.125f;
        slide->t = t;
        ((GfxSprite **)screen->data)[i]->posX = from + t * t * dist;
        if (!(slide->t < 1.0f)) {
          ((GfxSprite **)screen->data)[i]->posX = 704.0f;
          ((GfxSprite **)screen->data)[i]->scaleX = 1.0f;
          ((GfxSprite **)screen->data)[i]->scaleY = 1.0f;
          sprite = ((GfxSprite **)screen->data)[i];
          GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
          slide->fadingOut = 1;
          slide->t = 0.0f;
          slide->order = 4;
          slide->fromX = (s16)(s32)((GfxSprite **)screen->data)[i]->posX;
          if (menu->choice == i - 4) {
            slide->distance =
                (s16)(s32)(((GfxSprite **)screen->data)[i]->posX - menu->buttonX);
          } else {
            slide->distance = (s16)(s32)(((GfxSprite **)screen->data)[i]->posX - -224.0f);
          }
        }
      } else {
        t = slide->t + 0.0625f;
        u = t - 1.0f;
        slide->t = t;
        ((GfxSprite **)screen->data)[i]->posX = from - (1.0f - u * u) * dist;
        if (!(slide->t < 1.0f)) {
          if (menu->choice == i - 4) {
            ((GfxSprite **)screen->data)[i]->posX = menu->buttonX;
          } else {
            ((GfxSprite **)screen->data)[i]->posX = -224.0f;
          }
          sprite = ((GfxSprite **)screen->data)[i];
          if (menu->enabled[i - 4] == 0) {
            sprite->tint[0] = 0.6f;
            sprite->tint[1] = 0.6f;
            sprite->tint[2] = 0.6f;
            sprite->alpha = 1.0f;
          } else {
            sprite->tint[0] = 1.0f;
            sprite->tint[1] = 1.0f;
            sprite->tint[2] = 1.0f;
            sprite->alpha = 1.0f;
          }
          done++;
        }
      }
    }
    return done == 2;
  }

  idx = menu->choice;
  slide = &menu->buttonSlide[idx];
  t = slide->t + 0.125f;
  slide->t = t;
  ((GfxSprite **)screen->data)[idx + 4]->alpha = slide->startAlpha - t * t;
  ((GfxSprite **)screen->data)[idx + 4]->scaleX = slide->startScale + slide->t * slide->t;
  ((GfxSprite **)screen->data)[idx + 4]->scaleY = ((GfxSprite **)screen->data)[idx + 4]->scaleX;
  sprite = ((GfxSprite **)screen->data)[idx + 4];
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  if (!(slide->t < 1.0f)) {
    ((GfxSprite **)screen->data)[idx + 4]->flags &= ~1u;
    return true;
  }
  return false;
}
