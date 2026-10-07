// bdc 0x08919814 UiAdvSelectSetAttributeRow
#include "bdc.h"

/* Sets a sprite's cell to row `UiBakuganGetAttribute(id)` (column 0), i.e. the attribute icon of Bakugan
   `id`. */

void UiAdvSelectSetAttributeRow(UiAdvSelect *self, GfxSprite *sprite, u8 id)
{
    u8 attribute;

    attribute = UiBakuganGetAttribute((u32)id);
    GfxSpriteSetCell(sprite, 0.0f, (float)attribute);
}
