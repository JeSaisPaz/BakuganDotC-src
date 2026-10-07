// bdc 0x0892bdac UiCardSetThumbnailTexture
#include "bdc.h"

/* Sets a sprite's texture to the card thumbnail `card_SS_%03d` (`card + 1`), or `card_SS_non_card`
   for 0xff. */

void UiCardSetThumbnailTexture(GfxSprite *sprite, u32 card)
{
  char name[64];

  if ((card & 0xff) == 0xff) {
    sprintf(name, "card_SS_non_card", 0x100);
  } else {
    sprintf(name, "card_SS_%03d", (card & 0xff) + 1);
  }
  sprite->texture = GfxFindTexture(name);
}
