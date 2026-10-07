// bdc 0x08860340 BtlBakuganTargetInvolvesPlayer
#include "bdc.h"

/* Returns 1 when the unit has a target (`BtlBakuganGetTarget`) and either the unit or its target
   is player-controlled (`isPlayer`), else 0. */
int BtlBakuganTargetInvolvesPlayer(BtlBakugan *self)
{
    BtlBakugan *target = (BtlBakugan *)BtlBakuganGetTarget(self);

    if (target != NULL && (self->isPlayer != 0 || target->isPlayer != 0)) {
        return 1;
    }
    return 0;
}
