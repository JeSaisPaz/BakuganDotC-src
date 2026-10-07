// bdc 0x08893fa4 BtlAiExecObstacleDash
#include "bdc.h"

/* Executes the obstacle-dash command record `cmd` (0x20 bytes: state `+0`, timer
   limit/elapsed/expired `+4`/`+8`/`+0xc`, flags `+0x10`, finished byte `+0x1c`, failed byte
   `+0x1d`) of `BtlAi` in relative direction `dir`: state 0 clears the virtual pad; 1
   waits for 1000 energy (`BtlCombatGetEnergy`) and a dash-capable owner (ability flag 0x200,
   state 0/1/0xc/0xd/0x13), giving up (failed, state 5) once the owner is free but cannot dash; 2
   steers towards `dir` (`BtlAiPadSteer`) for 0.1 s, then continues only if the ray of length
   `ai+0xa18` in `dir` (`BtlAiRaycastDir8`) hits a real obstruction (`BtlAiIsBlockingKind`); 3
   presses dash (`BtlAiPadPressDash`) and steers: owners whose parameter byte `stats+2` is 1 dash
   on a 30-frame latch of the obstacle (`ai+0xa25`/`+0xa24`) and finish with a 0.2-s dash tail
   (state 4), others dash only while the obstruction is seen; running out of energy ends it, and in
   `BtlAiIsScoreMode1` mode hit kind 3 ends it at once; 5 waits until the owner is free (no bits
   0x400000/0x100, not state 8/10, no `+0x16c & 0xc00`), then 6 sets the finished byte. */

void BtlAiExecObstacleDash(BtlAi *self, BtlAiCommand *cmd, s16 dir)
{
    float velY;
    float elapsed;
    s32 hit;
    bool canDash;
    bool press;

    if (cmd->timerExpired == 0) {
        elapsed = cmd->timerElapsed + 0.0333333351f;
        cmd->timerElapsed = elapsed;
        if (!(elapsed < cmd->timerLimit)) {
            cmd->timerElapsed = cmd->timerLimit;
            cmd->timerExpired = 1;
        }
    }
    switch (cmd->state) {
    case 0:
        self->pad.cur.aiActions = 0;
        self->pad.prev.aiActions = 0;
        cmd->state = cmd->state + 1;
        break;
    case 1:
        if (BtlCombatGetEnergy(&self->owner->combat) < 1000.0f) {
            break;
        }
        canDash = false;
        if ((self->allowedCmds & 0x200) != 0) {
            switch (self->owner->state) {
            case 0:
            case 1:
            case 0xc:
            case 0xd:
            case 0x13:
                canDash = true;
                break;
            }
        }
        if (canDash) {
            cmd->state = cmd->state + 1;
            cmd->timerLimit = 0.100000001f;
            cmd->timerElapsed = 0.0f;
            cmd->timerExpired = 0;
        } else if ((self->owner->stateFlags & 0x400000) == 0 &&
                   self->owner->state != 8 && self->owner->state != 10 &&
                   (self->owner->commands & 0xc00) == 0 &&
                   (self->owner->stateFlags & 0x100) == 0) {
            cmd->failed = 1;
            cmd->state = 5;
        }
        break;
    case 2:
        BtlAiPadSteer(self, dir, 0);
        if (cmd->timerExpired != 0) {
            hit = BtlAiRaycastDir8(self->rayLength, self, dir);
            if (hit != 0 && BtlAiIsBlockingKind(self, hit) != 0) {
                self->obstacleSeen = 0;
                self->obstacleLatch = 0;
                cmd->state = cmd->state + 1;
            } else {
                cmd->state = 5;
            }
        }
        break;
    case 3:
        hit = BtlAiRaycastDir8(self->rayLength, self, dir);
        if (hit != 0) {
            self->obstacleSeen = 30;
        }
        if (self->obstacleSeen == 0) {
            self->obstacleLatch = 0;
        } else {
            self->obstacleSeen = self->obstacleSeen - 1;
            self->obstacleLatch = 1;
        }
        if (BtlAiIsScoreMode1() != 0 && hit == 3) {
            cmd->state = 5;
            break;
        }
        if (self->owner->combat.stats->moveStyle == 1) {
            if (self->obstacleLatch != 0) {
                BtlAiPadPressDash(&self->pad);
                cmd->flags = cmd->flags | 1;
                BtlAiPadSteer(self, dir, 0);
                if (BtlCombatGetEnergy(&self->owner->combat) <= 0.0f) {
                    cmd->state = 5;
                }
            } else if ((cmd->flags & 1) != 0) {
                cmd->timerElapsed = 0.0f;
                cmd->timerLimit = 0.200000003f;
                cmd->timerExpired = 0;
                cmd->state = 4;
            } else {
                cmd->state = 5;
            }
        } else if (hit != 0 && BtlAiIsBlockingKind(self, hit) != 0) {
            /* lv.q/sv.q copy of the owner's velocity; only its y lane is read. */
            velY = self->owner->base.velocity[1];
            press = true;
            if (self->owner->combat.stats->dashMode == 1) {
                press = velY <= 0.0f;
            }
            if (press) {
                BtlAiPadPressDash(&self->pad);
            }
            BtlAiPadSteer(self, dir, 0);
            if (BtlCombatGetEnergy(&self->owner->combat) <= 0.0f) {
                cmd->failed = 1;
            }
            if (cmd->failed != 0) {
                cmd->state = 5;
            }
        } else {
            cmd->state = 5;
        }
        break;
    case 4:
        BtlAiPadPressDash(&self->pad);
        BtlAiPadSteer(self, dir, 0);
        if (cmd->timerExpired != 0) {
            cmd->state = 5;
        }
        break;
    case 5:
        /* owner free: no busy bits 0x400000/0x100, not state 8/10, no command bits 0xc00 */
        if (!((self->owner->stateFlags & 0x400000) == 0 &&
              self->owner->state != 8 && self->owner->state != 10 &&
              (self->owner->commands & 0xc00) == 0 &&
              (self->owner->stateFlags & 0x100) == 0)) {
            break;
        }
        cmd->state = cmd->state + 1;
        cmd->finished = 1;
        break;
    case 6:
        cmd->finished = 1;
        break;
    }
}
