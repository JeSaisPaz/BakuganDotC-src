// bdc 0x08896414 BtlAiRunMoveRules
#include "bdc.h"

/* Movement step of `BtlAi` (called by `BtlAiThink`) on the move channel `+0x2d8`,
   skipped while its step counter `+0x2f4` is non-zero. When idle (command 0x1b) it picks a rule
   (`BtlAiSelectRule` with `g_btlAiMoveRuleCond`, i.e. `BtlAiEvalConditionXZ`) and queues it
   (`BtlAiRollRange`, `BtlAiChannelSetCommand`). A new command draws a start delay from the
   weight table `ai+0xa04` (`BtlAiWeightTablePick`, frames), clears the odometer `ai+0x9a0` and
   loads the channel's pad predicates (`+0x3c8..+0x3dc`, `MemberFnPtr`s from
   `g_btlAiMovePadConds` to `BtlAiPadStickForward`, `BtlAiPadStickDirC000`,
   `BtlAiPadStickDir4000`, `BtlAiPadStickBack`, `BtlAiPadPress04`, `BtlAiPadPressDash`):
   1/2 forward, 3/4 sideways 0xc000, 5/6 sideways 0x4000, 7/8 back (0x8000) — the even ones also
   hold button 4 (`BtlAiPadPress04`) —, 9 a random direction (`CoreRandNext``(4)`, forward
   while `ai+0xa26` is set) plus dash, 0x18/0x19 forward plus a random side (0x18 also holding
   button 4); 0 goes straight to waiting for the end condition, 0x1a is a delayed combo attack with
   opening input 2 (step 0xd); attack commands 10..0xe/0x10..0x15 are forwarded to the attack
   channel `+0x404`; 0xf/0x16/0x17 and unknown ones reset the channel and return. Every step
   but 0 also runs the command timer `+0x30..+0x38`. The step machine `+0x2f8` then applies the
   predicates (`BtlAiChannelTestPadConditions`) and, for directional moves, casts a ray in the
   move direction (`BtlAiRaycastDir8`): a hit leads to a dash (`BtlAiExecObstacleDash`,
   obstruction kinds, `BtlAiIsBlockingKind`), an attack on the obstacle
   (`BtlAiExecComboAttack`, hit kind 1/4 by a roll against the kind parameter object `ai+0x2cc`
   virtual `+0x1c`, with the target pushed aside by `BtlAiPushTarget`/`BtlAiPopTarget`,
   stopped by an enemy within 1500, `BtlAiHasEnemyWithin`) or a detour
   (`BtlAiExecDetourMove`), while a non-obstruction hit with an enemy within 550 ends the move.
   The command ends when its condition holds (`BtlAiEvalRuleCondition`), resetting the channel
   with `BtlAiChannelResetKeepTag`. */

/* Inlined reset of the channel's executing command record. */
static inline void BtlAiMoveCommandClear(BtlAiCommand *cmd)
{
    cmd->state = 0;
    cmd->timerLimit = 0.0f;
    cmd->timerElapsed = 0.0f;
    cmd->timerExpired = 1;
    cmd->flags = 0;
    cmd->arg = 0;
    cmd->argF = 0.0f;
    cmd->finished = 0;
    cmd->failed = 0;
}

