// bdc 0x089a5660 UiSetButtonIcon
#include "bdc.h"

/* Points `sprite` at the 32x32 button icon `icon`: cells 0..9 of `menu_all_button01`, 10.. of
   `menu_all_button02` (`GfxFindTexture`, `GfxSpriteSetUvRectXYWH`, `GfxSpriteSetCell`). */

void UiSetButtonIcon(GfxSprite *sprite, u8 icon)
{
  char name[64];
  float rectHigh[4];
  float rectLow[4];

  if (icon < 10) {
    sprintf(name, "menu_all_button01");
    sprite->texture = GfxFindTexture(name);
    rectLow[0] = 0.0f;
    rectLow[1] = 0.0f;
    rectLow[2] = 32.0f;
    rectLow[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(sprite, rectLow);
    GfxSpriteSetCell(sprite, 0.0f, (float)icon);
  } else {
    sprintf(name, "menu_all_button02");
    sprite->texture = GfxFindTexture(name);
    rectHigh[0] = 0.0f;
    rectHigh[1] = 0.0f;
    rectHigh[2] = 32.0f;
    rectHigh[3] = 32.0f;
    GfxSpriteSetUvRectXYWH(sprite, rectHigh);
    GfxSpriteSetCell(sprite, 0.0f, (float)(int)(icon - 10));
  }
}
