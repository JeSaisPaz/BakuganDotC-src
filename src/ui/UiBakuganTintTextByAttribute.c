// bdc 0x0892bc14 UiBakuganTintTextByAttribute
#include "bdc.h"

/* Colours a chain of 7 text sprites (`next` links) with the RGB of Bakugan `id`'s attribute (table
   `g_bakuganAttributeTints`, 7 RGB triples per attribute), switching sprites after the fifth to
   blend mode 2. */

void UiBakuganTintTextByAttribute(GfxSprite *first, u8 id)
{
  float tints[6][7][3];
  u8 attr;
  int i;

  memcpy(tints, g_bakuganAttributeTints, 0x1f8);
  attr = UiBakuganGetAttribute(id);
  for (i = 0; i < 7; i++) {
    first->tint[0] = tints[attr][i][0];
    first->tint[1] = tints[attr][i][1];
    first->tint[2] = tints[attr][i][2];
    first->alpha = 1.0f;
    if (4 < i) {
      first->blendMode = 2;
    }
    first = first->next;
  }
}