void BtlAiRunMoveRules(BtlAi *self)
{
    BtlAiChannel *channel;
    BtlAiRuleRecord *rule;
    BtlBakugan *owner;
    const VtblEntry *entry;
    float value;
    float limit;
    float elapsed;
    u32 cmd;
    u32 phase;
    s32 delay;
    s32 hit;
    s32 next;
    s32 chance;
    s32 roll;
    s32 input;

    channel = &self->channels[0];
    if (channel->step != 0) {
        return;
    }
    if (channel->pendingCmd == 0x1b) {
        rule = BtlAiSelectRule(self, channel, &g_btlAiMoveRuleCond);
        if (rule == NULL) {
            return;
        }
        value = BtlAiRollRange(self, (BtlAiRange *)&rule->byte30);
        BtlAiChannelSetCommand(value, channel, rule->byte02, rule->byte30, rule->byte3c,
                               rule->short40);
        channel->ruleId = rule->command;
    }

    if (BtlAiChannelHasPending(channel) != 0) {
        channel->pendingCmd = channel->cmd;
        delay = BtlAiWeightTablePick(&self->delayWeights[1]);
        cmd = (u32)channel->pendingCmd;
        channel->phaseElapsed = 0.0f;
        limit = (float)delay * 0.033333335f;
        channel->phaseLimit = limit;
        channel->phaseExpired = (limit <= 0.0f);
        switch (cmd) {
        case 0:
            channel->argsSet = 0;
            channel->phase = 3;
            break;
        case 1:
            channel->padConds[0] = g_btlAiMovePadConds[0];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0;
            break;
        case 2:
            channel->padConds[0] = g_btlAiMovePadConds[0];
            channel->padConds[1] = g_btlAiMovePadConds[1];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0;
            break;
        case 3:
            channel->padConds[0] = g_btlAiMovePadConds[2];
            channel->argsSet = 1;
            channel->arg0 = 0xc000;
            channel->arg1 = 0xc000;
            break;
        case 4:
            channel->padConds[0] = g_btlAiMovePadConds[2];
            channel->padConds[1] = g_btlAiMovePadConds[1];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0xc000;
            break;
        case 5:
            channel->padConds[0] = g_btlAiMovePadConds[3];
            channel->argsSet = 1;
            channel->arg0 = 0x4000;
            channel->arg1 = 0x4000;
            break;
        case 6:
            channel->padConds[0] = g_btlAiMovePadConds[3];
            channel->padConds[1] = g_btlAiMovePadConds[1];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0x4000;
            break;
        case 7:
            channel->padConds[0] = g_btlAiMovePadConds[4];
            channel->argsSet = 1;
            channel->arg0 = 0x8000;
            channel->arg1 = 0x8000;
            break;
        case 8:
            channel->padConds[0] = g_btlAiMovePadConds[4];
            channel->padConds[1] = g_btlAiMovePadConds[1];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0x8000;
            break;
        case 9:
            if (self->forwardDashFrames == 0) {
                switch (CoreRandNext(4)) {
                case 0:
                    channel->padConds[0] = g_btlAiMovePadConds[0];
                    break;
                case 1:
                    channel->padConds[0] = g_btlAiMovePadConds[2];
                    break;
                case 2:
                    channel->padConds[0] = g_btlAiMovePadConds[3];
                    break;
                case 3:
                    channel->padConds[0] = g_btlAiMovePadConds[4];
                    break;
                default:
                    break;
                }
            } else {
                channel->padConds[0] = g_btlAiMovePadConds[0];
            }
            channel->padConds[1] = g_btlAiMovePadConds[5];
            channel->argsSet = 0;
            break;
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
            BtlAiChannelSetCommand(channel->condValue, &self->channels[1], (s32)cmd,
                                   channel->condKind, channel->condOp, -1);
            channel->phase = 3;
            break;
        case 0x18:
            channel->padConds[0] = g_btlAiMovePadConds[0];
            roll = (s32)CoreRandNext(2);
            if (roll == 0) {
                channel->padConds[1] = g_btlAiMovePadConds[6];
            } else if (roll == 1) {
                channel->padConds[1] = g_btlAiMovePadConds[7];
            }
            channel->padConds[2] = g_btlAiMovePadConds[8];
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0;
            break;
        case 0x19:
            channel->padConds[0] = g_btlAiMovePadConds[0];
            roll = (s32)CoreRandNext(2);
            if (roll == 0) {
                channel->padConds[1] = g_btlAiMovePadConds[6];
            } else if (roll == 1) {
                channel->padConds[1] = g_btlAiMovePadConds[7];
            }
            channel->argsSet = 1;
            channel->arg0 = 0;
            channel->arg1 = 0;
            break;
        case 0x1a:
            channel->phase = 0xd;
            channel->arg0 = 2;
            break;
        default: /* 0xf, 0x16, 0x17 and unknown commands */
            BtlAiChannelReset(channel);
            return;
        }
        self->odometer = 0.0f;
    }

    phase = (u32)channel->phase;
    if (phase != 0 && channel->cmdExpired == 0) {
        elapsed = channel->cmdElapsed + 0.033333335f;
        limit = channel->cmdLimit;
        channel->cmdElapsed = elapsed;
        if (!(elapsed < limit)) {
            channel->cmdElapsed = limit;
            channel->cmdExpired = 1;
        }
    }

    switch (phase) {
    case 0:
        if (channel->phaseExpired == 0) {
            elapsed = channel->phaseElapsed + 0.033333335f;
            limit = channel->phaseLimit;
            channel->phaseElapsed = elapsed;
            if (elapsed < limit) {
                return;
            }
            channel->phaseElapsed = limit;
            channel->phaseExpired = 1;
            if (channel->phaseExpired == 0) {
                return;
            }
        }
        channel->phase = 1;
        /* fall through */
    case 1:
        channel->done = 1;
        if (BtlAiChannelTestPadConditions(self, channel) != 0) {
            channel->phase = channel->phase + 1;
        }
        break;
    case 2:
        BtlAiChannelTestPadConditions(self, channel);
        if (channel->argsSet != 0) {
            hit = BtlAiRaycastDir8(self->rayLength, self, channel->arg0);
            if (hit != 0) {
                if (BtlAiIsScoreMode1() != 0 && hit == 3) {
                    channel->phase = 0x11;
                    BtlAiChannelResetKeepTag(channel);
                    BtlAiChannelReset(&self->channels[1]);
                } else if (BtlAiHasEnemyWithin(550.0f, self) != 0) {
                    if (BtlAiIsBlockingKind(self, hit) != 0) {
                        channel->phase = 4;
                    } else {
                        channel->phase = 0x11;
                        BtlAiChannelResetKeepTag(channel);
                    }
                } else {
                    next = 0;
                    if (BtlAiIsMoveMode1Or4(self, hit) != 0) {
                        entry = &self->params->vtbl[3];
                        chance = ((s32 (*)(void *))entry->fn)((u8 *)self->params + entry->delta);
                        if ((s32)CoreRandNext(99) < chance) {
                            next = 6;
                        }
                    }
                    if (next == 0) {
                        next = 0xb;
                        if (BtlAiIsBlockingKind(self, hit) != 0) {
                            next = 4;
                        }
                    }
                    channel->phase = next;
                }
            }
        }
        /* fall through */
    case 3:
        if (BtlAiEvalRuleCondition(self, channel)) {
            channel->phase = 0x11;
            BtlAiChannelResetKeepTag(channel);
        }
        break;
    case 4:
    case 5:
        if (phase == 4) {
            BtlAiChannelAdvanceStep(&self->channels[1]);
            self->moveFlags |= 0x2000;
            channel->phase = channel->phase + 1;
        }
        BtlAiExecObstacleDash(self, &channel->exec, (s16)channel->arg1);
        if (channel->exec.finished != 0) {
            BtlAiChannelReset(&self->channels[1]);
            self->moveFlags &= ~0x2000u;
            if (channel->exec.failed != 0) {
                channel->phase = 0xb;
                BtlAiMoveCommandClear(&channel->exec);
            } else {
                channel->phase = 0x11;
                BtlAiMoveCommandClear(&channel->exec);
                BtlAiChannelResetKeepTag(channel);
            }
        }
        break;
    case 6:
    case 7:
        if (phase == 6) {
            BtlAiChannelAdvanceStep(&self->channels[2]);
            BtlAiChannelAdvanceStep(&self->channels[1]);
            self->moveFlags |= 0x1000;
            BtlAiPushTarget(self, NULL);
            channel->phase = channel->phase + 1;
        }
        hit = BtlAiRaycastDir8(self->rayLength, self, 0);
        if (BtlAiIsMoveMode1Or4(self, hit) == 0) {
            channel->phase = 10;
        } else {
            owner = self->owner;
            if ((owner->stateFlags & 0x400000) == 0 && owner->state != 8 && owner->state != 10 &&
                (owner->commands & 0xc00) == 0 && (owner->stateFlags & 0x100) == 0) {
                channel->phase = 8;
            }
        }
        break;
    case 8:
    case 9:
        if (phase == 8 && BtlAiHasEnemyWithin(1500.0f, self) != 0) {
            BtlAiCommandFinish(self, &channel->exec);
            BtlBakuganResetCombo(self->owner);
            channel->phase = 10;
            return;
        }
        BtlAiExecComboAttack(self, &channel->exec, 0);
        hit = BtlAiRaycastDir8(self->rayLength, self, 0);
        if (BtlAiIsMoveMode1Or4(self, hit) == 0) {
            BtlAiCommandFinish(self, &channel->exec);
            BtlBakuganResetCombo(self->owner);
            channel->phase = 10;
        } else if (channel->exec.finished != 0) {
            if (channel->exec.failed != 0) {
                BtlAiPopTarget(self);
                BtlAiChannelReset(&self->channels[2]);
                BtlAiChannelReset(&self->channels[1]);
                self->moveFlags &= ~0x1000u;
                channel->phase = 0xb;
            } else {
                channel->phase = 8;
            }
            BtlAiMoveCommandClear(&channel->exec);
        }
        break;
    case 10:
        BtlAiPopTarget(self);
        BtlAiChannelReset(&self->channels[2]);
        BtlAiChannelReset(&self->channels[1]);
        self->moveFlags &= ~0x1000u;
        channel->phase = 0x11;
        BtlAiChannelResetKeepTag(channel);
        break;
    case 0xb:
        BtlAiChannelAdvanceStep(&self->channels[2]);
        BtlAiChannelAdvanceStep(&self->channels[1]);
        BtlAiPushTarget(self, NULL);
        channel->phase = channel->phase + 1;
        /* fall through */
    case 0xc:
        BtlAiExecDetourMove(self, &channel->exec);
        if (channel->exec.finished != 0) {
            BtlAiPopTarget(self);
            BtlAiChannelReset(&self->channels[2]);
            BtlAiChannelReset(&self->channels[1]);
            channel->phase = 0x11;
            BtlAiMoveCommandClear(&channel->exec);
            BtlAiChannelResetKeepTag(channel);
        }
        break;
    case 0xd:
        channel->done = 1;
        channel->phase = (s32)phase + 1;
        break;
    case 0xe:
    case 0xf:
        if (phase == 0xe) {
            if (channel->phaseExpired == 0) {
                elapsed = channel->phaseElapsed + 0.033333335f;
                limit = channel->phaseLimit;
                channel->phaseElapsed = elapsed;
                if (elapsed < limit) {
                    return;
                }
                channel->phaseElapsed = limit;
                channel->phaseExpired = 1;
                if (channel->phaseExpired == 0) {
                    return;
                }
            }
            channel->phase = (s32)phase + 1;
        }
        input = channel->arg0;
        BtlAiExecComboAttack(self, &channel->exec, input);
        if (channel->exec.finished != 0) {
            BtlAiMoveCommandClear(&channel->exec);
            channel->phase = 0x10;
        }
        break;
    case 0x10:
        channel->phase = 3;
        break;
    default: /* 0x11 (done) and above */
        break;
    }
}
