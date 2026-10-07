// bdc 0x088978cc BtlAiGuardLayerRun
#include "bdc.h"

/* Run method of behaviour layer 0 of `BtlAi` (stagger/guard reaction): in owner state
   3, with a pending hit `+0x9a4`, rolls the counter chance `ai+0x9e3` (only with ability flag
   0x10000000) and presses bit 0x40000 (`BtlAiPadPress40000`), then re-runs `BtlAiUpdateTarget`
   and flushes channels `+0x2d8`/`+0x404`. While guarding/stunned with ability flag 0x20000000 it
   waits a random 0.033-s-step delay (`+0x9ec..+0x9f0`, chance `+0x9e0`) and, when the target
   threatens (`BtlAiIsUnitThreatening`, `BtlAiUnitHasCommandC00`,
   `BtlAiTargetInRecoveryState`, `BtlAiFindIncomingAttack`), sidesteps where there is room
   (`BtlAiRaycastDir8` 0xc000/0x4000/0x8000/0), otherwise steps per the kind parameter object;
   finally presses bit 2 (`BtlAiPadPress02`, dodge) and sets `+0x915`. */

void BtlAiGuardLayerRun(BtlAi *self)
{
    BtlBakugan *owner = self->owner;
    BtlBakugan *target;
    BtlAiPad *pad;
    const VtblEntry *entry;
    s32 chance;
    s32 mode;
    float delayMin;
    float delay;
    float elapsed;
    float limit;
    bool threat;

    if (owner->state == 3) {
        if (self->pendingHit != -1) {
            chance = (s8)self->counterChance;
            if ((self->allowedCmds & 0x10000000) == 0) {
                chance = 0;
                self->guardRoll = 0;
            }
            self->guardRoll = (s32)CoreRandNext(99) < chance;
            if (self->guardRoll != 0) {
                BtlAiPadPress40000(&self->pad);
            }
        }
        if (BtlAiUpdateTarget(self) != 0) {
            BtlAiChannelReset(&self->channels[0]);
            BtlAiChannelReset(&self->channels[1]);
        }
        return;
    }
    if (!((owner->stateFlags & 0x30000000) != 0 || owner->state == 4 || owner->state == 5)) {
        return;
    }
    if ((self->allowedCmds & 0x20000000) == 0) {
        return;
    }
    if (self->guard.base.timerExpired == 0) {
        limit = self->guard.base.timerLimit;
        elapsed = self->guard.base.timerElapsed + 0.0333333351f;
        self->guard.base.timerElapsed = elapsed;
        if (!(elapsed < limit)) {
            self->guard.base.timerElapsed = limit;
            self->guard.base.timerExpired = 1;
        }
    }
    switch (self->guard.base.state) {
    case 0:
        delayMin = self->guardDelayMin;
        delay = CoreRandFloat(self->guardDelayMax - delayMin) + delayMin;
        self->guard.base.timerElapsed = 0.0f;
        self->guard.base.timerLimit = delay;
        self->guard.base.timerExpired = delay <= 0.0f;
        chance = (s8)self->guardDelayChance;
        self->guard.counterArmed = (s32)CoreRandNext(99) < chance;
        self->guard.base.state = self->guard.base.state + 1;
        return;
    case 1:
        if (self->guard.base.timerExpired == 0) {
            return;
        }
        break;
    case 2:
        break;
    default:
        return;
    }

    pad = &self->pad;
    if ((self->owner->stateFlags & 0x40000) == 0 && self->guard.counterArmed != 0) {
        threat = false;
        target = self->target;
        if (target != NULL) {
            entry = &((const VtblEntry *)target->base.base.vtable)[10];
            if (((s32 (*)(void *))entry->fn)((u8 *)target + entry->delta) != 0) {
                target = self->target;
                if (BtlAiIsUnitThreatening(self, target) != 0 ||
                    BtlAiUnitHasCommandC00(self, target) != 0 ||
                    BtlAiTargetInRecoveryState(self, target) != 0) {
                    threat = true;
                }
            }
        }
        if (!threat && BtlAiFindIncomingAttack(self) != NULL) {
            threat = true;
        }
        if (threat) {
            /* sidestep where there is room */
            if (BtlAiRaycastDir8(self->rayLength, self, 0xc000) == 0) {
                BtlAiPadStickDirC000(pad);
            } else if (BtlAiRaycastDir8(self->rayLength, self, 0x4000) == 0) {
                BtlAiPadStickDir4000(pad);
            }
        } else {
            /* step direction preference from the kind parameter object (vtable entry 2) */
            entry = &self->params->vtbl[2];
            mode = ((s32 (*)(void *))entry->fn)((u8 *)self->params + entry->delta);
            switch (mode) {
            case 2:
                if (BtlAiRaycastDir8(self->rayLength, self, 0x8000) == 0) {
                    BtlAiPadStickBack(pad);
                    break;
                }
                /* fall through */
            case 1:
                if (BtlAiRaycastDir8(self->rayLength, self, 0xc000) == 0) {
                    BtlAiPadStickDirC000(pad);
                    break;
                }
                if (BtlAiRaycastDir8(self->rayLength, self, 0x4000) == 0) {
                    BtlAiPadStickDir4000(pad);
                    break;
                }
                /* fall through */
            case 0:
                if (BtlAiRaycastDir8(self->rayLength, self, 0) == 0) {
                    BtlAiPadStickForward(pad);
                }
                break;
            default:
                break;
            }
        }
    }
    BtlAiPadPress02(pad);
    self->dodgePressed = 1;
}
