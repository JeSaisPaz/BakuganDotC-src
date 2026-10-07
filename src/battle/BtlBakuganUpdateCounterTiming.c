// bdc 0x08871174 BtlBakuganUpdateCounterTiming
#include "bdc.h"

/* Per-frame counter-input timing (from `BtlBakuganUpdate`): clears flags 0x100 and 0x80 of
   `flags`; starts the press timer `counterPress` on command 0x40000, or counts a running one up
   (calling `SaveGetProfileFlag0` and ignoring it) and resets it to 0 and returns once it passes
   9 frames. While the unit has energy (`BtlCombatHasEnergy`), a recorded `attacker`, a running
   timer, an open `counterWindow`, is in state 3 and the attacker's combat status 10 is inactive,
   it sets 0x100 (precise: pressed within 1 frame, 2 when `SaveGetProfileFlag0` is set) and 0x80
   (within 3, or 6) frames. */

void BtlBakuganUpdateCounterTiming(BtlBakugan *self)
{
    s32 limit;

    self->flags &= ~0x100u;
    self->flags &= ~0x80u;
    if (self->counterPress == 0) {
        if ((self->commands & 0x40000) != 0) {
            self->counterPress = self->counterPress + 1;
        }
    } else {
        self->counterPress = self->counterPress + 1;
        SaveGetProfileFlag0();
        if (9 < self->counterPress) {
            self->counterPress = 0;
            return;
        }
    }
    if (BtlCombatHasEnergy(&self->combat) == 0) {
        return;
    }
    if (self->attacker == NULL || self->counterPress <= 0 || self->counterWindow <= 0
        || self->state != 3) {
        return;
    }
    if (((BtlBakugan *)self->attacker)->combat.status[10].active != 0) {
        return;
    }
    limit = 1;
    if (SaveGetProfileFlag0() != 0) {
        limit = 2;
    }
    if (self->counterPress <= limit) {
        self->flags |= 0x100;
    }
    limit = 3;
    if (SaveGetProfileFlag0() != 0) {
        limit = 6;
    }
    if (self->counterPress <= limit) {
        self->flags |= 0x80;
    }
}
