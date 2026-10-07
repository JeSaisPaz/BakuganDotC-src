// bdc 0x0888fdf4 BtlAiSelectRule
#include "bdc.h"

/* Picks the next rule of a command channel of the CPU AI object (`BtlAiCreate`). Walks the
   channel's rule group: a rule whose `modeMask` includes the AI's `ruleMode`, whose command
   (`byte02`) has its ability bit in `allowedCmds` and passes `BtlAiIsCommandAllowed` is returned
   at once when its id (`command`) equals the channel's `tag`; with no tag (`tag < 0`) it is a
   candidate when `minLevel <= level <= maxLevel` and its up to three conditions hold (evaluated
   through the `MemberFnPtr` `evalPmf`, e.g. `BtlAiEvalCondition` or `BtlAiEvalConditionXZ`).
   Each rule's weight in weight table `byte03` becomes `weightMin + (weightMax - weightMin) *
   (level - 1) / 9` for candidates and 0 otherwise (borrowed tables untouched). Then, from table 9
   down to 1, a table with a non-zero total is taken when `CoreRandNext``(99)` is below its total
   (table 0 is the fallback); a weighted `CoreRandNext` draw over that table picks the record.
   Returns the chosen rule record, or NULL without a rule group or when the chosen table's total
   is 0. */

BtlAiRuleRecord *BtlAiSelectRule(BtlAi *self, BtlAiChannel *channel, MemberFnPtr *evalPmf)
{
    s16 pmfDelta = evalPmf->delta;
    s16 pmfIndex = evalPmf->index;
    void *pmfFn = evalPmf->pfn;
    BtlAiRuleGroup *group;
    BtlAiRuleRecord *rec;
    BtlAiWeightTable *table;
    BtlAiRuleCondition *cond;
    const VtblEntry *entry;
    u8 *obj;
    void *fn;
    u32 i;
    u32 k;
    u32 c;
    u32 cmd;
    u32 allowed;
    bool ok;
    u16 weight;
    float levelStep;
    s32 sum;
    s32 pick;
    s32 index;
    u8 t;

    group = (BtlAiRuleGroup *)channel->rules;
    if (group == NULL) {
        return NULL;
    }

    rec = group->records;
    for (i = 0; i < (u32)group->count; i++, rec++) {
        ok = false;
        if ((rec->modeMask & (1u << (self->ruleMode & 0x1f))) != 0) {
            cmd = rec->byte02;
            allowed = self->allowedCmds;
            switch (cmd) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 0x19:
                ok = (allowed & 2) != 0;
                break;
            case 2:
            case 4:
            case 6:
            case 8:
            case 0x18:
                ok = (allowed & 4) != 0;
                break;
            case 0xb:
            case 0x10:
                ok = (allowed & 0x800) != 0;
                break;
            case 0xe:
            case 0x1a:
                ok = (allowed & 0x4000) != 0;
                break;
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
                ok = (allowed & 0x40000) != 0;
                break;
            default:
                ok = (allowed & (1u << (cmd & 0x1f))) != 0;
                break;
            }
            if (ok) {
                ok = BtlAiIsCommandAllowed(self, (s32)cmd);
            }
            if (ok) {
                if (channel->tag >= 0) {
                    if (rec->command == channel->tag) {
                        return rec;
                    }
                    continue; /* tagged lookup: weights left as they are */
                }
                ok = !((u32)self->level < rec->minLevel) && !(rec->maxLevel < (u32)self->level);
            }
        }

        /* conditions, each evaluated only while the previous one held */
        cond = rec->cond;
        for (c = 0; c < 3 && ok; c++, cond++) {
            obj = (u8 *)self + pmfDelta;
            fn = pmfFn;
            /* GCC 2.x pointer-to-member: index != 0 is a virtual slot, pfn then holds the vptr offset */
            if (pmfIndex != 0) {
                entry = &(*(const VtblEntry **)(obj + (intptr_t)pmfFn))[pmfIndex];
                obj += entry->delta;
                fn = entry->fn;
            }
            ok = ((bool (*)(float, void *, u8, s32))fn)(cond->value, obj, cond->op, cond->arg);
        }

        table = &channel->weights[rec->byte03];
        if (ok) {
            levelStep = (float)(u32)(self->level - 1) * 0.111111112f;
            weight = (u16)(rec->weightMin +
                           (u16)(s32)((float)((s32)rec->weightMax - (s32)rec->weightMin) * levelStep));
            if (table->borrowed == 0 && i < table->count && table->weights[i] != weight) {
                table->weights[i] = weight;
                table->totalValid = 0;
            }
        } else {
            if (table->borrowed == 0 && i < table->count && table->weights[i] != 0) {
                table->weights[i] = 0;
                table->totalValid = 0;
            }
        }
    }

    /* tables 9..1: take the first whose non-zero total beats CoreRandNext(99); else table 0 */
    t = 9;
    table = &channel->weights[9];
    do {
        if (table->weights != NULL && table->totalValid == 0) {
            table->total = 0;
            for (k = 0; k < table->count; k++) {
                table->total += table->weights[k];
            }
            table->totalValid = 1;
        }
        if (table->total != 0 && (s32)CoreRandNext(99) < (s32)table->total) {
            break;
        }
        t = (u8)(t - 1);
        table = &channel->weights[t];
    } while (t != 0);

    if (table->weights != NULL && table->totalValid == 0) {
        table->total = 0;
        for (k = 0; k < table->count; k++) {
            table->total += table->weights[k];
        }
        table->totalValid = 1;
    }
    if (table->total == 0) {
        return NULL;
    }

    index = -1;
    if (table->weights != NULL && table->totalValid == 0) {
        table->total = 0;
        for (k = 0; k < table->count; k++) {
            table->total += table->weights[k];
        }
        table->totalValid = 1;
    }
    if (table->total != 0) {
        pick = (s32)CoreRandNext(table->total);
        index = 0;
        sum = 0;
        while ((u32)index < table->count) {
            sum += table->weights[index];
            if (pick < sum) {
                break;
            }
            index++;
        }
    }
    return &group->records[index];
}
