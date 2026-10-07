// bdc 0x089198b4 UiAdvSelectSetAttributeCell
#include "bdc.h"

/* Sets a sprite's cell from the attribute index `UiBakuganGetAttribute(id)` laid out 3 per row (`a / 3`, `a
   % 3`). */

void UiAdvSelectSetAttributeCell(UiAdvSelect *self, GfxSprite *sprite, u8 id)

{
  u8 attr;
  
  attr = UiBakuganGetAttribute(id);
  GfxSpriteSetCell(sprite,(float)(attr / 3),(float)((u32)attr % 3));
}

