// bdc 0x088c17b8 GameFieldSyncProgressFlags
#include "bdc.h"

/* Copies story unlock progress from the event flags into the save profile (`SaveGetProfile`):
   - for each of the 20 events in `g_bakuganUnlockEventIds` that is set (`GameEventFlagTest`;
     ids 0x399..0x39f also need event 0x215), sets bit `k + 1` of `ownedBakugan`, `bakuganBitsA`
     and `bakuganBitsB`;
   - for each of the 12 events in `g_specialUnlockEventIds` that is set, sets bit `k % 6 + 1` of
     `specialBits[k / 6]`;
   - when bits 1..6 of `specialBits[0]` (resp. `specialBits[1]`) are all set, sets bit 2 of byte 0
     (resp. byte 1) of the three Bakugan bitsets;
   - mirrors global script bits 0xb, 8, 0x14, 0x10, 0x11, 0xd of `g_scriptGlobalBits`
     (`CoreBitsetTest`) into bits 4, 6, 7, 1, 5, 3 of `bakuganBitsA[1]`.
   Only ever sets bits, never clears them. */

void GameFieldSyncProgressFlags(void)
{
    u32 bakuganIds[20];
    u32 specialIds[12];
    s32 i;
    s32 n;
    u32 id;

    memcpy(bakuganIds, g_bakuganUnlockEventIds, sizeof(bakuganIds));
    for (i = 0; i < 20; ) {
        if (!GameEventFlagTest(bakuganIds[i] & 0xffff)) {
            i++;
            continue;
        }
        id = bakuganIds[i];
        i++;
        if (id == 0x399 || id == 0x39a || id == 0x39b || id == 0x39c ||
            id == 0x39d || id == 0x39e || id == 0x39f) {
            if (!GameEventFlagTest(0x215))
                continue;
        }
        {
            SaveProfile *profile = SaveGetProfile();
            s32 byteIdx = i / 8;

            profile->data->ownedBakugan[byteIdx] |= 1 << (i % 8);
            profile->data->bakuganBitsA[byteIdx] |= 1 << (i % 8);
            profile->data->bakuganBitsB[byteIdx] |= 1 << (i % 8);
        }
    }

    memcpy(specialIds, g_specialUnlockEventIds, sizeof(specialIds));
    for (i = 0; i < 12; i++) {
        if (GameEventFlagTest(specialIds[i] & 0xffff)) {
            SaveProfile *profile = SaveGetProfile();
            s32 bit = i % 6 + 1;

            profile->data->specialBits[i / 6 + bit / 8] |= 1 << (bit % 8);
        }
    }

    /* All six bits of group 0 set? */
    i = 0;
    do {
        n = i + 1;
        if (!(SaveGetProfile()->data->specialBits[n / 8] & (1 << (n % 8))))
            break;
        i = n;
    } while (i < 6);
    if (i == 6) {
        SaveProfile *profile = SaveGetProfile();

        profile->data->ownedBakugan[0] |= 4;
        profile->data->bakuganBitsA[0] |= 4;
        profile->data->bakuganBitsB[0] |= 4;
    }

    /* All six bits of group 1 set? */
    i = 0;
    do {
        n = i + 1;
        if (!(SaveGetProfile()->data->specialBits[1 + n / 8] & (1 << (n % 8))))
            break;
        i = n;
    } while (i < 6);
    if (i == 6) {
        SaveProfile *profile = SaveGetProfile();

        profile->data->ownedBakugan[1] |= 4;
        profile->data->bakuganBitsA[1] |= 4;
        profile->data->bakuganBitsB[1] |= 4;
    }

    if (CoreBitsetTest(0xb, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 0x10;
    if (CoreBitsetTest(8, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 0x40;
    if (CoreBitsetTest(0x14, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 0x80;
    if (CoreBitsetTest(0x10, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 2;
    if (CoreBitsetTest(0x11, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 0x20;
    if (CoreBitsetTest(0xd, g_scriptGlobalBits))
        SaveGetProfile()->data->bakuganBitsA[1] |= 8;
}
