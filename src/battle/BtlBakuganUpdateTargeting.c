// bdc 0x08866514 BtlBakuganUpdateTargeting
#include "bdc.h"

/* Per-frame targeting of the Bakugan. Does nothing in state 0xb, while a combo lock holds
   (`BtlBakuganValidateComboLock`), with state flags 0x30000000, or in states 0x14/0x10. Without a
   target it counts `adviceTimer` down to -1 (states other than 6/0xf/0x10/0x11 with enabled input)
   and, outside profile flag 0 mode, re-targets (`BtlBakuganRetarget`) after a target switch when
   the battle main task's phase is 1; with a target it rearms `adviceTimer` (0x1e0) and clears the
   player's `adviceFlag`. Outside states 0xf/0x11, command 0x1000 drops the target (counting stat
   0x11) and command 0x40 re-targets. A target that is dead or untargetable (status 9) is replaced
   unless every other unit is dead (then the target is cleared; in rule mode 2 profile word 7 is
   read and ignored) or the unit is in state 0xb; a lost target id re-targets. With a target it
   stores the 3D distance (`targetDistance`), `noTurn` (horizontal distance² below 0.7 × reach²,
   reach = both units' `reachRadius` capped at 150), `targetCloseness` and `targetNear` (distance²
   with y scaled by 0.85 below 0.75 × (reach + 150)²). With state flag 0x400000 a changed target
   id sets state flag 0x4000. */

void BtlBakuganUpdateTargeting(BtlBakugan *self)
{
    u32 prevTargetId = self->targetId;
    bool canRetarget;
    BtlBakugan *target;

    if (self->state == 0xb) {
        return;
    }
    if (BtlBakuganValidateComboLock(self) != 0) {
        return;
    }
    if ((self->stateFlags & 0x30000000) != 0) {
        return;
    }
    if (self->state == 0x14) {
        return;
    }
    if (self->state == 0x10) {
        return;
    }
    canRetarget = self->state != 0xf && self->state != 0x11;

    if (BtlBakuganGetTarget(self) != NULL) {
        self->adviceTimer = 0x1e0;
        if (self->isPlayer != 0) {
            self->adviceFlag = 0;
        }
    } else {
        if (canRetarget && self->state != 6 && self->state != 0x10 && self->input != NULL &&
            self->input->disabled == 0) {
            if (--self->adviceTimer <= 0) {
                self->adviceTimer = -1;
            }
        }
        if (SaveGetProfileFlag0() == 0 && self->retargeted != 0 && BtlCameraTaskExists() != 0 &&
            ((BtlMain *)BtlGetCameraTask())->phase == 1) {
            BtlBakuganRetarget(self, 1);
        }
    }

    if (canRetarget) {
        if ((self->commands & 0x1000) != 0) {
            if (BtlBakuganGetTarget(self) != NULL) {
                self->retargeted = 0;
            }
            if (self->stats != NULL) {
                BtlStatsAddCounter(self->stats, 0x11, 1);
            }
            self->targetId = 0;
            self->targetAux = 0;
        } else if ((self->commands & 0x40) != 0) {
            BtlBakuganRetarget(self, 1);
        }
    }

    target = BtlBakuganGetTarget(self);
    if (target == NULL) {
        if (self->targetId != 0) {
            self->targetAux = 0;
            BtlBakuganRetarget(self, 1);
        }
    } else {
        if (target->combat.dead != 0 || target->combat.status[9].active != 0) {
            if (BtlBakuganAreOthersAllDead(self) == 0) {
                if (self->state != 0xb) {
                    self->targetAux = 0;
                    BtlBakuganRetarget(self, 1);
                }
            } else if (BtlBakuganAreOthersAllDead(self) != 0) {
                if (g_scriptGlobalVars[8] == 2) {
                    SaveProfileGetWord(SaveGetProfile(), 7);
                }
                self->targetId = 0;
                self->targetAux = 0;
            }
        }
        if (self->targetId != 0) {
            float dx;
            float dy;
            float dz;
            float horizSq;
            float scaledSq;
            float reach;
            float reachSq;

            dx = target->base.pos[0] - self->base.pos[0];
            dy = target->base.pos[1] - self->base.pos[1];
            dz = target->base.pos[2] - self->base.pos[2];
            self->targetDistance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

            reach = self->combat.stats->reachRadius + target->combat.stats->reachRadius;
            if (!(reach <= 150.0f)) {
                reach = 150.0f;
            }
            reachSq = reach * reach * 0.7f;
            horizSq = dx * dx + dz * dz;
            horizSq = horizSq - reachSq;
            self->noTurn = horizSq < 0.0f;
            self->targetCloseness = 1.0f - horizSq / reachSq;

            dy = dy * 0.85f;
            scaledSq = dx * dx + dy * dy + dz * dz;
            self->targetNear = scaledSq < (reach + 150.0f) * (reach + 150.0f) * 0.75f;
        }
    }

    if ((self->stateFlags & 0x400000) != 0 && self->targetId != prevTargetId) {
        self->stateFlags |= 0x4000;
    }
}
