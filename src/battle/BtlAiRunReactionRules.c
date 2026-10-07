// bdc 0x08895ddc BtlAiRunReactionRules
#include "bdc.h"

/* Reaction step of `BtlAi`, run first by `BtlAiThink` on `channels[3]` (skipped
   while its `step` is non-zero). When the channel is idle (pending command 0x1b) it records a
   target that answers virtual slot 10 as `threat` with `moveFlags` bit 0x80
   (`BtlAiIsUnitThreatening`), 0x100 (`BtlAiUnitHasCommandC00`) or 0x200
   (`BtlAiTargetInRecoveryState`), the incoming attack (`BtlAiFindIncomingAttack`), then picks a
   rule (`BtlAiSelectRule` with `g_btlAiReactionRuleCond`) and queues it (`BtlAiRollRange`,
   `BtlAiChannelSetCommand`). Each frame it drops a threat whose test no longer holds and an
   attack that is gone (`BtlAttackListFind`), ended (`endFrame`) or aimed at another unit. A new
   command is dispatched: 0 just enters phase 1; attacks 0xa..0xe/0x10/0x12..0x15 are forwarded to
   `channels[1]`; 0x11 holds channels 0/1 (`BtlAiChannelAdvanceStep`), rolls `waitChance` percent
   (needs ability flag 0x8000000) to wait for the threat to commit (state 7/9,
   `BtlAiUnitStateIs7Or9`, or a live attack, `BtlAiIsNonNull`) and then presses pad bit 8
   (`BtlAiPadPress08`) until the end condition; 0x1a runs a combo starting with input 2
   (`BtlAiExecComboAttack`) after the phase delay; 0xf, 0x16, 0x17 and values above 0x1a reset
   the channel; every other command is forwarded to `channels[0]` while channels 2 and 1 are held.
   A command ends when its condition holds (`BtlAiEvalRuleCondition`) or (except 0x1a) no threat
   or attack remains, resetting the channel (`BtlAiChannelResetKeepTag`) plus, for forwarded
   moves, channels 2 and 1 and, for 0x11, channels 0 and 1. */

