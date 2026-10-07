// bdc 0x08878c84 BtlAttackSetPendingHit
#include "bdc.h"

/* Marks a pending hit on the attack: sets the pending flag, the impact effect id and the impact
   position (translation row of `mtx`) from `pos`. Resolved at the end of the frame by
   `BtlAttackResolvePendingHit`. */
void BtlAttackSetPendingHit(BtlAttack *self, int effect, float *pos)
{
    self->pendingHit = 1;
    self->impactEffect = effect;
    /* quad copy pos -> mtx[3] (lv.q/sv.q) */
    self->mtx[3][0] = pos[0];
    self->mtx[3][1] = pos[1];
    self->mtx[3][2] = pos[2];
    self->mtx[3][3] = pos[3];
}
