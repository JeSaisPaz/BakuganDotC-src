// bdc 0x0884bea4 BtlMainGetField
#include "bdc.h"

/* GetField override (vtable slot 6) of the main battle-scene task: returns
   the flag bytes and words written by BtlMainSetField (ids 3, 4, 5, 7, 8, 10,
   11), byte field12 for id 12, the battle outcome for id 6, and for id 9
   whether talk window 12 is active (0 when the talk task does not exist).
   Any other id returns 0. */
u32 BtlMainGetField(BtlMain *self, u32 field)
{
    u32 value = 0;

    switch (field) {
    case 3:
        value = self->fieldFlags[0];
        break;
    case 4:
        value = self->fieldFlags[1];
        break;
    case 5:
        value = self->fieldFlags[2];
        break;
    case 6:
        value = g_btlBattleOutcome;
        break;
    case 7:
        value = self->fieldFlags[3];
        break;
    case 8:
        value = self->field8;
        break;
    case 9:
        if (UiTalkTaskExists()) {
            UiGetTalkTask();
            value = UiGetWindowActive(0xc);
        }
        break;
    case 10:
        value = self->field10;
        break;
    case 11:
        value = self->field11;
        break;
    case 12:
        value = self->field12;
        break;
    }
    return value;
}
