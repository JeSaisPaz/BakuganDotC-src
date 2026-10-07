// bdc 0x0888d0d8 BtlAiApplyLevel
#include "bdc.h"

/* Re-derives the level-dependent tuning of a CPU unit's AI object (`BtlAiCreate`) from its skill
   level (1..10). With `t = (level - 1) / 9` (computed as `(float)(u32)(level - 1) * (1/9)`) it
   interpolates `guardDelayChance` and `seekItemChance` (truncated), `comboFalloff`,
   `guardDelayMin` and `guardDelayMax` between the level-1 and level-10 endpoints of
   `g_btlAiLevelTuning`; reads `waitChance`, `counterChance` and `slot6Chance` from the
   per-species params object (vtable entries 5, 4, 6, called with `level - 1`); sets `rayLength`
   to 90, then to 270 for owner kinds 2 and 10 and 180 otherwise; refills the six weights of
   `delayWeights[0]`/`[1]` from `delaySet0`/`delaySet1` (vtable entry `level + 1` for levels 2..10,
   entry 2 otherwise), skipping borrowed tables and indices past `count` and clearing `totalValid`
   when a weight changes; ORs command bits 0..29 into `allowedCmds` and resets the AI
   (`BtlAiResetAll`). */

typedef s32 (*BtlAiLevelGetFn)(void *self, s32 arg);

static inline s32 BtlAiCallParams(BtlAiParams *params, s32 slot, s32 arg)
{
    const VtblEntry *entry = &params->vtbl[slot];
    return ((BtlAiLevelGetFn)entry->fn)((u8 *)params + entry->delta, arg);
}

static inline s32 BtlAiCallParamSet(BtlAiParamSet *set, s32 slot, s32 arg)
{
    const VtblEntry *entry = &set->vtbl[slot];
    return ((BtlAiLevelGetFn)entry->fn)((u8 *)set + entry->delta, arg);
}

static inline void BtlAiStoreWeight(BtlAiWeightTable *table, u32 index, s32 weight)
{
    if (!table->borrowed && index < table->count && table->weights[index] != weight) {
        table->weights[index] = weight;
        table->totalValid = 0;
    }
}

static inline float BtlAiLevelT(s32 levelIndex)
{
    return (float)(u32)levelIndex * 0.111111112f;
}

void BtlAiApplyLevel(BtlAi *self)
{
    const BtlAiLevelTuning *tune = &g_btlAiLevelTuning;
    s32 levelIndex;
    float t;
    float rayScale;
    u32 kind;
    u32 i;
    u32 bit;
    u32 bits;
    s32 slot;
    s32 level;

    levelIndex = self->level - 1;
    t = BtlAiLevelT(levelIndex);
    self->guardDelayChance = (u8)(tune->guardDelayChance[0] +
        (s32)((float)(tune->guardDelayChance[1] - tune->guardDelayChance[0]) * t));
    t = BtlAiLevelT(levelIndex);
    self->seekItemChance = (u8)(tune->seekItemChance[0] +
        (s32)((float)(tune->seekItemChance[1] - tune->seekItemChance[0]) * t));

    self->waitChance = (u8)BtlAiCallParams(self->params, 5, levelIndex);
    self->counterChance = (u8)BtlAiCallParams(self->params, 4, self->level - 1);
    self->slot6Chance = (u8)BtlAiCallParams(self->params, 6, self->level - 1);

    levelIndex = self->level - 1;
    t = BtlAiLevelT(levelIndex);
    self->comboFalloff = tune->comboFalloff[0] + (tune->comboFalloff[1] - tune->comboFalloff[0]) * t;
    t = BtlAiLevelT(levelIndex);
    self->guardDelayMin = tune->guardDelayMin[0] + (tune->guardDelayMin[1] - tune->guardDelayMin[0]) * t;
    t = BtlAiLevelT(levelIndex);
    self->rayLength = 90.0f;
    self->guardDelayMax = tune->guardDelayMax[0] + (tune->guardDelayMax[1] - tune->guardDelayMax[0]) * t;

    kind = self->owner->base.base.unk08;
    rayScale = (kind == 10 || kind == 2) ? 3.0f : 2.0f;
    self->rayLength = 90.0f * rayScale;

    for (i = 0; (s32)i < 6; i++) {
        level = self->level;
        slot = ((u32)(level - 2) < 9) ? level + 1 : 2;
        BtlAiStoreWeight(&self->delayWeights[0], i, BtlAiCallParamSet(self->delaySet0, slot, (s32)i));
        BtlAiStoreWeight(&self->delayWeights[1], i, BtlAiCallParamSet(self->delaySet1, slot, (s32)i));
    }

    bits = self->allowedCmds;
    for (bit = 0; (s32)bit < 30; bit++) {
        bits |= 1u << bit;
    }
    self->allowedCmds = bits;
    BtlAiResetAll(self);
}
