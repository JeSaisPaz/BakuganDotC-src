// bdc 0x088905e0 BtlAiEvalCondition
#include "bdc.h"

/* Calls unit virtual `slot` (a no-argument predicate) through the GCC 2.x vtable entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

/* Float comparison `x <op> y`. Ops 4..6 are the negated compares the binary uses
   (`c.eq`/`c.lt`/`c.le` + `bc1f`), so they are true when either side is NaN. */
static bool CompareFloat(float x, float y, s32 op)
{
    switch (op) {
    case 0: return true;
    case 1: return x < y;
    case 2: return x <= y;
    case 3: return x == y;
    case 4: return !(x == y);
    case 5: return !(x < y);
    case 6: return !(x <= y);
    default: return false;
    }
}

/* Signed integer comparison `x <op> y`. */
static bool CompareInt(s32 x, s32 y, s32 op)
{
    switch (op) {
    case 0: return true;
    case 1: return x < y;
    case 2: return x <= y;
    case 3: return x == y;
    case 4: return x != y;
    case 5: return x >= y;
    case 6: return x > y;
    default: return false;
    }
}

/* Unsigned integer comparison `x <op> y`. */
static bool CompareUInt(u32 x, u32 y, s32 op)
{
    switch (op) {
    case 0: return true;
    case 1: return x < y;
    case 2: return x <= y;
    case 3: return x == y;
    case 4: return x != y;
    case 5: return x >= y;
    case 6: return x > y;
    default: return false;
    }
}

/* Flag kinds compare the 0/1 flag with 1 (op 3/5 "set", ops 1/4 "clear", op 2 always, op 6 never). */
static bool CompareFlag(bool flag, s32 op)
{
    return CompareInt(flag ? 1 : 0, 1, op);
}

/* HP of `combat` as a percentage of its maximum (the maximum converted as unsigned). */
static float HpPercent(BtlCombatState *combat)
{
    float hp = BtlCombatGetHp(combat);
    u32 maxHp = (u32)BtlCombatGetMaxHp(combat);

    return hp / (float)maxHp * 100.0f;
}

/* Evaluates one AI rule condition of `BtlAi`: `kind` selects a measured quantity or
   flag of the owner (`self->owner`) or the target, and `op` compares it (0 always, 1 `<`, 2 `<=`,
   3 `==`, 4 `!=`, 5 `>=`, 6 `>`, 7 and others never; float `!=`/`>=`/`>` are true for NaN).
   A "threat" is the current target when it passes the unit virtual tests: slots 10 or 11 in score
   mode 1 (`BtlAiIsScoreMode1`), else slots 10, 15 or 16 (all slots of the mode are called).
   Kinds 1, 2 and 0x29 compare the target distance, `odometer` and `targetHeight` with
   `value * 10`; kinds 7..10 HP/energy percentages, 0x10/0x16 relative angles and 0x1e the
   threat's `targetDistance` compare with `value`; kinds 6, 0x13 and 0x2b compare integers with
   `(s32)value`; flag kinds compare their 0/1 flag with 1; kind 0x22 ignores `op` and is true
   when the target's slot 15 or 16 is non-zero. Kind 0 compares 0 with 0; unknown kinds compare
   `value` with 0 (op 1 is `!(value <= 0)`, 2 `!(value < 0)`, 5 `value <= 0`, 6 `value < 0`).
   A kind whose unit is missing returns false, except kind 0x10 without a target, which falls
   through to kind 0x16. */
