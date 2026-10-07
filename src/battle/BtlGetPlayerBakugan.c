// bdc 0x088660a0 BtlGetPlayerBakugan
#include "bdc.h"

/* Returns the Bakugan unit the local player controls: walks `g_btlBakuganList` and returns the
   first unit with `isPlayer` set for which either the profile flag is clear
   (`SaveGetProfileFlag0`), or a profile exists (`SaveHasProfile`) and its `playerSlot` equals
   profile word 0x13 (`SaveProfileGetWord`). A player unit is skipped when the flag is set but no
   profile exists. Returns NULL when the list is missing or no unit qualifies. */

void *BtlGetPlayerBakugan(void)
{
    BtlBakugan *unit;
    s32 playerSlot;

    unit = NULL;
    if (g_btlBakuganList != NULL) {
        unit = *(BtlBakugan **)g_btlBakuganList;
    }
    for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (unit->isPlayer == 0) {
            continue;
        }
        if (SaveGetProfileFlag0() == 0) {
            return unit;
        }
        if (!SaveHasProfile()) {
            continue;
        }
        playerSlot = unit->playerSlot;
        if ((u32)playerSlot == SaveProfileGetWord(SaveGetProfile(), 0x13)) {
            return unit;
        }
    }
    return NULL;
}
