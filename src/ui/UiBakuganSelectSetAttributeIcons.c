// bdc 0x0892f8c0 UiBakuganSelectSetAttributeIcons
#include "bdc.h"

/* Sets the two attribute icon sprites `0x82`/`0x83` of `UiBakuganSelect` for
   the focused Bakugan: looks up its attribute (`UiBakuganGetAttribute` on
   `entries[current].bakugan`) and the attribute's two icon cells (`UiAttributeGetIconCell`, bytes
   1 and 2 of the packed result), and shows them on sprites 0x82 and 0x83 as cells of a 3-column
   sheet (`GfxSpriteSetCell``(v/3, v%3)`). */

void UiBakuganSelectSetAttributeIcons(UiBakuganSelect *self)
{
  u8 attr;
  u32 packed;
  int cellA;
  int cellB;
  int i;

  attr = UiBakuganGetAttribute(self->entries[self->current].bakugan);
  packed = UiAttributeGetIconCell(attr);
  cellA = (u8)(packed >> 8);
  cellB = (u8)(packed >> 16);
  for (i = 0x82; i < 0x84; i++) {
    if (i < 0x83) {
      if (i >= 0x82) {
        GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(cellA / 3),
                         (float)(cellA % 3));
      }
    } else if (i < 0x84) {
      GfxSpriteSetCell(((GfxSprite **)self->base.data)[i], (float)(cellB / 3),
                       (float)(cellB % 3));
    }
  }
}
