// bdc 0x08897ea4 BtlAiExecComboAttack
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Allocates `count` weights from the low end of the game heap and sets each to 1 (no NULL check,
   as in the binary). */
static s32 *WeightsAlloc(u32 count)
{
    bool fromLow;
    s32 *data;
    u32 i;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(count * sizeof(s32), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    for (i = 0; i < count; i++) {
        data[i] = 1;
    }
    return data;
}

static void WeightsFree(s32 *data)
{
    MemLock();
    MemFree(data, NULL, 0);
    MemUnlock();
}

/* Weighted draw: returns the first index whose running weight sum exceeds
   CoreRandNext(total), or -1 when there are no weights or they sum to 0. */
static s32 WeightsPick(const s32 *data, u32 count)
{
    s32 total = 0;
    s32 acc = 0;
    s32 roll;
    u32 i;

    if (data == NULL) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        total += data[i];
    }
    if (total == 0) {
        return -1;
    }
    roll = (s32)CoreRandNext((u32)total);
    for (i = 0; i < count; i++) {
        acc += data[i];
        if (roll < acc) {
            break;
        }
    }
    return (s32)i;
}

static bool IsChargeGroup(s32 group)
{
    return group >= 4 && group < 6;
}

/* State 1: picks the hit count and, outside the charge groups, the combo-table entry. */
static void ChooseCombo(BtlAi *self, BtlAiCommand *cmd)
{
    s32 group = self->owner->comboIndex;
    bool charge = IsChargeGroup(group);
    u32 maxLen = 0;
    u32 count = 0;
    s32 *data = NULL;
    float weight;
    float falloff;
    s32 hits;
    s32 entry;
    s32 bestIndex;
    u32 bestLen;
    u32 i;

    cmd->state = 2;
    for (i = 0; i < (u32)BtlAiGetComboCount(self); i++) {
        if (BtlAiGetComboGroup(self, i) == group && maxLen < (u32)BtlAiGetComboLength(self, i)) {
            maxLen = BtlAiGetComboLength(self, i);
        }
    }
    if (charge) {
        s32 kind = (s32)self->owner->base.base.unk08;

        if ((kind == 7 || kind == 0xd) && (s32)maxLen < 5) {
            maxLen = 5;
        }
    }

    /* Hit-count weights: 100 * comboFalloff^i, truncated to an unsigned integer. */
    if (maxLen != 0) {
        data = WeightsAlloc(maxLen);
        count = maxLen;
    }
    falloff = self->comboFalloff;
    weight = 100.0f;
    for (i = 0; (s32)i < (s32)maxLen; i++) {
        u32 w;

        if (weight < 2147483648.0f) {
            w = (u32)(s32)weight;
        } else {
            w = (u32)(s32)(weight - 2147483648.0f) + 0x80000000u;
        }
        if (i < count && data[i] != (s32)w) {
            data[i] = (s32)w;
        }
        weight = falloff * weight;
        if (weight < 0.0f) {
            weight = 0.0f;
        }
    }
    hits = WeightsPick(data, count) + 1;
    cmd->argF = (float)hits;

    if (!charge) {
        /* Entry weights: 1 for entries of the group whose length is exactly `hits`, else 0. */
        u32 entries = BtlAiGetComboCount(self);

        if (data != NULL) {
            WeightsFree(data);
        }
        data = NULL;
        count = 0;
        if (entries != 0) {
            data = WeightsAlloc(entries);
            count = entries;
        }
        /* bestLen starts at 0xffffffff, so the unsigned `bestLen < length` test never passes and
           bestIndex stays -1. */
        bestLen = 0xffffffffu;
        bestIndex = -1;
        for (i = 0; i < (u32)BtlAiGetComboCount(self); i++) {
            if (BtlAiGetComboGroup(self, i) != group) {
                if (i < count && data[i] != 0) {
                    data[i] = 0;
                }
                continue;
            }
            if (BtlAiGetComboLength(self, i) == hits) {
                continue;
            }
            if (i < count && data[i] != 0) {
                data[i] = 0;
            }
            if (bestLen < (u32)BtlAiGetComboLength(self, i)) {
                bestLen = BtlAiGetComboLength(self, i);
                bestIndex = (s32)i;
            }
        }
        entry = WeightsPick(data, count);
        cmd->arg = entry;
        if (entry < 0) {
            if (bestIndex != -1) {
                cmd->arg = bestIndex;
            } else {
                cmd->state = 3;
            }
        }
    }
    if (data != NULL) {
        WeightsFree(data);
    }
}

