// bdc 0x0888e968 BtlAiPickNearestClass84InView
#include "bdc.h"

/* Target picker `MemberFnPtr` of `BtlAi` (installed by `BtlAiSetTargetPickers`):
   walks the unit list (`BtlGetBakuganList`) and returns the object id of the nearest unit that is
   not the owner (ids differ), alive (`combat.dead` clear), not respawn-protected, answers non-zero
   to its virtual predicate in vtable slot 16 (`+0x80`), is not excluded
   (`BtlAiIsExcludedTarget`) and has a positive in-view distance
   `BtlAiDistanceInViewToUnit``(1000.0, 90.0, …)` (distances start at 10000.0). Returns 0 when
   there is no list or no such unit. */
s32 BtlAiPickNearestClass84InView(BtlAi *self)
{
    void **list;
    BtlBakugan *unit;
    const VtblEntry *vtbl;
    s32 bestId = 0;
    float best = 10000.0f;
    float dist;

    list = (void **)BtlGetBakuganList();
    if (list == NULL || *list == NULL) {
        return 0;
    }
    for (unit = (BtlBakugan *)*list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
        if (self->owner->base.base.id == unit->base.base.id || unit->combat.dead != 0 ||
            unit->respawnProtect != 0) {
            continue;
        }
        vtbl = (const VtblEntry *)unit->base.base.vtable;
        if (((int (*)(void *))vtbl[16].fn)((u8 *)unit + vtbl[16].delta) == 0 ||
            BtlAiIsExcludedTarget(self, unit)) {
            continue;
        }
        dist = BtlAiDistanceInViewToUnit(1000.0f, 90.0f, self, unit);
        if (!(dist <= 0.0f) && dist < best) {
            bestId = unit->base.base.id;
            best = dist;
        }
    }
    return bestId;
}
