// bdc 0x0894e020 UiNetMenuBuildSprites
#include "bdc.h"

/* Builds the sprites of `UiNetMenu`: creates the 0x0e layout sprites
   (`UiLayoutCreateSprites`), clears the work area `+0x78` (600 bytes), enables the buttons
   (`UiNetMenuInitEnabledButtons`), resets the choice `+0x74`, hides all 15 sprites with alpha 0,
   then shows the help window sprite 0x0e centred, linearly filtered, scale 1. */

void UiNetMenuBuildSprites(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite **sprites;
  int i;

  UiLayoutCreateSprites(screen->spriteLayer, screen->data, 0xe);
  memset(&menu->workArea, 0, 600);
  UiNetMenuInitEnabledButtons(screen);
  menu->choice = 0;
  for (i = 0; i < 0xf; i++) {
    sprites = (GfxSprite **)screen->data;
    sprites[i]->flags &= ~1u;
    sprites = (GfxSprite **)screen->data;
    sprites[i]->alpha = 0.0f;
  }
  sprites = (GfxSprite **)screen->data;
  sprites[0xe]->flags |= 1;
  GfxSpriteCenterPivot(((GfxSprite **)screen->data)[0xe]);
  sprites = (GfxSprite **)screen->data;
  sprites[0xe]->flags |= 0x20;
  UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[0xe], 1.0f, 1.0f, 0.0f);
}
