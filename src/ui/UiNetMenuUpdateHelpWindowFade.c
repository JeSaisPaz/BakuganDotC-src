// bdc 0x0894ef74 UiNetMenuUpdateHelpWindowFade
#include "bdc.h"

/* Advances the help-window fade of `UiNetMenu` by 1/8 (overshoot in, t² out) on
   sprite 0x0e and the help text alpha `+0x2e0`; hides the window at the end of a close. Returns 1
   when finished. */

s32 UiNetMenuUpdateHelpWindowFade(UiScreen *screen, u8 closing)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  float t;
  float d;

  t = menu->helpFadeT + 0.125f;
  if (closing == 0) {
    menu->helpFadeT = t;
    d = t - 1.0f;
    sprites[14]->alpha = menu->helpFadeAlpha + (1.0f - d * d);
    t = menu->helpFadeT;
    d = t - 1.0f;
    menu->titleHelpAlpha = menu->helpFadeAlpha + (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)screen->data)[14]->alpha = 1.0f;
      return 1;
    }
  }
  else {
    menu->helpFadeT = t;
    sprites[14]->alpha = menu->helpFadeAlpha - t * t;
    t = menu->helpFadeT;
    d = t - 1.0f;
    menu->titleHelpAlpha = menu->helpFadeAlpha - (1.0f - d * d);
    if (!(t < 1.0f)) {
      ((GfxSprite **)screen->data)[14]->flags = ((GfxSprite **)screen->data)[14]->flags & 0xfffffffe;
      return 1;
    }
  }
  return 0;
}
