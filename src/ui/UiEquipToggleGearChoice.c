// bdc 0x08960750 UiEquipToggleGearChoice
#include "bdc.h"

/* Toggles equipment entry `entry` of player `player` on the UiEquip Bakugan/gear loadout screen
   (task 302, `UiEquipCtor`) (`add` = 1 adds, 0 removes) in the panel flags
   `panelCardFlags[player*4 + entry]` (bit 0 chosen, bit 1 kept), allowing at most two entries
   chosen: adding with two already chosen drops every other chosen entry that already had bit 1 and
   marks the rest; adding to an empty set also sets bit 1 on the new one. Mirrors the result in the
   player's two-entry chosen list `gearPick[player]` (0xff = free) by card id
   (`panelCardIds`). Does nothing when the current count is outside 0..2 (add) or 1..2 (remove). */

void UiEquipToggleGearChoice(UiEquip *self, bool add, u8 player, u8 entry)
{
    u8 *flags = &self->panelCardFlags[player * 4];
    u8 *ids = &self->panelCardIds[player * 4];
    u8 *pick = self->gearPick[player];
    s32 count = 0;
    s32 i;
    s32 k;
    u8 id;
    u8 v;

    for (i = 0; i < 4; i++) {
        if (flags[i] & 1) {
            count++;
        }
    }

    if (add) {
        if (count <= 0) {
            if (count < 0) {
                return;
            }
            flags[entry] |= 1;
            flags[entry] |= 2;
            pick[0] = ids[entry];
            return;
        }
        if (count < 2) {
            flags[entry] |= 1;
            for (i = 0; i < 4; i++) {
                if (i != entry && (flags[i] & 1)) {
                    flags[i] |= 2;
                }
            }
            for (k = 0; k < 2; k++) {
                if (pick[k] == 0xff) {
                    pick[k] = ids[entry];
                    return;
                }
            }
            return;
        }
        if (count < 3) {
            for (i = 0; i < 4; i++) {
                if (i == entry) {
                    continue;
                }
                v = flags[i];
                if (!(v & 1)) {
                    continue;
                }
                if (v & 2) {
                    /* drop the older kept choice and free its pick slot */
                    flags[i] = 0;
                    for (k = 0; k < 2; k++) {
                        if (pick[k] == ids[i]) {
                            pick[k] = 0xff;
                            break;
                        }
                    }
                } else {
                    flags[i] = v | 2;
                }
            }
            flags[entry] |= 1;
            for (k = 0; k < 2; k++) {
                if (pick[k] == 0xff) {
                    pick[k] = ids[entry];
                    return;
                }
            }
        }
        return;
    }

    if (count <= 0) {
        return;
    }
    if (count < 2) {
        flags[entry] = 0;
        id = ids[entry];
        for (k = 0; k < 2; k++) {
            if (pick[k] == id) {
                pick[k] = 0xff;
                return;
            }
        }
        return;
    }
    if (count < 3) {
        flags[entry] = 0;
        id = ids[entry];
        for (i = 0; i < 4; i++) {
            if (flags[i] & 1) {
                flags[i] |= 2;
            }
        }
        for (k = 0; k < 2; k++) {
            if (pick[k] == id) {
                pick[k] = 0xff;
                return;
            }
        }
    }
}
