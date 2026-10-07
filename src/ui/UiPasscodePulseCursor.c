// bdc 0x0893e694 UiPasscodePulseCursor
#include "bdc.h"

/* Per-frame cursor pulse of `UiPasscode`: picks the command-row cursor
   (sprite 0x25, `tween[0x25]`) when `onCommandRow` ≠ 0, else the pad cursor (sprite 0,
   `tween[0]`); ramps the tween's `startAlpha` between 0 and 0.8 in ±0.04 steps, `flag` holding the
   direction (0 up, 1 down), and sets the sprite's alpha to 1 − value. */

void UiPasscodePulseCursor(UiScreen *screen)
{
  UiPasscode *pc = (UiPasscode *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  int idx = (pc->onCommandRow != 0) ? 0x25 : 0;
  UiPasscodePartTween *tw = &pc->tween[idx];
  float v = tw->startAlpha;

  if ((signed char)tw->flag == 0) {
    v = v + 0.04f;
    tw->startAlpha = v;
    if (!(v < 0.8f)) {
      v = 0.8f;
      tw->startAlpha = 0.8f;
      tw->flag = 1;
    }
  } else {
    v = v - 0.04f;
    tw->startAlpha = v;
    if (v <= 0.0f) {
      tw->startAlpha = 0.0f;
      v = 0.0f;
      tw->flag = 0;
    }
  }
  sprites[idx]->alpha = 1.0f - v;
}
