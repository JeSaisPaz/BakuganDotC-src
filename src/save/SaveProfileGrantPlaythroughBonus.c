// bdc 0x089b2348 SaveProfileGrantPlaythroughBonus
#include "bdc.h"

/* Grants the play-through bonus Bakugan and gear for the profile's play-through counter
   `playthrough`: at count 1, every Bakugan `i` flagged in g_playthrough1BonusBakugan gets its bit
   set in `ownedBakugan`, `bakuganBitsA` and `bakuganBitsB`, and every Bakugan flagged there or in
   g_playthrough1BonusGear gets its four gear items `4i-4 .. 4i-1` unlocked (bit in `ownedItems`,
   first free slot of `equipSlots[i]`, new marks in `newItems`/`newItemGroups`); at count 2 it
   unlocks the gear of the Bakugan in g_playthrough2BonusGear. Other counts do nothing. */

void SaveProfileGrantPlaythroughBonus(void)
{
    u8 bonusBakugan[22];
    u8 bonusGear[22];
    u8 bonusGear2[22];
    SaveProfile *profile;
    u8 *slots;
    int bakugan;
    int item;
    int n;

    if (SaveGetProfile()->data->playthrough == 1) {
        memcpy(bonusBakugan, g_playthrough1BonusBakugan, 0x16);
        memcpy(bonusGear, g_playthrough1BonusGear, 0x16);

        for (bakugan = 0; bonusBakugan[bakugan] != 0xff; bakugan++) {
            if (bonusBakugan[bakugan] == 0)
                continue;
            profile = SaveGetProfile();
            profile->data->ownedBakugan[bakugan / 8] |= 1 << (bakugan % 8);
            profile->data->bakuganBitsA[bakugan / 8] |= 1 << (bakugan % 8);
            profile->data->bakuganBitsB[bakugan / 8] |= 1 << (bakugan % 8);

            item = bakugan * 4 - 4;
            for (n = 0; n < 4; n++, item++) {
                profile = SaveGetProfile();
                if ((u8)(profile->data->ownedItems[item / 8] & (1 << (item % 8))) == 0) {
                    profile->data->ownedItems[item / 8] |= 1 << (item % 8);
                    slots = profile->data->equipSlots[item / 4 + 1];
                    if (slots[0] == 0xff)
                        slots[0] = (u8)item;
                    else if (slots[1] == 0xff)
                        slots[1] = (u8)item;
                }
                profile->data->newItems[item / 8] |= 1 << (item % 8);
                profile->data->newItemGroups[item / 8] |= 1 << (item % 8);
            }
        }

        for (bakugan = 0; bonusGear[bakugan] != 0xff; bakugan++) {
            if (bonusGear[bakugan] == 0)
                continue;
            item = bakugan * 4 - 4;
            for (n = 0; n < 4; n++, item++) {
                profile = SaveGetProfile();
                if ((u8)(profile->data->ownedItems[item / 8] & (1 << (item % 8))) == 0) {
                    profile->data->ownedItems[item / 8] |= 1 << (item % 8);
                    slots = profile->data->equipSlots[item / 4 + 1];
                    if (slots[0] == 0xff)
                        slots[0] = (u8)item;
                    else if (slots[1] == 0xff)
                        slots[1] = (u8)item;
                }
                profile->data->newItems[item / 8] |= 1 << (item % 8);
                profile->data->newItemGroups[item / 8] |= 1 << (item % 8);
            }
        }
    } else if (SaveGetProfile()->data->playthrough == 2) {
        memcpy(bonusGear2, g_playthrough2BonusGear, 0x16);

        for (bakugan = 0; bonusGear2[bakugan] != 0xff; bakugan++) {
            if (bonusGear2[bakugan] == 0)
                continue;
            item = bakugan * 4 - 4;
            for (n = 0; n < 4; n++, item++) {
                profile = SaveGetProfile();
                if ((u8)(profile->data->ownedItems[item / 8] & (1 << (item % 8))) == 0) {
                    profile->data->ownedItems[item / 8] |= 1 << (item % 8);
                    slots = profile->data->equipSlots[item / 4 + 1];
                    if (slots[0] == 0xff)
                        slots[0] = (u8)item;
                    else if (slots[1] == 0xff)
                        slots[1] = (u8)item;
                }
                profile->data->newItems[item / 8] |= 1 << (item % 8);
                profile->data->newItemGroups[item / 8] |= 1 << (item % 8);
            }
        }
    }
}
