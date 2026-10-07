// bdc 0x0892c8e0 UiBakuganSetAttributeCell
#include "bdc.h"

/* Sets a sprite's cell from `UiBakuganGetAttribute``(id)` laid out 3 per row. */

void UiBakuganSetAttributeCell(GfxSprite *sprite, u8 id)

{
  u8 attribute;

  attribute = UiBakuganGetAttribute((u32)id);
  GfxSpriteSetCell(sprite, (float)(attribute / 3), (float)(attribute % 3));
  return;
}
