// bdc 0x0892c8a0 UiBakuganSetAttributeRow
#include "bdc.h"

/* Sets a sprite's cell to row `UiBakuganGetAttribute``(id)` (column 0). */

void UiBakuganSetAttributeRow(GfxSprite *sprite, u8 id)

{
  u8 row = UiBakuganGetAttribute(id);

  GfxSpriteSetCell(sprite, 0.0f, (float)row);
  return;
}

