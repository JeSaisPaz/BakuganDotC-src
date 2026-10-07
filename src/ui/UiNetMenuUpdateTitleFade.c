// bdc 0x0894e7fc UiNetMenuUpdateTitleFade
#include "bdc.h"

/* Advances the title fade of `UiNetMenu` by 1/8: alpha of sprite 0 and of the help
   text follow an overshoot curve in, or t² out (hiding sprite 0 at the end). Returns 1 when
   finished, 0 otherwise. */

s32 UiNetMenuUpdateTitleFade(UiScreen *screen, u8 closing)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  float t;
  float d;

  t = menu->titleFadeT + 0.125f;
  if (closing == 0) {
    menu->titleFadeT = t;
    d = t - 1.0f;
    sprites[0]->alpha = menu->titleFadeAlpha + (1.0f - d * d);
    t = menu->titleFadeT;
    d = t - 1.0f;
    menu->titleHelpAlpha = menu->titleFadeAlpha + (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)screen->data)[0]->alpha = 1.0f;
      return 1;
    }
  }
  else {
    menu->titleFadeT = t;
    sprites[0]->alpha = menu->titleFadeAlpha - t * t;
    t = menu->titleFadeT;
    d = t - 1.0f;
    menu->titleHelpAlpha = menu->titleFadeAlpha - (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)screen->data)[0]->flags = ((GfxSprite **)screen->data)[0]->flags & 0xfffffffe;
      return 1;
    }
  }
  return 0;
}
