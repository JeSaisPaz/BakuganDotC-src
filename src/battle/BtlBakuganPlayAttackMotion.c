// bdc 0x0886ab00 BtlBakuganPlayAttackMotion
#include "bdc.h"

/* Plays the motion of phase `phase` (stored in `attackPhase`) from the unit's current attack
   motion set (`BtlAttackMotionSet`, `attackMotions[attackIndex]`): 0 = `startMotion` (falls back
   to phase 2 when it is -1), 1 = `loopMotion`, 2 = `mainMotion` (low 12 bits motion, high nibble →
   `attackMotionFlags` loop count), 3 = `endMotion`, 4 = `endMotion + 1`; phases above 4 play
   nothing. Phase 1 or a positive loop count loops the motion; blend 0.1, or 0.03 when
   `motionEnded` is set (`BtlBakuganPlayMotion`, not forced). Returns 0 when the set or motion is
   missing (-1), else 1. */

int BtlBakuganPlayAttackMotion(BtlBakugan *self, u32 phase)
{
    BtlAttackMotionSet *set = (BtlAttackMotionSet *)self->attackMotions[self->attackIndex];
    s32 motion = -1;
    u8 loop;
    float blend;

    self->attackPhase = phase;
    if (set == NULL) {
        return 0;
    }
    self->attackMotionFlags = 0;
    switch (phase) {
    case 0:
        motion = set->startMotion;
        if (motion != -1) {
            break;
        }
        self->attackPhase = 2;
        /* fall through */
    case 2:
        motion = set->mainMotion;
        if (motion != -1) {
            motion = set->mainMotion & 0xfff;
            self->attackMotionFlags = ((u32)set->mainMotion & 0xf000) >> 12;
        }
        break;
    case 1:
        motion = set->loopMotion;
        break;
    case 3:
        motion = set->endMotion;
        break;
    case 4:
        motion = (s16)(set->endMotion + 1);
        break;
    default:
        break;
    }
    if (motion == -1) {
        return 0;
    }
    loop = 0;
    if (phase == 1 || (s32)self->attackMotionFlags > 0) {
        loop = 1;
    }
    if (self->base.motionEnded != 0) {
        blend = 0.0299999993f;
    } else {
        blend = 0.100000001f;
    }
    BtlBakuganPlayMotion(blend, self, motion, loop, 0);
    return 1;
}