bool BtlAiEvalCondition(float value, BtlAi *self, u8 kind, s32 op)
{
    BtlBakugan *threat = NULL;
    BtlBakugan *unit;
    BtlAiFollowLayer *follow;
    CoreObjectList *list;
    CoreObject *found;
    s32 count;
    bool flag;

    if (self->owner != NULL && self->target != NULL) {
        int any;

        if (BtlAiIsScoreMode1() != 0) {
            any = UnitVirtual(self->target, 10);
            any |= UnitVirtual(self->target, 11);
        } else {
            any = UnitVirtual(self->target, 10);
            any |= UnitVirtual(self->target, 15);
            any |= UnitVirtual(self->target, 16);
        }
        if (any != 0) {
            threat = self->target;
        }
    }

    switch (kind) {
    case 0x00:
        return CompareInt(0, 0, op);
    case 0x01:
        return CompareFloat(BtlAiDistanceToUnit(self, NULL), value * 10.0f, op);
    case 0x02:
        return CompareFloat(self->odometer, value * 10.0f, op);
    case 0x04:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag((threat->stateFlags & 0x100000) != 0, op);
    case 0x05:
        return CompareFlag((self->owner->stateFlags & 0x800000) != 0 ||
                               (self->owner->stateFlags & 0x40000000) != 0,
                           op);
    case 0x06:
        return CompareInt(BtlCombatGetSelectedArtTier(&self->owner->combat), (s32)value, op);
    case 0x07:
        return CompareFloat(HpPercent(&self->owner->combat), value, op);
    case 0x08:
        if (self->target == NULL) {
            return false;
        }
        return CompareFloat(HpPercent(&self->target->combat), value, op);
    case 0x09:
        return CompareFloat(BtlCombatGetEnergy(&self->owner->combat) * 0.001f * 100.0f, value, op);
    case 0x0a:
        if (threat == NULL) {
            return false;
        }
        return CompareFloat(BtlCombatGetEnergy(&threat->combat) * 0.001f * 100.0f, value, op);
    case 0x10:
        if (self->target != NULL) {
            return CompareFloat(BtlAiRelativeAngleDeg(self, self->owner, self->target), value, op);
        }
        /* fall through */
    case 0x16:
        if (threat == NULL) {
            return false;
        }
        return CompareFloat(BtlAiRelativeAngleDeg(self, threat, self->owner), value, op);
    case 0x12:
        if (threat == NULL) {
            return false;
        }
        unit = self->owner;
        return CompareFlag((unit->stateFlags & 0x30000000) != 0 || unit->state == 4 ||
                               unit->state == 5,
                           op);
    case 0x13:
        if (threat == NULL) {
            return false;
        }
        return CompareUInt(threat->base.base.unk08, (u32)(s32)value, op);
    case 0x14:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag((threat->stateFlags & 0x200000) != 0, op);
    case 0x15:
        return CompareFlag((self->owner->stateFlags & 0x1000000) != 0, op);
    case 0x17:
        if (self->incomingAttack == NULL) {
            return false;
        }
        return CompareFlag(((BtlAttack *)self->incomingAttack)->params.reflectMode == 1, op);
    case 0x18:
        return CompareFlag((self->moveFlags & 0x80) != 0, op);
    case 0x19:
        return CompareFlag(self->incomingAttack != NULL, op);
    case 0x1a:
        return CompareFlag((self->moveFlags & 0x100) != 0, op);
    case 0x1b:
        return CompareFlag((self->moveFlags & 0x200) != 0, op);
    case 0x1c:
        return CompareFlag((self->owner->stateFlags & 0x200000) != 0, op);
    case 0x1d:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag((threat->stateFlags & 0x800000) != 0, op);
    case 0x1e:
        /* The threat is targeting the unit our follow layer follows. */
        if (threat == NULL || BtlBakuganGetTarget(threat) == NULL) {
            return false;
        }
        follow = (BtlAiFollowLayer *)self->layerOrder[1];
        if (follow == NULL) {
            return false;
        }
        found = (CoreObject *)BtlBakuganListFind(follow->leader);
        if (found == NULL || found->id != threat->targetId) {
            return false;
        }
        return CompareFloat(threat->targetDistance, value, op);
    case 0x1f:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag((threat->stateFlags & 0x40000) != 0, op);
    case 0x20:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag(BtlCombatHasAnyStatus(&threat->combat) != 0, op);
    case 0x21:
        return CompareFlag(self->owner->state == 0x13, op);
    case 0x22:
        unit = self->target;
        if (unit != NULL && (UnitVirtual(unit, 15) != 0 || UnitVirtual(self->target, 16) != 0)) {
            return true;
        }
        return false;
    case 0x23:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag(UnitVirtual(threat, 10) != 0, op);
    case 0x24:
        return CompareFlag(BtlBakuganIsInStageWater(self->owner) != 0, op);
    case 0x25:
        return CompareFlag(BtlBakuganIsInWaterStage12To1B(self->owner) != 0, op);
    case 0x26:
        return CompareFlag(BtlBakuganIsInWaterStage0CTo0F(self->owner) != 0, op);
    case 0x28:
        return CompareFlag(self->dodgePressed != 0, op);
    case 0x29:
        return CompareFloat(self->targetHeight, value * 10.0f, op);
    case 0x2a:
        flag = false;
        if (self->owner != NULL) {
            flag = BtlAiDistanceInViewToUnit(2000.0f, 30.0f, self, self->owner) != 0.0f;
        }
        return CompareFlag(flag, op);
    case 0x2b:
        /* Live units other than the owner that the AI may target. */
        count = 0;
        list = (CoreObjectList *)BtlGetBakuganList();
        if (list != NULL) {
            for (unit = (BtlBakugan *)list->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                if (self->owner == unit || unit->combat.dead != 0) {
                    continue;
                }
                if (BtlAiIsExcludedTarget(self, unit)) {
                    continue;
                }
                count++;
            }
        }
        return CompareInt(count, (s32)value, op);
    case 0x2c:
        if (threat == NULL) {
            return false;
        }
        return CompareFlag(threat->state == 0, op);
    default:
        switch (op) {
        case 0: return true;
        case 1: return !(value <= 0.0f);
        case 2: return !(value < 0.0f);
        case 3: return value == 0.0f;
        case 4: return !(value == 0.0f);
        case 5: return value <= 0.0f;
        case 6: return value < 0.0f;
        default: return false;
        }
    }
}
