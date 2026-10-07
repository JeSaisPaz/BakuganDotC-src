// bdc 0x0898f544 UiCollectionFigureSetButtonGuide
#include "bdc.h"

/* Switches the button-guide sprites of `UiCollectionFigure` between the
   detail view and the second (rotatable) detail view: with `altView` shows sprites 0x41/0x42 (0x41
   gets button icon 1 via `UiSetButtonIcon`) on layer mask 8 at full alpha and hides 0x24..0x27;
   otherwise shows 0x24..0x27 (0x24 icon 2, 0x25 icon 1, layer mask 8) and hides 0x41/0x42. The
   sprite table is re-read from `base.data` on every access, as the asm does. */

#define FIGURE_SPRITE(self, i) (((GfxSprite **)(self)->base.data)[i])

void UiCollectionFigureSetButtonGuide(UiCollectionFigure *self, u8 altView)
{
  int i;

  if (altView == 0) {
    for (i = 0x24; i < 0x28; i++) {
      if (i < 0x25) {
        if (!(i < 0x24)) {
          UiSetButtonIcon(FIGURE_SPRITE(self, i), 2);
        }
      }
      else if (i < 0x26) {
        UiSetButtonIcon(FIGURE_SPRITE(self, i), 1);
      }
      FIGURE_SPRITE(self, i)->flags |= 1;
      FIGURE_SPRITE(self, i)->layerMask = 8;
    }
    for (i = 0x41; i < 0x43; i++) {
      FIGURE_SPRITE(self, i)->flags &= ~1u;
    }
  }
  else {
    for (i = 0x41; i < 0x43; i++) {
      if (i == 0x41) {
        UiSetButtonIcon(FIGURE_SPRITE(self, i), 1);
      }
      FIGURE_SPRITE(self, i)->flags |= 1;
      FIGURE_SPRITE(self, i)->layerMask = 8;
      FIGURE_SPRITE(self, i)->alpha = 1.0f;
    }
    for (i = 0x24; i < 0x28; i++) {
      FIGURE_SPRITE(self, i)->flags &= ~1u;
    }
  }
}
