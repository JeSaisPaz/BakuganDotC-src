// bdc 0x089761d8 UiCollectionMenuSetSubEntryLabel
#include "bdc.h"

/* Sets the label cell of a sub-page entry sprite of `UiCollectionMenu` from
   the main entry and the sub-entry index. */

void UiCollectionMenuSetSubEntryLabel(UiCollectionMenu *self, GfxSprite *sprite, u8 entry, u8 subEntry)
{
    u8 cell = 0;

    if (entry < 5) {
        if (entry == 2) {
            cell = (subEntry == 0) ? 0 : 2;
        } else {
            cell = subEntry;
        }
    }
    GfxSpriteSetCell(sprite, 0.0f, (float)cell);
}
