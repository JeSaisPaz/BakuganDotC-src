// bdc 0x088604b4 BtlBakuganCanStartKnockOut
#include "bdc.h"

/* Returns 1 when the unit is dead and may go into its knock-out sequence: a local player unit
   (`BtlBakuganIsLocalPlayer`) while the battle is not over (`g_btlBattleOver` clear), any other
   unit while its `knockOutMode` is 0; else 0. */
int BtlBakuganCanStartKnockOut(BtlBakugan *self)
{
    if (self->combat.dead == 0) {
        return 0;
    }
    if (BtlBakuganIsLocalPlayer(self)) {
        return g_btlBattleOver == 0;
    }
    return self->knockOutMode == 0;
}
