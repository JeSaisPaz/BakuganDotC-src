// bdc 0x08834218 BtlHudGetNthCrystal
#include "bdc.h"

/* Returns the `n`-th crystal actor (`ActorCrystal`, class predicate vtable
   entry 11 true) of `BtlGetBakuganList`, skipping the player unit (`BtlGetPlayerBakugan`)
   and TargetPoint units (vtable entry 14 true); NULL when there are fewer, when there is no
   player unit or no list. The matching ids are collected into a 21-slot buffer without a bound
   check, and the object is looked up again by id (`CoreObjectListFindById`). `self` is
   unused. Used by `BtlHudUpdateRadar` for its 5 crystal blips. */

CoreObject *BtlHudGetNthCrystal(BtlHud *self, s32 n)
{
  CoreObject **head;
  CoreObject *player;
  CoreObject *obj;
  const VtblEntry *entry;
  s32 count;
  u32 ids[21];

  (void)self;
  head = BtlGetBakuganList();
  player = BtlGetPlayerBakugan();
  if (player == NULL || head == NULL) {
    return NULL;
  }
  count = 0;
  obj = *head;
  if (obj == NULL) {
    return NULL;
  }
  do {
    entry = &((const VtblEntry *)obj->vtable)[14];
    if (((s32 (*)(void *))entry->fn)((u8 *)obj + entry->delta) == 0 && obj != player) {
      entry = &((const VtblEntry *)obj->vtable)[11];
      if (((s32 (*)(void *))entry->fn)((u8 *)obj + entry->delta) != 0) {
        ids[count] = obj->id;
        count++;
      }
    }
    obj = obj->next;
  } while (obj != NULL);
  if (n < count) {
    return CoreObjectListFindById(head, ids[n]);
  }
  return NULL;
}
