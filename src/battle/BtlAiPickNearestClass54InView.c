// bdc 0x0888e660 BtlAiPickNearestClass54InView
#include "bdc.h"

/* Target picker `MemberFnPtr` of `BtlAi` (installed by `BtlAiSetTargetPickers`):
   walks the unit list (`BtlGetBakuganList`) over units that are not the owner (ids differ),
   alive (`combat.dead` clear), not respawn-protected, answer non-zero to vtable entry 10 (`+0x54`),
   have status 9 inactive and are not excluded (`BtlAiIsExcludedTarget`). Each such unit's view
   distance (`BtlAiDistanceInViewToUnit` with range 1000 and 90°) is taken when no unit has been
   picked yet, or when it is positive and below the best so far (which starts at 0). Returns the
   picked unit's object id, 0 when there is no list or no candidate. */
s32 BtlAiPickNearestClass54InView(BtlAi *self)
{
  void **list;
  BtlBakugan *unit;
  const VtblEntry *vtbl;
  s32 bestId = 0;
  float best = 0.0f;
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
    if (((int (*)(void *))vtbl[10].fn)((u8 *)unit + vtbl[10].delta) == 0 ||
        unit->combat.status[9].active != 0 || BtlAiIsExcludedTarget(self, unit)) {
      continue;
    }
    dist = BtlAiDistanceInViewToUnit(1000.0f, 90.0f, self, unit);
    if ((!(dist <= 0.0f) && dist < best) || bestId == 0) {
      bestId = unit->base.base.id;
      best = dist;
    }
  }
  return bestId;
}
