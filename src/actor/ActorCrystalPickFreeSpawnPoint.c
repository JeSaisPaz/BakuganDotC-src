// bdc 0x08856664 ActorCrystalPickFreeSpawnPoint
#include "bdc.h"

/* Returns a random crystal spawn point index (0..`ActorStageObjRecordCountType4()`-1, `CoreRandNext`)
   not used by any live crystal in `g_btlBakuganList` (units answering the "is a crystal" virtual
   slot 11, their `spawnPoint`); retries until a free one comes up. Returns 0 outside a battle or
   when there are fewer points than live crystals + 1. The used list holds at most 21 entries (no
   bounds check). */

u32 ActorCrystalPickFreeSpawnPoint(void)

{
  BtlBakugan **list;
  BtlBakugan *unit;
  const VtblEntry *vtbl;
  u32 used[21];
  s32 count;
  s32 n;
  s32 i;
  u32 pick;
  bool taken;

  list = BtlGetBakuganList();
  if (list == NULL) {
    return 0;
  }
  count = 0;
  for (unit = *list; unit != NULL; unit = (BtlBakugan *)unit->base.base.next) {
    vtbl = (const VtblEntry *)unit->base.base.vtable;
    if (((s32 (*)(void *))vtbl[11].fn)((u8 *)unit + vtbl[11].delta) != 0) {
      used[count] = ((ActorCrystal *)unit)->spawnPoint;
      count++;
    }
  }
  pick = 0;
  n = ActorStageObjRecordCountType4();
  taken = false;
  if (n < count + 1) {
    return pick;
  }
  do {
    pick = CoreRandNext(n);
    if (count > 0) {
      taken = false;
      for (i = 0; i < count; i++) {
        if (used[i] == pick) {
          taken = true;
        }
      }
    }
  } while (taken);
  return pick;
}
