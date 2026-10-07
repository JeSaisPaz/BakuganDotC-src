// bdc 0x08893660 BtlAiUpdateTarget
#include "bdc.h"

/* Target-selection step of `BtlAi`: when ability flag 0x8000 of `ai+0x918` is set,
   the target channel `+0x530` is not mid-step (`+0x54c` = 0) and has a rule list (`+0x57c`), it
   ticks the channel's hold timer (`+0x560`/`+0x564`/`+0x568`) and walks the rules (0x48-byte
   records) whose mode mask `+0x44` includes the rule mode `ai+0x2c0`. Only rule kinds 0xf, 0x16
   and 0x17 (`+2`; 0x10..0x15 and >= 0x18 are skipped) are tried with a level-interpolated chance
   `min + (max − min) × (level − 1) / 9` (`+4`/`+6`)
   per `CoreRandNext``(99)`; the condition kind `+0xc` picks the candidate: 0xb the units returned
   by the four `MemberFnPtr` getters at `ai+0x94c` (not in `BtlAiIsScoreMode1` mode when the
   unit answers virtual `+0x54`), 0xc a live unit for which `BtlAiIsExcludedTarget` returns true, in state 3..6,
   0xd a unit targeting the owner while the owner guards/staggers (bits 0x30000000 or states 3..6),
   0xe a unit whose target is valid and which is attacking (bit 0x400000 or state 8/10), 0xf the
   first unit with bit 0x100000 (flushing `BtlAiResetCommands` when it differs from the target);
   other kinds test `BtlAiEvalConditionXZ` and then take the nearest unit of class 0x16 (rule kind
   0x16) or 1 (rule kind 0x17) (`BtlAiFindNearestUnitByClass`). A new target is set with
   `BtlAiSetTarget` and `BtlAiMarkTargetChanged`, clears the saved target `ai+0x970` and arms
   the hold timer with `BtlAiRollRange` of the rule's `+0x30` range. Finally a defeated (`+0x4c1`)
   or removed (`+0x574`) target is dropped (also clearing `owner+0x168 → +0x1c` and resetting the
   attack channel `+0x404`), as is a live target with byte `+0x476` set. Returns 1 when a new target
   was chosen this call, else 0. */

/* Virtual call `slot` of a unit's `CoreObject` vtable (`+0x14`), `this` adjusted by the entry. */
static int UnitVirtual(BtlBakugan *unit, int slot)
{
    const VtblEntry *entry = &((const VtblEntry *)unit->base.base.vtable)[slot];

    return ((int (*)(void *))entry->fn)((u8 *)unit + entry->delta);
}

