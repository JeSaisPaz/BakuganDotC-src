// bdc 0x08932858 UiGauntletSetupSetCardTexture
#include "bdc.h"

/* Points a sprite's texture (`+0xd4`) at the large card image of card `card`: `"cc_card_L_%03d"`
   with `card + 1`, or `"cc_cus_non_card"` when `card` = 0xff (no card), looked up with
   `GfxFindTexture`. */

void UiGauntletSetupSetCardTexture(GfxSprite *sprite, u8 card)

{
  char name[64];

  if (card == 0xff) {
    sprintf(name,"cc_cus_non_card",0x100);
  }
  else {
    sprintf(name,"cc_card_L_%03d",card + 1);
  }
  sprite->texture = GfxFindTexture(name);
  return;
}
