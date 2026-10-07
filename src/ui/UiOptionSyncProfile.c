// bdc 0x0896f7bc UiOptionSyncProfile
#include "bdc.h"

/* Copies the option values of the battle-options screen (task 304, `maybe_UiScreen304Ctor`;
   `"option_battle_t_%02d"` / `"option_sol00"` sprites; edits the battle rules stored in profile
   words 0x18..0x1b) between the save profile and the screen: `store` = 0 loads them (word 0x18 →
   `values[0]` (3 choices), word 0x19 100/200/300 → `values[1]` 0..2 or 3 = none, word 0x1a →
   `values[2]` (2 choices), word 0x1b 5/3/1 → `values[3]` 2/1/0 or -1, plus the per-row choice
   counts `valueCounts[]`, which depend on profile word 7 (battle type)); `store` != 0 writes them
   back (0x19 = value*100+100 or -1, 0x1b = 1/3/5 or 0). */

void UiOptionSyncProfile(UiOption *self, bool store)
{
    u32 word;
    s8 value;

    if (!store) {
        self->values[0] = (s8)SaveProfileGetWord(SaveGetProfile(), 0x18);
        self->valueCounts[0] = 3;

        word = SaveProfileGetWord(SaveGetProfile(), 0x19);
        if (word == 300 || word == 200 || word == 100) {
            self->values[1] = (s8)((s32)SaveProfileGetWord(SaveGetProfile(), 0x19) / 100 - 1);
        } else {
            self->values[1] = 3;
        }
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 0) {
            self->valueCounts[1] = 4;
        } else {
            self->valueCounts[1] = 3;
        }

        self->values[2] = (s8)SaveProfileGetWord(SaveGetProfile(), 0x1a);
        self->valueCounts[2] = 2;

        word = SaveProfileGetWord(SaveGetProfile(), 0x1b);
        if (word == 5) {
            self->values[3] = 2;
        } else if (word == 3) {
            self->values[3] = 1;
        } else if (word == 1) {
            self->values[3] = 0;
        } else {
            self->values[3] = -1;
        }
        if (SaveProfileGetWord(SaveGetProfile(), 7) == 0) {
            self->valueCounts[3] = 3;
        } else {
            self->valueCounts[3] = 1;
        }
        return;
    }

    SaveProfileSetWord(SaveGetProfile(), 0x18, (s32)self->values[0]);
    if (self->values[1] < 0 || self->values[1] >= 3) {
        SaveProfileSetWord(SaveGetProfile(), 0x19, (u32)-1);
    } else {
        SaveProfileSetWord(SaveGetProfile(), 0x19, self->values[1] * 100 + 100);
    }
    SaveProfileSetWord(SaveGetProfile(), 0x1a, (s32)self->values[2]);

    value = self->values[3];
    if (value == 0) {
        SaveProfileSetWord(SaveGetProfile(), 0x1b, 1);
    } else if (value == 1) {
        SaveProfileSetWord(SaveGetProfile(), 0x1b, 3);
    } else if (value == 2) {
        SaveProfileSetWord(SaveGetProfile(), 0x1b, 5);
    } else {
        SaveProfileSetWord(SaveGetProfile(), 0x1b, 0);
    }
}
