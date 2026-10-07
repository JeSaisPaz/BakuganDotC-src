// bdc 0x08894c00 BtlAiMoveToGoalPoint
#include "bdc.h"

/* Drives the move channel `+0x2d8` of `BtlAi` towards the goal point
   `ai+0x9b0`/`+0x9b8` set by `BtlAiSetGoalPoint` (heading from `atan2f` of goal − owner
   position): step `+0x2f8` 1/2 waits until the owner is free, then every third call (counter
   `+0x2e0`) casts a ray of length `ai+0xa18` along the heading (`BtlAiRaycastHeading`). On a hit
   — unless kind 3 in `BtlAiIsScoreMode1` mode, which ends the move and resets the attack
   channel `+0x404` — it picks a sub-step: with no enemy within 550 (`BtlAiHasEnemyWithin`) and
   a hit kind 1/4 (`BtlAiIsMoveMode1Or4`) a roll against the kind parameter object (`ai+0x2cc`
   virtual `+0x1c`) chooses 7 (attack it with `BtlAiExecComboAttack`, aborted with
   `BtlAiCommandFinish`/`BtlBakuganResetCombo` when an enemy comes within 1500); otherwise 6
   (dash, `BtlAiExecObstacleDash`) for obstruction kinds (`BtlAiIsBlockingKind`) or 8 (detour,
   `BtlAiExecDetourMove`; 9 instead of 8 when an enemy is near). Unless it is within 100 units
   (XZ) of the goal, which moves it to step 9, it then steers (`BtlAiAngleToDir8`,
   `BtlAiPadSteer`), holding beyond 500 units when energy allows (≥ 10 %, or ≥ 70 % after a
   refusal; never for kinds 0x15..0x20 or on non-ray frames). Step 9 or any unknown step resets
   the channel keeping its tag (`BtlAiChannelResetKeepTag`). */
