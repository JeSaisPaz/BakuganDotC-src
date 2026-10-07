// bdc 0x0888e3cc BtlAiPickTargetId
#include "bdc.h"

/* Returns the id of the unit the CPU AI (BtlAi) should target, or 0 when there is none. It walks
   the battle unit chain (BtlGetBakuganList) skipping its owner (same id), defeated units
   (`combat.dead`), units with status 9 active or `respawnProtect` set, and those
   BtlAiIsExcludedTarget rejects. In battle rule mode 2 (script global variable 8) with profile
   word 7 == 2 it picks the unit with the highest score `(1 - hp/maxHp) * 100 +
   profileWord(playerSlot + 0xe) * k` (BtlCombatGetHp, BtlCombatGetMaxHp read as unsigned,
   SaveProfileGetWord; `k` from virtual entry 8 of `params` for `level - 1`; the hp term is 0 when
   maxHp is 0), starting from -1. Otherwise it uses BtlAiDistanceInViewToUnit`(1000, 90)`: the
   first candidate is always taken, later ones only when their distance is > 0 and below the kept
   one's distance. */

s32 BtlAiPickTargetId(BtlAi *self)
{
    BtlBakugan **list;
    BtlBakugan *unit;
    const VtblEntry *entry;
    s32 bestId;
    float bestDist;
    float bestScore;
    float hpTerm;
    float hp;
    float maxHp;
    float k;
    float score;
    float dist;
    s32 maxHpInt;
    u32 word;

    bestDist = 0.0f;
    bestScore = -1.0f;
    list = (BtlBakugan **)BtlGetBakuganList();
    if (list == NULL || *list == NULL) {
        return 0;
    }
    bestId = 0;
    for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (self->owner->base.base.id == unit->base.base.id) {
            continue;
        }
        if (unit->combat.dead || unit->combat.status[9].active || unit->respawnProtect) {
            continue;
        }
        if (BtlAiIsExcludedTarget(self, unit)) {
            continue;
        }
        if (g_scriptGlobalVars[8] == 2 && SaveProfileGetWord(SaveGetProfile(), 7) == 2) {
            hpTerm = 0.0f;
            hp = BtlCombatGetHp(&unit->combat);
            maxHpInt = BtlCombatGetMaxHp(&unit->combat);
            maxHp = (float)(u32)maxHpInt;
            if (!(maxHp == 0.0f)) {
                hpTerm = (1.0f - hp / maxHp) * 100.0f;
            }
            word = SaveProfileGetWord(SaveGetProfile(), unit->playerSlot + 0xe);
            entry = &self->params->vtbl[8];
            k = ((float (*)(void *, s32))entry->fn)((u8 *)self->params + entry->delta,
                                                    self->level - 1);
            score = hpTerm + (float)(s32)word * k;
            if (bestScore < score) {
                bestId = unit->base.base.id;
                bestScore = score;
            }
            continue;
        }
        dist = BtlAiDistanceInViewToUnit(1000.0f, 90.0f, self, unit);
        if (!(dist <= 0.0f) && dist < bestDist) {
            bestId = unit->base.base.id;
            bestDist = dist;
        } else if (bestId == 0) {
            bestId = unit->base.base.id;
            bestDist = dist;
        }
    }
    return bestId;
}
