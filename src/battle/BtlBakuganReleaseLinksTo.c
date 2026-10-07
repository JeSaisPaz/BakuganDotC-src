// bdc 0x08860414 BtlBakuganReleaseLinksTo
#include "bdc.h"

/* Walks `g_btlBakuganList` and, for every unit other than `target` whose current target
   (`BtlBakuganGetTarget`) is `target`, drops that link with `BtlBakuganClearLink`; when the
   unit is a CPU unit (vtable entry 13, `+0x68`, non-zero) with an AI object, also clears the AI's
   `target`. */
void BtlBakuganReleaseLinksTo(void *target)
{
    BtlBakugan *unit = NULL;

    if (g_btlBakuganList != NULL) {
        unit = *(BtlBakugan **)g_btlBakuganList;
    }
    for (; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        const VtblEntry *isCpuUnit;
        BtlAi *ai;

        if (unit == target || BtlBakuganGetTarget(unit) != target) {
            continue;
        }
        BtlBakuganClearLink(unit);
        isCpuUnit = &((const VtblEntry *)unit->base.base.vtable)[13];
        if (((int (*)(void *))isCpuUnit->fn)((u8 *)unit + isCpuUnit->delta) == 0) {
            continue;
        }
        ai = ((BtlCpuUnit *)unit)->ai;
        if (ai != NULL) {
            ai->target = NULL;
        }
    }
}
