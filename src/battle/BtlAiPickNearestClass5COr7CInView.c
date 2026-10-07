// bdc 0x0888e7d8 BtlAiPickNearestClass5COr7CInView
#include "bdc.h"

/* Target picker `MemberFnPtr` of `BtlAi` (installed by `BtlAiSetTargetPickers`):
   walks the unit list (`BtlGetBakuganList`) over units that are not the owner (ids differ),
   alive (`combat.dead` clear), not respawn-protected, answer non-zero to vtable entry 11 (`+0x5c`)
   or else entry 15 (`+0x7c`), and are not excluded (`BtlAiIsExcludedTarget`). Returns the object
   id of the one with the smallest positive view distance (`BtlAiDistanceInViewToUnit` with range
   1000 and 90°) below 10000; 0 when there is no list or no such unit. */
s32 BtlAiPickNearestClass5COr7CInView(BtlAi *self)
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
    if (((int (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) == 0) {
      vtbl = (const VtblEntry *)unit->base.base.vtable;
      if (((int (*)(void *))vtbl[15].fn)((u8 *)unit + vtbl[15].delta) == 0) {
        continue;
      }
    }
    if (BtlAiIsExcludedTarget(self, unit)) {
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
