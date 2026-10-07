// bdc 0x08985b48 UiCollectionCardShowBgForZoomView
#include "bdc.h"

/* Swaps the button-guide sprites of `UiCollectionCard` for the card zoom
   view: with `hide` set shows sprites 0x3e/0x3f (0x3e gets button icon 1 via `UiSetButtonIcon`)
   on layer mask 0x10 at full alpha and hides 0x20..0x23; with `hide` = 0 shows 0x20..0x23 (0x20
   gets icon 2) on layer mask 0x10 and hides 0x3e/0x3f. The sprite table is re-read from
   `base.data` on every access, as the asm does. */

#define CARD_SPRITE(self, i) (((GfxSprite **)(self)->base.data)[i])

void UiCollectionCardShowBgForZoomView(UiCollectionCard *self, u8 hide)
{
  int i;

  if (hide == 0) {
    for (i = 0x20; i < 0x24; i++) {
      if (i < 0x21) {
        if (!(i < 0x20)) {
          UiSetButtonIcon(CARD_SPRITE(self, i), 2);
        }
      }
      else if (i == 0x27) { /* never true in 0x20..0x23 */
        UiSetButtonIcon(CARD_SPRITE(self, i), 1);
      }
      CARD_SPRITE(self, i)->flags |= 1;
      CARD_SPRITE(self, i)->layerMask = 0x10;
    }
    for (i = 0x3e; i < 0x40; i++) {
      CARD_SPRITE(self, i)->flags &= ~1u;
    }
  }
  else {
    for (i = 0x3e; i < 0x40; i++) {
      if (i == 0x3e) {
        UiSetButtonIcon(CARD_SPRITE(self, i), 1);
      }
      CARD_SPRITE(self, i)->flags |= 1;
      CARD_SPRITE(self, i)->layerMask = 0x10;
      CARD_SPRITE(self, i)->alpha = 1.0f;
    }
    for (i = 0x20; i < 0x24; i++) {
      CARD_SPRITE(self, i)->flags &= ~1u;
    }
  }
}