void BtlAiRunReactionRules(BtlAi *self)
{
    BtlAiChannel *channel = &self->channels[3];
    BtlBakugan *unit;
    const VtblEntry *entry;
    BtlAiRuleRecord *rule;
    BtlAttack *attack;
    float value;
    s8 chance;
    bool done;
    u32 cmd;

    if (channel->step != 0) {
        return;
    }

    if (channel->pendingCmd == 0x1b) {
        unit = self->target;
        if (unit != NULL) {
            entry = &((const VtblEntry *)unit->base.base.vtable)[10];
            if (((s32 (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
                unit = self->target;
                if (BtlAiIsUnitThreatening(self, unit) != 0) {
                    self->threat = unit;
                    self->moveFlags |= 0x80;
                } else if (BtlAiUnitHasCommandC00(self, unit) != 0) {
                    self->threat = unit;
                    self->moveFlags |= 0x100;
                } else if (BtlAiTargetInRecoveryState(self, unit) != 0) {
                    self->threat = unit;
                    self->moveFlags |= 0x200;
                }
            }
        }
        self->incomingAttack = BtlAiFindIncomingAttack(self);
        rule = BtlAiSelectRule(self, channel, &g_btlAiReactionRuleCond);
        if (rule != NULL) {
            value = BtlAiRollRange(self, (BtlAiRange *)&rule->byte30);
            BtlAiChannelSetCommand(value, channel, rule->byte02, rule->byte30, rule->byte3c,
                                   rule->short40);
            channel->ruleId = rule->command;
        }
    }

    self->incomingAttack = BtlAttackListFind((BtlAttack *)self->incomingAttack);
    unit = self->threat;
    done = false;
    if (unit != NULL) {
        if ((self->moveFlags & 0x80) != 0) {
            if (BtlAiIsUnitThreatening(self, unit) == 0) {
                self->threat = NULL;
                self->moveFlags &= ~0x80u;
            }
        } else if ((self->moveFlags & 0x100) != 0) {
            if (BtlAiUnitHasCommandC00(self, unit) == 0) {
                self->threat = NULL;
                self->moveFlags &= ~0x100u;
            }
        } else if ((self->moveFlags & 0x200) != 0) {
            if (BtlAiTargetInRecoveryState(self, unit) == 0) {
                self->threat = NULL;
                self->moveFlags &= ~0x200u;
            }
        }
    }
    attack = (BtlAttack *)self->incomingAttack;
    if (attack != NULL && attack->owner != NULL) {
        if (attack->endFrame != 0) {
            self->incomingAttack = NULL;
        } else if (attack->owner->targetId != self->owner->base.base.id) {
            self->incomingAttack = NULL;
        }
    }
    if (self->threat == NULL && self->incomingAttack == NULL) {
        done = true;
    }

    if (BtlAiChannelHasPending(channel) != 0) {
        cmd = (u32)channel->cmd;
        channel->pendingCmd = (s32)cmd;
        switch (cmd) {
        case 0:
            channel->phase = 1;
            return;
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x10:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
            BtlAiChannelSetCommand(channel->condValue, &self->channels[1], (s32)cmd, channel->condKind,
                                   channel->condOp, -1);
            channel->phase = 1;
            channel->done = 1;
            return;
        case 0x11:
            BtlAiChannelAdvanceStep(&self->channels[0]);
            BtlAiChannelAdvanceStep(&self->channels[1]);
            chance = (s8)self->waitChance;
            if ((s32)CoreRandNext(99) < chance) {
                channel->argsSet = 1;
            }
            if ((self->allowedCmds & 0x8000000) == 0) {
                channel->argsSet = 0;
            }
            channel->phase = 5;
            channel->done = 1;
            return;
        case 0x1a:
            channel->phase = 9;
            channel->arg0 = 2;
            return;
        case 0xf:
        case 0x16:
        case 0x17:
            break;
        default:
            if (cmd >= 0x1b) {
                break;
            }
            BtlAiChannelAdvanceStep(&self->channels[2]);
            BtlAiChannelSetCommand(channel->condValue, &self->channels[0], channel->pendingCmd,
                                   channel->condKind, channel->condOp, -1);
            BtlAiChannelAdvanceStep(&self->channels[1]);
            channel->phase = 3;
            channel->done = 1;
            return;
        }
        BtlAiChannelReset(channel);
        return;
    }

    if (channel->cmdExpired == 0) {
        channel->cmdElapsed = channel->cmdElapsed + 0.0333333351f;
        if (!(channel->cmdElapsed < channel->cmdLimit)) {
            channel->cmdElapsed = channel->cmdLimit;
            channel->cmdExpired = 1;
        }
    }

    switch ((u32)channel->phase) {
    case 1:
        if (!done) {
            done = BtlAiEvalRuleCondition(self, channel);
        }
        /* fall through */
    case 2:
        if (done) {
            BtlAiChannelResetKeepTag(channel);
        }
        return;
    case 3:
        if (!done) {
            done = BtlAiEvalRuleCondition(self, channel);
        }
        /* fall through */
    case 4:
        if (done) {
            BtlAiChannelResetKeepTag(channel);
            BtlAiChannelReset(&self->channels[2]);
            BtlAiChannelReset(&self->channels[1]);
        }
        return;
    case 5:
        if (channel->argsSet != 0) {
            if (self->threat != NULL) {
                if (BtlAiUnitStateIs7Or9(self, self->threat) == 0) {
                    return;
                }
            } else if (self->incomingAttack != NULL) {
                if (BtlAiIsNonNull(self, self->incomingAttack) == 0) {
                    return;
                }
            }
        }
        channel->phase = channel->phase + 1;
        /* fall through */
    case 6:
        BtlAiPadPress08(&self->pad);
        /* fall through */
    case 7:
        if (!done) {
            done = BtlAiEvalRuleCondition(self, channel);
        }
        /* fall through */
    case 8:
        if (done) {
            BtlAiChannelReset(&self->channels[0]);
            BtlAiChannelReset(&self->channels[1]);
            BtlAiChannelResetKeepTag(channel);
        }
        return;
    case 9:
        channel->done = 1;
        channel->phase = 10;
        return;
    case 10:
        if (channel->phaseExpired == 0) {
            channel->phaseElapsed = channel->phaseElapsed + 0.0333333351f;
            if (!(channel->phaseElapsed < channel->phaseLimit)) {
                channel->phaseElapsed = channel->phaseLimit;
                channel->phaseExpired = 1;
            }
        }
        if (channel->phaseExpired == 0) {
            return;
        }
        channel->phase = 11;
        /* fall through */
    case 11:
        BtlAiExecComboAttack(self, &channel->exec, channel->arg0);
        if (channel->exec.finished != 0) {
            channel->exec.state = 0;
            channel->exec.timerLimit = 0.0f;
            channel->exec.timerElapsed = 0.0f;
            channel->exec.timerExpired = 1;
            channel->exec.flags = 0;
            channel->exec.arg = 0;
            channel->exec.argF = 0.0f;
            channel->exec.finished = 0;
            channel->exec.failed = 0;
            channel->phase = 0xc;
        }
        return;
    case 0xc:
        channel->phase = 0xd;
        return;
    case 0xd:
        if (BtlAiEvalRuleCondition(self, channel)) {
            channel->phase = 0xe;
            BtlAiChannelResetKeepTag(channel);
        }
        return;
    default:
        return;
    }
}