/* State 2: plays the next combo input, or moves to state 3 when the combo should stop. */
static void PlayCombo(BtlAi *self, BtlAiCommand *cmd)
{
    BtlBakugan *owner = self->owner;
    BtlBakugan *target;
    s32 step;
    s32 input;

    if (owner->attackResult == 0) {
        return;
    }
    if (owner->attackResult == 4 || !(owner->targetDistance < 500.0f)) {
        BtlAiCommandFinish(self, cmd);
        return;
    }
    step = owner->comboStep + 1;
    if (step == 0) {
        return;
    }
    if (BtlAiGetComboLength(self, cmd->arg) == step + 1 && BtlBakuganGetTarget(self->owner) != NULL &&
        UnitVirtual(BtlBakuganGetTarget(self->owner), 10) != 0) {
        target = BtlBakuganGetTarget(self->owner);
        if ((target->stateFlags & 0x200000) != 0) {
            s32 threshold = (s32)(BtlCombatGetEnergy(&target->combat) * 0.00100000005f * 100.0f);

            if ((s32)CoreRandNext(99) < threshold) {
                cmd->state = 3;
                return;
            }
        }
    }
    if ((s32)cmd->argF == step) {
        cmd->state = 3;
        return;
    }
    if (IsChargeGroup(self->owner->comboIndex)) {
        BtlAiPadRelease04Press10(&self->pad);
        BtlAiPadPress04(&self->pad);
        return;
    }
    input = BtlAiGetComboInput(self, cmd->arg, step);
    if (input == 1) {
        BtlAiPadRelease04Press10(&self->pad);
    } else if (input == 2) {
        BtlAiPadRelease04Press10010(&self->pad);
    }
}

/* Executes the combo-attack command `cmd` of `BtlAi` on its virtual pad `pad`. State
   0 presses the opening input `firstInput` (0 `BtlAiPadRelease04Press10`, 1
   `BtlAiPadRelease04Press10010`, 2 `BtlAiPadPress14`, anything else nothing) and, when the
   press reports success, goes to state 1 and sets `failed`. State 1 finishes the command and sets
   `failed` when the owner is not attacking (bit 0x400000 of `stateFlags`), finishes it when the
   owner's `attackResult` is 4, waits while it is 0, and otherwise enters state 2 and chooses the
   combo: the hit count is drawn (`CoreRandNext`) from the weights `100 × comboFalloff^i`
   (truncated to integers) for i below the longest combo-table length of the owner's combo group
   `comboIndex` (raised to 5 for groups 4/5 when the owner kind `unk08` is 7 or 0xd), held in a
   low-heap array; it is stored as a float in `argF` (0 when every weight is 0). Outside groups 4/5
   it then draws `arg` uniformly among the combo-table entries of that group whose length equals
   the hit count (`BtlAiGetComboCount`, `BtlAiGetComboGroup`, `BtlAiGetComboLength`); when
   there is none it goes to state 3 (the longest-other-entry fallback the code also computes can
   never be chosen: its running maximum starts at 0xffffffff in an unsigned compare). State 2
   finishes the command when the owner stops attacking, its `attackResult` becomes 4 or
   `targetDistance` is not below 500; waits while `attackResult` is 0; otherwise, for the next
   combo step (`comboStep + 1`), on the last step of the entry against a target that answers
   virtual slot 10 and guards (bit 0x200000), goes to state 3 when `CoreRandNext(99)` is below
   `energy × 0.001 × 100` (`BtlBakuganGetTarget`, `BtlCombatGetEnergy`); goes to state 3 when
   the step equals the hit count; else presses `BtlAiPadRelease04Press10` + `BtlAiPadPress04`
   in groups 4/5, or replays the entry's input (`BtlAiGetComboInput`: 1 →
   `BtlAiPadRelease04Press10`, 2 → `BtlAiPadRelease04Press10010`). State 3 waits until the AI's
   `target` is in state 3..6 or has a bit of 0x30000000 set, the owner's `attackResult` is 4 or the
   owner stops attacking, then finishes the command (`BtlAiCommandFinish`); states 3 and 4 then
   end the owner's combo (`BtlBakuganResetCombo`) and set `finished`. In every state `failed` is
   cleared whenever the owner's `combo` counter is non-zero. */
void BtlAiExecComboAttack(BtlAi *self, BtlAiCommand *cmd, s32 firstInput)
{
    BtlBakugan *target;
    s32 pressed;
    bool reacted;

    switch (cmd->state) {
    case 0:
        pressed = 0;
        if (firstInput == 0) {
            pressed = BtlAiPadRelease04Press10(&self->pad);
        } else if (firstInput == 1) {
            pressed = BtlAiPadRelease04Press10010(&self->pad);
        } else if (firstInput == 2) {
            pressed = BtlAiPadPress14(&self->pad);
        }
        if (pressed != 0) {
            cmd->state = 1;
            cmd->failed = 1;
        }
        break;
    case 1:
        if ((self->owner->stateFlags & 0x400000) == 0) {
            BtlAiCommandFinish(self, cmd);
            cmd->failed = 1;
        } else if (self->owner->attackResult == 4) {
            BtlAiCommandFinish(self, cmd);
        } else if (self->owner->attackResult != 0) {
            ChooseCombo(self, cmd);
        }
        break;
    case 2:
        if ((self->owner->stateFlags & 0x400000) == 0) {
            BtlAiCommandFinish(self, cmd);
        } else {
            PlayCombo(self, cmd);
        }
        break;
    case 3:
        target = self->target;
        reacted = false;
        if (target != NULL &&
            ((target->state >= 3 && target->state < 7) || (target->stateFlags & 0x30000000) != 0)) {
            reacted = true;
        }
        if (!reacted && self->owner->attackResult != 4 && (self->owner->stateFlags & 0x400000) != 0) {
            break;
        }
        BtlAiCommandFinish(self, cmd);
        BtlBakuganResetCombo(self->owner);
        cmd->finished = 1;
        break;
    case 4:
        BtlBakuganResetCombo(self->owner);
        cmd->finished = 1;
        break;
    default:
        break;
    }
    if (self->owner->combo != 0) {
        cmd->failed = 0;
    }
}
