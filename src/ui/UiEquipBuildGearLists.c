// bdc 0x08956558 UiEquipBuildGearLists
#include "bdc.h"

/* Builds the per-player equipment lists of the Bakugan/gear loadout screen (`UiEquipCtor`,
   task 302). Clears `panelCardIds[]` and `gearPick[][]` to 0xff and `panelCardFlags[]` to 0, then
   for each of the `playerCount` players puts in `panelCardIds[p*4 + i]` (i = 0..3) the equipment
   id `bakuganPick[p]*4 + i - 4` whose bit is set in the profile's `newItems` bit field.
   When the previous screen was task 0x136 (`g_lastScreenTaskId`) the picks are taken from the
   profile's battle words (`words[0x36 + p*2 + j]`, 0xff = none): matching panel entries get flag
   bit0 (bit1 on the first one of the player), `gearPick[p][j]` copies the non-0xff words and
   `handicap[i]` copies `words[0x46 + i]`. Otherwise the battle words are reset to 0xff, the first
   two owned entries of each player become its `gearPick` and are flagged the same way, and every
   handicap is set to 100. */

void UiEquipBuildGearLists(UiEquip *self)
{
    SaveProfile *profile;
    s32 playerCount;
    s32 p;
    s32 i;
    s32 j;
    s32 idx;
    u32 word;
    u8 gear;
    u8 first;
    u8 count;

    memset(self->panelCardIds, 0xff, 0x10);
    memset(self->panelCardFlags, 0, 0x10);
    memset(self->gearPick, 0xff, 8);
    playerCount = self->playerCount;
    if (g_lastScreenTaskId == 0x136) {
        for (p = 0; p < playerCount; p++) {
            first = 0;
            for (i = 0; i < 4; i++) {
                profile = SaveGetProfile();
                idx = (s8)self->bakuganPick[p] * 4 + i - 4;
                /* signed idx / 8 and idx % 8, as the asm computes them (shift count masked to 5 bits) */
                if ((u8)(profile->data->newItems[idx / 8] & (1 << ((idx % 8) & 0x1f))) == 0)
                    continue;
                self->panelCardIds[p * 4 + i] = (u8)idx;
                for (j = 0; j < 2; j++) {
                    profile = SaveGetProfile();
                    word = 0;
                    if (profile->words != NULL)
                        word = profile->words[0x36 + p * 2 + j];
                    gear = (u8)word;
                    if (gear != 0xff && self->panelCardIds[p * 4 + i] == gear) {
                        self->panelCardFlags[p * 4 + i] |= 1;
                        if (first == 0) {
                            self->panelCardFlags[p * 4 + i] |= 2;
                            first++;
                        }
                        break;
                    }
                }
            }
            playerCount = self->playerCount;
        }
        for (p = 0; p < playerCount; p++) {
            for (j = 0; j < 2; j++) {
                profile = SaveGetProfile();
                word = 0;
                if (profile->words != NULL)
                    word = profile->words[0x36 + p * 2 + j];
                gear = (u8)word;
                if (gear != 0xff)
                    self->gearPick[p][j] = gear;
            }
            playerCount = self->playerCount;
        }
        for (i = 0; i < 4; i++) {
            profile = SaveGetProfile();
            word = 0;
            if (profile->words != NULL)
                word = profile->words[0x46 + i];
            self->handicap[i] = (u8)word;
        }
    } else {
        for (p = 0; p < playerCount; p++) {
            for (j = 0; j < 2; j++) {
                profile = SaveGetProfile();
                if (profile->words != NULL)
                    profile->words[0x36 + p * 2 + j] = 0xff;
            }
            count = 0;
            first = 0;
            for (i = 0; i < 4; i++) {
                profile = SaveGetProfile();
                idx = (s8)self->bakuganPick[p] * 4 + i - 4;
                if ((u8)(profile->data->newItems[idx / 8] & (1 << ((idx % 8) & 0x1f))) == 0)
                    continue;
                self->panelCardIds[p * 4 + i] = (u8)idx;
                if (count < 2) {
                    self->gearPick[p][count] = self->panelCardIds[p * 4 + i];
                    self->panelCardFlags[p * 4 + i] |= 1;
                    if (first == 0) {
                        self->panelCardFlags[p * 4 + i] |= 2;
                        first++;
                    }
                }
                count++;
            }
            playerCount = self->playerCount;
        }
        memset(self->handicap, 100, 4);
    }
}
