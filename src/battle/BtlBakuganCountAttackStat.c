// bdc 0x0886b424 BtlBakuganCountAttackStat
#include "bdc.h"

/* Counts an attack: increments `attackCount` and adds 1 to the unit's battle statistics counter
   (`BtlStatsAddCounter` on `stats`) chosen from the commands and state: 6 for command bit 0x80,
   else 7 for bit 0x100, else by state: 7 gives 3 (4 with command bit 0x10000), 8 gives 1 when
   `stateFlags` bit 0x80000 is set (else 0), 9 gives 5, 10 gives 2; any other state counts
   nothing. */
void BtlBakuganCountAttackStat(BtlBakugan *self)
{
    u32 commands = self->commands;
    s32 index;

    self->attackCount++;
    index = -1;
    if (commands & 0x80) {
        index = 6;
    } else if (commands & 0x100) {
        index = 7;
    } else {
        switch (self->state) {
        case 7:
            index = (commands & 0x10000) ? 4 : 3;
            break;
        case 8:
            index = (self->stateFlags & 0x80000) != 0;
            break;
        case 9:
            index = 5;
            break;
        case 10:
            index = 2;
            break;
        default:
            break;
        }
    }
    if (index >= 0) {
        BtlStatsAddCounter(self->stats, index, 1);
    }
}
