// bdc 0x08998b58 UiWorldMapSetFlag
#include "bdc.h"

/* Sets a sprite of `UiWorldMap` to the national flag of country `country` (1
   Japan, 2 UK, 3 China, 4 Egypt, 5 USA: `"hata_*"`); other values leave it unchanged. */

void UiWorldMapSetFlag(UiScreen *screen, GfxSprite *sprite, u8 country)
{
  char name[64];
  u32 idx = (u8)(country - 1);

  if (idx < 5) {
    if (idx == 1) {
      sprintf(name, "hata_UK");
    } else if (idx == 2) {
      sprintf(name, "hata_China");
    } else if (idx == 3) {
      sprintf(name, "hata_Ezypt");
    } else if (idx == 4) {
      sprintf(name, "hata_USA");
    } else {
      sprintf(name, "hata_Japan");
    }
    sprite->texture = GfxFindTexture(name);
  }
}
