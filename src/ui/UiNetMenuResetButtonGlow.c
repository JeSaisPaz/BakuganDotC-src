// bdc 0x0894e8e4 UiNetMenuResetButtonGlow
#include "bdc.h"

/* Resets the glow sprite 6+`index` of button `index` of `UiNetMenu`: hidden when
   `show` is 0 or the button is disabled (`enabled[index]`); otherwise visible, centred, linearly
   filtered, scale 1 / rotation 0, alpha 0, and its pulse record `glowPulse[index]` cleared. */

void UiNetMenuResetButtonGlow(UiScreen *screen, u8 show, u8 index)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites;
  GfxSprite *glow;
  UiNetMenuGlowPulse *pulse;

  glow = ((GfxSprite **)screen->data)[index + 6];
  if (show == 0) {
    glow->flags &= ~1u;
  }
  else if (menu->enabled[index] == 0) {
    glow->flags &= ~1u;
  }
  else {
    glow->flags |= 1;
    sprites = (GfxSprite **)screen->data;
    GfxSpriteCenterPivot(sprites[index + 6]);
    sprites = (GfxSprite **)screen->data;
    sprites[index + 6]->flags |= 0x20;
    sprites = (GfxSprite **)screen->data;
    UiSpriteSetScaleRotation(sprites[index + 6], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)screen->data;
    sprites[index + 6]->alpha = 0.0f;
    pulse = &menu->glowPulse[index];
    pulse->t = 0.0f;
    pulse->startAlpha = 0.0f;
    pulse->fadingOut = 0;
  }
}
