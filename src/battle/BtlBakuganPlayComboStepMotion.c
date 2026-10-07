// bdc 0x0886ac94 BtlBakuganPlayComboStepMotion
#include "bdc.h"

/* Melee-combo version of `BtlBakuganPlayAttackMotion`: plays the motion of phase `phase` (stored
   in `attackPhase`, also when there is no set) from the current combo step's motion set
   (`BtlBakuganGetComboStepMotionSet`): 0 = `startMotion`, or when it is -1 the `loopMotion` as
   phase 1 if the set's flag 0x100 is set, else `mainMotion` as phase 2; 1 = `loopMotion`; 2 =
   `mainMotion` (low 12 bits motion, high nibble → `attackMotionFlags`, cleared first); 3 =
   `endMotion`; 4 = `endMotion + 1` when the unit has that motion (`BtlBakuganHasMotion`), else
   `endMotion`. The motion loops for phase 1 (as passed or switched to) or a non-zero loop count;
   blend 0, or 0.03 when `motionEnded` is set; always forced (`BtlBakuganPlayMotion`). Returns 1,
   or 0 when the set is missing, the motion is -1 or `phase` is above 4. */

int BtlBakuganPlayComboStepMotion(BtlBakugan *self, u32 phase)
{
    BtlAttackMotionSet *set = (BtlAttackMotionSet *)BtlBakuganGetComboStepMotionSet(self);
    int motion = -1;
    bool useMain = false;

    self->attackPhase = phase;
    if (set == NULL) {
        return 0;
    }
    self->attackMotionFlags = 0;
    switch (phase) {
    case 0:
        motion = set->startMotion;
        if (motion == -1) {
            if (set->flags & 0x100) {
                phase = 1;
                self->attackPhase = 1;
                motion = set->loopMotion;
            } else {
                self->attackPhase = 2;
                useMain = true;
            }
        }
        break;
    case 1:
        motion = set->loopMotion;
        break;
    case 2:
        useMain = true;
        break;
    case 3:
        motion = set->endMotion;
        break;
    case 4:
        motion = (s16)(set->endMotion + 1);
        if (!BtlBakuganHasMotion(self, motion)) {
            motion = set->endMotion;
        }
        break;
    default:
        break;
    }
    if (useMain) {
        motion = set->mainMotion;
        if (motion != -1) {
            motion = set->mainMotion & 0xfff;
            self->attackMotionFlags = (set->mainMotion & 0xf000) >> 12;
        }
    }
    if (motion == -1) {
        return 0;
    }
    BtlBakuganPlayMotion(self->base.motionEnded ? 0.0299999993f : 0.0f, self, motion,
                         phase == 1 || (int)self->attackMotionFlags > 0, 1);
    return 1;
}