void BtlAiMoveToGoalPoint(BtlAi *self)
{
    BtlAiChannel *ch = &self->channels[0];
    s32 rayFrame = 1;
    u8 hold = 0;
    float angle;
    float dist;
    float minEnergy;
    s32 hit;
    s32 next;
    s32 kind;
    s32 chance;
    const VtblEntry *entry;

    angle = atan2f(self->goal[2] - self->owner->base.pos[2],
                   self->goal[0] - self->owner->base.pos[0]);
    if (!(angle <= 3.1415927f)) {
        angle = angle - 6.2831855f;
    } else if (angle <= -3.1415927f) {
        angle = angle + 6.2831855f;
    }

    switch (ch->phase) {
    case 0:
        break;
    case 1:
        ch->phase = ch->phase + 1;
        ch->done = 1;
        /* fallthrough */
    case 2: {
        /* wait until the owner is free to move */
        s32 busy = 0;
        s32 free = 0;
        if ((self->owner->stateFlags & 0x400000) == 0) {
            if (self->owner->state == 8 || self->owner->state == 10) {
                busy = 1;
            }
            if (!busy && (self->owner->commands & 0xc00) == 0 &&
                (self->owner->stateFlags & 0x100) == 0) {
                free = 1;
            }
        }
        if (!free) {
            break;
        }
        ch->phase = ch->phase + 1;
    }
        /* fallthrough */
    case 3:
        /* ray test every third call */
        if (ch->arg0++ < 2) {
            rayFrame = 0;
        } else {
            ch->arg0 = 0;
            ch->phase = ch->phase + 1;
        }
        /* fallthrough */
    case 4:
        hit = BtlAiRaycastHeading(angle, self->rayLength, self);
        if (rayFrame && hit != 0) {
            if (BtlAiIsScoreMode1() != 0 && hit == 3) {
                ch->phase = 9;
                BtlAiChannelReset(&self->channels[1]);
            } else {
                if (BtlAiHasEnemyWithin(550.0f, self) != 0) {
                    next = 9;
                    if (BtlAiIsBlockingKind(self, hit) != 0) {
                        next = 6;
                    }
                } else {
                    next = 0;
                    if (BtlAiIsMoveMode1Or4(self, hit) != 0) {
                        entry = &self->params->vtbl[3];
                        chance = ((s32 (*)(void *))entry->fn)((u8 *)self->params + entry->delta);
                        if ((s32)CoreRandNext(99) < chance) {
                            next = 7;
                        }
                    }
                    if (next == 0) {
                        next = 8;
                        if (BtlAiIsBlockingKind(self, hit) != 0) {
                            next = 6;
                        }
                    }
                }
                ch->phase = next;
            }
        }
        /* fallthrough */
    case 5:
        /* |owner->pos - goal| over x and z (y zeroed) */
    {
        float dx = self->owner->base.pos[0] - self->goal[0];
        float dz = self->owner->base.pos[2] - self->goal[2];
        dist = __builtin_sqrtf(dx * dx + dz * dz);
    }
        if (dist <= 100.0f) {
            ch->phase = 9;
            break;
        }
        if (!(dist <= 500.0f)) {
            if (ch->toggle == 0) {
                minEnergy = 10.0f;
            } else {
                minEnergy = 70.0f;
            }
            kind = -1;
            if (self->owner != NULL) {
                kind = (s32)self->owner->base.base.unk08;
            }
            if ((kind >= 0x15 && kind < 0x21) || !rayFrame ||
                BtlCombatGetEnergy(&self->owner->combat) * 0.001f * 100.0f < minEnergy) {
                ch->toggle = 1;
            } else {
                hold = 1;
                ch->toggle = 0;
            }
        }
        BtlAiPadSteer(self, (s16)BtlAiAngleToDir8(angle), hold);
        break;
    case 6:
        self->moveFlags = self->moveFlags | 0x2000;
        BtlAiExecObstacleDash(self, &ch->exec, (s16)BtlAiAngleToDir8(angle));
        if (ch->exec.finished != 0) {
            self->moveFlags = self->moveFlags & ~0x2000u;
            ch->phase = ch->exec.failed != 0 ? 8 : 4;
            ch->exec.state = 0;
            ch->exec.timerLimit = 0.0f;
            ch->exec.timerElapsed = 0.0f;
            ch->exec.timerExpired = 1;
            ch->exec.flags = 0;
            ch->exec.arg = 0;
            ch->exec.argF = 0.0f;
            ch->exec.finished = 0;
            ch->exec.failed = 0;
        }
        break;
    case 7:
        if (BtlAiHasEnemyWithin(1500.0f, self) != 0) {
            BtlAiCommandFinish(self, &ch->exec);
            BtlBakuganResetCombo(self->owner);
            ch->phase = 9;
            break;
        }
        self->moveFlags = self->moveFlags | 0x1000;
        BtlAiExecComboAttack(self, &ch->exec, 0);
        if (ch->exec.finished != 0) {
            self->moveFlags = self->moveFlags & ~0x1000u;
            ch->phase = ch->exec.failed != 0 ? 8 : 4;
            ch->exec.state = 0;
            ch->exec.timerLimit = 0.0f;
            ch->exec.timerElapsed = 0.0f;
            ch->exec.timerExpired = 1;
            ch->exec.flags = 0;
            ch->exec.arg = 0;
            ch->exec.argF = 0.0f;
            ch->exec.finished = 0;
            ch->exec.failed = 0;
        }
        break;
    case 8:
        BtlAiExecDetourMove(self, &ch->exec);
        if (ch->exec.finished != 0) {
            ch->phase = ch->exec.failed != 0 ? 9 : 4;
            ch->exec.state = 0;
            ch->exec.timerLimit = 0.0f;
            ch->exec.timerElapsed = 0.0f;
            ch->exec.timerExpired = 1;
            ch->exec.flags = 0;
            ch->exec.arg = 0;
            ch->exec.argF = 0.0f;
            ch->exec.finished = 0;
            ch->exec.failed = 0;
        }
        break;
    default:
        BtlAiChannelResetKeepTag(ch);
        break;
    }
}
