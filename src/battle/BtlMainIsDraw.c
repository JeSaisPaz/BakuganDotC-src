// bdc 0x0884d460 BtlMainIsDraw
#include "bdc.h"

/* Draw check: false in script mode 1 (script var 8); when profile word 7 is 1 or 2 (ranked)
   `BtlMainIsScoreTie`; otherwise compares the first two units of the Bakugan list by HP
   percentage of `stats->levelHp[4]` (truncated, low byte): different → false, equal and non-zero
   → true; both 0 % → true unless exactly one of the two HP values is above 0 (both exactly 0 is a
   draw). */

bool BtlMainIsDraw(BtlMain *self)
{
    CoreObjectList *list;
    BtlBakugan *a;
    BtlBakugan *b;
    float hpA;
    float hpB;
    float maxA;
    u8 pctA;
    u8 pctB;

    (void)self;
    if (g_scriptGlobalVars[8] == 1) {
        return false;
    }
    {
        s32 ranked = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
        if (ranked > 0 && ranked < 3) {
            return BtlMainIsScoreTie();
        }
    }
    list = (CoreObjectList *)BtlGetBakuganList();
    a = (BtlBakugan *)list->head;
    hpA = BtlCombatGetHp(&a->combat);
    maxA = a->combat.stats->levelHp[4];
    b = (BtlBakugan *)a->base.base.next;
    hpB = BtlCombatGetHp(&b->combat);
    pctA = (u8)(s32)((hpA * 100.0f) / maxA);
    pctB = (u8)(s32)((hpB * 100.0f) / b->combat.stats->levelHp[4]);
    if (pctA != pctB) {
        return false;
    }
    if (pctA != 0) {
        return true;
    }
    if (hpA == 0.0f && hpB == 0.0f) {
        return true;
    }
    if (!(hpA <= 0.0f) && hpB <= 0.0f) {
        return false;
    }
    if (hpA <= 0.0f && !(hpB <= 0.0f)) {
        return false;
    }
    return true;
}
