// bdc 0x0894f8a4 UiNetMenuAnimateCursorMove
#include "bdc.h"

/* One frame of the button swap of the network-play menu (`UiNetMenu`, task 1999):
   steps flash slot 0; a repeat or press of the direction being moved (bit 0x80 for `dir` 0, 0x20
   for `dir` 1) speeds the slide up to 4, a press also sets `cursorMove.queued`. Advances
   `cursorMove.t` by 1/speed; the title sprite (sprite 0) alpha dips 1→0 over the first half and
   rises from 1 in the second, where the new choice's help text is printed once
   (`UiNetMenuSetHelpText`, `titleFadingOut`). The old and new buttons (sprites 4+prevChoice,
   4+choice) move from `fromX` by `(1 - (t-1)^2) * distance` (added for `dir` 0, subtracted for
   `dir` 1). Returns 0 while `t < 1`; otherwise snaps both to `toX`, sets the title alpha to 1 and
   returns 1. `titleHelpAlpha` mirrors the title alpha. */

s32 UiNetMenuAnimateCursorMove(UiScreen *screen)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  PadState *pad;
  GfxSprite **sprites;
  UiNetMenuButtonSlide *oldSlide;
  UiNetMenuButtonSlide *newSlide;
  float t;
  float from;
  float dist;
  float k;
  s32 oldIdx;
  s32 newIdx;

  UiFlashStep(0);
  pad = screen->pad;
  t = menu->cursorMove.t;
  sprites = (GfxSprite **)screen->data;
  if ((pad->repeat & 0x80) != 0) {
    if (menu->cursorMove.dir == 0) {
      menu->cursorMove.speed = 4.0f;
    }
  } else if ((pad->repeat & 0x20) != 0) {
    if (menu->cursorMove.dir == 1) {
      menu->cursorMove.speed = 4.0f;
    }
  }
  if ((pad->pressed & 0x80) != 0) {
    if (menu->cursorMove.dir == 0) {
      menu->cursorMove.speed = 4.0f;
      menu->cursorMove.queued = 1;
    }
  } else if ((pad->pressed & 0x20) != 0) {
    if (menu->cursorMove.dir == 1) {
      menu->cursorMove.queued = 1;
      menu->cursorMove.speed = 4.0f;
    }
  }
  t = t + 1.0f / menu->cursorMove.speed;
  menu->cursorMove.t = t;
  if (t < 0.5f) {
    sprites[0]->alpha = 1.0f - t * 2.0f;
  } else {
    sprites[0]->alpha = (t - 0.5f) * 2.0f + 1.0f;
    if (menu->titleFadingOut == 0) {
      UiNetMenuSetHelpText(screen, (u8)menu->choice);
      menu->titleFadingOut = 1;
    }
  }
  newIdx = menu->choice;
  oldIdx = menu->prevChoice;
  menu->titleHelpAlpha = ((GfxSprite **)screen->data)[0]->alpha;
  oldSlide = &menu->buttonSlide[oldIdx];
  newSlide = &menu->buttonSlide[newIdx];
  if (menu->cursorMove.dir == 0) {
    from = (float)oldSlide->fromX;
    dist = (float)oldSlide->distance;
    k = menu->cursorMove.t - 1.0f;
    ((GfxSprite **)screen->data)[4 + oldIdx]->posX = from + (1.0f - k * k) * dist;
    k = menu->cursorMove.t - 1.0f;
    dist = (float)newSlide->distance;
    from = (float)newSlide->fromX;
    ((GfxSprite **)screen->data)[4 + newIdx]->posX = from + (1.0f - k * k) * dist;
  } else {
    from = (float)oldSlide->fromX;
    dist = (float)oldSlide->distance;
    k = menu->cursorMove.t - 1.0f;
    ((GfxSprite **)screen->data)[4 + oldIdx]->posX = from - (1.0f - k * k) * dist;
    k = menu->cursorMove.t - 1.0f;
    dist = (float)newSlide->distance;
    from = (float)newSlide->fromX;
    ((GfxSprite **)screen->data)[4 + newIdx]->posX = from - (1.0f - k * k) * dist;
  }
  if (menu->cursorMove.t < 1.0f) {
    return 0;
  }
  ((GfxSprite **)screen->data)[4 + oldIdx]->posX = (float)oldSlide->toX;
  ((GfxSprite **)screen->data)[4 + newIdx]->posX = (float)newSlide->toX;
  ((GfxSprite **)screen->data)[0]->alpha = 1.0f;
  menu->titleHelpAlpha = ((GfxSprite **)screen->data)[0]->alpha;
  return 1;
}
