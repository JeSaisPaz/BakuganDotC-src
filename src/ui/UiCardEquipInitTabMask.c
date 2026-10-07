// bdc 0x089692fc UiCardEquipInitTabMask
#include "bdc.h"

/* Sets the enabled-tab bit mask `+0x2a58` of the UiCardEquip ability-card loadout sub-screen (task
   303, `UiCardEquipCtor`): 0x0b (tabs 0, 1, 3) with up to two slots; 0x37 (tabs 0–2, 4, 5) with
   more, minus tab 5 when there are exactly three slots (0x17). */

void UiCardEquipInitTabMask(UiCardEquip *self)
{
    u8 enabled[10] = {1, 1, 0, 1, 1, 1, 1, 0, 1, 1};
    u32 mask;
    s32 i;

    self->tabMask = 0;
    mask = self->tabMask;
    if (self->bakuganCount < 3) {
        for (i = 0; i < 4; i++) {
            if (enabled[i] != 0) {
                mask = (mask | (1 << i)) & 0xff;
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            if (enabled[4 + i] != 0) {
                mask = (mask | (1 << i)) & 0xff;
            }
            if (i == 5 && self->bakuganCount < 4) {
                mask = (mask & ~(1 << i)) & 0xff;
            }
        }
    }
    self->tabMask = (u8)mask;
}