s32 BtlAiUpdateTarget(BtlAi *self)
{
    BtlAiChannel *channel = &self->channels[2];
    BtlAiRuleGroup *group;
    BtlAiRuleRecord *rec;
    /* the candidate survives from one rule to the next, as in the listing (register s2) */
    BtlBakugan *cand = NULL;
    BtlBakugan *unit;
    BtlBakugan *target;
    CoreObjectList *list;
    s32 changed = 0;
    u32 i;

    if ((self->allowedCmds & 0x8000) == 0 || channel->step != 0) {
        return 0;
    }
    group = (BtlAiRuleGroup *)channel->rules;
    if (group == NULL) {
        return 0;
    }
    if (channel->cmdExpired == 0) {
        float elapsed = channel->cmdElapsed + 0.0333333351f;
        float limit = channel->cmdLimit;

        channel->cmdElapsed = elapsed;
        if (!(elapsed < limit)) {
            channel->cmdElapsed = limit;
            channel->cmdExpired = 1;
        }
    }

    rec = group->records;
    for (i = 0; i < (u32)group->count; i++, rec++) {
        u32 minW;
        float lvl;
        u32 chance;
        u8 op;

        if ((rec->modeMask & (1u << (self->ruleMode & 0x1f))) == 0) {
            continue;
        }
        if (rec->byte02 < 0x16) {
            if (rec->byte02 != 0xf) {
                continue;
            }
        } else if (rec->byte02 >= 0x18) {
            continue;
        }

        /* chance = min + (max - min) * ((level - 1) / 9), level - 1 converted as unsigned */
        minW = rec->weightMin;
        lvl = (float)(u32)(self->level - 1) * 0.111111112f;
        chance = (minW + ((u32)(s32)((float)(s32)((u32)rec->weightMax - minW) * lvl) & 0xffff)) &
                 0xffff;

        op = rec->cond[0].op;
        switch (op) {
        case 0xb: {
            s32 j;

            if (channel->cmdExpired == 0 && self->target != NULL) {
                break;
            }
            for (j = 0; j < 4; j++) {
                const MemberFnPtr *e = &self->targetPickers[j];
                u8 *obj;
                void *fn;
                u32 id;

                if (e->index == 0 && e->delta == 0 && e->pfn == NULL) {
                    continue;
                }
                obj = (u8 *)self + e->delta;
                fn = e->pfn;
                /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the
                   vptr offset */
                if (e->index != 0) {
                    const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
                    const VtblEntry *entry = &vtbl[e->index];

                    obj += entry->delta;
                    fn = entry->fn;
                }
                id = ((u32 (*)(void *))fn)(obj);
                if (id == 0) {
                    continue;
                }
                cand = (BtlBakugan *)BtlFindBakuganById(id);
                if (UnitVirtual(cand, 10) != 0 && BtlAiIsScoreMode1() != 0) {
                    cand = NULL;
                    continue;
                }
                if (!((s32)CoreRandNext(99) < (s32)chance)) {
                    cand = NULL;
                    continue;
                }
                if (self->target != NULL && self->target->base.base.id == cand->base.base.id) {
                    cand = NULL;
                }
                if (cand != NULL) {
                    break;
                }
            }
            break;
        }
        case 0xc:
            list = (CoreObjectList *)BtlGetBakuganList();
            if (list == NULL) {
                break;
            }
            for (unit = (BtlBakugan *)list->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                if (self->owner->base.base.id == unit->base.base.id) {
                    continue;
                }
                if (UnitVirtual(unit, 10) == 0) {
                    continue;
                }
                if (!BtlAiIsExcludedTarget(self, unit)) {
                    continue;
                }
                if (unit->state < 3 || unit->state >= 7) {
                    continue;
                }
                if ((s32)CoreRandNext(99) < (s32)chance) {
                    cand = unit;
                    break;
                }
            }
            break;
        case 0xd: {
            BtlBakugan *owner = self->owner;

            /* owner guards/staggers: bits 0x30000000, or state 3..6 (the listing also tests 4,
               5 and 3 separately) */
            if ((owner->stateFlags & 0x30000000) == 0 && !(owner->state >= 3 && owner->state < 7)) {
                break;
            }
            list = (CoreObjectList *)BtlGetBakuganList();
            if (list == NULL) {
                break;
            }
            for (unit = (BtlBakugan *)list->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                if (self->owner->base.base.id == unit->base.base.id) {
                    continue;
                }
                if (UnitVirtual(unit, 10) == 0) {
                    continue;
                }
                if (BtlBakuganGetTarget(unit) == self->owner) {
                    cand = unit;
                    break;
                }
            }
            break;
        }
        case 0xe:
            list = (CoreObjectList *)BtlGetBakuganList();
            if (list == NULL) {
                break;
            }
            for (unit = (BtlBakugan *)list->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                if (self->owner->base.base.id == unit->base.base.id) {
                    continue;
                }
                if (UnitVirtual(unit, 10) == 0) {
                    continue;
                }
                if (BtlBakuganGetTarget(unit) == NULL) {
                    continue;
                }
                if (UnitVirtual((BtlBakugan *)BtlBakuganGetTarget(unit), 11) == 0) {
                    continue;
                }
                if ((unit->stateFlags & 0x400000) == 0 && unit->state != 8 && unit->state != 10) {
                    continue;
                }
                if ((s32)CoreRandNext(99) < (s32)chance) {
                    cand = unit;
                    break;
                }
            }
            break;
        case 0xf:
            list = (CoreObjectList *)BtlGetBakuganList();
            if (list == NULL) {
                break;
            }
            for (unit = (BtlBakugan *)list->head; unit != NULL;
                 unit = (BtlBakugan *)unit->base.base.next) {
                if (self->owner->base.base.id == unit->base.base.id) {
                    continue;
                }
                if (UnitVirtual(unit, 10) == 0) {
                    continue;
                }
                if ((unit->stateFlags & 0x100000) == 0) {
                    continue;
                }
                if ((s32)CoreRandNext(99) < (s32)chance) {
                    cand = unit;
                }
                if (self->target != NULL) {
                    if (self->target->base.base.id != unit->base.base.id) {
                        BtlAiResetCommands(self);
                    } else {
                        cand = NULL;
                    }
                }
                break;
            }
            break;
        default:
            if (!BtlAiEvalConditionXZ(rec->cond[0].value, self, op, rec->cond[0].arg)) {
                break;
            }
            if (rec->byte02 == 0x16) {
                u32 mask = 0x16;

                cand = BtlAiFindNearestUnitByClass(self, &mask);
            } else if (rec->byte02 == 0x17) {
                u32 mask = 1;

                cand = BtlAiFindNearestUnitByClass(self, &mask);
            }
            if (cand != NULL && !((s32)CoreRandNext(99) < (s32)chance)) {
                cand = NULL;
            }
            break;
        }

        if (cand == NULL) {
            continue;
        }
        target = self->target;
        changed = (target == NULL || target->base.base.id != cand->base.base.id);
        if (changed) {
            s32 frames = (s32)BtlAiRollRange(self, (BtlAiRange *)&rec->byte30);
            float limit = (float)frames * 0.0333333351f;

            channel->cmdElapsed = 0.0f;
            channel->cmdLimit = limit;
            channel->cmdExpired = limit <= 0.0f;
            BtlAiSetTarget(self, cand);
            self->savedTarget = NULL;
            BtlAiMarkTargetChanged(self);
            break;
        }
    }

    target = self->target;
    if (target != NULL) {
        if ((target->combat.dead | (target->respawnProtect != 0)) != 0) {
            BtlAiSetTarget(self, NULL);
            BtlAiMarkTargetChanged(self);
            changed = 0;
            self->owner->input->aiActions = 0;
            BtlAiChannelReset(&self->channels[1]);
        } else if (UnitVirtual(target, 10) != 0 && self->target->combat.status[9].active != 0) {
            BtlAiSetTarget(self, NULL);
            BtlAiMarkTargetChanged(self);
            changed = 0;
        }
    }
    return changed;
}
