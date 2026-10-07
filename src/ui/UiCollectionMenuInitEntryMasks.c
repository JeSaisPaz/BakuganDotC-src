// bdc 0x08973488 UiCollectionMenuInitEntryMasks
#include "bdc.h"

/* Builds the enabled-entry masks of `UiCollectionMenu`: all five main
   entries on (`entryMask`), the per-main-entry sub-page masks `subMasks[0..4]` from
   `g_uiCollectionMenuSubEntryTable` (three flags per main entry; only `subMasks[0..2]` are
   cleared first, `[3]`/`[4]` are OR-ed into their old value), and sets `hideFifth` when profile
   word 0x1d equals 500. */

void UiCollectionMenuInitEntryMasks(UiCollectionMenu *self)
{
    u8 mainOn[5];
    u8 subOn[5][4];
    s32 i;
    s32 j;
    u8 mask;

    mainOn[0] = 1;
    mainOn[1] = 1;
    mainOn[2] = 1;
    mainOn[3] = 1;
    mainOn[4] = 1;
    memcpy(subOn, g_uiCollectionMenuSubEntryTable, sizeof(subOn));

    self->entryMask = 0;
    mask = self->entryMask;
    for (i = 0; i < 5; i++) {
        if (mainOn[i] != 0) {
            mask |= (u8)(1 << i);
        }
    }
    self->entryMask = mask;

    self->subMasks[0] = 0;
    self->subMasks[1] = 0;
    self->subMasks[2] = 0;
    for (i = 0; i < 5; i++) {
        mask = self->subMasks[i];
        for (j = 0; j < 3; j++) {
            if (subOn[i][j] != 0) {
                mask |= (u8)(1 << j);
            }
        }
        self->subMasks[i] = mask;
    }

    self->hideFifth = 0;
    if (SaveProfileGetWord(SaveGetProfile(), 0x1d) == 500) {
        self->hideFifth = 1;
    }
}
