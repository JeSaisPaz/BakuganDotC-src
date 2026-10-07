// bdc 0x08871570 BtlBakuganStartAttackOrArt
#include "bdc.h"

/* Enters attack state 7 (`BtlBakuganSetState` with `keep`). Without the special-art command
   (0x20 in `commands`) it switches directly. With it, it selects the art
   (`BtlBakuganSelectArtFromCommand`) and only when the art is ready
   (`BtlCombatIsSelectedArtReady`) and bit 0x100 of `stateFlags` is clear does the same as
   `BtlBakuganTryCancelIntoArt`: clears `motionDone`, re-arms the body collider (`collider0`:
   clears hit-pending 0x10, zeroes `hitTimer`, sets hit active 0x1), switches to state 7, ends the
   unit's own sustained attacks (`BtlAttackEndOwnedSustained`), clears bit 0x10 of `flags` and
   timed statuses 6..0xa (`BtlCombatClearStatus`); otherwise it stays in its state. */
void BtlBakuganStartAttackOrArt(BtlBakugan *self, char keep)
{
    CollisionCollider *collider;
    int id;

    if ((self->commands & 0x20) == 0) {
        BtlBakuganSetState(self, 7, keep);
        return;
    }
    BtlBakuganSelectArtFromCommand(self);
    if (BtlCombatIsSelectedArtReady(&self->combat) == 0 || (self->stateFlags & 0x100) != 0) {
        return;
    }
    self->motionDone = 0;
    collider = (CollisionCollider *)self->collider0;
    collider->flags &= ~0x10u;
    collider = (CollisionCollider *)self->collider0;
    collider->hitTimer = 0;
    collider->flags |= 1;
    BtlBakuganSetState(self, 7, keep);
    BtlAttackEndOwnedSustained(self);
    if ((self->flags & 0x10) != 0) {
        self->flags &= ~0x10u;
    }
    for (id = 6; id < 0xb; id++) {
        BtlCombatClearStatus(&self->combat, id);
    }
}
