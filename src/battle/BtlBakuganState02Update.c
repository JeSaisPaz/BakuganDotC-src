// bdc 0x088726ac BtlBakuganState02Update
#include "bdc.h"

/* Per-frame handler of Bakugan battle state 2 (the energy dash), run through
   `BtlBakuganRunState`. Runs the dash step `BtlBakuganDashStep` and loops the motion it
   returns (blend 0.2); the motion speed (virtual slot 6) is 0.7 when `slowWalk` is set and the
   unit's attribute (virtual slot 20) is not 2, else 1. An attack command switches to the attack
   (`BtlBakuganStartAttackOrArt`, keep). The dash stops when command bit 2 is released, the
   energy is depleted or state flag 0x10 is set, provided the dash speed fell below 0.9 x the scaled
   dash start speed (`BtlBakuganGetScaledStat48`) or flag 0x10 is set: the charge cooldown of the
   input becomes 5 and the unit enters state 0x17 (airborne, or no motion 0xc) or 0x18 (sub-timer 1
   when the stat table's moveStyle is not 0). Otherwise, with command bit 1, energy and moveStyle
   2, it chains into state 0xc started at 30% of its motion. */

void BtlBakuganState02Update(BtlBakugan *self)
{
    const VtblEntry *entry;
    bool slow;
    int motion;

    motion = BtlBakuganDashStep(self);
    BtlBakuganPlayMotion(0.200000003f, self, motion, 1, 0);
    slow = false;
    if (self->slowWalk != 0) {
        entry = &((const VtblEntry *)self->base.base.vtable)[20];
        if (((int (*)(void *))entry->fn)((u8 *)self + entry->delta) != 2) {
            slow = true;
        }
    }
    entry = &((const VtblEntry *)self->base.base.vtable)[6];
    if (slow) {
        ((float (*)(float, void *))entry->fn)(0.699999988f, (u8 *)self + entry->delta);
    } else {
        ((float (*)(float, void *))entry->fn)(1.0f, (u8 *)self + entry->delta);
    }
    if (BtlBakuganHasAttackCommand(self) != 0) {
        BtlBakuganStartAttackOrArt(self, 1);
        return;
    }
    if ((self->commands & 4) == 0 || BtlCombatIsEnergyDepleted(&self->combat) != 0 ||
        (self->stateFlags & 0x10) != 0) {
        float speed = self->dashSpeed;

        if (speed < BtlBakuganGetScaledStat48(self) * 0.899999976f ||
            (self->stateFlags & 0x10) != 0) {
            self->input->chargeCooldown = 5;
            if (BtlBakuganIsAirborne(self, 1) != 0) {
                BtlBakuganSetState(self, 0x17, 0);
            } else if (BtlBakuganHasMotion(self, 0xc) != 0) {
                BtlBakuganSetState(self, 0x18, 0);
                if (self->combat.stats->moveStyle != 0) {
                    self->subTimer = 1;
                }
            } else {
                BtlBakuganSetState(self, 0x17, 0);
            }
            return;
        }
    }
    if ((self->commands & 2) != 0 && BtlCombatHasEnergy(&self->combat) != 0 &&
        self->combat.stats->moveStyle == 2) {
        BtlBakuganSetState(self, 0xc, 0);
        GfxModelSwapMotionFrame(&self->base, GfxModelGetMotionEnd(&self->base) * 0.300000012f);
    }
}
