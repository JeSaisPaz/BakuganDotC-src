// bdc 0x089221bc UiHologramGalleryCheckHint
#include "bdc.h"

/* Decides whether help hint `hint` of the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`; main phase `UiHologramGalleryMainPhase`) must be shown now; a hint
   is shown only while its bit `1 << hint` in the save-profile `viewSeenMask` is clear:
   - during tutorial battle 4 (`UiHologramGalleryIsTutorialBattle4`) hints 1..4 and 7 are shown at once;
   - hint 9 when at least 3 of the first 80 `ownedItems` bits are set;
   - hint 6 when flag 8 of profile word 0x30 is set; that flag is then cleared, and the hint is
     shown only if save word 0x32 is 1.
   Showing a hint stores it in `pendingHint` and returns 1; otherwise returns 0. */

s32 UiHologramGalleryCheckHint(UiHologramGallery *self, u8 hint)
{
    int i;
    u8 count;

    if (UiHologramGalleryIsTutorialBattle4() == 1 &&
        (SaveGetProfile()->data->viewSeenMask & (1 << hint)) == 0) {
        switch (hint) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
            self->pendingHint = hint;
            return 1;
        default:
            break;
        }
    }
    if (hint == 9 && (SaveGetProfile()->data->viewSeenMask & (1 << hint)) == 0) {
        count = 0;
        for (i = 0; i < 80; i++) {
            if (SaveGetProfile()->data->ownedItems[i / 8] & (1 << (i % 8))) {
                count++;
            }
        }
        if (count >= 3) {
            self->pendingHint = hint;
            return 1;
        }
    }
    if (hint == 6 && (SaveGetProfile()->data->viewSeenMask & (1 << hint)) == 0 &&
        SaveProfileTestWord30Bits(8) == 1) {
        SaveProfileModifyWord30Bits(0, 8);
        if (SaveProfileGetWord(SaveGetProfile(), 0x32) == 1) {
            self->pendingHint = hint;
            return 1;
        }
    }
    return 0;
}
