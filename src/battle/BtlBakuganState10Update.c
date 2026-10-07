// bdc 0x0886b6c8 BtlBakuganState10Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 10 (`state`), vtable slot `+0x120` called through
   `BtlBakuganRunState`: the dash attack. Returns at once when `BtlBakuganTryCancelIntoArt`
   switched to an art. Otherwise sets `artCancelTimer` to 10, runs the dash step
   (`BtlBakuganDashStep`), sets state flag 0x400000 (and 0x8000000 when the current attack
   motion set (`BtlAttackMotionSet`) has flag 2), counts `attackFrame`, and steps `attackPhase`
   once the motion is 90 % done (`GfxModelMotionReached`): phase 0 plays phase 2
   (`BtlBakuganPlayAttackMotion`); phases 1 and 3 return to state 2; phase 2 first consumes its
   remaining loops (`attackMotionFlags`, one per `motionEnded`), then clears flag 0x400000, plays
   phase 3 (state 2 when that fails) and sets phase 3. Ends with `BtlBakuganSetStateFlag04`. */
void BtlBakuganState10Update(BtlBakugan *self)
{
    const BtlAttackMotionSet *set =
        (const BtlAttackMotionSet *)self->attackMotions[self->attackIndex];
    s32 phase;

    if (BtlBakuganTryCancelIntoArt(self) != 0) {
        return;
    }
    self->artCancelTimer = 10;
    BtlBakuganDashStep(self);
    self->stateFlags |= 0x400000;
    if (set != NULL && (set->flags & 2) != 0) {
        self->stateFlags |= 0x8000000;
    }
    self->attackFrame++;
    phase = (s32)self->attackPhase;
    switch (phase) {
    case 0:
        if (GfxModelMotionReached(&self->base, 0.899999976f)) {
            BtlBakuganPlayAttackMotion(self, 2);
        }
        break;
    case 2:
        if ((s32)self->attackMotionFlags > 0) {
            if (self->base.motionEnded != 0) {
                self->base.motionEnded = 0;
                self->attackMotionFlags--;
            }
        } else if (GfxModelMotionReached(&self->base, 0.899999976f)) {
            self->stateFlags &= ~0x400000u;
            if (BtlBakuganPlayAttackMotion(self, 3) == 0) {
                self->state = 2;
            }
            self->attackPhase = 3;
        }
        break;
    case 1:
    case 3:
        if (GfxModelMotionReached(&self->base, 0.899999976f)) {
            self->state = 2;
        }
        break;
    default:
        break;
    }
    BtlBakuganSetStateFlag04(self);
}
