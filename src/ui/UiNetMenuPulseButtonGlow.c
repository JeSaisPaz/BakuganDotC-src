// bdc 0x0894e9fc UiNetMenuPulseButtonGlow
#include "bdc.h"

/* Per-frame glow pulse of button `index` of `UiNetMenu` (enabled buttons only):
   alpha of sprite 6+index ramps 0→1 and back over 30 frames each way. The pulse timer `t` grows by
   1/30 per frame and the sprite alpha is `startAlpha ± t`; once `t` reaches 1 the alpha is pinned
   (1 fading in, 0 fading out), `t` resets, the direction flips and the pinned alpha becomes the new
   `startAlpha`. Disabled buttons are left untouched. */

void UiNetMenuPulseButtonGlow(UiScreen *screen, u8 index)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  UiNetMenuGlowPulse *glow;
  GfxSprite *sprite;
  float t;
  float startAlpha;

  if (menu->enabled[index] == 0) {
    return;
  }
  glow = &menu->glowPulse[index];
  t = glow->t + 0.033333335f;
  startAlpha = glow->startAlpha;
  glow->t = t;
  sprite = ((GfxSprite **)screen->data)[index + 6];
  if (glow->fadingOut == 0) {
    sprite->alpha = startAlpha + t;
    if (glow->t < 1.0f) {
      return;
    }
    ((GfxSprite **)screen->data)[index + 6]->alpha = 1.0f;
    glow->t = 0.0f;
    startAlpha = ((GfxSprite **)screen->data)[index + 6]->alpha;
    glow->fadingOut = 1;
    glow->startAlpha = startAlpha;
  }
  else {
    sprite->alpha = startAlpha - t;
    if (glow->t < 1.0f) {
      return;
    }
    ((GfxSprite **)screen->data)[index + 6]->alpha = 0.0f;
    glow->t = 0.0f;
    startAlpha = ((GfxSprite **)screen->data)[index + 6]->alpha;
    glow->fadingOut = 0;
    glow->startAlpha = startAlpha;
  }
}
