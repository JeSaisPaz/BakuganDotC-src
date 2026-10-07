// bdc 0x08870e74 BtlBakuganTryCancelIntoArt
#include "bdc.h"

/* From an attack state (7..10): when the special-art command (0x20 in `commands`) is given,
   selects the art (`BtlBakuganSelectArtFromCommand`) and, if it is ready
   (`BtlCombatIsSelectedArtReady`) and bit 0x100 of `stateFlags` is clear, clears `motionDone`,
   re-arms the body collider (`collider0`: clears hit-pending 0x10, zeroes `hitTimer`, sets hit
   active 0x1), switches to state 7 (`BtlBakuganSetState`), ends the unit's own sustained
   attacks (`BtlAttackEndOwnedSustained`), clears bit 0x10 of `flags` and timed statuses 6..0xa
   (`BtlCombatClearStatus`), and returns 1; otherwise 0. */
int BtlBakuganTryCancelIntoArt(BtlBakugan *self)
{
    CollisionCollider *collider;
    int id;

    if ((self->commands & 0x20) == 0) {
        return 0;
    }
    BtlBakuganSelectArtFromCommand(self);
    if (BtlCombatIsSelectedArtReady(&self->combat) == 0 || (self->stateFlags & 0x100) != 0) {
        return 0;
    }
    self->motionDone = 0;
    collider = (CollisionCollider *)self->collider0;
    collider->flags &= ~0x10u;
    collider = (CollisionCollider *)self->collider0;
    collider->hitTimer = 0;
    collider->flags |= 1;
    BtlBakuganSetState(self, 7, 0);
    BtlAttackEndOwnedSustained(self);
    if ((self->flags & 0x10) != 0) {
        self->flags &= ~0x10u;
    }
    for (id = 6; id < 0xb; id++) {
        BtlCombatClearStatus(&self->combat, id);
    }
    return 1;
}
