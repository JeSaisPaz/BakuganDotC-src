// bdc 0x0894e194 UiNetMenuStartButtonTween
#include "bdc.h"

/* Opening (`out` = 0): centres the button sprites 4–7 of the network-play menu (task 1999,
   `UiNetMenuCtor`) at scale 1 with linear filtering, saves button 4's layout position
   (`buttonX`/`buttonY`), shows the two host/join buttons (sprites 4/5) with tint 0.6 if disabled
   (`enabled[i]`) and alpha 1 for the chosen one (`choice`) or 0.4 otherwise, places them at X −224
   and fills their `buttonSlide` records (distance 704 − X). Closing (`out` = 1): hides the chosen
   button's glow sprite (6+`choice`) and records its alpha/X scale as `glowPulse` start values. */

void UiNetMenuStartButtonTween(UiScreen *screen, bool out)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites;
  GfxSprite *sprite;
  UiNetMenuButtonSlide *slide;
  UiNetMenuGlowPulse *pulse;
  s32 i;
  s32 idx;

  sprites = (GfxSprite **)screen->data;
  if (out) {
    idx = menu->choice;
    sprites[idx + 6]->flags &= ~1u;
    pulse = &menu->glowPulse[idx];
    pulse->t = 0.0f;
    pulse->startAlpha = ((GfxSprite **)screen->data)[idx + 6]->alpha;
    pulse->startScale = ((GfxSprite **)screen->data)[idx + 6]->scaleX;
    return;
  }

  for (i = 4; i < 8; i++) {
    GfxSpriteCenterPivot(sprites[i]);
    ((GfxSprite **)screen->data)[i]->flags |= 0x20;
    UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 1.0f, 0.0f);
    sprites = (GfxSprite **)screen->data;
  }
  menu->buttonX = sprites[4]->posX;
  menu->buttonY = sprites[4]->posY;

  for (i = 0; i < 2; i++) {
    slide = &menu->buttonSlide[i];
    sprites[4 + i]->flags |= 1;
    sprite = ((GfxSprite **)screen->data)[4 + i];
    if (menu->choice == i) {
      if (menu->enabled[i] == 0) {
        sprite->tint[0] = 0.6f;
        sprite->tint[1] = 0.6f;
        sprite->tint[2] = 0.6f;
      } else {
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
      }
      sprite->alpha = 1.0f;
    } else {
      if (menu->enabled[i] == 0) {
        sprite->tint[0] = 0.6f;
        sprite->tint[1] = 0.6f;
        sprite->tint[2] = 0.6f;
      } else {
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
      }
      sprite->alpha = 0.4f;
    }
    sprite = ((GfxSprite **)screen->data)[4 + i];
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    ((GfxSprite **)screen->data)[4 + i]->posX = -224.0f;
    slide->order = (u8)(i * 2);
    sprites = (GfxSprite **)screen->data;
    slide->fadingOut = 0;
    slide->t = 0.0f;
    slide->fromX = (s16)(s32)sprites[4 + i]->posX;
    slide->distance = (s16)(s32)(704.0f - sprites[4 + i]->posX);
  }
}
