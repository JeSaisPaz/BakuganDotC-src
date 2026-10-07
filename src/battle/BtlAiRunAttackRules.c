// bdc 0x08898918 BtlAiRunAttackRules
#include "bdc.h"

/* Attack step of `BtlAi` (last rule step of `BtlAiThink`) on the attack channel
   `+0x404`: advances its step counter while `ai+0x944` is clear and is skipped while that counter
   `+0x420` is non-zero. When idle (command 0x1b) and a target exists it picks a rule (`BtlAiSelectRule` with `BtlAiEvalConditionXZ` through
   `g_btlAiAttackRuleCond`) and queues it
   (`BtlAiRollRange`, `BtlAiChannelSetCommand`). A new command draws a start delay from the
   weight table `ai+0x9f4` (`BtlAiWeightTablePick`) and sets up the step machine `+0x424`: 0xa
   presses `BtlAiPadPress20010` (or, for owner kinds 0x19/0x1a, a combo with input 0); 0xb/0x10
   press `BtlAiPadPress810` after adding a full charge (`BtlBakuganAddCharge``(100)`) and start
   the cooldown `ai+0x948` = owner parameter `+0xcc` × 3 checked by `BtlAiIsCommandAllowed`;
   0xc/0xd/0xe/0x1a run combos with opening input 0/1/2/2 (`BtlAiExecComboAttack` on the command
   record `+0x50c`); 0x11 presses `BtlAiPadPress08`; arts 0x12..0x15 press `BtlAiPadPressB0`
   (slot 0) or `BtlAiPadPress130` depending on which slot holds the art of tier `cmd − 0x12`
   (`BtlAiFindOwnerArtSlot`); moves are forwarded to the move channel `+0x2d8`; 0xf/0x16/0x17 and
   unknown ones reset the channel. The steps hold the other channels (`BtlAiChannelAdvanceStep`),
   wait the delay, apply the channel's pad predicates (`BtlAiChannelTestPadConditions`,
   `MemberFnPtr` constants such as `g_btlAiPress810Cond`) and wait for the owner's attack state (8/10) or
   bit 0x100 to pass, then reset the move/reaction channels or end on the rule condition
   (`BtlAiEvalRuleCondition`, `BtlAiChannelResetKeepTag`). */

/* One 1/30-s tick of the channel's phase timer unless it already expired; returns the expired
   byte read back afterwards. */
static u8 PhaseTimerTick(BtlAiChannel *ch)
{
    float elapsed;
    float limit;

    if (ch->phaseExpired != 0) {
        return ch->phaseExpired;
    }
    elapsed = ch->phaseElapsed + 0.0333333351f;
    limit = ch->phaseLimit;
    ch->phaseElapsed = elapsed;
    if (elapsed < limit) {
        return 0;
    }
    ch->phaseElapsed = limit;
    ch->phaseExpired = 1;
    return ch->phaseExpired;
}

/* One 1/30-s tick of the channel's command timer unless it already expired. */
static void CmdTimerTick(BtlAiChannel *ch)
{
    float elapsed;
    float limit;

    if (ch->cmdExpired != 0) {
        return;
    }
    elapsed = ch->cmdElapsed + 0.0333333351f;
    limit = ch->cmdLimit;
    ch->cmdElapsed = elapsed;
    if (!(elapsed < limit)) {
        ch->cmdElapsed = limit;
        ch->cmdExpired = 1;
    }
}

static int OwnerAttacking(BtlAi *self)
{
    s32 state = self->owner->state;

    return state == 8 || state == 10;
}

void BtlAiRunAttackRules(BtlAi *self)
{
    BtlAiChannel *ch = &self->channels[1];
    u32 phase;

    if (self->startExpired == 0) {
        BtlAiChannelAdvanceStep(ch);
    }
    if (ch->step != 0) {
        return;
    }
    if (ch->pendingCmd == 0x1b) {
        BtlAiRuleRecord *rule;
        float value;

        if (self->target == NULL) {
            return;
        }
        rule = (BtlAiRuleRecord *)BtlAiSelectRule(self, ch, &g_btlAiAttackRuleCond);
        if (rule == NULL) {
            return;
        }
        value = BtlAiRollRange(self, (BtlAiRange *)&rule->byte30);
        BtlAiChannelSetCommand(value, ch, rule->byte02, rule->byte30, rule->byte3c, rule->short40);
        ch->ruleId = rule->command;
    }

    if (BtlAiChannelHasPending(ch) != 0) {
        s32 frames;
        float limit;
        u32 cmd;

        ch->pendingCmd = ch->cmd;
        frames = BtlAiWeightTablePick(&self->delayWeights[0]);
        cmd = (u32)ch->pendingCmd;
        limit = (float)frames * 0.0333333351f;
        ch->phaseElapsed = 0.0f;
        ch->phaseLimit = limit;
        ch->phaseExpired = limit <= 0.0f;
        switch (cmd) {
        case 0:
            ch->phase = 0;
            break;
        case 10:
            if (self->owner->base.base.unk08 == 0x19 || self->owner->base.base.unk08 == 0x1a) {
                ch->phase = 2;
                ch->arg0 = 0;
            } else {
                ch->phase = 6;
                ch->padConds[0] = g_btlAiPress20010Cond;
            }
            break;
        case 0xb:
        case 0x10:
            ch->phase = 10;
            ch->padConds[0] = g_btlAiPress810Cond;
            BtlBakuganAddCharge(100.0f, self->owner);
            self->chargeCooldown = (s32)(self->owner->combat.stats->chargeFrames * 3.0f);
            break;
        case 0xc:
            ch->phase = 2;
            ch->arg0 = 0;
            break;
        case 0xd:
            ch->phase = 2;
            ch->arg0 = 1;
            break;
        case 0xe:
        case 0x1a:
            ch->phase = 2;
            ch->arg0 = 2;
            break;
        case 0x11:
            ch->phase = 0x14;
            ch->padConds[0] = g_btlAiPress08Cond;
            break;
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
            ch->phase = 0xf;
            if (BtlAiFindOwnerArtSlot(self, (s32)cmd - 0x12) == 0) {
                ch->padConds[0] = g_btlAiPressB0Cond;
            } else {
                ch->padConds[0] = g_btlAiPress130Cond;
            }
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        case 0x18:
        case 0x19:
            /* a move command: hand it to the move channel with this channel's end condition */
            BtlAiChannelSetCommand(ch->condValue, &self->channels[0], (s32)cmd, ch->condKind,
                                   ch->condOp, -1);
            ch->phase = 0;
            break;
        default: /* 0xf, 0x16, 0x17 and anything above 0x1a */
            BtlAiChannelReset(ch);
            return;
        }
    }

    phase = (u32)ch->phase;
    switch (phase) {
    case 0:
        CmdTimerTick(ch);
        if (BtlAiEvalRuleCondition(self, ch)) {
            ch->phase++;
        }
        break;
    case 1:
        BtlAiChannelResetKeepTag(ch);
        break;
    case 2:
        /* combo: hold the move channel */
        BtlAiChannelAdvanceStep(&self->channels[0]);
        ch->done = 1;
        ch->phase++;
        break;
    case 3:
        if (PhaseTimerTick(ch) == 0) {
            return;
        }
        ch->phase = 4;
        /* fall through */
    case 4:
        BtlAiExecComboAttack(self, &ch->exec, ch->arg0);
        if (ch->exec.finished != 0) {
            ch->exec.state = 0;
            ch->exec.timerLimit = 0.0f;
            ch->exec.timerElapsed = 0.0f;
            ch->exec.timerExpired = 1;
            ch->exec.flags = 0;
            ch->exec.arg = 0;
            ch->exec.failed = 0;
            ch->exec.argF = 0.0f;
            ch->exec.finished = 0;
            /* `failed` was cleared just above, so this always picks 5 (as in the listing) */
            ch->phase = ch->exec.failed != 0 ? 1 : 5;
        }
        break;
    case 5:
        BtlAiChannelReset(&self->channels[0]);
        ch->phase = 0;
        break;
    case 6: /* dash (0xa): press, wait for the owner's attack state */
        ch->done = 1;
        ch->phase = 7;
        /* fall through */
    case 7:
        if (PhaseTimerTick(ch) == 0) {
            return;
        }
        ch->phase++;
        /* fall through */
    case 8:
        if (BtlAiChannelTestPadConditions(self, ch) != 0) {
            ch->phase++;
        }
        break;
    case 9:
        if (!OwnerAttacking(self)) {
            ch->phase = 0;
        }
        break;
    case 10: /* charge (0xb/0x10): hold the reaction channel */
        BtlAiChannelAdvanceStep(&self->channels[3]);
        ch->done = 1;
        ch->phase++;
        /* fall through */
    case 0xb:
        if (PhaseTimerTick(ch) == 0) {
            return;
        }
        ch->phase++;
        /* fall through */
    case 0xc:
        if (BtlAiChannelTestPadConditions(self, ch) != 0) {
            ch->phase++;
        }
        break;
    case 0xd:
        BtlAiChannelTestPadConditions(self, ch);
        if (OwnerAttacking(self)) {
            ch->phase++;
        }
        break;
    case 0xe:
        if (!OwnerAttacking(self)) {
            BtlAiChannelReset(&self->channels[3]);
            ch->phase = 0;
        }
        break;
    case 0xf: /* art (0x12..0x15): hold the move and reaction channels */
        BtlAiChannelAdvanceStep(&self->channels[0]);
        BtlAiChannelAdvanceStep(&self->channels[3]);
        ch->done = 1;
        ch->phase++;
        /* fall through */
    case 0x10:
        if (PhaseTimerTick(ch) == 0) {
            return;
        }
        ch->phase++;
        /* fall through */
    case 0x11:
        if (BtlAiChannelTestPadConditions(self, ch) != 0) {
            ch->phase++;
        }
        break;
    case 0x12:
        BtlAiChannelTestPadConditions(self, ch);
        if ((self->owner->stateFlags & 0x100) != 0) {
            ch->phase++;
        }
        break;
    case 0x13:
        if ((self->owner->stateFlags & 0x100) == 0) {
            BtlAiChannelReset(&self->channels[0]);
            BtlAiChannelReset(&self->channels[3]);
            ch->phase = 0;
        }
        break;
    case 0x14: /* 0x11: press, then end on the rule condition */
        ch->done = 1;
        ch->phase = 0x15;
        /* fall through */
    case 0x15:
        if (PhaseTimerTick(ch) == 0) {
            return;
        }
        ch->phase++;
        /* fall through */
    case 0x16:
        BtlAiChannelTestPadConditions(self, ch);
        /* fall through */
    case 0x17:
        CmdTimerTick(ch);
        if (BtlAiEvalRuleCondition(self, ch)) {
            ch->phase = 0x18;
        }
        break;
    case 0x18:
        BtlAiChannelResetKeepTag(ch);
        break;
    default:
        break;
    }
}
